#include <omnetpp.h>
#include "Mail_m.h"

using namespace omnetpp;


class Queue : public cSimpleModule
{
protected:
    cMessage *msgInServer;
    cMessage *endOfServiceMsg;

    cQueue queue;

    simsignal_t qlenSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;
    simsignal_t responseTimeSignal;

    double avgServiceTime;

    bool serverBusy;

public:
    Queue();
    virtual ~Queue();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void startPacketService(cMessage* msg);
    void putPacketInQueue(cMessage *msg);

    //NEW
    cMessage* get_shortest_packet(cQueue* queue);
};

Define_Module(Queue);


Queue::Queue()
{
    msgInServer = endOfServiceMsg = nullptr;
}

Queue::~Queue()
{
    delete msgInServer;
    cancelAndDelete(endOfServiceMsg);
}

void Queue::initialize()
{


    endOfServiceMsg = new cMessage("end-service");
    queue.setName("queue");
    serverBusy = false;

    //signal registering
    qlenSignal = registerSignal("qlen");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");
    responseTimeSignal = registerSignal("responseTime");

    //initial messages
    emit(qlenSignal, queue.getLength());
    emit(busySignal, serverBusy);

    //get avgServiceTime parameter
    avgServiceTime = par("avgServiceTime").doubleValue();
}

void Queue::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) { //Packet in server has been processed

        //log service completion
        EV << "Completed service of " << msgInServer->getName() << endl;

        //Send processed packet to sink
        send(msgInServer, "out");

        //emit response time signal
        emit(responseTimeSignal, simTime() - msgInServer->getTimestamp());

        //start next packet processing if queue not empty
        if (!queue.isEmpty()) {

            //Put the next message from the queue inside the server. NO THE NEXT
            //PICK THE SHORTEST SERVICE TIME INSTEAD

            //msgInServer = (cMessage *)queue.pop();
            msgInServer = get_shortest_packet(&queue);

            //Emit queue len and queuing time for this packet
            emit(qlenSignal, queue.getLength());
            emit(queueingTimeSignal, simTime() - msgInServer->getTimestamp());

            //start service
            startPacketService(msg);
        } else {
            //server is not busy anymore
            msgInServer = nullptr;
            serverBusy = false;
            emit(busySignal, serverBusy);

            //log idle server
            EV << "Empty queue, server goes IDLE" <<endl;
        }

    }
    else { //packet from source has arrived

        //Setting arrival timestamp as msg field
        msg->setTimestamp();

        if (serverBusy) {
            putPacketInQueue(msg);
        }
        else { //server idle, start service right away
            //Put the message in server and start service
            msgInServer = msg;
            startPacketService(msg);

            //server is now busy
            serverBusy=true;
            emit(busySignal, serverBusy);

            //queueing time was ZERO
            emit(queueingTimeSignal, SIMTIME_ZERO);
        }
    }
}

void Queue::startPacketService(cMessage* msg)
{

    //generate service time and schedule completion accordingly NO MORE. INSTEAD
    //pick the specific service time and use that for schedule

    Mail* mail = (Mail*)msgInServer;

    simtime_t service_time = mail->getService_time();
    scheduleAt(simTime()+service_time, endOfServiceMsg);

    //log service start
    EV << "Starting service of " << mail->getName()
            << " with a service time of " << mail->getService_time() << " s." << endl;

}

void Queue::putPacketInQueue(cMessage *msg)
{
    queue.insert(msg);
    emit(qlenSignal, queue.getLength());

    //log new message in queue
    EV << msg->getName() << " enters queue"<< endl;
}

cMessage* Queue::get_shortest_packet(cQueue* queue){

    int counter = 0;
    int length = queue->getLength();
    int shortest_index = 0;

    cMessage* shortest = nullptr;

    // if queue is empty (should never happen)
    if (length == 0 ) {
        return nullptr;

    // if queue has only one item
    } else if (length == 1) {
        return (cMessage* )queue->pop();

    // if queue has some elements in it
    } else {

        for (counter=0; counter<length; counter++) {

            // if shortest has no value
            if (shortest == nullptr) {
                shortest = (cMessage* )queue->get(counter);

            // compare shortest time with new time
            } else {

                simtime_t shortest_time = ((Mail* )shortest)->getService_time();
                simtime_t new_time = ((Mail* )queue->get(counter))->getService_time();

                // new time is shorter than saved one
                if (new_time < shortest_time) {
                    shortest = (cMessage *)queue->get(counter);
                    shortest_index = counter;
                }

            }

        }
        queue->remove(shortest);
        return shortest;

    }

}

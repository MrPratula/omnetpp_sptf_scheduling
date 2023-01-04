#include <omnetpp.h>
#include<iostream>
#include<fstream>

#include "Mail_m.h"


using namespace omnetpp;


class Sink : public cSimpleModule
{
  private:
    simsignal_t lifetimeSignal;
    simsignal_t arrivedMsgSignal;
    int nb_arrivedMsg;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Sink);


void Sink::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
    arrivedMsgSignal = registerSignal("arrivedMsg");
    nb_arrivedMsg = 0;

    //clear the log file
    std::ofstream log;
    log.open("log.txt", std::ios::out | std::ios::trunc);
    log.close();
}

void Sink::handleMessage(cMessage *msg)
{
    //compute packet lifetime
    simtime_t lifetime = simTime() - msg->getCreationTime();
    emit(lifetimeSignal, lifetime);

    //log lifetime on terminal
    EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;

    //log all msg data on file

    Mail* mail = (Mail*)msg;

    double queue_time = mail->getQueue_time();
    double service_time = mail->getService_time();

    std::ofstream log;

    log.open("log.txt", std::ios::app);
    log << mail->getName() << "-" << queue_time << "-" << service_time << "\n";
    log.close();

    nb_arrivedMsg ++;
    emit(arrivedMsgSignal, nb_arrivedMsg);

    delete msg;
}

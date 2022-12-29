simple Source
{
    parameters:
        volatile double avgInterArrivalTime @unit(s);
        volatile double start_uniform @unit(s);
        volatile double end_uniform @unit(s);
        
        @display("i=block/source");
    gates:
        output out;
}
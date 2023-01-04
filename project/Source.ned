simple Source
{
    parameters:
        volatile double avgInterArrivalTime @unit(s);
        volatile double start_uniform @unit(s);
        volatile double end_uniform @unit(s);
        
        @display("i=block/source");
        
        @signal[plen](type="simtime_t");
        
        @statistic[plen](title="packet length";unit=s;record=vector,mean,max;interpolationmode=none);
        
    gates:
        output out;
}
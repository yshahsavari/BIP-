package proj_20;

simple BTCNode
{
    //parameters:
      //  volatile double delayTime @unit(s);   // delay
    gates:
        inout gate[];
}

network Network
{
    parameters:
        int n = 15;
        double connectedness = default(0.15); // 0.0<x<1.0
    types:
        channel C extends ned.DatarateChannel
        {
            datarate = 124.1Mbps;
            delay = 20ms;
        }
    submodules:
        node[n]: BTCNode {
           
            @display("p=100,100,ri,50,50");
        }
    connections:
        for i=0..n-1, for j=0..n-1 {
           
            node[i].gate++ <--> C <--> node[j].gate++ if i!=j && uniform(0,1)<connectedness;
        }
}




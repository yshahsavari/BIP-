#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;
class BTCNode : public cSimpleModule
{
  private:
    cMessage *event1;
    cMessage *event2;


  public:
   int n;
    int counter;
    float Block_Validation_Time=0.15;
      int U1;

    BTCNode();
      virtual ~BTCNode();

    protected:
      virtual void initialize() override;
      virtual void handleMessage(cMessage *msg) override;
  };
Define_Module(BTCNode);

BTCNode::BTCNode()
{

    event1= event2= nullptr;
}

BTCNode::~BTCNode()
{

    cancelAndDelete(event1);
    cancelAndDelete(event2);

}
void BTCNode::initialize()
{
    counter = 0;
    WATCH(counter);


    event1 = new cMessage("event1");


    if (getIndex()==0) {

     //   EV << "Scheduling inv send at t=0.0s\n";

        scheduleAt(0.0, event1);
    }
}

void BTCNode::handleMessage(cMessage *msg)
{

    if (strcmp("event1", msg->getName()) == 0) {

        cPacket *pk = new cPacket("inv");
        pk->setByteLength(37);
       // EV << "The scenario is started, sending inv message to the neighboring nodes. \n";
        n = gateSize("gate");
        for (int i = 0; i < n; i++)
                       {
                            cPacket *copy = pk->dup();

                            send(copy, "gate$o", i);

                       }
               delete pk;
               counter++;
    }
    else if (strcmp("inv", msg->getName()) == 0){
                                                    if(counter!=0){
                                                        delete msg;

                                               //        EV << "Duplicate inv message- I have this block! \n";
                                                     }
                                                     else {

                                                         U1= msg->getArrivalGate()->getIndex();

                                                     //   EV << "Received this inv for the 1st time. wait:" <<  << " secs...\n";
                                                         event2 = new cMessage("event2");
                                                         scheduleAt(simTime()+0.001, event2);
                                                         counter++;
                                                          }
                                                 }
    else if (strcmp("event2", msg->getName()) == 0) {

        cPacket *pk = new cPacket("GetData");
        pk->setByteLength(37);

                   send(pk, "gate$o",U1);
                   //counter++;
                        }


     else if(strcmp("GetData", msg->getName()) == 0)
                     {
                     int U2= msg->getArrivalGate()->getIndex();
                     cPacket *pk = new cPacket("Block");
                     pk->setByteLength(1000000);
                    send(pk, "gate$o",U2);
                  //  counter++;
                     }
     else if(strcmp("Block", msg->getName()) == 0){
    // else {

        EV << "Block received. starting block validation process...\n";
   //    inv = msg;
        //cMessage *msg = new cMessage("event");
        event1 = new cMessage("event1");
        scheduleAt(simTime()+Block_Validation_Time, event1);
    }
}






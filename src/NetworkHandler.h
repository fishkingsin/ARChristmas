#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "ofxThread.h"

#define HOST "localhost"
#define FLASHPORT 3000
#define OSCPORT 3334
class OSCEventArgs : public ofEventArgs
{
     public:
    string arg;
};
class NetworkHandler :public ofxThread
{
public:
    virtual void setup();

    void threadedFunction();
    void recieveMessage();
    virtual void sendMessage(string message){};
    virtual void CommandSearch(string _command){};

    ofxUDPManager udpRecieve;
    ofxOscSender sender;

     ofEvent <OSCEventArgs> MESSAGE_AVAILABLE;


};

#endif // NETWORKHANDLER_H

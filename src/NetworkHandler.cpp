#include "NetworkHandler.h"


void NetworkHandler::setup()
{


//create the socket and bind to port 11999
    udpRecieve.Create();
    udpRecieve.Bind(FLASHPORT);
    udpRecieve.SetNonBlocking(true);


    sender.setup(HOST,OSCPORT);
    startThread(true,false);

}
void NetworkHandler::threadedFunction(){
    while(isThreadRunning()!=0)
    {
        if(lock())
        {
            recieveMessage();
            unlock();
            ofSleepMillis(100);
        }
    }
}
void NetworkHandler::recieveMessage()
{
    char udpMessage[100000];
    udpRecieve.Receive(udpMessage,100000);
    string _message=udpMessage;
    if (_message!="")
    {
        CommandSearch(_message);

    }
}

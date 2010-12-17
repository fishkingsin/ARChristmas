#ifndef MYNETWORKHANDLER_H_INCLUDED
#define MYNETWORKHANDLER_H_INCLUDED

#include "NetworkHandler.h"

class myNetworkHandler :public NetworkHandler
{
public :
    void setup()
    {
        NetworkHandler::setup();
        bLoaded = false;
        bShow = false;
        bTakePhoto = false;


    }
    void sendMessage(string message)
    {
        ofxOscMessage m;
        m.setAddress( "/openframeworks" );
        ofLog(OF_LOG_VERBOSE,"myNetworkHandler sending message: "+message);
        m.addStringArg(message);
        sender.sendMessage( m );

    };
    string getBackground()
    {
        string tmp(fn);
        fn = "";
        return tmp;
    }
    string getForeground()
    {
        string tmp(fn);
        fn = "";
        return tmp;
    }

    bool bLoaded;
    bool bTakePhoto;
    bool bShow;

    ofImage bg;
    string fn;

private:
    void CommandSearch(string _command)
    {
        ofLog(OF_LOG_VERBOSE, _command);
        size_t changeBGfound = _command.find("bg_number");
        size_t changeFGfound = _command.find("fg_number");
        size_t takePhotofound = _command.find("take_photo");
        //size_t emailSent = _command.find("emailSent");
        if (changeBGfound!=string::npos)
        {
            fn = "images/background/"+_command+".jpg";
            static OSCEventArgs e;
            e.arg = _command;
            ofNotifyEvent(MESSAGE_AVAILABLE, e, this);
            //bLoaded = bg.loadImage("images/background/"+_command+".jpg");
            if (bLoaded)
            {
                string addr = "/openframeworks";
                string arg = e.arg;
                ofxOscMessage m;
                m.setAddress(  addr );
                m.addStringArg( arg );
                sender.sendMessage( m );
                ofLog(OF_LOG_VERBOSE,"NetworkHandler sent "+addr+" " +arg);

            }
        }
        else if (changeFGfound!=string::npos)
        {
            fn = "images/foreground/"+_command+".png";
            static OSCEventArgs e;
            e.arg = _command;
            ofNotifyEvent(MESSAGE_AVAILABLE, e, this);
            if (bLoaded)
            {
                string addr = "/openframeworks";
                string arg = e.arg;
                ofxOscMessage m;
                m.setAddress(  addr );
                m.addStringArg( arg );
                sender.sendMessage( m );
                ofLog(OF_LOG_VERBOSE,"NetworkHandler sent "+addr+" " +arg);

            }
        }
        else if (takePhotofound!=string::npos)
        {
            bTakePhoto = true;
            ofLog(OF_LOG_VERBOSE,"Networkhandler recieve take photo");
            static OSCEventArgs e;
            e.arg = _command;
            ofNotifyEvent(MESSAGE_AVAILABLE, e, this);

        }
        /*else if (emailSent)
        {
            bLoaded = false;
            ofLog(OF_LOG_VERBOSE,"Networkhandler recieve email sent");
            static OSCEventArgs e;
            e.arg = _command;
            ofNotifyEvent(MESSAGE_AVAILABLE, e, this);

        }*/
        else
        {
            static OSCEventArgs e;
            e.arg = _command;
            ofNotifyEvent(MESSAGE_AVAILABLE, e, this);
        }

    }
};

#endif // MYNETWORKHANDLER_H_INCLUDED




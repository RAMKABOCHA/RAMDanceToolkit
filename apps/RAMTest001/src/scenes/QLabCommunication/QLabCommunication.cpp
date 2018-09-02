//
//  QlabCommunication.cpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 2/9/2018.
//

#include "QLabCommunication.h"
QLabCommunication* QLabCommunication::__instance = NULL;
void QLabCommunication::setup(string senderHost, int senderPort, int receivePort){
    oscVar.sender.setup(senderHost, senderPort);
    oscVar.receiver.setup(receivePort);
    oscVar.startThread();
}
void QLabCommunication::update(){
    
}


//
//  QLabCommunication.h
//  RAM Dance Toolkit
//
//  Created by James Kong on 26/8/2018.
//

#ifndef QLabCommunication_h
#define QLabCommunication_h
#include "ramUnit.h"
#include "ramControlPanel.h"
#include "ramBaseHasFbo.h"
#include "ramImGuiPanel.h"


class QLabCommunication{
private:
    
    
    rdtk::OscManager*        oscManager;
    rdtk::OscReceiveTag    oscReceiver;
public:
    const std::string RAM_OSC_ADDR_COMMUNICATE_QLAB    = "/ram/communicate/qlab/";
    void setup(string address)
    {
        oscManager = &rdtk::OscManager::instance();
        oscReceiver.addAddress(address);
        oscManager->addReceiverTag(&oscReceiver);
    }
    void update()
    {
        while (oscReceiver.hasWaitingMessages()) {
            ofxOscMessage m;
            oscReceiver.getNextMessage(&m);
            updateWithOscMessage(m);
        }
    }
    virtual void updateWithOscMessage(const ofxOscMessage &m) = 0;
};

#endif /* QLabCommunication_h */

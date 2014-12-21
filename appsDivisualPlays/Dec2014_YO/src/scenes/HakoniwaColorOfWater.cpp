//
//  HakoniwaColorOfWater.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "HakoniwaColorOfWater.h"

HakoniwaColorOfWater::Valve::Valve() :
on(false),
time(0.f),
state(false),
pState(false),
prevTime(0.f),
distance(0.f),
threshould(200.f),
blinkOpen(0.5f),
blinkClose(1.0f)
{
    
}

void HakoniwaColorOfWater::Valve::update(const ramNode& n0, const ramNode& n1)
{
    nodeA = n0;
    nodeB = n1;
    
    distance = n0.getGlobalPosition().distance(n1.getGlobalPosition());

    (distance >= threshould) ? (on = true) : (on = false);
    
    pState = state;
    if (on) {
        time += ofGetLastFrameTime();
        
        if (!state && time - prevTime >= blinkOpen) {
            state = true;
            prevTime = time;
        }
        else if (state && time - prevTime >= blinkClose) {
            state = false;
            prevTime = time;
        }
    }
    else {
        state = false;
    }
}

void HakoniwaColorOfWater::Valve::draw(int color, float x, float y)
{
    ofPushStyle();
    
    switch (color) {
        case 0:
            ofSetColor(ofColor::blue, 128);
            break;
        case 1:
            ofSetColor(ofColor::red, 128);
            break;
        case 2:
            ofSetColor(ofColor::green, 128);
            break;
    }
    
    ofSetLineWidth(2.f);
    
    ofEnableDepthTest();
    ofDisableAlphaBlending();
    
    ramBeginCamera();
    ofLine(nodeA, nodeB);
    ramEndCamera();
    
    ofSetCircleResolution(64);
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    state ? ofFill() : ofNoFill();
    ofCircle(x, y, 100);
    
    ofSetColor(ofColor::white);
    ofDrawBitmapString(ofToString(distance), x, y);
    
    ofPopStyle();
}

HakoniwaColorOfWater::HakoniwaColorOfWater()
{
    try {
        mOscSender.setup("192.168.20.60", 8528);
    }
    catch (exception& e) {
        ofLogWarning("HakoniwaColorOfWater") << e.what();
    }
    
    mNode0a = ramActor::JOINT_LEFT_ELBOW;
    mNode0b = ramActor::JOINT_RIGHT_WRIST;
    mNode1a = ramActor::JOINT_LEFT_ELBOW;
    mNode1b = ramActor::JOINT_LEFT_KNEE;
    mNode2a = ramActor::JOINT_RIGHT_KNEE;
    mNode2b = ramActor::JOINT_LEFT_SHOULDER;
    
    mBlinkOpen = 0.5f;
    mBlinkClose = 1.0f;
    
    for (int i=0; i<kNumValves; i++) {
        mValves[i].threshould = 110.f;
        mValves[i].blinkOpen = mBlinkOpen;
        mValves[i].blinkClose = mBlinkClose;
    }
}

HakoniwaColorOfWater::~HakoniwaColorOfWater()
{
    
}

void HakoniwaColorOfWater::update()
{
    mValves[0].update(mActor.getNode(mNode0a), mActor.getNode(mNode0b));
    mValves[1].update(mActor.getNode(mNode1a), mActor.getNode(mNode1b));
    mValves[2].update(mActor.getNode(mNode2a), mActor.getNode(mNode2b));
    
    for (int i=0; i<kNumValves; i++) {
        
        if ((ofGetFrameNum()+i*30) % 60 == 0) {
            if (mValves[i].state == false) {
                ofxOscMessage m;
                m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(6+i));
                m.addIntArg(0);
                mOscSender.sendMessage(m);
            }
        }
        
        if (mValves[i].stateChanged()) {
            ofxOscMessage m;
            m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(6+i));
            m.addIntArg(mValves[i].state);
            mOscSender.sendMessage(m);
        }
    }
}

void HakoniwaColorOfWater::draw()
{
    for (int i=0; i<kNumValves; i++) {
        mValves[i].draw(i, ofGetWidth()/4*(i+1), ofGetHeight()-150.f);
    }
}

void HakoniwaColorOfWater::exit()
{
    for (int i=0; i<kNumValves; i++) {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/"+ofToString(6+i));
        m.addIntArg(0);
        mOscSender.sendMessage(m);
    }
}

void HakoniwaColorOfWater::setupControlPanel()
{
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    ofxUIRadio *radio = NULL;
    ofxUISlider *slider = NULL;
    const float dim = 12.0f;
    
    const float thickness = 16.f;
    const float length = 300.f;
    
    panel->addWidgetDown(new ofxUISlider("Blink Open", 0.1f, 5.f, &mBlinkOpen, length, thickness));
    panel->addWidgetDown(new ofxUISlider("Blink Close", 0.1f, 5.f, &mBlinkClose, length, thickness));
    panel->addWidgetDown(new ofxUISpacer(0.f, 0.f, 840.f, 1.f));
    panel->addWidgetDown(new ofxUISlider("L Threshould", 40, 140, &mValves[0].threshould, length, thickness));
    panel->addWidgetDown(new ofxUISlider("C Threshould", 40, 140, &mValves[1].threshould, length, thickness));
    panel->addWidgetDown(new ofxUISlider("R Threshould", 40, 140, &mValves[2].threshould, length, thickness));
    panel->addWidgetDown(new ofxUISpacer(0.f, 0.f, 840.f, 1.f));
    
    radio = new ofxUIRadio("L Joint A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode0a)->setValue(true);
    panel->addWidgetDown(radio);
    
    radio = new ofxUIRadio("L Joint B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode0b)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("C Joint A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode1a)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("C Joint B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode1b)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("R Joint A", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode2a)->setValue(true);
    panel->addWidgetRight(radio);
    
    radio = new ofxUIRadio("R Joint B", ramActor::getJointNames(), OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    radio->getToggles().at(mNode2b)->setValue(true);
    panel->addWidgetRight(radio);
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &HakoniwaColorOfWater::onPanelChanged);
}

static int _getJointIdFromName(const string& name)
{
    const vector<string>& names = ramActor::getJointNames();
    int id = 0;
    for (int i=0; i<names.size(); i++) {
        if (names.at(i) == name) {
            id = i;
            break;
        }
    }
    return id;
}

void HakoniwaColorOfWater::onPanelChanged(ofxUIEventArgs& e)
{
    const string name = e.widget->getName();
    const string radioName = e.widget->getParent()->getName();
    
    if (radioName == "L JOINT A") {
        mNode0a = _getJointIdFromName(name);
    }
    else if (radioName == "L JOINT B") {
        mNode0b = _getJointIdFromName(name);
    }
    else if (radioName == "C JOINT A") {
        mNode1a = _getJointIdFromName(name);
    }
    else if (radioName == "C JOINT B") {
        mNode1b = _getJointIdFromName(name);
    }
    else if (radioName == "R JOINT A") {
        mNode2a = _getJointIdFromName(name);
    }
    else if (radioName == "R JOINT B") {
        mNode2b = _getJointIdFromName(name);
    }
    else if (name == "Blink Open") {
        for (int i=0; i<kNumValves; i++) {
            mValves[i].blinkOpen = mBlinkOpen;
        }
    }
    else if (name == "Blink Close") {
        for (int i=0; i<kNumValves; i++) {
            mValves[i].blinkOpen = mBlinkClose;
        }
    }
}

void HakoniwaColorOfWater::drawActor(const ramActor &actor)
{
    mActor = actor;
}

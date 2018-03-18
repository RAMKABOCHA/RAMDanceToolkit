//
//  FlickerControl2.h
//  RAMDanceToolkit
//
//  Created by David Haylock on 25/10/2017.
//

#ifndef FlickerControl2_h
#define FlickerControl2_h

#import "ofMain.h"
#import "ramMain.h"
#import "ofxMidi.h"

class FlickerControl2 : public rdtk::BaseScene
{
public:
    
    //--------------------------------------------------------------
    FlickerControl2() {
        
        name = "Flicker Control2";
        
        lowpassamount = 1.0;
        iVelocity = 0;
        midiOut.listPorts();
        midiOut.openPort(0);
        
        
    }
    
    //--------------------------------------------------------------
    ~FlickerControl2()
    {
        midiOut.closePort();
    }
    
    //--------------------------------------------------------------
    string getName() const
    {
        return name;
    }
    
    //--------------------------------------------------------------
    void drawImGui()
    {
//        ImGui::Checkbox("isDebug", &isDebug);
        ImGui::Checkbox("Constant Trigger", &constantTrigger);
        ImGui::SliderFloat("Low Pass", &lowpassamount,0.0,1.0);
        ImGui::SliderInt("Velocity", &iVelocity,0,127);
        
        
    }
    
    //--------------------------------------------------------------
    void drawActor(const rdtk::Actor &actor){
        
        const rdtk::NodeArray actor1 = lowPassFilter.update(actor);
//        if(isDebug){
//            rdtk::DrawBasicActor(actor1);
//
//            ofDrawBitmapString("Left Elbow: " + ofToString(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler()) ,10,300);
//            ofDrawBitmapString("Right Elbow: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler()) ,10,315);
//            ofDrawBitmapString("Left Knee: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler()) ,10,330);
//            ofDrawBitmapString("Right Knee: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler()) ,10,345);
//        }
//        int leftE = 0;
//        switch(activeOrientation[0])
//        {
//            case 0:
//            {
//                leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().x,
//                              rotationRangeLeftElbowX[0],rotationRangeLeftElbowX[1],
//                              0,9);
//            }
//                break;
//            case 1:
//            {
//                leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().y,
//                              rotationRangeLeftElbowY[0],rotationRangeLeftElbowY[1],
//                              0,9);
//            }
//                break;
//            case 2:
//            {
//                leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().z,
//                              rotationRangeLeftElbowZ[0],rotationRangeLeftElbowZ[1],
//                              0,9);
//            }
//                break;
//            default: break;
//        }
//
//        int rightE = 0;
//        switch(activeOrientation[1])
//        {
//            case 0:
//            {
//                rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().x,
//                               rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
//                               10,19);
//            }
//                break;
//            case 1:
//            {
//                rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().y,
//                               rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
//                               10,19);
//            }
//                break;
//            case 2:
//            {
//                rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().z,
//                               rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
//                               10,19);
//            }
//                break;
//            default:
//                break;
//        }
//
//        int leftK = 0;
//        switch(activeOrientation[2])
//        {
//            case 0:
//            {
//                leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().x,
//                              rotationRangeLeftKneeX[0],rotationRangeLeftKneeX[1],
//                              20,29);
//            }
//                break;
//            case 1:
//            {
//                leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().y,
//                              rotationRangeLeftKneeY[0],rotationRangeLeftKneeY[1],
//                              20,29);
//            }
//                break;
//            case 2:
//            {
//                leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().z,
//                              rotationRangeLeftKneeZ[0],rotationRangeLeftKneeZ[1],
//                              20,29);
//            }
//                break;
//            default: break;
//        }
//
//        int rightK = 0;
//        switch(activeOrientation[3])
//        {
//            case 0:
//            {
//                rightK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().x,
//                              rotationRangeRightKneeX[0],rotationRangeRightKneeX[1],
//                              30,39);
//            }
//                break;
//            case 1:
//            {
//                rightK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().y,
//                              rotationRangeRightKneeY[0],rotationRangeRightKneeY[1],
//                              30,39);
//            }
//                break;
//            case 2:
//            {
//                rightK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().z,
//                              rotationRangeRightKneeZ[0],rotationRangeRightElbowZ[1],
//                              30,39);
//            }
//                break;
//            default: break;
//        }
//
//        currentTrackId[0] = ofClamp(leftE,currentTrackIdRange1[0],currentTrackIdRange1[1]);//ofClamp(leftE,0,9);
//        currentTrackId[1] = ofClamp(rightE,currentTrackIdRange2[0],currentTrackIdRange2[1]);
//        currentTrackId[2] = ofClamp(leftK,currentTrackIdRange3[0],currentTrackIdRange3[1]);
//        currentTrackId[3] = ofClamp(rightK,currentTrackIdRange4[0],currentTrackIdRange4[1]);
//
//        if(constantTrigger) {
//            if(currentTrackId[0] != previousTrackId[0]) {
//                midiOut.sendNoteOn(1, currentTrackId[0],iVelocity);
//                previousTrackId[0] = currentTrackId[0];
//            }
//
//            if(currentTrackId[1] != previousTrackId[1]) {
//                midiOut.sendNoteOn(2, currentTrackId[1],iVelocity);
//                previousTrackId[1] = currentTrackId[1];
//            }
//
//            if(currentTrackId[2] != previousTrackId[2]) {
//                midiOut.sendNoteOn(3, currentTrackId[2],iVelocity);
//                previousTrackId[2] = currentTrackId[2];
//            }
//
//            if(currentTrackId[3] != previousTrackId[3]) {
//                midiOut.sendNoteOn(4, currentTrackId[3],iVelocity);
//                previousTrackId[3] = currentTrackId[3];
//            }
//
//            usleep(5);
//            midiOut.sendNoteOn(1,currentTrackId[0],0);
//            midiOut.sendNoteOn(2,currentTrackId[1],0);
//            midiOut.sendNoteOn(3,currentTrackId[2],0);
//            midiOut.sendNoteOn(4,currentTrackId[3],0);
//
//            float back = ofMap(actor.getNode(rdtk::Actor::JOINT_CHEST).getOrientationEuler().x,-180,180,10,127);
//            back = ofClamp(back,10,127);
//            midiOut.sendControlChange(2,10,back);
//        }
    }
    
    void draw()
    {
//        if(isDebug){
//            ofDrawBitmapString("Left Elbow Active Orientation: " + ofToString(activeOrientation[0]) ,10,100);
//            ofDrawBitmapString("Right Elbow Active Orientation: "+ ofToString(activeOrientation[1]) ,10,115);
//            ofDrawBitmapString("Left Knee Active Orientation: "+ ofToString(activeOrientation[2]) ,10,130);
//            ofDrawBitmapString("Right Knee Active Orientation: "+ ofToString(activeOrientation[3]) ,10,145);
//
//            ofDrawBitmapString("Left Elbow Track ID: " + ofToString(currentTrackId[0]) ,10,200);
//            ofDrawBitmapString("Right Elbow Track ID: "+ ofToString(currentTrackId[1]) ,10,215);
//            ofDrawBitmapString("Left Knee Track ID: "+ ofToString(currentTrackId[2]) ,10,230);
//            ofDrawBitmapString("Right Knee Track ID: "+ ofToString(currentTrackId[3]) ,10,245);
//        }
    }
    
    ofxMidiOut midiOut;
//    bool isDebug;
//    float startAngle;
//    float endAngle;
//    float speedWhichRotated;
//    
    int iVelocity;
//
//    int activeOrientation[4];
//    int currentTrackId[4];
//    int previousTrackId[4];
//    
//    int rotationRangeLeftElbowX[2];
//    int rotationRangeLeftElbowY[2];
//    int rotationRangeLeftElbowZ[2];
//    
//    int rotationRangeRightElbowX[2];
//    int rotationRangeRightElbowY[2];
//    int rotationRangeRightElbowZ[2];
//    
//    int rotationRangeLeftKneeX[2];
//    int rotationRangeLeftKneeY[2];
//    int rotationRangeLeftKneeZ[2];
//    
//    int rotationRangeRightKneeX[2];
//    int rotationRangeRightKneeY[2];
//    int rotationRangeRightKneeZ[2];
//    
//    int currentTrackIdRange1[2];
//    int currentTrackIdRange2[2];
//    int currentTrackIdRange3[2];
//    int currentTrackIdRange4[2];
    
    
    rdtk::LowPassFilter lowPassFilter;
    float lowpassamount;
    bool constantTrigger;
    
    string name;
    
};

#endif /* FlickerControl2_h */

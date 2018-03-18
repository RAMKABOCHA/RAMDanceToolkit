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
    FlickerControl2() :
    constantTrigger(false),
    fadeOut(0.1),
    isCoolDownConstant(100),
    minAcc(0.5){
        
        name = "Flicker Control2";
        
        lowpassamount = 1.0;
        iVelocity = 0;
        midiOut.listPorts();
        midiOut.openPort(0);
        
        accumAcceleration.clear();
        accumAcceleration.resize(rdtk::Actor::NUM_JOINTS);
        std::fill(accumAcceleration.begin(), accumAcceleration.end(), 0);
        
        isTriggered.clear();
        isTriggered.resize(rdtk::Actor::NUM_JOINTS);
        std::fill(isTriggered.begin(), isTriggered.end(), false);
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            
            mNodeVisibility[i] = false;
        }
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
        ImGui::SliderFloat("fadeOut", &fadeOut,0.0,1.0);
        ImGui::SliderFloat("minAcc", &minAcc,0.0,1.0);
        ImGui::SliderInt("isCoolDownConstant", &isCoolDownConstant,1,1000);
        if (ImGui::Button("Set All")) {
            for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            {
                mNodeVisibility[i] = true;
            }
        }
        if (ImGui::Button("Unset All")) {
            for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            {
                mNodeVisibility[i] = false;
            }
        }
        ImGui::Separator();
        ImGui::Columns(3, NULL, true);
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
        }
        ImGui::Columns(1, NULL, true);
        ImGui::Separator();
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            if(mNodeVisibility[i]){
                
                //                ImGui::Columns(1, NULL, true);
                //                ImGui::PushID(ofToString(i).c_str());
                //                ImGui::Text("%s value",rdtk::Actor::getJointName(i).c_str());
                ImGui::DragFloat((rdtk::Actor::getJointName(i)+" acc").c_str(), &accumAcceleration[i],0.01,0,1);
                
                //
                //
            }
        }
        ImGui::Columns(1, NULL, true);
    }
    
    //--------------------------------------------------------------
    void drawActor(const rdtk::Actor &actor){
        
        //        const rdtk::NodeArray actor1 = lowPassFilter.update(actor);
        for(int nodeID = 0 ; nodeID < rdtk::Actor::NUM_JOINTS ; nodeID++){
            const rdtk::Node &node = actor.getNode(nodeID);
            if(node.getAcceleration().length()>minAcc){
            
                accumAcceleration[nodeID] += abs(node.getAcceleration().length());
                
            
                
            }
        }
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
    }
    void update(){
        for(int nodeID = 0 ; nodeID < rdtk::Actor::NUM_JOINTS ; nodeID++){
            int midiNodeID = nodeID;
            if(mNodeVisibility[nodeID] && constantTrigger){
                
                if(accumAcceleration[nodeID]>1){
                    if(!isTriggered[nodeID]){
                        ofLogVerbose() << "nodeID%4 " << nodeID%4 << " midiNodeID " << midiNodeID;
                        midiOut.sendNoteOn(nodeID%4,midiNodeID,iVelocity);
                        isTriggered[nodeID] = true;
                        isCoolDown[nodeID] = isCoolDownConstant;
                    }
                }
//                else if(accumAcceleration[nodeID].y>1){
//                    if(!isTriggered[midiNodeID+1]){
//                        ofLogVerbose() << "nodeID%4 " << nodeID%4 << " midiNodeID " << midiNodeID+1;
//                        midiOut.sendNoteOn(nodeID%4,midiNodeID+1,iVelocity);
//                        isTriggered[midiNodeID+1] = true;
//                        isCoolDown[nodeID] = isCoolDownConstant;
//                    }
//                }
//                else if(accumAcceleration[nodeID].z>1){
//                    if(!isTriggered[midiNodeID+2]){
//                        ofLogVerbose() << "nodeID%4 " << nodeID%4 << " midiNodeID " << midiNodeID+2;
//                        midiOut.sendNoteOn(nodeID%4,midiNodeID+2,iVelocity);
//                        isTriggered[midiNodeID+2] = true;
//                        isCoolDown[nodeID] = isCoolDownConstant;
//                    }
//                }
            }
            isCoolDown[nodeID] = ofLerp(isCoolDown[nodeID], 0, fadeOut);
            if(isTriggered[nodeID] && accumAcceleration[nodeID] < 1 && isCoolDown[nodeID]<1){
                isTriggered[nodeID] = false;
                midiOut.sendNoteOff(nodeID%4,midiNodeID,0);
            }
//            if(isTriggered[midiNodeID+1] && accumAcceleration[nodeID].y < 1 && isCoolDown[nodeID]<1){
//                isTriggered[midiNodeID+1] = false;
//                midiOut.sendNoteOff(nodeID%4,midiNodeID+1,0);
//            }
//            if(isTriggered[midiNodeID+2] && accumAcceleration[nodeID].z < 1 && isCoolDown[nodeID]<1){
//                isTriggered[midiNodeID+2] = false;
//                midiOut.sendNoteOff(nodeID%4,midiNodeID+2,0);
//            }
            accumAcceleration[nodeID] = ofLerp(accumAcceleration[nodeID], 0, fadeOut);
//            accumAcceleration[nodeID].y = ofLerp(accumAcceleration[nodeID].y, 0, fadeOut);
//            accumAcceleration[nodeID].z = ofLerp(accumAcceleration[nodeID].z, 0, fadeOut);
            
            
        }
    }
    
    
    ofxMidiOut midiOut;
    //    bool isDebug;
    //    float startAngle;
    //    float endAngle;
    //    float speedWhichRotated;
    //
    int iVelocity;
    
    
    rdtk::LowPassFilter lowPassFilter;
    float lowpassamount;
    bool constantTrigger;
    float fadeOut;
    string name;
    
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    float isCoolDown[rdtk::Actor::NUM_JOINTS];
    int isCoolDownConstant;
    vector<bool> isTriggered;
    vector<float> accumAcceleration;
    float minAcc;
    
};

#endif /* FlickerControl2_h */

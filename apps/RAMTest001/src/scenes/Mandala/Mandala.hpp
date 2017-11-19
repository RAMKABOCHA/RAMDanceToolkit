//
//  Mandala.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 17/11/2017.
//


#pragma once
#include "ramMain.h"
#include "ofMain.h"
class Mandala : public rdtk::BaseScene{
public:
    
    
    void drawImGui(){
        //        ImGui::SliderFloat("threshold", &threshold, 0.0, 200.0);
        //        ImGui::SliderFloat("opacity", &opacity, 0.0, 1.0);
        //        ImGui::SliderInt("mode", &mode, 0, 2);
        //
        //        ImGui::SliderFloat("lineWidth", &lineWidth, 0.0, 10.0);
        
        ImGui::SliderInt("maxPoint", &maxPoint,300,5000);
        if(ImGui::SliderInt("step", &step,3,32)){
            reset();
        }
        ImGui::Checkbox("isDebug", &isDebug);
        ImGui::Checkbox("Constant Trigger", &constantTrigger);
        ImGui::SliderFloat("Low Pass", &lowpassamount,0.0,1.0);
        ImGui::SliderInt("Velocity", &iVelocity,0,127);
        ImGui::Text("Active X");
        ImGui::RadioButton("LeftE_X",&activeX, 0); ImGui::SameLine();
        ImGui::RadioButton("RightE_X",&activeX, 1); ImGui::SameLine();
        ImGui::RadioButton("LeftK_X",&activeX, 2); ImGui::SameLine();
        ImGui::RadioButton("RightK_X",&activeX, 3);
        
        ImGui::Text("Active Y");
        ImGui::RadioButton("LeftE_Y",&activeY, 0); ImGui::SameLine();
        ImGui::RadioButton("RightE_Y",&activeY, 1); ImGui::SameLine();
        ImGui::RadioButton("LeftK_Y",&activeY, 2); ImGui::SameLine();
        ImGui::RadioButton("RightK_Y",&activeY, 3);
        
        int activeY[4];
        ImGui::PushID("1");
        ImGui::Text("Left Elbow");
        ImGui::RadioButton("X",&activeOrientation[0], 0); ImGui::SameLine();
        ImGui::RadioButton("Y",&activeOrientation[0], 1); ImGui::SameLine();
        ImGui::RadioButton("Z",&activeOrientation[0], 2);
        ImGui::DragIntRange2("Rotation Range X", &rotationRangeLeftElbowX[0], &rotationRangeLeftElbowX[1]);
        ImGui::DragIntRange2("Rotation Range Y", &rotationRangeLeftElbowY[0], &rotationRangeLeftElbowY[1]);
        ImGui::DragIntRange2("Rotation Range Z", &rotationRangeLeftElbowZ[0], &rotationRangeLeftElbowZ[1]);
        ImGui::PopID();
        
        ImGui::Separator();
        
        ImGui::PushID("2");
        ImGui::Text("Right Elbow");
        ImGui::RadioButton("X",&activeOrientation[1], 0); ImGui::SameLine();
        ImGui::RadioButton("Y",&activeOrientation[1], 1); ImGui::SameLine();
        ImGui::RadioButton("Z",&activeOrientation[1], 2);
        ImGui::DragIntRange2("Rotation Range X", &rotationRangeRightElbowX[0], &rotationRangeRightElbowX[1]);
        ImGui::DragIntRange2("Rotation Range Y", &rotationRangeRightElbowY[0], &rotationRangeRightElbowY[1]);
        ImGui::DragIntRange2("Rotation Range Z", &rotationRangeRightElbowZ[0], &rotationRangeRightElbowZ[1]);
        ImGui::PopID();
        
        ImGui::Separator();
        
        ImGui::PushID("3");
        ImGui::Text("Left Knee");
        ImGui::RadioButton("X",&activeOrientation[2], 0); ImGui::SameLine();
        ImGui::RadioButton("Y",&activeOrientation[2], 1); ImGui::SameLine();
        ImGui::RadioButton("Z",&activeOrientation[2], 2);
        ImGui::DragIntRange2("Rotation Range X", &rotationRangeLeftKneeX[0], &rotationRangeLeftKneeX[1]);
        ImGui::DragIntRange2("Rotation Range Y", &rotationRangeLeftKneeY[0], &rotationRangeLeftKneeY[1]);
        ImGui::DragIntRange2("Rotation Range Z", &rotationRangeLeftKneeZ[0], &rotationRangeLeftKneeZ[1]);
        ImGui::PopID();
        
        ImGui::Separator();
        
        ImGui::PushID("4");
        ImGui::Text("Right Knee");
        ImGui::RadioButton("X",&activeOrientation[3], 0); ImGui::SameLine();
        ImGui::RadioButton("Y",&activeOrientation[3], 1); ImGui::SameLine();
        ImGui::RadioButton("Z",&activeOrientation[3], 2);
        ImGui::DragIntRange2("Rotation Range X", &rotationRangeRightKneeX[0], &rotationRangeRightKneeX[1]);
        ImGui::DragIntRange2("Rotation Range Y", &rotationRangeRightKneeY[0], &rotationRangeRightKneeY[1]);
        ImGui::DragIntRange2("Rotation Range Z", &rotationRangeRightKneeZ[0], &rotationRangeRightKneeZ[1]);
        ImGui::PopID();
    };
    void setup(){
        rotationRangeLeftElbowX[0] = -180;
        rotationRangeLeftElbowX[1] = 180;
        rotationRangeLeftElbowY[0] = -180;
        rotationRangeLeftElbowY[1] = 180;
        rotationRangeLeftElbowZ[0] = -180;
        rotationRangeLeftElbowZ[1] = 180;
        
        rotationRangeRightElbowX[0] = -180;
        rotationRangeRightElbowX[1] = 180;
        rotationRangeRightElbowY[0] = -180;
        rotationRangeRightElbowY[1] = 180;
        rotationRangeRightElbowZ[0] = -180;
        rotationRangeRightElbowZ[1] = 180;
        
        rotationRangeLeftKneeX[0] = -180;
        rotationRangeLeftKneeX[1] = 180;
        rotationRangeLeftKneeY[0] = -180;
        rotationRangeLeftKneeY[1] = 180;
        rotationRangeLeftKneeZ[0] = -180;
        rotationRangeLeftKneeZ[1] = 180;
        
        rotationRangeRightKneeX[0] = -180;
        rotationRangeRightKneeX[1] = 180;
        rotationRangeRightKneeY[0] = -180;
        rotationRangeRightKneeY[1] = 180;
        rotationRangeRightKneeZ[0] = -180;
        rotationRangeRightKneeZ[1] = 180;
        midX = ofGetWidth() *0.5;
        midY = ofGetHeight() *0.5;
        points.resize(step);
        maxPoint = 500;
        
    };
    void reset(){
        midX = ofGetWidth() *0.5;
        midY = ofGetHeight() *0.5;
        for(int i = 0 ; i < points.size(); i++){
            points[i].clear();
        }
        points.resize(step);
    }
    void update(){};
    void drawActor(const rdtk::Actor &actor){
        
        const rdtk::NodeArray actor1 = lowPassFilter.update(actor);
        if(isDebug){
            rdtk::DrawBasicActor(actor1);
            
            ofDrawBitmapString("Left Elbow: " + ofToString(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler()) ,10,300);
            ofDrawBitmapString("Right Elbow: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler()) ,10,315);
            ofDrawBitmapString("Left Knee: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler()) ,10,330);
            ofDrawBitmapString("Right Knee: "+ ofToString(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler()) ,10,345);
        }
        float leftE = 0;
        switch(activeOrientation[0])
        {
            case 0:
            {
                leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().x,
                              rotationRangeLeftElbowX[0],rotationRangeLeftElbowX[1],
                              0,1);
            }
                break;
            case 1:
            {
                leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().y,
                              rotationRangeLeftElbowY[0],rotationRangeLeftElbowY[1],
                              0,1);
            }
                break;
            case 2:
            {
                leftE = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_ELBOW).getOrientationEuler().z,
                              rotationRangeLeftElbowZ[0],rotationRangeLeftElbowZ[1],
                              0,1);
            }
                break;
            default: break;
        }
        
        float rightE = 0;
        switch(activeOrientation[1])
        {
            case 0:
            {
                rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().x,
                               rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
                               0,1);
            }
                break;
            case 1:
            {
                rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().y,
                               rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
                               0,1);
            }
                break;
            case 2:
            {
                rightE = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_ELBOW).getOrientationEuler().z,
                               rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
                               0,1);
            }
                break;
            default:
                break;
        }
        
        float leftK = 0;
        switch(activeOrientation[2])
        {
            case 0:
            {
                leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().x,
                              rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
                              0,1);
            }
                break;
            case 1:
            {
                leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().y,
                              rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
                              0,1);
            }
                break;
            case 2:
            {
                leftK = ofMap(actor.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getOrientationEuler().z,
                              rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
                              0,1);
            }
                break;
            default: break;
        }
        
        float rightK = 0;
        switch(activeOrientation[3])
        {
            case 0:
            {
                rightK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().x,
                               rotationRangeRightElbowX[0],rotationRangeRightElbowX[1],
                               0,1);
            }
                break;
            case 1:
            {
                rightK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().y,
                               rotationRangeRightElbowY[0],rotationRangeRightElbowY[1],
                               0,1);
            }
                break;
            case 2:
            {
                rightK = ofMap(actor.getNode(rdtk::Actor::JOINT_RIGHT_KNEE).getOrientationEuler().z,
                               rotationRangeRightElbowZ[0],rotationRangeRightElbowZ[1],
                               0,1);
            }
                break;
            default: break;
        }
        currentTrackId[0] = ofClamp(leftE,0,1);
        currentTrackId[1] = ofClamp(rightE,0,1);
        currentTrackId[2] = ofClamp(leftK,0,1);
        currentTrackId[3] = ofClamp(rightK,0,1);
    }
    void draw(){
        float x = currentTrackId[activeX]*ofGetWidth();
        float y = currentTrackId[activeY]*ofGetHeight();
        
        
        float angle = 360.0f / (step*1.0f) ;
        float radius = sqrt( pow(y - midY, 2)+pow(x - midX, 2) );
        float degree = RAD_TO_DEG* atan2f((x - midX),(y - midY));
        ofSetColor(255);
        
        for (int  i = 0; i < step; i++) {
            float rad = DEG_TO_RAD * (degree+(angle*i));
            float x2 = sin(rad)*radius;
            float y2 = cos(rad)*radius;
            
            //            ofDrawCircle(x2, y2, 10);
            points[i].push_back(ofPoint(x2,y2));
            if(points[i].size() > maxPoint){
                points[i].erase(points[i].begin());
            }
        }
        
        ofPushMatrix();
        ofTranslate(midX,midY);
        for(int i = 0 ; i < points.size(); i++){
            vector<ofPoint> pts = points[i];
            ofPolyline polyline(pts);
            
            polyline.draw();
        }
        ofPopMatrix();
        
        ofPushMatrix();
        
        ofTranslate(midX,midY);
        ofScale(1,-1,1);
        for(int i = 0 ; i < points.size(); i++){
            vector<ofPoint> pts = points[i];
            ofPolyline polyline(pts);
            
            polyline.draw();
        }
        ofPopMatrix();
        if(isDebug){
        ofDrawBitmapString("Left Elbow Active Orientation: " + ofToString(activeOrientation[0]) ,10,100);
        ofDrawBitmapString("Right Elbow Active Orientation: "+ ofToString(activeOrientation[1]) ,10,115);
        ofDrawBitmapString("Left Knee Active Orientation: "+ ofToString(activeOrientation[2]) ,10,130);
        ofDrawBitmapString("Right Knee Active Orientation: "+ ofToString(activeOrientation[3]) ,10,145);
        
        ofDrawBitmapString("Left Elbow Track ID: " + ofToString(currentTrackId[0]) ,10,200);
        ofDrawBitmapString("Right Elbow Track ID: "+ ofToString(currentTrackId[1]) ,10,215);
        ofDrawBitmapString("Left Knee Track ID: "+ ofToString(currentTrackId[2]) ,10,230);
        ofDrawBitmapString("Right Knee Track ID: "+ ofToString(currentTrackId[3]) ,10,245);
        }
    };
    
    string getName() const { return "Mandala"; }
    
    int iVelocity;
    int activeOrientation[4];
    float currentTrackId[4];
    int previousTrackId[4];
    
    int activeX;
    int activeY;
    
    
    int rotationRangeLeftElbowX[2];
    int rotationRangeLeftElbowY[2];
    int rotationRangeLeftElbowZ[2];
    
    int rotationRangeRightElbowX[2];
    int rotationRangeRightElbowY[2];
    int rotationRangeRightElbowZ[2];
    
    int rotationRangeLeftKneeX[2];
    int rotationRangeLeftKneeY[2];
    int rotationRangeLeftKneeZ[2];
    
    int rotationRangeRightKneeX[2];
    int rotationRangeRightKneeY[2];
    int rotationRangeRightKneeZ[2];
    
    rdtk::LowPassFilter lowPassFilter;
    float lowpassamount;
    bool constantTrigger;
    
    int mode = 0;
    float midX, midY;
    int step = 8;
    vector<vector<ofPoint>> points;
    int maxPoint;
    bool isDebug;
};

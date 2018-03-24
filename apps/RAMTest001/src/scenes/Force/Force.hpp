
//
//  Force.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 20/3/2018.
//

#ifndef Force_hpp
#define Force_hpp

#import "ofMain.h"
#import "ramMain.h"

class Force : public rdtk::BaseScene
{
public:
    
    //--------------------------------------------------------------
    Force():
    minAcc(50),
    alpha(1),
    maxElement(1000),
    maxWidth(0.5),
    circleRes(3),
    satuation(255),
    brightness(255)
    {
        name = "Force";
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mNodeVisibility[i] = false;
        }
    }
    
    //--------------------------------------------------------------
    ~Force(){
        
    }
    
    //--------------------------------------------------------------
    string getName() const {
        return name;
    }
    
    //--------------------------------------------------------------
    void drawImGui(){
        if (ImGui::Button("Reset")){reset();}
        if (ImGui::Button("Show All")){
            for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            {
                mNodeVisibility[i] = true;
            }
        }
        if (ImGui::Button("Hide All")) {
            for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            {
                mNodeVisibility[i] = false;
            }
        }
        ImGui::SliderInt("maxElement", &maxElement, 1, 2000);
        ImGui::SliderFloat("alpha", &alpha, 0, 255);
        ImGui::SliderFloat("maxWidth", &maxWidth, 0, 1);
        ImGui::SliderFloat("minAcc", &minAcc, 0, 100);
        ImGui::SliderInt("circleRes", &circleRes, 3, 36);
        ImGui::SliderInt("satuation", &satuation, 0, 255);
        ImGui::SliderInt("brightness", &brightness, 0, 255);
        ImGui::Columns(3, NULL, true);
        for (int i = 0;i < rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
            
        }
        ImGui::Columns(1);
    }
    
    //--------------------------------------------------------------
    void drawActor(const rdtk::Actor &actor){
        
       
    }
    
    void draw(){
        rdtk::BeginCamera();
        for (int i=0; i<getNumNodeArray(); i++)
        {
            rdtk::NodeArray &src = getNodeArray(i);
            
            for (int nodeId=0; nodeId<src.getNumNode(); nodeId++)
            {
                if (mNodeVisibility[nodeId] == false) continue;
                
                rdtk::Node &node = src.getNode(nodeId);
                ofVec3f acc = node.getAcceleration();
                if(acc.length() > minAcc){
                    sizes.push_back(acc.length());
                    startPos.push_back(node.getGlobalTransformMatrix());
                    int w = maxWidth * acc.length();
                    ofColor color = ofColor::fromHsb(w%360,satuation,brightness,alpha);
                    colors.push_back(color);
                }
            }
        }
        ofPushStyle();
        ofSetCircleResolution(circleRes);
        for(int i = 0 ; i < startPos.size(); i++){
            ofPushStyle();
            ofNoFill();
            int w = maxWidth * sizes[i];
            
            ofSetColor(colors[i]);
            ofPushMatrix();
            glMultMatrixf(startPos[i].getPtr());
            
            w = w < 100 ? w : 100;
            float wh = w*0.5;
            ofDrawCircle(-wh,-wh,w);
            ofPopMatrix();
            ofPopStyle();
        }
        ofPopStyle();
        
        rdtk::EndCamera();
    }
    void update(){
        while(sizes.size()>maxElement){
            sizes.erase(sizes.begin());
        }
        while(startPos.size()>maxElement){
            startPos.erase(startPos.begin());
        }
        while(colors.size()>maxElement){
            colors.erase(colors.begin());
        }
    }
    void reset(){
        while(sizes.size()>0){
            sizes.erase(sizes.begin());
        }
        while(startPos.size()>0){
            startPos.erase(startPos.begin());
        }
        while(colors.size()>0){
            colors.erase(colors.begin());
        }
    }
private:
    string name;
    float minAcc,maxWidth,alpha;
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    vector<ofMatrix4x4> startPos;
    vector<float> sizes;
    vector<ofColor>colors;
    int circleRes,maxElement,satuation,brightness;
};

#endif /* force_h */

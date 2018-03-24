
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

class Force2 : public rdtk::BaseScene
{
public:
    
    //--------------------------------------------------------------
    Force2():
    minAcc(50){
        name = "Force2";
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mNodeVisibility[i] = false;
        }
    }
    
    //--------------------------------------------------------------
    ~Force2(){
        
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
        ImGui::SliderFloat("alpha", &alpha, 0, 255);
        ImGui::SliderFloat("maxWidth", &maxWidth, 0, 1);
        ImGui::SliderFloat("minAcc", &minAcc, 0, 100);
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
                }
            }
        }
        
        for(int i = 0 ; i < startPos.size(); i++){
            ofPushStyle();
            ofNoFill();
            ofSetColor(255,255,255,alpha);
            ofPushMatrix();
            glMultMatrixf(startPos[i].getPtr());
            int w = maxWidth * sizes[i];
            w = w < 100 ? w : 100;
            float wh = w*0.5;
            ofDrawRectangle(-wh,-wh,w,w);
            ofPopMatrix();
            ofPopStyle();
        }
        
        rdtk::EndCamera();
    }
    void update(){
        while(sizes.size()>1000){
            sizes.erase(sizes.begin());
        }
        while(startPos.size()>1000){
            startPos.erase(startPos.begin());
        }
    }
    void reset(){
        while(sizes.size()>0){
            sizes.erase(sizes.begin());
        }
        while(startPos.size()>0){
            startPos.erase(startPos.begin());
        }
    }
private:
    string name;
    float minAcc,maxWidth,alpha;
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    vector<ofMatrix4x4> startPos;
    vector<float> sizes;
};

#endif /* force_h */

//
//  Moji.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 26/10/2017.
//
#pragma once

#include "ramMain.h"
#include "ofMain.h"


class Moji : public rdtk::BaseScene
{
    public:
    Moji(){
        font.load("FreeUniversal-Regular.ttf",24,true,true,true);
    }
    void selectPoints(int i0)
    {
        selectedJoint = i0;
        
        m4p1->setAllToggles(false);
        m4p1->getToggles()[i0]->setValue(true);
        
    }
    void setupControlPanel()
    {
#ifdef RAM_GUI_SYSTEM_OFXUI
        
        rdtk::GetGUI().addSlider("mMinDistance", 1.0f, 100.0f, &mMinDistance);
//        rdtk::GetGUI().addColorSelector("line color", &mLineColor);
//        rdtk::GetGUI().addSlider("Line width", 0.0, 10.0, &mBoxLineWidth);
//        rdtk::GetGUI().addSlider("Master box size", 0.0, 1000.0, &mMasterBoxSize);
//
//        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
//            rdtk::GetGUI().addSlider(rdtk::Actor::getJointName(i), 0.0, 1000.0, &mSizeArray.at(i));
        int matrixSize = 10;
        m4p1 = (ofxUIToggleMatrix*) rdtk::GetGUI().getCurrentUIContext()->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "4p.1"));
        m4p1->setAllowMultiple(false);
        
        ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &Moji::onPanelChanged);
        
#endif
    }
    
    void onPanelChanged(ofxUIEventArgs& e)
    {
        string name = e.widget->getName();
        
//        if (name == "Master box size")
//        {
//            for (int i=0; i<mSizeArray.size(); i++)
//                mSizeArray.at(i) = mMasterBoxSize;
//        }
    }
    
    void drawImGui()
    {
        
//        ImGui::Checkbox("Use single color", &mUseSingleColor);
//        ImGui::ColorEdit3("Line color", &mLineColor[0]);
        ImGui::DragFloat("mMinDistance", &mMinDistance, 0.1, 0.0, 100.0);
        if (ImGui::Button("Reset")) {
            while(nodes.size()>0){
                nodes.pop_back();
            }
        }
        
        
//        if (ImGui::DragFloat("Master box size", &mMasterBoxSize, 1, 0, 1000.0))
//        {
//            for (int i=0; i<mSizeArray.size(); i++)
//                mSizeArray.at(i) = mMasterBoxSize;
//        }
//
//        ImGui::Columns(2, NULL, true);
//        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
//        {
//            ImGui::DragFloat(rdtk::Actor::getJointName(i).c_str(), &mSizeArray.at(i), 1, 0, 1000);
//            ImGui::NextColumn();
//        }
        bool updated = false;
        for (int j = 0;j < rdtk::Actor::NUM_JOINTS;j++)
        {
            
            ImGui::PushID((rdtk::Actor::getJointName(j)).c_str());
            if (ImGui::RadioButton(rdtk::Actor::getJointName(j).c_str() , &selectedJoint, j)){
                updated = true;
            };
            ImGui::PopID();
            
        }
        ImGui::Columns(1);
        if (updated) selectPoints(selectedJoint);
    }
    int getChoice(ofxUIToggleMatrix* matrix) {
        vector<ofxUIToggle*> toggles = matrix->getToggles();
        for(int i = 0; i < toggles.size(); i++) {
            if(toggles[i]->getValue()) {
                return i;
            }
        }
        return 0;
    }
    
    void update(){
        for(int i=0; i<getNumNodeArray(); i++)
        {
            const rdtk::NodeArray &NA = getNodeArray(i);
            if(i==0){
            for (int j=0; j<NA.getNumNode(); j++){
                ofNode node = NA.getNode(j);
                if(j==selectedJoint){
                    if(nodes.size()==0){
                        nodes.push_front(node.getGlobalTransformMatrix());
                    }
                    else if(nodes.front().getTranslation().distance(node.getGlobalTransformMatrix().getTranslation())>mMinDistance){
                        nodes.push_front(node.getGlobalTransformMatrix());
                    }
                }
            }
            }
        }
//        for (deque<rdtk::Node> n: nodes) {
            while (nodes.size() > 1000){
                nodes.pop_back();
            }
//        }
    }
    void draw()
    {
        rdtk::BeginCamera();
        int index = nodes.size()-1;
//        for (deque<rdtk::Node> n: nodes) {
            for(int i = 0; i < nodes.size(); i++){
                ofMatrix4x4 node = nodes[i];
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                glPushMatrix();
                {
                    ofPushStyle();
                    ofNoFill();
                    
                    glEnable(GL_DEPTH_TEST);
                    
                    
                    glMultMatrixf(node.getPtr());
                    string text;
                    switch(index%6){
                        case 0:
                            text = "K";
                            break;
                        case 1:
                            text = "A";
                            break;
                        case 2:
                            text = "B";
                            break;
                        case 3:
                            text = "O";
                            break;
                        case 4:
                            text = "C";
                            break;
                        case 5:
                            text = "H";
                            break;
                        case 6:
                            text = "A";
                            break;
                        default:
                            text = "K";
                            break;
                    }
                    
                    
                    font.drawStringAsShapes(text,0,0);
                    
//                    node.endTransform();
                    
                    ofPopStyle();
                    index--;
                }
                glPopMatrix();
                glPopAttrib();
            }
            
//        }
        rdtk::EndCamera();
    }
    string getName() const { return "Moji"; }
private:
    ofTrueTypeFont font;
    deque<ofMatrix4x4> nodes;
    float mMinDistance;
    int selectedJoint;
    ofxUIToggleMatrix *m4p1;
};

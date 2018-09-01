//
// Expansion.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "QLabCommunication.h"
class Expansion : public rdtk::BaseScene
{
    
public:
    
    Expansion() :
    mShowName(true),
    mShowBox(false),
    mShowAxis(false),
    mShowLine(false),
    mExpasionRatio(1.5),
    mBoxSize(10.0),
    mBoxSizeRatio(5.0),
    speed(0.03),
    mFontSize(1.0),
    bFixCenter(true),
    mIs3d(false){
        font.load("Scrawlerz.otf",48);
        reset();
    }
    
    
    void drawImGui()
    {
        ImGui::Checkbox("Fixe Center", &bFixCenter);    ImGui::SameLine();
        ImGui::Checkbox("Show Joint name", &mShowName);	ImGui::SameLine();
        ImGui::Checkbox("Show Box", &mShowBox);
        ImGui::Checkbox("Show Axis", &mShowAxis);		ImGui::SameLine();
        ImGui::Checkbox("Show Line", &mShowLine);
        if(ImGui::Button("reset")){
            reset();
        }
        ImGui::ColorEdit3("Box Color", &mBoxColor[0]);
        
        ImGui::DragFloat("Expansion Ratio", &mExpasionRatio, 0.1, 1.0, 20.0);
        ImGui::DragFloat("Box size", &mBoxSize, 1.0, 3.0, 100.0);
        ImGui::DragFloat("BigBox ratio", &mBoxSizeRatio, 0.5, 2.0, 10.0);
        ImGui::DragFloat("Font Size", &mFontSize, 0.001, 0.0001, 1.0);
        
        ImGui::DragFloat("speed", &speed, 0.01, 0.001, 0.5);
        static bool boxSize = false;
        static bool showAll = false;
        if (ImGui::Checkbox("Toggle box size", &boxSize)) seteAllSizeBigger(boxSize);
        ImGui::SameLine();
        if (ImGui::Checkbox("Show All", &showAll)) setAllVisiblity(showAll);
        if (ImGui::Checkbox("3d", &mIs3d)) setAllVisiblity(false);
        
        ImGui::Columns(2, NULL, true);
        for (int i = 0;i < rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::PushID(ofToString(i).c_str());
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
            ImGui::Checkbox("Bigger", &mBiggerSize[i]);
            ImGui::NextColumn();
            ImGui::PopID();
        }
        ImGui::Columns(1);
    }
    
    void setupControlPanel()
    {
        
#ifdef RAM_GUI_SYSTEM_OFXUI
        
        ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
        
        panel->getRect()->width =500.0f;
        
        rdtk::GetGUI().addToggle("Show Joint name", &mShowName);
        rdtk::GetGUI().addToggle("Show Box", &mShowBox);
        rdtk::GetGUI().addToggle("Show Axis", &mShowAxis);
        rdtk::GetGUI().addToggle("Show Line", &mShowLine);
        rdtk::GetGUI().addColorSelector("Box Color", &mBoxColor);
        
        rdtk::GetGUI().addSlider("Expasion Ratio", 0.1, 20.0, &mExpasionRatio);
        rdtk::GetGUI().addSlider("Box size", 1.0, 100.0, &mBoxSize);
        rdtk::GetGUI().addSlider("Font size", 0.01, 1, &mFontSize);
        rdtk::GetGUI().addSlider("Big Box ratio", 2.0, 10.0, &mBoxSizeRatio);
        
        panel->addToggle("Toggle box size", false, 20, 20);
        panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        panel->addToggle("Show All", true, 20, 20);
        panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        
        for(int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mBiggerSize[i] = false;
            mNodeVisibility[i] = false;
            
            string name = (i<10 ? " " : "") + ofToString(i);
            ofxUIToggle *toggleSize = new ofxUIToggle("Size" + name, &mBiggerSize[i], 8, 8);
            panel->addWidgetDown(toggleSize);
            mToggleSize[i] = toggleSize;
            
            ofxUIToggle *toggleVisible = new ofxUIToggle(rdtk::Actor::getJointName(i), &mNodeVisibility[i], 8, 8);
            panel->addWidgetRight(toggleVisible);
            mToggleDraw[i] = toggleVisible;
            mNodeAlpha[i] = 0;
        }
        
        ofAddListener(panel->newGUIEvent, this, &Expansion::onValueChanged);
        
#endif
    }
    
    void onValueChanged(ofxUIEventArgs& e)
    {
        string name = e.widget->getName();
        
        if (name == "Expasion Ratio")
        {
            ofxUISlider *slider = (ofxUISlider *)e.widget;
            setExpasionRatio(slider->getValue());
        }
        
        if (name == "Show All")
        {
            ofxUIToggle *t = (ofxUIToggle *)e.widget;
            bool newValue = t->getValue();
            
            setAllVisiblity(newValue);
        }
        
        if (name == "Toggle box size")
        {
            ofxUIToggle *t = (ofxUIToggle *)e.widget;
            bool newValue = t->getValue();
            
            seteAllSizeBigger(newValue);
        }
    }
    
    vector<string> substrings(string filename){
        ofFile file;
        
        file.open(ofToDataPath(filename), ofFile::ReadWrite, false);
        ofBuffer buff = file.readToBuffer();
        return ofSplitString(buff.getText(),"\n");
    }
    
    void reset()
    {
        subjects.clear();
        vector<string> temp = substrings("subjects-expansion.txt");
        int n = temp.size();
        while(!temp.empty()){
            vector<string>::iterator it = temp.begin();
            subjects.push_back(*it);
            temp.erase(it);
        }
        
    }
    
    void draw()
    {
        vector<rdtk::NodeArray> expNAs = mExpansion.update(getAllNodeArrays());
        vector<rdtk::NodeArray> lpNAs = mLowpass.update(expNAs);
        
        rdtk::BeginCamera();
        for (int i=0; i<getNumNodeArray(); i++)
        {
            rdtk::NodeArray &src = getNodeArray(i);
            rdtk::NodeArray &processedNA = lpNAs.at(i);
            
            ofPushStyle();
            ofNoFill();
            if(bFixCenter){
                ofPushMatrix();
                ofTranslate(-src.getNode(rdtk::Actor::JOINT_CHEST).getGlobalPosition().x,0,
                            -src.getNode(rdtk::Actor::JOINT_CHEST).getGlobalPosition().z);
            }
            for (int nodeId=0; nodeId<processedNA.getNumNode(); nodeId++)
            {
                if (!mNodeVisibility[nodeId] && mNodeAlpha[nodeId] <= 0) continue;
                if(mNodeVisibility[nodeId]){
                    if(mNodeAlpha[nodeId]<1)mNodeAlpha[nodeId] += speed;
                }else{
                    if(mNodeAlpha[nodeId]>0)mNodeAlpha[nodeId] -= speed;
                }
                rdtk::Node &node = processedNA.getNode(nodeId);
                node.setName("node"+ofToString(nodeId));
                
                ofSetColor(255,255,255,255*mNodeAlpha[nodeId]);
                node.beginTransform();
                
                int boxSize = mBoxSize * (mBiggerSize[nodeId] ? mBoxSizeRatio : 1);
                
                if (mShowBox)
                {
                    ofSetColor(mBoxColor);
                    ofDrawBox(boxSize);
                }
                
                if (mShowAxis)
                {
                    ofDrawAxis(boxSize);
                }
                
                node.endTransform();
                
                if (mShowLine)
                {
                    ofSetColor(255,255,255,255*mNodeAlpha[nodeId]);
                    ofSetLineWidth(1);
                    ofDrawLine(src.getNode(nodeId), processedNA.getNode(nodeId));
                }
                
                
                if (mShowName)
                {
                    
                    ofSetColor(255,255,255,255*mNodeAlpha[nodeId]);
                    //                    node.drawNodeName(mBoxSize+20);
                    
                    
                    //                    ofTranslate(node.getGlobalPosition());
                    string s = subjects[nodeId%subjects.size()];
                    ofRectangle rect = font.getStringBoundingBox(s,0,0);
                    
                    if(mIs3d){
                        //-------------------------------3d
                        node.beginTransform();
                        ofScale(mFontSize,mFontSize,mFontSize);
                        font.drawString(s, rect.width * -0.5 * mFontSize, rect.height * -0.5 * mFontSize);
                        node.endTransform();
                        //---------------------------------
                    } else {
                        
                        //-------------------------------2d
                        ofPushMatrix();
                        ofTranslate(node.getGlobalPosition());
                        ofScale(mFontSize,mFontSize,mFontSize);
                        font.drawString(s, rect.width * -0.5 * mFontSize, rect.height * -0.5 * mFontSize);
                        ofPopMatrix();
                        //--------------------------------
                    }
                }
            }
            ofPopStyle();
            if(bFixCenter){
                ofPopMatrix();
            }
        }
        rdtk::EndCamera();
    }
    
    
    string getName() const { return "Expansion"; }
    
    
#pragma mark - 
    
    
    void setExpasionRatio(float ratio)
    {
        for (int i=0; i<mExpansion.getNumFilters(); i++)
        {
            mExpansion.getFilter(i).setExpand(ratio);
        }
    }
    
    void setAllVisiblity(bool b)
    {
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            mToggleDraw[i]->setValue(b);
    }
    
    void seteAllSizeBigger(bool b)
    {
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            mToggleSize[i]->setValue(b);
    }
    
    
private:
    
    ramFilterEach<rdtk::Expansion> mExpansion;
    ramFilterEach<rdtk::LowPassFilter> mLowpass;
    
    ofxUIToggle *mToggleDraw[rdtk::Actor::NUM_JOINTS];
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    float mNodeAlpha[rdtk::Actor::NUM_JOINTS];
    bool bFixCenter;
    
    ofxUIToggle *mToggleSize[rdtk::Actor::NUM_JOINTS];
    bool mBiggerSize[rdtk::Actor::NUM_JOINTS];
    
    bool mShowAxis;
    bool mShowBox;
    bool mShowName;
    bool mShowLine;
    bool mIs3d;
    float mExpasionRatio;
    float mBoxSize;
    float mFontSize;
    float mBoxSizeRatio;
    float speed;
    ofFloatColor mBoxColor;
    
    vector<string> subjects;
    ofTrueTypeFont font;
};


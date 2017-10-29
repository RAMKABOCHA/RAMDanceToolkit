//
//  Link.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 23/10/2017.
//


#pragma once
#include "ofxDelaunay.h"
class Link : public rdtk::BaseScene
{
//    vector<float> mSizeArray;
    bool selectedJoint[rdtk::Actor::NUM_JOINTS];
    float mBoxLineWidth;
    int numPoint;
//    float mMasterBoxSize;
    bool mUseSingleColor;
    bool needToReset;
    ofFloatColor mLineColor;
    ofxDelaunay triangulation;
    vector<ofVec3f>enternalNode;
    
public:
    
    Link() : mBoxLineWidth(2.0), mUseSingleColor(true), mLineColor(0.840, 1.000, 0.419)
    {
//        mSizeArray.clear();
//        mSizeArray.resize(rdtk::Actor::NUM_JOINTS);
//        selectedJoint.clear();
//        selectedJoint.resize(rdtk::Actor::NUM_JOINTS);
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            selectedJoint[i] = false;
//        for (int i=0; i<mSizeArray.size(); i++)
//            mSizeArray.at(i) = mMasterBoxSize;
        reset();
    }
    
    void setupControlPanel()
    {
#ifdef RAM_GUI_SYSTEM_OFXUI
        
        rdtk::GetGUI().addToggle("Use single color", &mUseSingleColor);
        rdtk::GetGUI().addColorSelector("line color", &mLineColor);
        rdtk::GetGUI().addSlider("Line width", 0.0, 10.0, &mBoxLineWidth);
//        rdtk::GetGUI().addSlider("Master box size", 0.0, 1000.0, &mMasterBoxSize);
        
//        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
//            rdtk::GetGUI().addSlider(rdtk::Actor::getJointName(i), 0.0, 1000.0, &mSizeArray.at(i));
//
        
        
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
        ImGui::Checkbox("Use single color", &mUseSingleColor);
        ImGui::ColorEdit3("Line color", &mLineColor[0]);
        ImGui::DragFloat("Line width", &mBoxLineWidth, 0.1, 0.0, 10.0);
//        if (ImGui::DragFloat("Master box size", &mMasterBoxSize, 1, 0, 1000.0))
//        {
//            for (int i=0; i<mSizeArray.size(); i++)
//                mSizeArray.at(i) = mMasterBoxSize;
//        }
        
        
        ImGui::Columns(2, NULL, true);
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &selectedJoint[i]);
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
        if (ImGui::Button("Reset")) needToReset = true;
        
    }
    void reset(){
        enternalNode.clear();
        for(int i = 
            0 ; i < 10 ; i++){
            
            enternalNode.push_back(ofVec3f(ofRandom(-300, 300),
                                           ofRandom(0, 300),
                                           ofRandom(-300, 300)));
        }
    }
    void update(){
        if(needToReset){
            
            reset();
        }
        triangulation.reset();
        for(int i=0; i<enternalNode.size(); i++)
        {
            triangulation.addPoint(enternalNode[i]);
        }
        for(int i=0; i<getNumNodeArray(); i++)
        {
            const rdtk::NodeArray &NA = getNodeArray(i);
            drawLine(NA);
        }
    }
    void draw()
    {
        rdtk::BeginCamera();
        
        ofNoFill();
        ofSetLineWidth(mBoxLineWidth);
        triangulation.draw();
        ofFill();
        rdtk::EndCamera();
    }
    void drawLine(const rdtk::NodeArray& NA)
    {
        for (int i=0; i<NA.getNumNode(); i++)
        {
            const rdtk::Node &node = NA.getNode(i);
            if(selectedJoint[i]){
                triangulation.addPoint(node);
            }
//            for (int j=0; j<NA.getNumNode(); j++)
//            {
//                if(i!=j ){
//
//                    const rdtk::Node &node = NA.getNode(j);
//                    const rdtk::Node &nodeI = NA.getNode(i);
//
//                    const int keyJoint = NA.isActor() ? rdtk::Actor::JOINT_HEAD : 0;
//
//
////                    float boxSize = (i==keyJoint) ? 6 : 3;
////                    float bigBoxSize = mSizeArray.at(i);
//
//                    glPushAttrib(GL_ALL_ATTRIB_BITS);
//                    glPushMatrix();
//                    {
//                        ofPushStyle();
//                        ofNoFill();
//
//                        glEnable(GL_DEPTH_TEST);
//
//                        /*!
//                         big box
//                         */
//                        if (mUseSingleColor)
//                        {
//                            ofSetColor(mLineColor);
//                        }
//                        else
//                        {
//                            if (i%3 == 0)
//                            {
//                                ofSetColor( rdtk::Color::BLUE_DEEP );
//                            }
//                            else if (i%3 == 1)
//                            {
//                                ofSetColor( rdtk::Color::BLUE_NORMAL );
//                            }
//                            else
//                            {
//                                ofSetColor( rdtk::Color::BLUE_LIGHT );
//                            }
//                        }
//
//                        ofSetLineWidth(mBoxLineWidth);
//
//                        ofDrawLine(node,nodeI);
////                        node.beginTransform();
////                        ofDrawBox(bigBoxSize);
////                        node.endTransform();
//
//                        ofPopStyle();
//                    }
//                    glPopMatrix();
//                    glPopAttrib();
//                }
//            }
        }
        triangulation.triangulate();
    }
    /*void drawBigBox(const rdtk::NodeArray& NA)
    {
        for (int i=0; i<NA.getNumNode(); i++)
        {
            const int keyJoint = NA.isActor() ? rdtk::Actor::JOINT_HEAD : 0;
            
            const rdtk::Node &node = NA.getNode(i);
//            float boxSize = (i==keyJoint) ? 6 : 3;
//            float bigBoxSize = mSizeArray.at(i);
            
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            {
                ofPushStyle();
                ofNoFill();
                
                glEnable(GL_DEPTH_TEST);
     
                if (mUseSingleColor)
                {
                    ofSetColor(mLineColor);
                }
                else
                {
                    if (i%3 == 0)
                    {
                        ofSetColor( rdtk::Color::BLUE_DEEP );
                    }
                    else if (i%3 == 1)
                    {
                        ofSetColor( rdtk::Color::BLUE_NORMAL );
                    }
                    else
                    {
                        ofSetColor( rdtk::Color::BLUE_LIGHT );
                    }
                }
                
                ofSetLineWidth(mBoxLineWidth);
                node.beginTransform();
                ofDrawBox(bigBoxSize);
                node.endTransform();
                
                ofPopStyle();
            }
            glPopMatrix();
            glPopAttrib();
        }
    }*/
    
    string getName() const { return "Link"; }
};


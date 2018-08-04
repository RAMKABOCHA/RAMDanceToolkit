//
//  Character.cpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//

#include "Character.h"




Character::Character(): minScale(0.00001), maxScale(1), selectedText(0){
    font.load("Constantia.ttf",24,true,true,true);
    setup();
    for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++){
        rangeOfMotion[i][0] = -180;
        rangeOfMotion[i][1] = 180;
    }
    
}

vector<string> substrings(string filename){
    ofFile file;
    
    file.open(ofToDataPath(filename), ofFile::ReadWrite, false);
    ofBuffer buff = file.readToBuffer();
    return ofSplitString(buff.getText(),"\n");
}
void Character::setup()
{
    reset();
    
    
}

void Character::reset()
{
    subjects = substrings("subjects.txt");
    
    
    
    for(int j = 0 ; j < subjects.size() ; j++){
        vector<string>substring1 ;
        for(int i = 0 ; i < subjects[j].size() ; i++){
            substring1.push_back(subjects[j].substr(i,1));
        }
        
        characterSet[j] = substring1;
    }
}


void Character::setupControlPanel(){
#ifdef RAM_GUI_SYSTEM_OFXUI
    ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &Character::onPanelChanged);
    
#endif
    
}
void Character::onPanelChanged(ofxUIEventArgs& e){
    
}
void Character::drawImGui(){
    if (ImGui::Button("Reset")) {
        reset();
    }
    ImGui::SliderFloat("maxScale", &maxScale, 0, 5);
    ImGui::SliderFloat("minScale", &minScale, 0, 5);
    ImGui::SliderInt("selectedText", &selectedText, 0, characterSet.size()-1);
    ImGui::LabelText("text ", "%s", subjects[selectedText].c_str());
    ImGui::Checkbox("isDebug", &isDebug);
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
            
            ImGui::DragIntRange2(rdtk::Actor::getJointName(i).c_str(), &rangeOfMotion[i][0], &rangeOfMotion[i][1]);
            
        }
    }
}
void Character::update(){
    
    
}
void Character::draw(){
    rdtk::BeginCamera();
    for(int j=0; j<getNumNodeArray(); j++)
    {
        {
            const rdtk::NodeArray &NA = getNodeArray(j);
            
            
            {
                vector<string> strings = characterSet[selectedText];
                
                
                
                
                //                float d = (j%2==0) ? NA.getNode(rdtk::Actor::JOINT_HEAD).getGlobalPosition().distance(NA.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getGlobalPosition()) :
                //                NA.getNode(rdtk::Actor::JOINT_RIGHT_SHOULDER).getGlobalPosition().distance(NA.getNode(rdtk::Actor::JOINT_LEFT_TOE).getGlobalPosition());
                float scale =  maxScale;//ofMap(d,0,100,maxScale,minScale);
                
                
                //                rdtk::Node nodeInterpolated (NA.getNode(rdtk::Actor::JOINT_LEFT_HAND));
                //                ofMatrix4x4 m1 = NA.getNode(rdtk::Actor::JOINT_LEFT_HAND).getGlobalTransformMatrix();
                //                ofMatrix4x4 m2 = NA.getNode(rdtk::Actor::JOINT_RIGHT_HAND).getGlobalTransformMatrix();
                //                ofQuaternion rot;
                //                rot.slerp(0.5, m1.getRotate(), m2.getRotate());
                //                nodeInterpolated.setGlobalOrientation(rot);
                //                ofVec3f p1 = m1.getTranslation(), p2 = m2.getTranslation();
                //                ofVec3f p = p1.getInterpolated(p2,0.5);
                //                nodeInterpolated.setGlobalPosition(p);
                
                {
                    
                    for (int i=0; i<NA.getNumNode(); i++)
                    {
                        if(mNodeVisibility[i]){
                            
                            const rdtk::Node &node = NA.getNode(i);
                            
                            glPushAttrib(GL_ALL_ATTRIB_BITS);
                            glPushMatrix();
                            {
                                ofPushStyle();
                                ofNoFill();
                                
                                glEnable(GL_DEPTH_TEST);
                                
                                ofPushMatrix();
                                ofTranslate(node.getGlobalTransformMatrix().getTranslation());
                                
                                ofScale(scale,scale,scale);
                                ofRotate(0,1,0,0);
                                ofRotate(0,0,1,0);
                                ofRotate(0,0,0,1);
                                int wave = NA.getNode(i).getOrientationEuler().y + NA.getNode(i).getOrientationEuler().x + NA.getNode(i).getOrientationEuler().z ;
                                //TODO fix it
                                int index = ofMap(wave,
                                             rangeOfMotion[i][0],rangeOfMotion[i][1],
                                             0,strings.size());
                                index = index < 0 ? 0 : index > strings.size() ? strings.size()-1 : index;
                                font.drawString(strings[index%strings.size()], 0, 0);
                                ofPopMatrix();
                                
                                
                                
                                ofPopStyle();
                            }
                            glPopMatrix();
                            glPopAttrib();
                            //
                        }
                    }
                }
            }
        }
    }
    rdtk::EndCamera();
    if(isDebug){
        for(int j=0; j<getNumNodeArray(); j++)
        {
            
            const rdtk::NodeArray &NA = getNodeArray(j);
            
            for (int i=0; i<NA.getNumNode(); i++)
            {
                if(mNodeVisibility[i]){
                    
                    
                    int wave = NA.getNode(i).getOrientationEuler().y + NA.getNode(i).getOrientationEuler().x + NA.getNode(i).getOrientationEuler().z ;
                    ofDrawBitmapString(NA.getNode(i).getName() + ofToString(wave) , 20, 20+(i*15));
                }
                
            }
            
        }
    }
}

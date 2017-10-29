//
//  Character.cpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//

#include "Character.h"




Character::Character(): minScale(0.00001), maxScale(1){
    font.load("FreeUniversal-Regular.ttf",24,true,true,true);
    setup();
//    isChacterOn.assign(4,false);
    //    characterSet[0]={};
    //    characterSet[1]={};
    //    characterSet[2]={};
    //    characterSet[3]={};
}

vector<string> substrings(string filename){
    ofFile file;
    
    file.open(ofToDataPath(filename), ofFile::ReadWrite, false);
    ofBuffer buff = file.readToBuffer();
    return ofSplitString(buff.getText(),"\n");
}
void Character::setup()
{
    
    subjects = substrings("subjects.txt");
    
    
    vector<string>substring1 ;
    for(int i = 0 ; i < subjects[0].size() ; i++){
        substring1.push_back(subjects[0].substr(i,1));
    }
    characterSet[0] = substring1;
    
    
    verbs = substrings("verbs.txt");
    vector<string>substring2 ;
    for(int i = 0 ; i < verbs[0].size() ; i++){
        substring2.push_back(verbs[0].substr(i,1));
    }
    characterSet[1] = substring2;
    
    objects = substrings("objects.txt");
    vector<string>substring3;
    for(int i = 0 ; i < objects[0].size() ; i++){
        substring3.push_back(objects[0].substr(i,1));
    }
    characterSet[2] = substring3;
    
    
}

void Character::reset()
{
    vector<string>substring1 ;
    int rndIdx = ofRandom(subjects.size());
    for(int i = 0 ; i < subjects[rndIdx].size() ; i++){
        substring1.push_back(subjects[rndIdx].substr(i,1));
    }
    characterSet[0] = substring1;
    
    vector<string>substring2 ;
    rndIdx = ofRandom(verbs.size());
    for(int i = 0 ; i < verbs[rndIdx].size() ; i++){
        substring2.push_back(verbs[rndIdx].substr(i,1));
    }
    characterSet[1] = substring2;
    
    vector<string>substring3;
    rndIdx = ofRandom(objects.size());
    for(int i = 0 ; i < objects[rndIdx].size() ; i++){
        substring3.push_back(objects[rndIdx].substr(i,1));
    }
    characterSet[2] = substring3;
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
    bool update = false;
    for(int i = 0 ; i < 4 ; i++){
        string name = "Character"+ofToString(i);
        if(ImGui::Checkbox(name.c_str(), &isChacterOn[i])){
            update = true;
        }
    }
    if(ImGui::Checkbox("handsOnly", &handsOnly)){
        
    }
    ImGui::SliderFloat("maxScale", &maxScale, 0, 5);
    ImGui::SliderFloat("minScale", &minScale, 0, 5);
    
}
void Character::update(){
    
    
}
void Character::draw(){
    rdtk::BeginCamera();
    for(int j=0; j<getNumNodeArray(); j++)
    {
        if(isChacterOn[j]){
            const rdtk::NodeArray &NA = getNodeArray(j);
            
            if(j<characterSet.size()){
                vector<string> strings = characterSet[j];
                
                
                int index = 0;
                
                float d = (j%2==0) ? NA.getNode(rdtk::Actor::JOINT_HEAD).getGlobalPosition().distance(NA.getNode(rdtk::Actor::JOINT_LEFT_KNEE).getGlobalPosition()) :
                NA.getNode(rdtk::Actor::JOINT_RIGHT_SHOULDER).getGlobalPosition().distance(NA.getNode(rdtk::Actor::JOINT_LEFT_TOE).getGlobalPosition());
                float scale =  ofMap(d,0,100,maxScale,minScale);
                
                
                rdtk::Node nodeInterpolated (NA.getNode(rdtk::Actor::JOINT_LEFT_HAND));
                ofMatrix4x4 m1 = NA.getNode(rdtk::Actor::JOINT_LEFT_HAND).getGlobalTransformMatrix();
                ofMatrix4x4 m2 = NA.getNode(rdtk::Actor::JOINT_RIGHT_HAND).getGlobalTransformMatrix();
                ofQuaternion rot;
                rot.slerp(0.5, m1.getRotate(), m2.getRotate());
                nodeInterpolated.setGlobalOrientation(rot);
                ofVec3f p1 = m1.getTranslation(), p2 = m2.getTranslation();
                ofVec3f p = p1.getInterpolated(p2,0.5);
                nodeInterpolated.setGlobalPosition(p);
                
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                glPushMatrix();
                {
                    ofPushStyle();
                    ofNoFill();
                    
                    glEnable(GL_DEPTH_TEST);
                    nodeInterpolated.beginTransform();
                    ofScale(scale,scale,scale);
                    font.drawStringAsShapes(strings[0], 0, 0);
                    
                    nodeInterpolated.endTransform();
                    
                    ofPopStyle();
                }
                glPopMatrix();
                glPopAttrib();
                
                if(!handsOnly){
                    
                    for (int i=0; i<NA.getNumNode(); i++)
                    {
                        if(i == rdtk::Actor::JOINT_HEAD ||
                           i == rdtk::Actor::JOINT_CHEST ||
                           i == rdtk::Actor::JOINT_ABDOMEN ||
                           i == rdtk::Actor::JOINT_HIPS ||
                           i == rdtk::Actor::JOINT_LEFT_SHOULDER ||
                           i == rdtk::Actor::JOINT_RIGHT_SHOULDER ||
                           i == rdtk::Actor::JOINT_LEFT_ELBOW ||
                           i == rdtk::Actor::JOINT_RIGHT_ELBOW ||
                           i == rdtk::Actor::JOINT_LEFT_WRIST ||
                           i == rdtk::Actor::JOINT_RIGHT_ELBOW ||
                           i == rdtk::Actor::JOINT_RIGHT_HAND ||
                           i == rdtk::Actor::JOINT_LEFT_HAND ||
                           i == rdtk::Actor::JOINT_LEFT_KNEE ||
                           i == rdtk::Actor::JOINT_RIGHT_KNEE ||
                           i == rdtk::Actor::JOINT_LEFT_ANKLE ||
                           i == rdtk::Actor::JOINT_RIGHT_ANKLE
                           ){
                            const rdtk::Node &node = NA.getNode(i);
                            
                            glPushAttrib(GL_ALL_ATTRIB_BITS);
                            glPushMatrix();
                            {
                                ofPushStyle();
                                ofNoFill();
                                
                                glEnable(GL_DEPTH_TEST);
                                node.beginTransform();
                                
                                ofScale(scale,scale,scale);
                                font.drawStringAsShapes(strings[index%strings.size()], 0, 0);
                                
                                node.endTransform();
                                
                                ofPopStyle();
                            }
                            glPopMatrix();
                            glPopAttrib();
                            index++;
                        }
                    }
                }
            }
        }
    }
    rdtk::EndCamera();
}

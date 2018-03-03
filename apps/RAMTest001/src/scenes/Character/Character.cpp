//
//  Character.cpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//

#include "Character.h"




Character::Character(): minScale(0.00001), maxScale(1), selectedText(0){
    font.load("FreeUniversal-Regular.ttf",24,true,true,true);
    setup();
//    isChacterOn.assign(4,false);
    //    characterSet[0]={};
    //    characterSet[1]={};
    //    characterSet[2]={};
    //    characterSet[3]={};
    
    mymap2 = {
        { rdtk::Actor::JOINT_LEFT_SHOULDER, rdtk::Actor::JOINT_LEFT_HIP},
        { rdtk::Actor::JOINT_LEFT_ELBOW, rdtk::Actor::JOINT_LEFT_KNEE },
        { rdtk::Actor::JOINT_LEFT_WRIST, rdtk::Actor::JOINT_LEFT_ANKLE },
        { rdtk::Actor::JOINT_LEFT_HAND, rdtk::Actor::JOINT_LEFT_TOE },
        { rdtk::Actor::JOINT_RIGHT_SHOULDER, rdtk::Actor::JOINT_RIGHT_HIP },
        { rdtk::Actor::JOINT_RIGHT_ELBOW, rdtk::Actor::JOINT_RIGHT_KNEE },
        { rdtk::Actor::JOINT_RIGHT_WRIST, rdtk::Actor::JOINT_RIGHT_ANKLE },
        { rdtk::Actor::JOINT_RIGHT_HAND, rdtk::Actor::JOINT_RIGHT_TOE } };
    mymap = {
        { rdtk::Actor::JOINT_LEFT_SHOULDER, rotationRangeLeftHipY },
        { rdtk::Actor::JOINT_LEFT_ELBOW, rotationRangeLeftKneeY },
        { rdtk::Actor::JOINT_LEFT_WRIST, rotationRangeLeftAnkleY },
        { rdtk::Actor::JOINT_LEFT_HAND, rotationRangeLeftToeY },
        { rdtk::Actor::JOINT_RIGHT_SHOULDER, rotationRangeRightHipY },
        { rdtk::Actor::JOINT_RIGHT_ELBOW, rotationRangeRightKneeY },
        { rdtk::Actor::JOINT_RIGHT_WRIST, rotationRangeRightAnkleY },
        { rdtk::Actor::JOINT_RIGHT_HAND, rotationRangeRightToeY } };
    
    for (auto& x: mymap) {
        x.second[0] = -180;
        x.second[1] = 180;
        std::cout << x.first << ": " << x.second << '\n';
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
//    bool update = false;
//    for(int i = 0 ; i < 4 ; i++){
//        string name = "Character"+ofToString(i);
//        if(ImGui::Checkbox(name.c_str(), &isChacterOn[i])){
//            update = true;
//        }
//    }
//    if(ImGui::Checkbox("handsOnly", &handsOnly)){
//
//    }
    ImGui::SliderFloat("maxScale", &maxScale, 0, 5);
    ImGui::SliderFloat("minScale", &minScale, 0, 5);
    ImGui::SliderInt("selectedText", &selectedText, 0, characterSet.size());
    
    ImGui::DragIntRange2("rotationRangeLeftHipY", &rotationRangeLeftHipY[0], &rotationRangeLeftHipY[1]);
    
    ImGui::DragIntRange2("rotationRangeLeftKneeY", &rotationRangeLeftKneeY[0], &rotationRangeLeftKneeY[1]);
    
    ImGui::DragIntRange2("rotationRangeLeftAnkleY", &rotationRangeLeftAnkleY[0], &rotationRangeLeftAnkleY[1]);
    
    ImGui::DragIntRange2("rotationRangeLeftToeY", &rotationRangeLeftToeY[0], &rotationRangeLeftToeY[1]);
    
    ImGui::DragIntRange2("rotationRangeRightHipY", &rotationRangeRightHipY[0], &rotationRangeRightHipY[1]);
    
    ImGui::DragIntRange2("rotationRangeRightKneeY", &rotationRangeRightKneeY[0], &rotationRangeRightKneeY[1]);
    
    ImGui::DragIntRange2("rotationRangeRightAnkleY", &rotationRangeRightAnkleY[0], &rotationRangeRightAnkleY[1]);
    
    ImGui::DragIntRange2("rotationRangeRightToeY", &rotationRangeRightToeY[0], &rotationRangeRightToeY[1]);

}
void Character::update(){
    
    
}
void Character::draw(){
    rdtk::BeginCamera();
    for(int j=0; j<getNumNodeArray(); j++)
    {
//        if(isChacterOn[j])
        {
            const rdtk::NodeArray &NA = getNodeArray(j);
            
//            if(j<characterSet.size())
            {
                vector<string> strings = characterSet[selectedText];
                
                
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
                
//                glPushAttrib(GL_ALL_ATTRIB_BITS);
//                glPushMatrix();
//                {
//                    ofPushStyle();
//                    ofNoFill();
//
//                    glEnable(GL_DEPTH_TEST);
//                    nodeInterpolated.beginTransform();
//                    ofScale(scale,scale,scale);
//                    font.drawStringAsShapes(strings[0], 0, 0);
//
//                    nodeInterpolated.endTransform();
//
//                    ofPopStyle();
//                }
//                glPopMatrix();
//                glPopAttrib();
                
                //if(!handsOnly)
                {
                    
                    for (int i=0; i<NA.getNumNode(); i++)
                    {
                        if(i == rdtk::Actor::JOINT_LEFT_SHOULDER ||
                           i == rdtk::Actor::JOINT_RIGHT_SHOULDER ||
                           i == rdtk::Actor::JOINT_LEFT_ELBOW ||
                           i == rdtk::Actor::JOINT_RIGHT_ELBOW ||
                           i == rdtk::Actor::JOINT_LEFT_WRIST ||
                           i == rdtk::Actor::JOINT_RIGHT_WRIST ||
                           i == rdtk::Actor::JOINT_RIGHT_HAND ||
                           i == rdtk::Actor::JOINT_LEFT_HAND
                           ){

                            const rdtk::Node &node = NA.getNode(i);
                            
                            glPushAttrib(GL_ALL_ATTRIB_BITS);
                            glPushMatrix();
                            {
                                ofPushStyle();
                                ofNoFill();
                                
                                glEnable(GL_DEPTH_TEST);
//                                node.beginTransform();
                                
//                                ofScale(scale,scale,scale);
//                                font.drawStringAsShapes(strings[index%strings.size()], 0, 0);
                                
//                                node.endTransform();
                                ofPushMatrix();
                                ofTranslate(node.getGlobalTransformMatrix().getTranslation());

                                ofScale(scale,scale,scale);
                                ofRotate(0,1,0,0);
                                ofRotate(0,0,1,0);
                                ofRotate(0,0,0,1);
                                int targetIndex = ofMap(NA.getNode(mymap2[i]).getOrientationEuler().y,
                                                        mymap[i][0],mymap[i][1],
                                                        0,strings.size());
                                font.drawString(strings[index%strings.size()], 0, 0);
                                ofPopMatrix();
                                

                                
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

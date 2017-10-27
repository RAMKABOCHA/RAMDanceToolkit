//
//  Character.cpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//

#include "Character.h"




Character::Character(){
    font.load("FreeUniversal-Regular.ttf",24,true,true,true);
    setup();
    
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
}
void Character::update(){
    
    
}
void Character::draw(){
    rdtk::BeginCamera();
    for(int j=0; j<getNumNodeArray(); j++)
    {
        const rdtk::NodeArray &NA = getNodeArray(j);
        
        if(j<characterSet.size()){
            vector<string> strings = characterSet[j];
            
            for (int i=0; i<NA.getNumNode(); i++)
            {
                const rdtk::Node &node = NA.getNode(i);
                
                glPushAttrib(GL_ALL_ATTRIB_BITS);
                glPushMatrix();
                {
                    ofPushStyle();
                    ofNoFill();
                    
                    glEnable(GL_DEPTH_TEST);
                    node.beginTransform();
                    font.drawStringAsShapes(strings[i%strings.size()], 0, 0);
                    
                    node.endTransform();
                    
                    ofPopStyle();
                }
                glPopMatrix();
                glPopAttrib();
            }
            
        }
    }
    rdtk::EndCamera();
}

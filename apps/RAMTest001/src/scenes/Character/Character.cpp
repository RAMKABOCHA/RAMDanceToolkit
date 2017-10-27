//
//  Character.cpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//

#include "Character.h"


Character::Character(){
    font.load("FreeUniversal-Regular.ttf",24,true,true,true);
}
void Character::setupControlPanel(){
#ifdef RAM_GUI_SYSTEM_OFXUI
    ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &Character::onPanelChanged);
    
#endif

}
void Character::onPanelChanged(ofxUIEventArgs& e){
    
}
void Character::drawImGui(){
    
}
void Character::update(){
    for(int i=0; i<getNumNodeArray(); i++)
    {
        const rdtk::NodeArray &NA = getNodeArray(i);
        
        for (int j=0; j<NA.getNumNode(); j++){
            const rdtk::Node &node = NA.getNode(i);
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            {
                ofPushStyle();
                ofNoFill();
                glEnable(GL_DEPTH_TEST);
                
                node.beginTransform();
                font.drawStringAsShapes("A", 0, 0);
                node.endTransform();
                ofPopStyle();
                
            }
            glPopMatrix();
            glPopAttrib();
        }
        
    }
    
}
void Character::draw(){
    
}

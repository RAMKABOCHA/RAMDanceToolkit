#pragma once

class Interp : public rdtk::BaseScene{
public:
    float value;
    
    
    void drawImGui(){
        ImGui::SliderFloat("value", &value, -1.0, 2.0);
    };
    void setup(){};
    void update(){};
    void draw(){
        // validate
        if (getNumNodeArray() != 2) return;
        
        rdtk::NodeArray actor1 = getNodeArray(0);
        rdtk::NodeArray actor2 = getNodeArray(1);
        rdtk::NodeArray na(actor1);
        
        int n = actor1.getNumNode();
        if (n != actor2.getNumNode()) return;
        
        for (int i = 0; i < n; i++) {
            
            ofMatrix4x4 m1 = actor1.getNode(i).getGlobalTransformMatrix();
            ofMatrix4x4 m2 = actor2.getNode(i).getGlobalTransformMatrix();
            
            ofQuaternion rot;
            rot.slerp(value, m1.getRotate(), m2.getRotate());
            
            ofNode& n = na.getNode(i);
            na.getNode(i).setGlobalOrientation(rot);
            
            ofVec3f p1 = m1.getTranslation(), p2 = m2.getTranslation();
            na.getNode(i).setGlobalPosition(p1.interpolate(p2,value));
        }
        rdtk::BeginCamera();
        ofPushStyle();
        ofSetColor(rdtk::Color::BLUE_DEEP);
        rdtk::DrawBasicActor(na);
        ofPopStyle();
        rdtk::EndCamera();
        
    };
    
    string getName() const { return "Interp"; }
};

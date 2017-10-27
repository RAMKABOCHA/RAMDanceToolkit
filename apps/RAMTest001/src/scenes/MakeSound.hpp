#pragma once

class MakeSound : public rdtk::BaseScene {
public:
    void drawImGui(){
        
        ImGui::Checkbox("draw line", &isDrawLine);
        ImGui::SliderFloat("volume", &volume, 0., 1.);
        ImGui::Checkbox("sound", &isSound);
        ImGui::SliderInt("frequency ratio", &frequency, 1, 10);
    };
    void setup(){
        handsline.assign(11, ofVec3f());
    };
    void update(){
        
        if (getNumNodeArray() <= 0) return;
        
        rdtk::NodeArray actor = getNodeArray(0);
        
        for (int i = 0; i < pointsIndex.size(); i++) {
            ofVec3f p = actor.getNode(pointsIndex[i]).getGlobalPosition();
            handsline[i] = p;
        }
        
        float least = handsline[0].y;
        float most = handsline[0].y;
        
        for (int i = 1; i < pointsIndex.size(); i++) {
            
            float value = handsline[i].y;
            if (least > value) least = value;
            if (most < value) most = value;
            
        }
        soundline.clear();
        for (int i = 0; i < pointsIndex.size(); i++) {
            
            float amp = most - least;
            float val = - 1. + (handsline[i].y - least) / 100. * 2.;
            soundline.curveTo(1. * i, val);
            
        }
        
        soundline = soundline.getResampledByCount(257);
        vnum = soundline.size();
        
    };
    
    void draw(){
        
        if (getNumNodeArray() <= 0) return;
        
        if (isDrawLine) {
            rdtk::BeginCamera();
            ofPushStyle();
            
            ofSetColor(128, 255, 200);
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i < pointsIndex.size(); i++) {
                glVertex3f(handsline[i].x, handsline[i].y, handsline[i].z);
            }
            glEnd();
            
            ofPopStyle();
            rdtk::EndCamera();
            
            ofPushMatrix();
            ofScale(100, -100);
            ofTranslate(0, -1);
            ofSetColor(255);
            soundline.draw();
            ofPopMatrix();
        }
    };
    
    void audioOut(float * output, int bufferSize, int nChannels){
        
        if (vnum == 0) return;
        if (!isSound) return;
        
        for (int i = 0; i < bufferSize; i++) {
            int index = (i * frequency) % vnum;
            
            output[i * nChannels] = soundline[index].y * volume;
            output[i * nChannels + 1] = soundline[index].y * volume;
            
            
        }
        
    };
    string getName() const { return "MakeSound"; }

private:
    vector<int> pointsIndex = {
        rdtk::Actor::JOINT_RIGHT_HAND,
        rdtk::Actor::JOINT_RIGHT_WRIST,
        rdtk::Actor::JOINT_RIGHT_ELBOW,
        rdtk::Actor::JOINT_RIGHT_SHOULDER,
        rdtk::Actor::JOINT_RIGHT_COLLAR,
        
        rdtk::Actor::JOINT_NECK,
        
        rdtk::Actor::JOINT_LEFT_COLLAR,
        rdtk::Actor::JOINT_LEFT_SHOULDER,
        rdtk::Actor::JOINT_LEFT_ELBOW,
        rdtk::Actor::JOINT_LEFT_WRIST,
        rdtk::Actor::JOINT_LEFT_HAND
    };
    vector<ofVec3f> handsline;
    ofPolyline soundline;
    int vnum = 0;
    bool isDrawLine = false;
    bool isSound = false;
    float volume = 0.1;
    int frequency = 1;
};

#pragma once
class MakeSound : public rdtk::BaseScene {
public:
    void drawImGui(){
        ImGui::Checkbox("draw line", &isDrawLine);
        ImGui::SliderFloat("volume", &volume, 0., 1.);
        ImGui::Checkbox("sound", &isSound);
        ImGui::SliderFloat("frequency ratio", &frequency, 0., 5.);
        ImGui::SliderFloat("phase", &phaseAdder, 0.01, 0.05);
        ImGui::Checkbox("head synth", &isHeadSynth);
    }
    void setup(){
        handsline.assign(11, ofVec3f());
        
    };
    void update(){
        
        if (getNumNodeArray() <= 0) return;
        
        for (int j = 0; j < getNumNodeArray(); j++) {
            rdtk::NodeArray actor = getNodeArray(j);
            
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
                
                float val = ofMap(handsline[i].y, least, most, -1., 1.);
                soundline.curveTo(1. * i, val);
                
            }
            
            soundline = soundline.getResampledByCount(128 + 1);
            vnum = soundline.size();
            
            if (vnum != 0) {
                int divNum = floor(256 / (frequency * 2));
                int nind = 0;
                bool isEven = true;
                int index = 0;
                
                for (int i = 0; i < 256; i++) {
                    
                    nind = (i % divNum) * frequency;
                    int num = (int)floor(i / divNum);
                    isEven = (num % 2) == 0;
                    
                    if (isEven) {
                        index = nind;
                    } else {
                        index = 127 - nind;
                    }
                    
                    soundBuffer[i] = soundline[index].y * volume;
                    
                }
            }
            
            if (phase > TWO_PI) phase -= TWO_PI;
            
            if (isHeadSynth) {
                ofVec3f p = actor.getNode(4).getGlobalPosition();
                frequency = ofMap(p.z, -300., 300., 0., 5.);
                phaseAdder = ofMap(p.x, -300., 300., 0.01, 0.05);
            }
        }
        
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
        
        while (phase > TWO_PI){
            phase -= TWO_PI;
        }
        
        for (int i = 0; i < bufferSize; i++) {
            output[i * nChannels] = soundBuffer[i] * sin(phase);
            output[i * nChannels + 1] = soundBuffer[i] * sin(phase);
            phase += phaseAdder;
        }
    };
    string getName() const { return "MakeSound"; }

private:
    vector<int> pointsIndex = {
        22, //rdtk::Actor::JOINT_RIGHT_HAND,
        21, //rdtk::Actor::JOINT_RIGHT_WRIST,
        20, //rdtk::Actor::JOINT_RIGHT_ELBOW,
        19, //rdtk::Actor::JOINT_RIGHT_SHOULDER,
        18, //rdtk::Actor::JOINT_RIGHT_COLLAR,
        
        3, //rdtk::Actor::JOINT_NECK,
        
        13, //rdtk::Actor::JOINT_LEFT_COLLAR,
        14, //rdtk::Actor::JOINT_LEFT_SHOULDER,
        15, //rdtk::Actor::JOINT_LEFT_ELBOW,
        16, //rdtk::Actor::JOINT_LEFT_WRIST,
        17  //rdtk::Actor::JOINT_LEFT_HAND
    };
    vector<ofVec3f> handsline;
    ofPolyline soundline;
    float soundBuffer[256];
    
    int vnum = 0;
    bool isDrawLine = false;
    bool isSound = false;
    bool isHeadSynth = false;
    float volume = 0.1;
    float frequency = 1.;
    float phase = 0.;
    float phaseAdder = 0.01;
};

#pragma once
#include "ofxStk.h"
#include "DrawSynth.h"
class MakeSound : public rdtk::BaseScene {
public:
    MakeSound(): attack(10),delay(200),sustain(1),release(500),gain(0.5) {
        
    }
    void drawImGui(){
        
        ImGui::Checkbox("draw line", &isDrawLine);
        ImGui::SliderFloat("Volume", &volume, 0., 1.);
        if (ImGui::Button("set wave to synth")) {
            StkFrames frames = createWaveTableFromDrawing();
            drawSynth.setWaveTable(frames);
        };
        bool update = false;
        
        if(ImGui::SliderFloat("attack(ms)",&attack,10,2000)){
            update = true;
        }
        if(ImGui::SliderFloat("decay(ms)",&decay,10,2000)){
            update = true;
        }
        if(ImGui::SliderFloat("sustain",&sustain,0,1)){
            update = true;
        }
        if(ImGui::SliderFloat("release(ms)",&release,10,2000)){
            update = true;
        }
        if(ImGui::SliderFloat("gain",&gain,0,1)){
            update = true;
        }
        if(update){
            drawSynth.setAttack(attack/1000.0);
            drawSynth.setDecay(decay/1000.0);
            drawSynth.setSustain(sustain);
            drawSynth.setRelease(release/1000.0);
            drawSynth.setGain(gain);
        }
    };
    void keyPressed(int key){
        // openframeworks calls keyPressed() even when key is heldDown
        // so we have check if key is already held down before calling noteOn
        if (key == 'a') {
            if (!aDown) {
                drawSynth.noteOn(NoteC, gain);
                aDown = true;
            }
        }else if (key == 's'){
            if (!sDown) {
                drawSynth.noteOn(NoteD, gain);
                sDown = true;
            }
        }else if (key == 'd'){
            if (!dDown) {
                drawSynth.noteOn(NoteE, gain);
                dDown = true;
            }
        }else if (key == 'f'){
            if (!fDown) {
                drawSynth.noteOn(NoteF, gain);
                fDown = true;
            }
        }else if (key == 'g'){
            if (!gDown) {
                drawSynth.noteOn(NoteG, gain);
                gDown = true;
            }
        }else if(key == 'h'){
            if (!hDown) {
                drawSynth.noteOn(NoteA, gain);
                hDown = true;
            }
        }else if (key == 'j'){
            if (!jDown) {
                drawSynth.noteOn(NoteB, gain);
                jDown = true;
            }
            
        }else if (key =='k'){
            if (!kDown) {
                drawSynth.noteOn(NoteC2, gain);
                kDown = true;
            }
        }else if (key == 'l'){
            if (!lDown) {
                drawSynth.noteOn(NoteD2, gain);
                lDown = true;
            }
        }
    }
    
    //--------------------------------------------------------------
    void keyReleased(int key){
        switch (key) {
            case 'a':
                aDown = false;
                break;
            case 's':
                sDown = false;
                break;
            case 'd':
                dDown = false;
                break;
            case 'f':
                fDown = false;
                break;
            case 'g':
                gDown = false;
                break;
            case 'h':
                hDown = false;
                break;
            case 'j':
                jDown = false;
                break;
            case 'k':
                kDown = false;
                break;
            case 'l':
                lDown = false;
            default:
                break;
        }
        
        // have to do this check to allow legato style playing
        if (noNotesDown()) {
            drawSynth.noteOff(.5);
        }
    }
    bool ofApp::noNotesDown() const{
        if (!(aDown || sDown || dDown || fDown || gDown || hDown || jDown || kDown || lDown)) {
            return true;
        }else{
            return false;
        }
    }
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
            float val = - 1. + (handsline[i].y - least) / amp * 2.;
            soundline.curveTo(1. * i, val);
            
        }
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
        for (int i = 0; i < bufferSize; i++) {
            float value = drawSynth.tick();
            output[2*i] = value;
            output[2*i+1] = value;
        }
//        if (vnum == 0) return;
//
//        for (int i = 0; i < bufferSize; i++) {
//            output[i * nChannels] = soundline[i % vnum].y * volume;
//            output[i * nChannels + 1] = soundline[i % vnum].y * volume;
//        }
        
    };
    string getName() const { return "MakeSound"; }
    stk::StkFrames createWaveTableFromDrawing(){
        int numberOfFrames = 5000;
        stk::StkFrames frames(numberOfFrames,1);
        for (int i = 0; i < numberOfFrames; i++) {
            float xValue = ofMap(i, 0, numberOfFrames-1, 0, pointsIndex.size());
            const vector<ofPoint> &points = soundline.getVertices();
            float lerpPercentage;
            float firstVal;
            float secondVal;
            float yValue;
            for (int j = 0; j < soundline.size()-1; j++) {
                firstVal = points[j].x;
                secondVal = points[j+1].x;
                if (xValue >= firstVal && xValue <=secondVal) {
                    lerpPercentage = (xValue-firstVal)/(secondVal - firstVal);
                    ofPoint newPoint = points[j].interpolated(points[j+1], lerpPercentage);
                    yValue = newPoint.y;
                    break;
                }
            }
            frames[i] = yValue;
        }
        return frames;
    }
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
    float volume = 0.1;
    
    float attack;
    float decay;
    float sustain;
    float release;
    float gain;
    
    DrawSynth drawSynth;
};

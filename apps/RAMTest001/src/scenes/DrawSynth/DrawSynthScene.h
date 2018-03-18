//
//  DrawSynth.h
//  RAM Dance Toolkit
//
//  Created by James Kong on 16/3/2018.
//

#pragma once

#include "ofMain.h"
#include "ofxTonic.h"
using namespace Tonic;
struct ParameterSlider{
    ControlParameter param;
    bool isActive;
    ParameterSlider(ControlParameter paramArg)
    :param(paramArg),
    isActive(false){}
};
class DrawSynthScene : public rdtk::BaseScene{
public:
    DrawSynthScene()
    {
        
        minRotation.clear();
        minRotation.resize(rdtk::Actor::NUM_JOINTS);
        
        maxRotation.clear();
        maxRotation.resize(rdtk::Actor::NUM_JOINTS);
        
        activeOrientation.clear();
        activeOrientation.resize(rdtk::Actor::NUM_JOINTS);
        
        for (int i=0; i< rdtk::Actor::NUM_JOINTS; i++){
            mNodeVisibility[i] = false;
            minRotation.at(i) = -180;
            maxRotation.at(i) = 180;
            activeOrientation.at(i) = 0;
        }
        
        sliderArea = ofRectangle(0, 0, ofGetWindowWidth() / 2, ofGetWindowHeight());
        ///////////////////////////////
        //      Audio Stuff
        ///////////////////////////////
        
        const int NUM_STEPS = 8;
        
        // synth paramters are like instance variables -- they're values you can set later, by
        // cally synth.setParameter()
        ControlGenerator bpm = synth.addParameter("tempo",100).min(50).max(300);
        ControlGenerator transpose = synth.addParameter("transpose", 0).min(-6).max(6);
        
        // ControlMetro generates a "trigger" message at a given bpm. We multiply it by four because we
        // want four 16th notes for every beat
        ControlGenerator metro = ControlMetro().bpm(4 * bpm);
        
        // ControlStepper increments a value every time it's triggered, and then starts at the beginning again
        // Here, we're using it to move forward in the sequence
        ControlGenerator step = ControlStepper().end(NUM_STEPS).trigger(metro);
        
        // ControlSwitcher holds a list of ControlGenerators, and routes whichever one the inputIndex is pointing
        // to to its output.
        ControlSwitcher pitches = ControlSwitcher().inputIndex(step);
        ControlSwitcher cutoffs = ControlSwitcher().inputIndex(step);
        ControlSwitcher glides = ControlSwitcher().inputIndex(step);
        
        // stick a bunch of random values into the pitch and cutoff lists
        for(int i = 0; i < NUM_STEPS; i++){
            ControlGenerator pitchForThisStep = synth.addParameter("step" + to_string(i) + "Pitch", randomFloat(10, 80)).min(10).max(80);
            pitches.addInput(pitchForThisStep);
            
            ControlGenerator cutoff = synth.addParameter("step" + to_string(i) + "Cutoff", 500).min(30).max(1500);
            cutoffs.addInput(cutoff);
            
            ControlGenerator glide = synth.addParameter("step" + to_string(i) + "Glide", 0).min(0).max(0.1);
            glides.addInput(glide);
            
        }
        
        // Define a scale according to steps in a 12-note octave. This is a pentatonic scale. Like using
        // just the black keys on a piano
        vector<float> scale;
        scale.push_back(0);
        scale.push_back(2);
        scale.push_back(3);
        scale.push_back(5);
        scale.push_back(7);
        scale.push_back(10);
        
        // ControlSnapToScale snaps a float value to the nearest scale value, no matter what octave its in
        ControlGenerator midiNote = transpose + ControlSnapToScale().setScale(scale).input(pitches);
        
        ControlGenerator frequencyInHertz = ControlMidiToFreq().input(midiNote);
        
        // now that we've done all that, we have a frequency signal that's changing 4x per beat
        Generator tone = RectWave().freq( frequencyInHertz.smoothed().length(glides) );
        
        // create an amplitude signal with an ADSR envelope, and scale it down a little so it's not scary loud
        Generator amplitude = ADSR(0.01, 0.1, 0,0).trigger(metro) * 0.3;
        
        // create a filter, and feed the cutoff sequence in to it
        LPF24 filter =  LPF24().cutoff(cutoffs).Q(0.1);
        filter.input(tone * amplitude);
        
        // rout the output of the filter to the synth's main output
        synth.setOutputGen( filter );
        
        // build a slider for each parameter
        vector<ControlParameter> synthParameters = synth.getParameters();
        for(int i = 0; i < synthParameters.size(); i++){
            sliders.push_back(ParameterSlider(synthParameters.at(i)));
            
            if(synthParameters.at(i).getDisplayName().find("Pitch") != string::npos){
                ofLogVerbose("synthParameters.at(i).getDisplayName() " + synthParameters.at(i).getDisplayName());
                pitchesSlider.push_back(ParameterSlider(synthParameters.at(i)));
            }
        }
        
        mPrevAngles.clear();
        mPrevAngles.resize(rdtk::Actor::NUM_JOINTS);
        std::fill(mPrevAngles.begin(), mPrevAngles.end(), ofVec3f(0.0));
    }
    void drawActor(const rdtk::Actor &actor)
    {
        int silderIndex = 0 ;
        for(int i = 0 ; i < rdtk::Actor::NUM_JOINTS ; i++){
            ofVec3f euler = actor.getNode(i).getOrientationEuler();
            
            
            
            const rdtk::Node &node = actor.getNode(i);
//            ofVec3f v0 = mPrevAngles[i];
            ofVec3f v1 = node.getGlobalOrientation().getEuler();
//            ofQuaternion q;
//            q.makeRotate(v0, v1);
//            float angle;
//            //ofVec3f v = q.getEuler();
//            ofVec3f v = (v0.cross(v1)).normalize();
//            q.getRotate(angle, v.x, v.y, v.z);
//
//            ofDrawBitmapString(actor.getJointName(i) + " angle " + ofToString(angle) ,10,130+i*20);
            
            if(mNodeVisibility[i]){
                if(silderIndex<pitchesSlider.size()){
                    float orientation = v1.x;
                    switch(activeOrientation.at(i)){
                        case 0:
                            orientation = v1.x;
                            break;
                        case 1:
                            orientation = v1.y;
                            break;
                        case 2:
                            orientation = v1.z;
                            break;
                        default:
                            orientation = v1.x;
                            break;
                    }
                    int value = floor(ofMap(orientation, minRotation.at(i), maxRotation.at(i), 1, 8))*10;
                    
                    pitchesSlider.at(silderIndex).param.value(value);
                    silderIndex++;
                }
            }
            // save for next frame
            mPrevAngles[i] = v1;
        }
    }
    void draw(){
        
        
        // create some sliders to control those parameters
        float sliderHeight = sliderArea.getHeight() / sliders.size();
        for(int i = 0; i< sliders.size(); i++){
            ofPoint sliderStart = ofPoint(sliderArea.getX(), sliderArea.getY() + i * sliderHeight);
            ParameterSlider& slider = sliders.at(i);
            float sliderPosition = (slider.param.getValue() - slider.param.getMin()) / (slider.param.getMax() - slider.param.getMin());
            ofSetColor(50, 50, 50);
            ofDrawRectangle(sliderStart.x, sliderStart.y, sliderArea.getWidth() * sliderPosition, sliderHeight - 3);
            ofSetColor(255, 255, 255);
            stringstream sliderLabel;
            sliderLabel << slider.param.getDisplayName()
            << ": "
            << slider.param.getValue();
            ofDrawBitmapString(sliderLabel.str(), sliderStart.x + 5, sliderStart.y + 20);
        }
    }
    void drawImGui()
    {
        
        ImGui::Columns(3, NULL, true);
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
        }
        
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            if(mNodeVisibility[i]){
                ImGui::Separator();
                ImGui::Columns(1, NULL, true);
                ImGui::PushID(ofToString(i).c_str());
                ImGui::Text("%s",rdtk::Actor::getJointName(i).c_str());
                ImGui::Columns(1, NULL, true);
                ImGui::RadioButton("X",&activeOrientation.at(i), 0); ImGui::SameLine();
                ImGui::RadioButton("Y",&activeOrientation.at(i), 1); ImGui::SameLine();
                ImGui::RadioButton("Z",&activeOrientation.at(i), 2);
                ImGui::Columns(2, NULL, true);
                ImGui::DragFloat((rdtk::Actor::getJointName(i)+ " min").c_str(), &minRotation.at(i), 1, -180, 180); ImGui::NextColumn();
                ImGui::DragFloat((rdtk::Actor::getJointName(i)+ " max").c_str(), &maxRotation.at(i), 1, -180, 180);
               ImGui::PopID();
            }
        }
        
        ImGui::Columns(1);
        
        vector<ControlParameter> synthParameters = synth.getParameters();
        ImGui::Separator();
        
        ImGui::Columns(3, NULL, true);
        
        for(int i = 0; i < synthParameters.size(); i++){
            ControlParameter slider = synthParameters.at(i);
            TonicFloat value = slider.getValue();
            ImGui::DragFloat(slider.getDisplayName().c_str(), &value, 1, slider.getMin(), slider.getMax());
            slider.value(value);
                ImGui::NextColumn();
        }
        ImGui::Columns(1, NULL, true);
        
        
    }
    
    void update(){
    }
    
    void audioOut(float * output, int bufferSize, int nChannels){
        if(isEnabled()){
            synth.fillBufferOfFloats(output, bufferSize, nChannels);
        }
    }
    vector<float> minRotation, maxRotation;
    vector<int>activeOrientation;
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    ofxTonicSynth synth;
    vector<ParameterSlider> sliders;
    vector<ParameterSlider> pitchesSlider;
    ofRectangle sliderArea;
    vector<ofVec3f> mPrevAngles;
private:
    string getName() const { return "DrawSynthScene"; }
};


//
//  Character.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 27/10/2017.
//


#pragma once

#include "ramMain.h"
#include "ofMain.h"


class Character : public rdtk::BaseScene
{
public:
    Character();
    void setup();
    void reset();
    void setupControlPanel();
    void onPanelChanged(ofxUIEventArgs& e);
    void drawImGui();
    void update();
    void draw();
    string getName() const { return "Character"; }
private:
    ofTrueTypeFont font;
    map<int,vector<string>> characterSet;
    vector<string> subjects,verbs,objects;
//    bool handsOnly;
    float maxScale,minScale;
//    bool isChacterOn[4] = {false,false,false,false};
    int selectedText;
    
//    int rotationRangeLeftHipY[2];
//    int rotationRangeLeftKneeY[2];
//    int rotationRangeLeftAnkleY[2];
//    int rotationRangeLeftToeY[2];
//
//    int rotationRangeRightHipY[2];
//    int rotationRangeRightKneeY[2];
//    int rotationRangeRightAnkleY[2];
//    int rotationRangeRightToeY[2];
    
//    std::map<int,int*> mymap;
//    std::map<int,int> mymap2;

    int rangeOfMotion[rdtk::Actor::NUM_JOINTS][2];
    rdtk::LowPassFilter lowPassFilter;
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    bool isDebug;

};

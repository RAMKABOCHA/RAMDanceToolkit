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
    void setupControlPanel();
    void onPanelChanged(ofxUIEventArgs& e);
    void drawImGui();
    void update();
    void draw();
    string getName() const { return "Moji"; }
private:
    ofTrueTypeFont font;
};

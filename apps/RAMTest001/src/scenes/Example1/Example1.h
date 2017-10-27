//
//  Example1.h
//  RAM Dance Toolkit
//
//  Created by James Kong on 22/10/2017.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// YCAM "RAM CAMP in Kyoto 2017, Kyoto Experiment 2017
#pragma once

#include "ramMain.h"
#include "ofxTwistedRibbon.h"

class Example1 : public rdtk::BaseScene
{
public:
    
    string getName() const { return "Example1"; }
    
    void setupControlPanel();
    void drawImGui();
    void setup();
    void update();
    void draw();
    void drawActor(const rdtk::Actor &actor);
    void drawRigid(rdtk::RigidBody &rigid);
    
    float mglobalThickness;
};

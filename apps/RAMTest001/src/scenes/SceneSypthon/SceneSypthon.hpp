//
//  SceneSypthon.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 25/11/2017.
//

//
//  Mandala.hpp
//  RAM Dance Toolkit
//
//  Created by James Kong on 17/11/2017.
//


#pragma once
#include "ramMain.h"
#include "ofMain.h"
#include "ofxSyphon.h"
class SceneSypthon : public rdtk::BaseScene{
public:
    
    ofxSyphonServer individualTextureSyphonServer;	
    
    void setup(){
        individualTextureSyphonServer.setName("mandala");
    };
    
    void draw(){
        
        individualTextureSyphonServer.publishScreen();
        
    };
    
    string getName() const { return "SceneSypthon"; }
    
    
};


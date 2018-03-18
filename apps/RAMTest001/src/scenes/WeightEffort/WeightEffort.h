//
//  DDA.h
//  RAMDanceToolkit
//

#pragma once
#include "ramMain.h"


class WeightEffort: public rdtk::BaseScene, public ofBaseSoundInput
{
public:    
    WeightEffort();
    void setup();
    void update();
    void draw();
    
    void drawBeat(const rdtk::NodeArray& NA);
    void drawImGui();
    
    const float computeWeightEffort(const rdtk::NodeArray& NA);
    string getName() const { return "WeightEffort"; }
    
    
    
private:
    bool mIsFirstFrame;
    int mFrameIndex;
    float mScaleY;
    
    vector<ofVec3f> mPrevAngles;
    vector<ofVec3f> mWEPoints;  // weight effort points for display the result as a line
    
    float 	mVolume;
    int mBufferSize;
    int mDrawWidth;
    vector<float> mWEAudioL, mWEAudioR;
    vector<ofPtr<ofSoundPlayer>>         players;
};

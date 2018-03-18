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
    
    const ofVec3f computeWeightEffort(const rdtk::NodeArray& NA);
    string getName() const { return "WeightEffort"; }
    
    void playAll();
    void stopAll();
    void pauseAll();
    
private:
    bool mIsFirstFrame;
    int mFrameIndex;
    float mScaleY;
    
    vector<float> accumAcceleration;
    
    vector<ofVec3f> mPrevAngles;
    vector<vector<ofVec3f>> mWEPoints;  // weight effort points for display the result as a line
    
    float 	mVolume;
    int mBufferSize;
    int mDrawWidth;
    bool playState;
    vector<float> mWEAudioL, mWEAudioR;
    vector<ofPtr<ofSoundPlayer>>         players;
    vector<float>         playerVolumes;
    float lerpValue,fadeOut;
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
};

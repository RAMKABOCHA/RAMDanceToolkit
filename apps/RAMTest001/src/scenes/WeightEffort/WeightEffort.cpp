//
//  DDA.cpp
//  RAMDanceToolkit
//

#include "WeightEffort.h"

WeightEffort::WeightEffort() :
mIsFirstFrame(false),
mFrameIndex(0),
mScaleY(0.3),
mBufferSize(512),
mDrawWidth(512),
mVolume(0.001),
lerpValue(0.9),
fadeOut(0.05)
{
    mPrevAngles.clear();
    mPrevAngles.resize(rdtk::Actor::NUM_JOINTS);
    std::fill(mPrevAngles.begin(), mPrevAngles.end(), ofVec3f(0.0));
    
    accumAcceleration.clear();
    accumAcceleration.resize(rdtk::Actor::NUM_JOINTS);
    std::fill(accumAcceleration.begin(), accumAcceleration.end(), 0);
    mWEPoints.clear();
    mWEPoints.resize(3);
    
    
    vector<string>files;
    files.push_back("1_dreambell.wav");
    files.push_back("2_elechamber.wav");
    files.push_back("3_glocken.wav");
    files.push_back("4_voices.wav");
    files.push_back("5_alto_flute.wav");
    
    playerVolumes.clear();
    //    playerVolumes.resize(rdtk::Actor::NUM_JOINTS);
    playerVolumes.resize(files.size());
    std::fill(playerVolumes.begin(), playerVolumes.end(), 0);
    
    for(int i = 0 ; i < files.size() ; i++){
        ofPtr<ofSoundPlayer> soundPlayer =  ofPtr<ofSoundPlayer>(new ofSoundPlayer());
        soundPlayer.get()->load(files.at(i));
        soundPlayer.get()->setLoop(true);
        soundPlayer.get()->setVolume(0.0);
        soundPlayer.get()->stop();
        soundPlayer.get()->setMultiPlay(false);
        players.push_back(soundPlayer);
    }
    for (int i=0; i< rdtk::Actor::NUM_JOINTS; i++){
        mNodeVisibility[i] = false;
    }
}

void WeightEffort::setup()
{
    mWEAudioL.assign(mBufferSize, 0.0);
    mWEAudioR.assign(mBufferSize, 0.0);
}

void WeightEffort::drawImGui()
{
    ImGui::Columns(3, NULL, true);
    for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
    {
        ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
        ImGui::NextColumn();
    }
    ImGui::Columns(1, NULL, true);
    ImGui::Separator();
    ImGui::Columns(3, NULL, true);
    for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
    {
        ImGui::DragFloat((rdtk::Actor::getJointName(i)+" weight").c_str(), &accumAcceleration[i],0.01,0,1);
        ImGui::NextColumn();
    }
    
    
    ImGui::Columns(1, NULL, true);
    if (ImGui::Button("Set All")) {
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mNodeVisibility[i] = true;
        }
    }
    if (ImGui::Button("Unset All")) {
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mNodeVisibility[i] = false;
        }
    }
    if (ImGui::Button("Play All")) playAll();
    if (ImGui::Button("Stop All")) stopAll();
    if (ImGui::Button("Pause All")) pauseAll();
    ImGui::DragInt("Buffer size", &mBufferSize, 10, 0.0, 10000.0);
    ImGui::DragFloat("lerpValue", &lerpValue, 0.01, 0.0, 1);
    ImGui::DragFloat("fadeOut", &fadeOut, 0.01, 0.0, 1);
    ImGui::DragFloat("ScaleY", &mScaleY, 0.1, 0.0, 100.0);
    for(int i = 0 ; i < playerVolumes.size() ; i++){
        if(ImGui::DragFloat(("Volume "+ofToString(i)).c_str(), &playerVolumes[i], 0.00001, 0.0, 2.0)){
            
            players.at(i).get()->setVolume(playerVolumes[i]);
        }
    }
    
    
    
}
void WeightEffort::playAll(){
    for(int i = 0 ; i < players.size() ; i++){
        players.at(i).get()->play();
        players.at(i).get()->setPaused(false);
    }
}
void WeightEffort::stopAll(){
    for(int i = 0 ; i < players.size() ; i++){
        players.at(i).get()->stop();
    }
}
void WeightEffort::pauseAll(){
    for(int i = 0 ; i < players.size() ; i++){
        players.at(i).get()->setPaused(true);
    }
}
void WeightEffort::update()
{
    if(mIsFirstFrame)
        mFrameIndex++;
    int playerIndex = 0;
    for(int i = 0 ; i < mWEPoints.size() ; i++){
        if(mWEPoints[i].size() > mDrawWidth)
        {
            
            if(mNodeVisibility[i]){
                
            }
            mWEPoints[i].erase(mWEPoints[i].begin());
        }
    }
}

void WeightEffort::draw()
{
    ofPushStyle();
    ofSetColor(255);
    
    rdtk::BeginCamera();
    
    for(int i=0; i<getNumNodeArray(); i++)
    {
        const rdtk::NodeArray &NA = getNodeArray(i);
        drawBeat(NA);
    }
    
    rdtk::EndCamera();
    ofPopStyle();
}

void WeightEffort::drawBeat(const rdtk::NodeArray& NA)
{
    ofPushMatrix();
    ofTranslate(-mFrameIndex + 300, 0, 0);
    
    ofVec3f WEValue = computeWeightEffort(NA);
    
    // ignore sometimes framerate is faster than captured data
    if(WEValue != ofVec3f::zero())
    {
        ofVec3f pos1 = ofVec3f(mFrameIndex, WEValue.x, 50);
        ofVec3f pos2 = ofVec3f(mFrameIndex, WEValue.y, 100);
        ofVec3f pos3 = ofVec3f(mFrameIndex, WEValue.z, 150);
        
        
        mWEPoints[0].push_back(pos1);
        mWEPoints[1].push_back(pos2);
        mWEPoints[2].push_back(pos3);
        
        
        float totalSelected = 0;
        for(int nodeID = 0; nodeID< rdtk::Actor::NUM_JOINTS; nodeID++){
            if(mNodeVisibility[nodeID]){
                
                if(totalSelected<players.size()){
                    float volume = ofLerp(playerVolumes[totalSelected],accumAcceleration[nodeID], lerpValue);
                    playerVolumes[totalSelected] = volume > 1 ? 1 : (volume < 0) ? 0 : volume ;
                    players.at(totalSelected).get()->setVolume(playerVolumes[totalSelected]);
                    accumAcceleration[nodeID] = ofLerp(accumAcceleration[nodeID], 0, fadeOut);
                    totalSelected++;
                }
                
            }
            
        }
        
    
    }
    
    for(int i = 0; i<mWEPoints.size(); i++){
        
        for(int j = 1; j<mWEPoints[i].size(); j++){
            ofVec3f p0 = mWEPoints[i][j-1];
            ofVec3f p1 = mWEPoints[i][j];
            p0.y *= mScaleY;
            p1.y *= mScaleY;
            ofDrawLine(p0, p1);
        }
    }
    
    ofPopMatrix();
}

const ofVec3f WeightEffort::computeWeightEffort(const rdtk::NodeArray& NA)
{
    ofVec3f totalWE = ofVec3f::zero();
    
    if(!mIsFirstFrame)
    {
        for (int i=0; i<NA.getNumNode(); i++)
        {
            const rdtk::Node &node = NA.getNode(i);
            mPrevAngles[i] = node.getGlobalOrientation().getEuler();
        }
        mIsFirstFrame = true;
    }else{
        int playerIndex = 1;
        for (int i=0; i<NA.getNumNode(); i++)
        {
            if(mNodeVisibility[i]){
                const rdtk::Node &node = NA.getNode(i);
                
                ofVec3f v0 = mPrevAngles[i];
                ofVec3f v1 = node.getGlobalOrientation().getEuler();
//                ofQuaternion q;
//                q.makeRotate(v0, v1);
                
//
                ofVec3f v = (v0.cross(v1)).normalize();
//                q.getRotate(angle, v.x, v.y, v.z);
                
                ofVec3f v2 = ofVec3f(abs(v.x), abs(v.y), abs(v.z));
                totalWE += v2;
                if(accumAcceleration[i] < 1 ) accumAcceleration[i] += node.getAcceleration().length();
                mPrevAngles[i] = v1;
                playerIndex ++;
            }
        }
        totalWE/=playerIndex;
    }
    return totalWE;
}



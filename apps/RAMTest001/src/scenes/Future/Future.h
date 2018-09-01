// 
// Future.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
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

#pragma once

#include "ParticleEngine.h"
#include "Sound3d.h"
#include <cmath>
#include "QLabCommunication.h"
class Future : public rdtk::BaseScene, public QLabCommunication
{
	
	ramFilterEach<rdtk::Ghost> ghostFilters;
	ramFilterEach<rdtk::LowPassFilter> lowPassFilters;
	float mNodeAlpha[rdtk::Actor::NUM_JOINTS];
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    bool mSoundVisibility[rdtk::Actor::NUM_JOINTS];
    ofVec3f mVelocitySmoothed[rdtk::Actor::NUM_JOINTS];
	float speed, distance, smooth;
    bool customDraw, drawLine;
    ofTrueTypeFont font;
    vector<string> subjects;
    bool bFixCenter;
    float decay;
    float mFontSize;
    vector<ofPtr<Sound3D>> players;
    float *trackVolume;
    float rangeOfMotion1[2];
    float rangeOfMotion2[2];
    ofVec3f lvelocity, lforward, lup, lposition, svelocity;
public:
	
    bool draw_line;
	
	Future() :
    distance(150),
    speed(27),
    draw_line(false),
    bFixCenter(true),
    mFontSize(1.0),
    decay(0.03),
    drawLine(false),
    smooth(35){
//        font.load("FreeUniversal-Regular.ttf",24,true,false,true);
        font.load("Scrawlerz.otf",24,true,false,true);
        customDraw = true;
        vector<string> temp = substrings("subjects-future.txt");
        int n = temp.size();
        while(!temp.empty()){
            vector<string>::iterator it = temp.begin();
            subjects.push_back(*it);
            temp.erase(it);
        }
        for(int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mNodeVisibility[i] = false;
            mSoundVisibility[i] = false;
            mNodeAlpha[i] = 0;
            mVelocitySmoothed[i] = ofVec3f::zero();
        }
        rangeOfMotion1[0] = 0.4;
        rangeOfMotion1[1] = 1.0;
        
        rangeOfMotion2[0] = 122;
        rangeOfMotion2[1] = 200;
        
        lvelocity.set(1, 1, 1);
        lup.set(0, 1, 0);
        lforward.set(0, 0, 1);
        lposition.set(0, 0, 0);
        svelocity.set(1, 1, 1);
        
        ofDirectory dir;
        dir.allowExt("m4a");
        dir.allowExt("wav");
        dir.sort();
        int nFile =  dir.listDir("./bubbles");
        trackVolume = new float[nFile];
        for(int i = 0 ;i < nFile ; i++){
            ofPtr<Sound3D> player = ofPtr<Sound3D>(new Sound3D());
            player.get()->loadSound(dir.getPath(i));
            player.get()->setVolume(1);
            player.get()->setPosition(ofRandomf());
            player.get()->setMultiPlay(true);
            player.get()->setPaused(true);
            players.push_back(player);
            trackVolume[i] = 1;
            
        }
        
    }
    
    vector<string> substrings(string filename){
        ofFile file;
        
        file.open(ofToDataPath(filename), ofFile::ReadWrite, false);
        ofBuffer buff = file.readToBuffer();
        return ofSplitString(buff.getText(),"\n");
    }
	
	struct Preset
	{
		rdtk::Ghost *self;
		float distance;
		float speed;
		
		Preset(rdtk::Ghost *self, float distance, float speed) : self(self), distance(distance), speed(speed) {}
		void operator()()
		{
			self->setSpeed(speed);
			self->setDistance(distance);
		}
	};
    void setup()
    {
        
        QLabCommunication::setup(QLabCommunication::RAM_OSC_ADDR_COMMUNICATE_QLAB+getName());
    }
	void setupControlPanel()
	{
		
		rdtk::GetGUI().addToggle("Draw line from actor to ghost", &draw_line);
		
		ofAddListener(rdtk::GetGUI().addButton("Speed: Ghost"), this, &Future::onPresetGhost);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Slow"), this, &Future::onPresetSlow);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Normal"), this, &Future::onPresetNormal);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Fast"), this, &Future::onPresetFast);
        ofAddListener(rdtk::GetGUI().addButton("Speed: Zero"), this, &Future::onPresetZero);
		
		rdtk::GetGUI().addSlider("Distance", 0.0, 400, &distance);
		rdtk::GetGUI().addSlider("Speed", 0.0, 255.0, &speed);
        rdtk::GetGUI().addSlider("decay", 0.001, 0.5, &decay);
		
		ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &Future::onValueChanged);
	}
	
	void onValueChanged(ofxUIEventArgs &e)
	{
		updateFilters();
	}
	
	void drawImGui()
	{
		ofEventArgs dummy;
        ImGui::Checkbox("Fixe Center", &bFixCenter);    ImGui::SameLine();
		if (ImGui::Checkbox("Draw line from actor to ghost", &draw_line)) updateFilters();
        ImGui::Checkbox("Toggle CustomDraw", &customDraw);
        ImGui::Checkbox("Toggle drawLine", &drawLine);
        static bool showAll = false;
        if (ImGui::Checkbox("Show All", &showAll)) setAllVisiblity(showAll);
        ImGui::DragFloat("Font Size", &mFontSize, 0.001, 0.0001, 1.0);
		if (ImGui::Button("speed: Ghost"))
		{
			onPresetGhost(dummy);
			updateFilters();
		}
		if (ImGui::Button("speed: Slow"))
		{
			onPresetSlow(dummy);
			updateFilters();
		}
		if (ImGui::Button("speed: Normal"))
		{
			onPresetNormal(dummy);
			updateFilters();
		}
		if (ImGui::Button("speed: Fast"))
		{
			onPresetFast(dummy);
			updateFilters();
		}
        if (ImGui::Button("speed: Zero"))
        {
            onPresetZero(dummy);
            updateFilters();
        }
		if (ImGui::DragFloat("Distance", &distance, 1, 0, 400)) updateFilters();
		if (ImGui::DragFloat("Speed", &speed, 1, 0, 255)) updateFilters();
        ImGui::DragFloat("Smooth", &smooth, 1.0, 1.0, 50.0);
        ImGui::DragFloat("decay", &decay, 0.001,  0.001, 0.5);
        ImGui::DragFloatRange2("range 1", &rangeOfMotion1[0], &rangeOfMotion1[1], 0.01, 0.0, 1.0);
        ImGui::DragFloatRange2("range 2", &rangeOfMotion2[0], &rangeOfMotion2[1], 1, 20, 400);
        
        for(int t = 0 ; t < players.size(); t++){
            ImGui::DragFloat(("trackVolume_"+ofToString(t)).c_str(), &trackVolume[t], 0.01, 0, 1);
        }
        
        ImGui::Columns(4, NULL, true);
        for (int i = 0;i < rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::PushID(ofToString(i).c_str());
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
            ImGui::Checkbox((rdtk::Actor::getJointName(i)+"_s").c_str(), &mSoundVisibility[i]);
            ImGui::NextColumn();
            ImGui::PopID();
        }
        ImGui::Columns(1);
		
	}
    void setAllVisiblity(bool b)
    {
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++){
            mNodeVisibility[i] = b;
        }
    }
    void update()
    {
        QLabCommunication::update();
    }
    void updateWithOscMessage(const ofxOscMessage &m)
    {
        ofLogVerbose("updateWithOscMessage") << "Furute | address: " << m.getAddress() << "| args : " << m.getArgAsString(0);
    }
	void draw()
	{
		const vector<rdtk::NodeArray>& NAs = ghostFilters.update(getAllNodeArrays());
		const vector<rdtk::NodeArray>& lowPassedNAs = lowPassFilters.update(NAs);
        
        
        
		rdtk::BeginCamera();
        
		for(int i=0; i<lowPassedNAs.size(); i++)
		{
			
			const rdtk::NodeArray &NA = getNodeArray(i);
            
			const rdtk::NodeArray &processedNA = lowPassedNAs[i];
			
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glEnable(GL_DEPTH_TEST);
			ofPushStyle();
			ofNoFill();
			
			const ofColor gcolor =
			i==0 ? rdtk::Color::BLUE_LIGHT :
			i==1 ? rdtk::Color::YELLOW_DEEP : rdtk::Color::BLUE_LIGHT;
			
			ofSetColor(gcolor);
            if(bFixCenter){
                ofPushMatrix();
                ofTranslate(-NA.getNode(rdtk::Actor::JOINT_CHEST).getGlobalPosition().x,0,
                            -NA.getNode(rdtk::Actor::JOINT_CHEST).getGlobalPosition().z);
            }
            if(customDraw){
                int soundIndex = 0;
                for (int nodeId=0; nodeId<processedNA.getNumNode(); nodeId++)
                {
                    const rdtk::Node &node = processedNA.getNode(nodeId);
                    if (mNodeVisibility[nodeId] == false && mNodeAlpha[nodeId] < 0.1f) continue;
                    ofVec3f mVelocity        = NA.getNode(nodeId).getVelocity();
                    mVelocitySmoothed[nodeId] += (mVelocity - mVelocitySmoothed[nodeId]) / smooth;
                    if(mNodeVisibility[nodeId]){
                        if(mNodeAlpha[nodeId]<1)mNodeAlpha[nodeId] += decay;
                    }else{
                        if(mNodeAlpha[nodeId]>0)mNodeAlpha[nodeId] -= decay;
                    }
                    if(drawLine){
                        Line(node);
                    }
                    node.beginTransform();
                    ofPushStyle();
                    ofSetColor(255,255,255,255*mNodeAlpha[nodeId]);
                    ofPushMatrix();
                    ofRotate(180, 0, 1, 0);
                    string s = subjects[nodeId%subjects.size()];
                    ofRectangle rect = font.getStringBoundingBox(s,0,0);
                    ofScale(mFontSize,mFontSize,mFontSize);
                    font.drawString(s, rect.getWidth() * (-0.5) * mFontSize, rect.getHeight() * 0.5 * mFontSize);
                    ofPopMatrix();
                    ofPopStyle();
                    node.endTransform();
                    
                    if(mNodeVisibility[nodeId] && mSoundVisibility[nodeId]) {
                        int index = soundIndex++;
                        if(index < players.size()){
                            
                            ofPtr<Sound3D> player = players[index];
                            if(!player.get()->isPlaying()){
                                player.get()->play();
                            }
                            
                            player.get()->updateListener(lposition, lvelocity, lforward, lup);
                            float l = mVelocitySmoothed[nodeId].length();
                            float div  = ofMap(l, rangeOfMotion1[0], rangeOfMotion1[1], rangeOfMotion2[0], rangeOfMotion2[1]);
                            ofVec3f v = ofVec3f(node.getGlobalPosition()/div);
                            player.get()->updateSound(v, svelocity);
                            player.get()->setVolume(trackVolume[index]*mNodeAlpha[nodeId]);
                            player.get()->update();
                        }
                    
                    }
                }
            }else{
                rdtk::DrawNodes(processedNA);
            }
			
			if (draw_line)
			{
				ofSetColor(gcolor);
				rdtk::DrawNodeCorresponds(NA, processedNA);
			}
            if(bFixCenter){
                ofPopMatrix();
            }
			ofPopStyle();
			glPopAttrib();
            
		}
		
		rdtk::EndCamera();
	}
	
	void onPresetGhost(ofEventArgs &e)
	{
		speed = 1.5;
		distance = 240;
	}
	
	void onPresetSlow(ofEventArgs &e)
	{
		speed = 8.3;
		distance = 74.4;
	}
	
	void onPresetNormal(ofEventArgs &e)
	{
		speed = 9.4;
		distance = 150;
	}
	
	void onPresetFast(ofEventArgs &e)
	{
		speed = 38.9;
		distance = 211;
	}
    
    void onPresetZero(ofEventArgs &e)
    {
        speed = 10;
        distance = 0;
    }
	
	void updateFilters()
	{
		for(int i=0; i<ghostFilters.getNumFilters(); i++)
		{
			rdtk::Ghost &filter = ghostFilters.getFilter(i);
			filter.setDistance(distance);
			filter.setSpeed(speed);
		}
	}
	
	void loadPreset(size_t preset_id=0)
	{
		for (int i=0; i<ghostFilters.getNumFilters(); i++)
		{
			ghostFilters.getFilter(i).setDistance(150);
			ghostFilters.getFilter(i).setSpeed(27);
		}
	}
	
	string getName() const { return "Monster"; }
    
    
};



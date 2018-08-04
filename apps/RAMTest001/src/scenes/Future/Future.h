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

class Future : public rdtk::BaseScene
{
	
	ramFilterEach<rdtk::Ghost> ghostFilters;
	ramFilterEach<rdtk::LowPassFilter> lowPassFilters;
	float mNodeAlpha[rdtk::Actor::NUM_JOINTS];
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
	float speed, distance;
    bool customDraw, drawLine;
    ofTrueTypeFont font;
    vector<string> subjects;
    bool bFixCenter;
    float decay;
    float mFontSize;
public:
	
    bool draw_line;
	
	Future() :
    distance(150),
    speed(27),
    draw_line(false),
    bFixCenter(true),
    mFontSize(1.0),
    decay(0.03){
        font.load("Constantia.ttf",12,true,true,true);
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
            mNodeAlpha[i] = 0;
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

	void setupControlPanel()
	{
		
		rdtk::GetGUI().addToggle("Draw line from actor to ghost", &draw_line);
		
		ofAddListener(rdtk::GetGUI().addButton("Speed: Ghost"), this, &Future::onPresetGhost);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Slow"), this, &Future::onPresetSlow);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Normal"), this, &Future::onPresetNormal);
		ofAddListener(rdtk::GetGUI().addButton("Speed: Fast"), this, &Future::onPresetFast);
        ofAddListener(rdtk::GetGUI().addButton("Speed: Zero"), this, &Future::onPresetZero);
		
		rdtk::GetGUI().addSlider("Distance", 0.0, 512, &distance);
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
		if (ImGui::DragFloat("Distance", &distance, 1, 0, 255)) updateFilters();
		if (ImGui::DragFloat("Speed", &speed, 1, 0, 255)) updateFilters();
        ImGui::DragFloat("decay", &decay, 0.001,  0.001, 0.5);
        
        ImGui::Columns(2, NULL, true);
        for (int i = 0;i < rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::PushID(ofToString(i).c_str());
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
            ImGui::PopID();
        }
        ImGui::Columns(1);
		
	}
    void setAllVisiblity(bool b)
    {
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
            mNodeVisibility[i] = b;
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
			i==0 ? rdtk::Color::RED_LIGHT :
			i==1 ? rdtk::Color::YELLOW_DEEP : rdtk::Color::BLUE_LIGHT;
			
			ofSetColor(gcolor);
            if(bFixCenter){
                ofPushMatrix();
                ofTranslate(-NA.getNode(rdtk::Actor::JOINT_CHEST).getGlobalPosition().x,0,
                            -NA.getNode(rdtk::Actor::JOINT_CHEST).getGlobalPosition().z);
            }
            if(customDraw){
                
                for (int nodeId=0; nodeId<processedNA.getNumNode(); nodeId++)
                {
                    const rdtk::Node &node = processedNA.getNode(nodeId);
                    if (mNodeVisibility[nodeId] == false && mNodeAlpha[nodeId]<0) continue;
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
	
	
	string getName() const { return "Future"; }
    
    
};



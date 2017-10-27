//
//  Example1.cpp
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
#include "Example1.h"
#define RIBBON_SIZE 50
class Tracker
{
public:
    
    deque<ofVec3f> samples;
    //Ribbon ribbon;
    ofMesh rmesh;
    float globalThickness;
    Tracker()
    {
        //ribbon.setup();
        rmesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        for(int i = 0; i < RIBBON_SIZE; i++){
            rmesh.addVertex(ofVec3f(0,0,-i*50));
            rmesh.addVertex(ofVec3f(0,0,-i*50));
            float revertIndex = RIBBON_SIZE-i;
            float c  = (float)revertIndex/RIBBON_SIZE;
            
            rmesh.addColor(ofFloatColor(c,1.0,1.0,(float)revertIndex/RIBBON_SIZE));
            rmesh.addColor(ofFloatColor(c,1.0,1.0,(float)revertIndex/RIBBON_SIZE));

        }
    }
    
    void update(const rdtk::Node &node)
    {
        //ribbon.update(joint->getPosition());
        // update sample
        {
            samples.push_front(node.getGlobalPosition());
            while (samples.size() > RIBBON_SIZE)
                samples.pop_back();
        }
        
//        float times = sin(ofGetElapsedTimef()*0.0001);
//        float timec = cos(ofGetElapsedTimef()*0.0001);
        for(int i = 0; i < samples.size(); i++){
            ofVec3f thisPoint;
            ofVec3f nextPoint;
            //find this point and the next point
            if(i>1)
            {
                
                thisPoint = samples[i-1];
                nextPoint = samples[i];
//                thisPoint.z+=timec*i*10;
//                nextPoint.z+=timec*i*10;
//                thisPoint.x+=times*i*10;
//                nextPoint.x+=times*i*10;
            }
            
            //get the direction from one to the next.
            //the ribbon should fan out from this direction
            ofVec3f direction = (nextPoint - thisPoint);
            
            //get the distance from one point to the next
            float distance = direction.length()/2;
            
            //get the normalized direction. normalized vectors always have a length of one
            //and are really useful for representing directions as opposed to something with length
            ofVec3f unitDirection = direction.normalize();
            
            //find both directions to the left and to the right
            ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,1,0));
            ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,1,0));
            
            //use the map function to determine the distance.
            //the longer the distance, the narrower the line.
            //this makes it look a bit like brush strokes
            float thickness = ofMap(sin(distance/direction.length()), 0, 10, 5, globalThickness, true)*(((samples.size()-i)*1.0f)/samples.size());
            
            //calculate the points to the left and to the right
            //by extending the current point in the direction of left/right by the length
            ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
            ofVec3f rightPoint = thisPoint+toTheRight*thickness;
            
            //add these points to the triangle strip
            
            rmesh.setVertex(i*2,ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
            rmesh.setVertex(i*2+1,ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
        }
    }
    
    float length()
    {
        if (samples.empty()) return 0;
        
        float v = 0;
        for (int i = 0; i < samples.size() - 1; i++)
            v += samples[i].distance(samples[i + 1]);
        
        return v;
    }
    
    float dot()
    {
        if (samples.empty()) return 0;
        
        float v = 0;
        
        for (int i = 1; i < samples.size() - 1; i++)
        {
            const ofVec3f &v0 = samples[i - 1];
            const ofVec3f &v1 = samples[i];
            const ofVec3f &v2 = samples[i + 1];
            
            if (v0.squareDistance(v1) == 0) continue;
            if (v1.squareDistance(v2) == 0) continue;
            
            const ofVec3f d0 = (v0 - v1).normalize();
            const ofVec3f d1 = (v1 - v2).normalize();
            
            v += (d0).dot(d1);
        }
        
        return v / ((float)samples.size() - 2);
    }
    
    void draw()
    {
        rmesh.draw();
        
//        ofPushStyle();
//        ofNoFill();
//        for(int i = 0; i < samples.size(); i++){
//            ofDrawBox(samples[i], 10);
//
//        }
//        ofPopStyle();
    }
};

class Trackers
{
public:
    
    vector<ofPtr<ofxTwistedRibbon>> ribbons;
    int hue = 0;
    //vector<Particle> particles;
    //ofVboMesh billboards;
    //int particle_index;
    
    Trackers()
    {
        hue = ofRandom(255);
        
    }
    
    void update(const rdtk::NodeArray& nodeArray, float globalThickness)
    {
        {
            if(ribbons.size() == nodeArray.getNumNode()){
                for (int i = 0; i < nodeArray.getNumNode(); i++)
                {
//                    ribbons[i]->globalThickness = globalThickness;
                    if(ribbons[i]->points.size()==0){
                        ribbons[i]->update(nodeArray.getNode(i).getGlobalPosition());
                    }
                    if(ribbons[i]->points.back().distance(nodeArray.getNode(i).getGlobalPosition())>2 ){
                        ribbons[i]->update(nodeArray.getNode(i).getGlobalPosition());
                    }
                    if(ribbons[i]->points.size()>50){
                        ribbons[i]->points.pop_front();
                    }
    //                tracker[i]->update();//particles);
         
                }
            }else{
                ofPtr<ofxTwistedRibbon> ribbon = ofPtr<ofxTwistedRibbon>(new ofxTwistedRibbon);
                ribbon->color = ofColor::fromHsb(hue, 255, ofRandom(150,255),128);
                ribbons.push_back(ribbon);
            }
        }
     
    }
    
    void draw()
    {
        ofEnableAlphaBlending();
        for (int i = 0; i < ribbons.size(); i++)
        {
            ribbons[i]->draw();
        }
    }
};
map<string,ofPtr<Trackers>> trackers;
void Example1::setupControlPanel()
{
    ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
    panel->addSlider("thickness", 1, 100, &mglobalThickness);
//    panel->addToggle("Only Limbs", &onlyLimbs, 20, 20);
//    panel->addSlider("Threshold", 0, .5, &threshold, 300, 20);
//    panel->addToggle("Show lines", &showLines, 20, 20);
//    panel->addSlider("Line width", 0, 10, &lineWidth, 300, 20);
//    panel->addSlider("Line length", 0, 1000, &lineLength, 300, 20);
//    panel->addToggle("Show planes", &showPlanes, 20, 20);
//    panel->addSlider("Scale", 0, 1000, &scale, 300, 20);
//    panel->addSlider("Ticks", 0, 10, &ticks, 300, 20);
}

void Example1::drawImGui()
{
//    ImGui::DragFloat("Fade out", &maxExample1MomentLife, 0.1, 0.0, 2.0);
//    ImGui::Checkbox("Only Limbs", &onlyLimbs);
//    ImGui::DragFloat("Threshold", &threshold, 0.1, 0.0, 0.5);
//    ImGui::Checkbox("Show Lines", &showLines);
//    ImGui::DragFloat("Line width", &lineWidth, 0.1, 0, 10);
//    ImGui::DragFloat("Line Length", &lineLength, 1, 0, 1000);
//    ImGui::Checkbox("Show planes", &showPlanes);
//    ImGui::DragFloat("Scale", &scale, 1, 0, 3000);
//    ImGui::DragFloat("Ticks", &ticks, 1, 0, 10);
}

void Example1::setup()
{
    
}

void Example1::update()
{
    for(int i=0; i<getNumNodeArray(); i++)
    {
        const rdtk::NodeArray &NA = getNodeArray(i);
    }
    vector<string> name = getNodeArrayNames();
    for (int n = 0; n < name.size(); n++){
        if(trackers.find(name[n]) != trackers.end()){
            trackers[name[n]]->update(getNodeArray(name[n]),mglobalThickness);
        }else{
            trackers[name[n]] = ofPtr<Trackers>(new Trackers);
        }
    }
//    const vector<rdtk::NodeArray>& NAs = getAllNodeArrays();
//
//    for (int n = 0; n < NAs.size(); n++)
//    {
//        const rdtk::NodeArray &NA = NAs[n];
//
//        for (int i = 0; i < NA.getNumNode(); i++)
//        {
//            const rdtk::Node &node = NA.getNode(i);
//
//        }
//    }
}

void Example1::draw()
{
    rdtk::BeginCamera();
    ofEnableAlphaBlending();
    glDisable(GL_DEPTH_TEST);
    //draw something
    map<string,ofPtr<Trackers>>::iterator it;
    for ( it = trackers.begin(); it!= trackers.end(); ++it){
        ofPtr<Trackers> tracker = it->second;
        tracker->draw();
    }
    rdtk::EndCamera();
}

void Example1::drawActor(const rdtk::Actor &actor)
{
    
//    trackers[0]->update();
    
//        if(onlyLimbs)
//        {
//            if(i != rdtk::Actor::JOINT_LEFT_ANKLE &&
//               i != rdtk::Actor::JOINT_RIGHT_ANKLE &&
//               i != rdtk::Actor::JOINT_LEFT_WRIST &&
//               i != rdtk::Actor::JOINT_RIGHT_WRIST)
//            {
//                continue;
//            }
//        }
//        const rdtk::Node &node = actor.getNode(i);
//        ofSetColor(255);
//        ofSetLineWidth(lineWidth);
//        if(node.hasParent())
//        {
//            ofVec3f start = node.getGlobalPosition();
//            ofVec3f end = node.getParent()->getGlobalPosition();
//            ofVec3f direction = (start - end);
//            direction.normalize();
//            if(showLines)
//            {
//            }
//            if(showPlanes)
//            {
//                if(abs(direction.x) < threshold)
//                {
//                    ofPushMatrix();
//                    ofTranslate(start);
//                    ofDrawGrid(scale, ticks, false, true, false, false);
//                    ofPopMatrix();
//                }
//                if(abs(direction.y) < threshold)
//                {
//                    ofPushMatrix();
//                    ofTranslate(start);
//                    ofDrawGrid(scale, ticks, false, false, true, false);
//                    ofPopMatrix();
//                }
//                if(abs(direction.z) < threshold)
//                {
//                    ofPushMatrix();
//                    ofTranslate(start);
//                    ofDrawGrid(scale, ticks, false, false, false, true);
//                    ofPopMatrix();
//                }
//            }
//        }
//    }
}

void Example1::drawRigid(rdtk::RigidBody &rigid)
{
}

// 
// ofApp.cpp - RAMDanceToolkit
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

#include "ofApp.h"

#pragma mark - oF methods
//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	/// ram setup
	// ------------------
	rdtk::Initialize(10000, true);
	
	/// scenes setup
	// ------------------
	rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();
    sceneManager.addScene<Interp>();
	sceneManager.addScene<Lines>();
    sceneManager.addScene<MakeSound>();
    sceneManager.addScene<Moji>();
    sceneManager.addScene<Character>();
    sceneManager.addScene<Link>();
    soundScene = (MakeSound*)(sceneManager.getScene(3).get());
    ofSoundStreamSetup(2, 0);
}

//--------------------------------------------------------------
void ofApp::update()
{
    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    soundScene->audioOut(output, bufferSize, nChannels);
}



#pragma mark - ram methods
//--------------------------------------------------------------
void ofApp::drawActor(const rdtk::Actor &actor)
{
	
}

//--------------------------------------------------------------
void ofApp::drawRigid(const rdtk::RigidBody &rigid)
{
	
}

#pragma mark - ram Events

//--------------------------------------------------------------
void ofApp::onActorSetup(const rdtk::Actor &actor)
{
	
}

//--------------------------------------------------------------
void ofApp::onActorExit(const rdtk::Actor &actor)
{

}

//--------------------------------------------------------------
void ofApp::onRigidSetup(const rdtk::RigidBody &rigid)
{

}

//--------------------------------------------------------------
void ofApp::onRigidExit(const rdtk::RigidBody &rigid)
{

}



#pragma mark - of Event
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	
}

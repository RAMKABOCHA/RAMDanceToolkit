#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("ofxSyphonServerDirectoryExample");
    ofSetWindowShape(800, 600);
    ofSetFrameRate(60);

    //setup our directory
    dir.setup();
    //setup our client
    client.setup();

    //register for our directory's callbacks
    ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
    // not yet implemented
    //ofAddListener(dir.events.serverUpdated, this, &ofApp::serverUpdated);
    ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);

    dirIdx = -1;
    
    fbo.allocate(1920, 1080);
    
    bezManager.setup(10); //WarpResolution
    bezManager.addFbo(&fbo);
    bezManager.loadSettings();
}

//these are our directory's callbacks
void ofApp::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Announced")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Updated")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

void ofApp::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    for( auto& dir : arg.servers ){
        ofLogNotice("ofxSyphonServerDirectory Server Retired")<<" Server Name: "<<dir.serverName <<" | App Name: "<<dir.appName;
    }
    dirIdx = 0;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofColor(255, 255, 255, 255);
    ofEnableAlphaBlending();
    fbo.begin();
    if(dir.isValidIndex(dirIdx))
        client.draw(0, 0,fbo.getWidth(), fbo.getHeight());
    fbo.end();
    bezManager.draw();
//    ofDrawBitmapString("Press any key to cycle through all available Syphon servers.", ofPoint(20, 580));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }
    bezManager.keyPressed(key);
    
    // ガイド表示/非表示
    if(key == OF_KEY_RETURN){
        bezManager.toggleGuideVisible();
    }
    // 設定保存
    if(key == 's'){
        bezManager.saveSettings();
    }
    // 設定読込
    if(key == 'l'){
        bezManager.loadSettings();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    //press any key to move through all available Syphon servers
    if (dir.size() > 0)
    {
        dirIdx++;
        if(dirIdx > dir.size() - 1)
            dirIdx = 0;

        client.set(dir.getDescription(dirIdx));
        string serverName = client.getServerName();
        string appName = client.getApplicationName();

        if(serverName == ""){
            serverName = "null";
        }
        if(appName == ""){
            appName = "null";
        }
        
        ofSetWindowTitle(serverName + ":" + appName);
    }
    else
    {
        ofSetWindowTitle("No Server");
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // マウスドラッグイベントを転送
    bezManager.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // マウスプレスイベントを転送
    bezManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

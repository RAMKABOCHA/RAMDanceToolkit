//
//  VoiceBubble.h
//  RAMDanceToolkit
//
//  Created by James Kong on 10/3/2018.
//

#ifndef VoiceBubble_h
#define VoiceBubble_h
#include "SphereMesh.h"
class SimpleBall {
public:
    ofMatrix4x4 matrix;
    float size;
    void draw(){
        
        glPushMatrix();
        ofPushStyle();
        ofSetColor(255);
        glMultMatrixf(matrix.getPtr());
        
        ofIcoSphere(ofVec3f(), size);
        
        ofPopStyle();
        
        glPopMatrix();

    }
};
class VoiceBubble : public rdtk::BaseScene
{
    
public:
    
    VoiceBubble() :
    isDebug(false),
    scale(190),
    minVol(90)
    {
        
        
        int bufferSize = 256;
        
        
        left.assign(bufferSize, 0.0);
        right.assign(bufferSize, 0.0);
        volHistory.assign(400, 0.0);
        
        bufferCounter    = 0;
        drawCounter        = 0;
        smoothedVol     = 0.0;
        scaledVol        = 0.0;
        
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            mNodeVisibility[i] = false;
        }
        
    }
    
    
    void drawImGui()
    {
        
        ImGui::Checkbox("isDebug", &isDebug);
        ImGui::DragFloat("scale", &scale, 1, 1, 190);
        ImGui::DragFloat("minVol", &minVol, 1, 0, 190);
        
        ImGui::Columns(3, NULL, true);
        
        for (int i=0; i<rdtk::Actor::NUM_JOINTS; i++)
        {
            ImGui::Checkbox(rdtk::Actor::getJointName(i).c_str(), &mNodeVisibility[i]);
            ImGui::NextColumn();
        }
         ImGui::Columns(1, NULL, true);
    }
    
    void update(){
        //lets scale the vol up to a 0-1 range
        scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
        
        //lets record the volume into an array
        volHistory.push_back( scaledVol );
        
        //if we are bigger the the size we want to record - lets drop the oldest value
        if( volHistory.size() >= 400 ){
            volHistory.erase(volHistory.begin(), volHistory.begin()+1);
        }
        
        
        
    }

    void drawActor(const rdtk::Actor &actor)
    {
        if(scaledVol*190 > minVol ){
            for(int i = 0 ; i < rdtk::Actor::NUM_JOINTS ; i++){
                if(mNodeVisibility[i]){
                    ofPtr<SimpleBall> ball = ofPtr<SimpleBall>(new SimpleBall());
                    ball.get()->size = scaledVol*scale;
                    
                    ball.get()->matrix = actor.getNode(i).getGlobalTransformMatrix();
                    balls.push_back(ball);
                }
            }
        }
    }
    
    
    void draw()
    {
        
        rdtk::BeginCamera();
        for (int i=0; i<getNumNodeArray(); i++)
        {
            rdtk::NodeArray &src = getNodeArray(i);
            
        }
        
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        
        ofPushStyle();
        ofNoFill();
        ofSetColor(0);
        ofEnableAlphaBlending();
        ofSetCircleResolution(6);
        for(int i = 0 ;i < balls.size() ; i++){
            balls[i].get()->draw();
        }
        ofPopStyle();
        glPopAttrib();
        
        rdtk::EndCamera();
        
        if(isDebug){
            ofSetColor(225);
            ofDrawBitmapString("AUDIO INPUT EXAMPLE", 32, 32);
            ofDrawBitmapString("press 's' to unpause the audio\n'e' to pause the audio", 31, 92);
            
            ofNoFill();
            
            // draw the left channel:
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(32, 170, 0);
            
            ofSetColor(225);
            ofDrawBitmapString("Left Channel", 4, 18);
            
            ofSetLineWidth(1);
            ofDrawRectangle(0, 0, 512, 200);
            
            ofSetColor(245, 58, 135);
            ofSetLineWidth(3);
            
            ofBeginShape();
            for (unsigned int i = 0; i < left.size(); i++){
                ofVertex(i*2, 100 -left[i]*180.0f);
            }
            ofEndShape(false);
            
            ofPopMatrix();
            ofPopStyle();
            
            // draw the right channel:
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(32, 370, 0);
            
            ofSetColor(225);
            ofDrawBitmapString("Right Channel", 4, 18);
            
            ofSetLineWidth(1);
            ofDrawRectangle(0, 0, 512, 200);
            
            ofSetColor(245, 58, 135);
            ofSetLineWidth(3);
            
            ofBeginShape();
            for (unsigned int i = 0; i < right.size(); i++){
                ofVertex(i*2, 100 -right[i]*180.0f);
            }
            ofEndShape(false);
            
            ofPopMatrix();
            ofPopStyle();
            
            // draw the average volume:
            ofPushStyle();
            ofPushMatrix();
            ofTranslate(565, 170, 0);
            
            ofSetColor(225);
            ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, 18);
            ofDrawRectangle(0, 0, 400, 400);
            
            ofSetColor(245, 58, 135);
            ofFill();
            ofDrawCircle(200, 200, scaledVol * 190.0f);
            
            
            //lets draw the volume history as a graph
            ofBeginShape();
            for (unsigned int i = 0; i < volHistory.size(); i++){
                if( i == 0 ) ofVertex(i, 400);
                
                ofVertex(i, 400 - volHistory[i] * 70);
                
                if( i == volHistory.size() -1 ) ofVertex(i, 400);
            }
            ofEndShape(false);
            
            ofPopMatrix();
            ofPopStyle();
            
            drawCounter++;
        }
    }
    
    void audioIn(float * input, int bufferSize, int nChannels){
        float curVol = 0.0;
        
        // samples are "interleaved"
        int numCounted = 0;
        
        //lets go through each sample and calculate the root mean square which is a rough way to calculate volume
        for (int i = 0; i < bufferSize; i++){
            left[i]        = input[i*2]*0.5;
            right[i]    = input[i*2+1]*0.5;
            
            curVol += left[i] * left[i];
            curVol += right[i] * right[i];
            numCounted+=2;
        }
        
        //this is how we get the mean of rms :)
        curVol /= (float)numCounted;
        
        // this is how we get the root of rms :)
        curVol = sqrt( curVol );
        
        smoothedVol *= 0.93;
        smoothedVol += 0.07 * curVol;
        
        bufferCounter++;
    }
private:
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    int     bufferCounter;
    int     drawCounter;
    
    float smoothedVol;
    float scaledVol;
    bool isDebug;
    
    bool mNodeVisibility[rdtk::Actor::NUM_JOINTS];
    float minVol;
    float scale;
    vector<ofPtr<SimpleBall>> balls;
    
    string getName() const { return "VoiceBubble"; }
};
#endif /* VoiceBubble_h */

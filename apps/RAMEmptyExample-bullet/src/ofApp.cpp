#include "ofApp.h"
#include "BulletScene.h"

#pragma mark - oF methods
//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	/// ram setup
	// ------------------
	rdtk::Initialize(10000, true);
  
  
  rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();
  sceneManager.addScene<BulletScene>();
}

//--------------------------------------------------------------
void ofApp::update()
{
	
}

//--------------------------------------------------------------
void ofApp::draw()
{
	
}



#pragma mark - ram methods
//--------------------------------------------------------------
void ofApp::drawActor(const rdtk::Actor &actor)
{
//  auto n1 = actor.getNode(rdtk::Actor::JOINT_HEAD);
//  auto n2 = actor.getNode(rdtk::Actor::JOINT_RIGHT_HAND);
//  auto n3 = actor.getNode(rdtk::Actor::JOINT_LEFT_HAND);
//  auto n4 = actor.getNode(rdtk::Actor::JOINT_RIGHT_TOE);
//  auto n5 = actor.getNode(rdtk::Actor::JOINT_LEFT_TOE);
//  
//  ofPushStyle();
//  {
//    // draw line betweem two nodes
//    ofNoFill();
//    ofSetColor( rdtk::Color::RED_LIGHT );
//    ofSetLineWidth( 3 );
//    ofDrawLine( n2, n3 );
//    
//    
//    // draw triangle using three nodes
//    ofFill();
//    ofSetColor( rdtk::Color::BLUE_LIGHT );
//    ofSetLineWidth( 3 );
//    ofDrawTriangle( n1, n4, n5 );
//    
//    
//    // ramNode::beginTransform() ~ endTransform()
//    n1.beginTransform();
//    {
//      // draw cube at JOINT_HEAD
//      ofNoFill();
//      ofSetColor( rdtk::Color::YELLOW_DEEP );
//      ofSetLineWidth( 3 );
//      ofDrawBox( 20 );
//      
//      // draw cone at JOINT_HEAD
//      ofNoFill();
//      ofSetColor( rdtk::Color::GRAY );
//      ofSetLineWidth( 2 );
//      ofDrawCone(3, -30);
//    }
//    n1.endTransform();
//    
//    
//    // draw plane using several nodes
//    ofNoFill();
//    ofSetColor( rdtk::Color::YELLOW_LIGHT );
//    ofSetLineWidth( 2 );
//    ofPushMatrix();
//    {
//      ofScale( 3, 3, 3 );
//      rdtk::Stripe( n1, n2, n3, n4, n5 );
//    }
//    ofPopMatrix();
//  }
//  ofPopStyle();  
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

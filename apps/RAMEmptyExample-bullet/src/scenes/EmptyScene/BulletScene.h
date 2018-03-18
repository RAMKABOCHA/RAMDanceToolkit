#pragma once
#include "ramMain.h"


/*
 add ofxBullet to {ofRoot}/addons folder
 add '../../../addons/ofxBullet/libs' to Header search path
 */
#include "ofxBullet.h"

class BulletScene : public rdtk::BaseScene
{
public:
  std::string getName() const { return "BulletScene"; }
	
  void setup();
  void update();
  void draw();
  void drawImGui();
  
private:
  
  
  // Bullet example code from SoftBodyExample
  ofxBulletWorldSoft world;
  ofxBulletBox * ground;
  
  ofxBulletRope * rope;
  ofxBulletPatch * patch;
  vector<ofxBulletEllipsoid*> ellipsoids;
  
  vector<ofxBulletRigidBody *> shapes;
  vector<ofxBulletJoint *> joints;
  vector<ofColor> shapeColors;
  ofColor colors[4];
  
  bool bDrawDebug;
  bool bSpacebar;
  bool bAddEllipsoid;
  bool bShapesNeedErase;
  
  float jointLength;
  
  ofLight light;
  ofVec3f	 mousePos;

  // for debugging...
  void add();
};

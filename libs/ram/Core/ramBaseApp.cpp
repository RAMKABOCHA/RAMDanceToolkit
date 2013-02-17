#include "ramSharedData.h"

#include "ramBaseApp.h"
#include "ramCameraManager.h"
#include "ramControlPanel.h"
#include "ramPhysics.h"

void ramBaseApp::exit(ofEventArgs &args)
{
	ramDisableAllEvents();
}

void ramBaseApp::update(ofEventArgs &args)
{
    getActorManager().update();
}

void ramBaseApp::draw(ofEventArgs &args)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glEnable(GL_DEPTH_TEST);
	
	ramBeginCamera();

	if (draw_floor_auto)
		drawFloor();
	
	getActorManager().draw();

	bool enable_physics = ramGetEnablePhysicsPrimitive();
	
	ramEnablePhysicsPrimitive(false);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	ofPushStyle();

	{
		// shadow
		
		ramBeginShadow();
		drawNodeArrays();
		ramEndShadow();
	}

	ofPopStyle();
	glPopMatrix();
	glPopAttrib();

	ramEnablePhysicsPrimitive(enable_physics);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	ofPushStyle();

	{
		// entities
		drawNodeArrays();
	}
    
	ofPopStyle();
	glPopMatrix();
	glPopAttrib();

	ramEndCamera();
	
	glPopAttrib();
}

void ramBaseApp::drawNodeArrays()
{
	// draw nodearray
	
	for (int n=0; n<getNumNodeArray(); n++)
	{
		ramNodeArray &o = getNodeArray(n);
		
		if (o.isActor())
			drawActor((ramActor&)o);
		else
			drawRigid((ramRigidBody&)o);
	}
	
	// draw bus
	
	map<string, ramNodeArray>::iterator it = getActorManager().getAllBus().begin();
	
	while( it != getActorManager().getAllBus().end() )
	{
		ramNodeArray &o = (*it).second;
		
		if (o.isActor())
			drawActor((ramActor&)o);
		else
			drawRigid((ramRigidBody&)o);
		++it;
	}
}

void ramBaseApp::drawFloor()
{
	ramControlPanel &gui = ramGetGUI();
	ramDrawBasicFloor(gui.getFloorPattern(),
				  gui.getFloorSize(),
				  gui.getGridSize(),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_DEEP);
}
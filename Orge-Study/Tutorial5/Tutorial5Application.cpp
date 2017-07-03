/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.8.x Application Wizard for Code::Blocks (May 2012)
      https://bitbucket.org/jacmoe/ogreappwizards
      -----------------------------------------------------------------------------
      */
#include "Tutorial5Application.h"
using namespace Ogre;
//-------------------------------------------------------------------------------------
Tutorial5Application::Tutorial5Application(void)
{
}
//-------------------------------------------------------------------------------------
Tutorial5Application::~Tutorial5Application(void)
{
}

//-------------------------------------------------------------------------------------

void createFrameListener(void)
{

}

// Ogre::FrameListener
bool Tutorial5Application::frameRenderingQueued(const Ogre::FrameEvent& evt )
{
	return true;
}

// OIS::KeyListener
bool Tutorial5Application::keyPressed( const OIS::KeyEvent& evt )
{
	return true;
}

bool Tutorial5Application::keyReleased( const OIS::KeyEvent& evt )
{
	return true;
}

// OIS::MouseListener
bool Tutorial5Application::mouseMoved( const OIS::MouseEvent& evt )
{
	return true;
}

bool Tutorial5Application::mousePressed( const OIS::MouseEvent& evt, OIS::MouseButtonID id )
{
	return true;
}

bool Tutorial5Application::mouseReleased( const OIS::MouseEvent& evt, OIS::MouseButtonID id )
{	
	return true;
}

void Tutorial5Application::createFrameListener(void)
{

}

void Tutorial5Application::createScene(void)
{
#if 0
	mSceneMgr->setAmbientLight(ColourValue(1, 1, 1));
	Ogre::Entity* ent1 = mSceneMgr->createEntity("Root", "robot.mesh");
	Ogre::SceneNode* node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode0");
	node1->attachObject(ent1);

	Entity *ent2 = mSceneMgr->createEntity( "Robot2", "robot.mesh" );
	SceneNode *node2 = node1->createChildSceneNode("RobotNode2", Vector3( 50, 0, 0 ) );
	node2->attachObject( ent2 );

	node2->translate( Vector3( 10, 0, 10 ) );
#endif 
	mSceneMgr->setAmbientLight(ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	Entity* ent = mSceneMgr->createEntity("Ninja", "ninja.mesh");
	ent->setCastShadows(true);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	Plane plane(Vector3::UNIT_Y, 0);

	MeshManager::getSingleton().createPlane("ground",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);

	ent = mSceneMgr->createEntity("GroundEntity", "ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

	ent->setMaterialName("Examples/Rockwall");
	ent->setCastShadows(false);

	Light* light = mSceneMgr->createLight("Light1");
	light->setType(Light::LT_POINT);
	light->setPosition(Vector3(0, 150, 250));

	light->setDiffuseColour(1.0, 0.0, 0.0);
	light->setSpecularColour(1.0, 0.0, 0.0);

	light = mSceneMgr->createLight("Light3");
	light->setType(Light::LT_DIRECTIONAL);
	light->setDiffuseColour(ColourValue(.25, .25, 0));
	light->setSpecularColour(ColourValue(.25, .25, 0));

	light->setDirection(Vector3( 0, -1, 1 )); 

	light = mSceneMgr->createLight("Light2");
	light->setType(Light::LT_SPOTLIGHT);
	light->setDiffuseColour(0, 0, 1.0);
	light->setSpecularColour(0, 0, 1.0);

	light->setDirection(-1, -1, 0);
	light->setPosition(Vector3(300, 300, 0));

	 light->setSpotlightRange(Degree(35), Degree(50));


}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Tutorial5Application app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

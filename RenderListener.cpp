#include "RenderListener.h"
#include "RenderManager.h"

#include <iostream>
using namespace std;

RenderListener::RenderListener(RenderManager* rm)
{
   render_manager = rm;
   render = true;
}

RenderListener::~RenderListener()
{
   render_manager = NULL;
}

void RenderListener::stopRendering()
{
   render = false;
}

//Ogre notifies this class when frames are processed
//as long as this method returns true, the game will continue (automatically done by Ogre)
bool RenderListener::frameStarted(const Ogre::FrameEvent& event) 
{
   float time_step = event.timeSinceLastFrame;
   render_manager->setTimeSinceLastFrame(time_step);

   render_manager->checkInput();  //must be checked each and every frame
   return render;
}

bool RenderListener::frameEnded(const Ogre::FrameEvent& event) {}
bool RenderListener::frameRenderingQueued(const Ogre::FrameEvent& event) {}

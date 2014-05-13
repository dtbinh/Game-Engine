#include "InputManager.h"
#include "GameManager.h"

#include <sstream>
#include <iostream>
using namespace std;

//called on each and every frame!
void InputManager::checkInput()
{
   OIS::MouseState mouse_state = mouse_ois->getMouseState();
   bool left_mouse_button_down = mouse_state.buttonDown(OIS::MB_Left);

/*
   //check unbuffered input (i.e. holding the left mouse button down)
   //generates a bunch of events even for a quick mouse press and release
   if (left_mouse_button_down)
   {
      OIS::MouseEvent e(mouse_ois, mouse_state);
      mousePressed(e, OIS::MB_Left);
   }
*/

   //check buffered input
   if (mouse_ois) mouse_ois->capture();
   if (keyboard_ois) keyboard_ois->capture();
   if (joystick_ois) joystick_ois->capture();
}

//callback
bool InputManager::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
   e.state.width = window_width;
   e.state.height = window_height;

cout << "MP" << endl;
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();

      int game_mouse = mouseMap(id);
      int x_loc = (int) e.state.X.abs;
      int y_loc = (int) e.state.Y.abs;
      listener->mousePressed(x_loc, y_loc, game_mouse);  //absolute location of mouse when pressed and which button was pressed
   }
   delete iter;

   return true;
}

//callback
bool InputManager::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
   e.state.width = window_width;
   e.state.height = window_height;

cout << "MR" << endl;
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();

      int game_mouse = mouseMap(id);
      int x_loc = (int) e.state.X.abs;
      int y_loc = (int) e.state.Y.abs;
      listener->mouseReleased(x_loc, y_loc, game_mouse);  //absolute location of mouse when released and which button was released
   }
   delete iter;

   return true;
}

//callback
bool InputManager::mouseMoved(const OIS::MouseEvent& e)
{
   e.state.width = window_width;
   e.state.height = window_height;

   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      int x_loc = (int) e.state.X.abs;
      int y_loc = (int) e.state.Y.abs;
      float x_rel = e.state.X.rel;
      float y_rel = e.state.Y.rel;
      listener->mouseMoved(x_loc, y_loc, x_rel, y_rel);  //current location of the mouse (absolute coords and relative coords)
   }
   delete iter;

   return true;
}

//callback
bool InputManager::keyPressed(const OIS::KeyEvent& e)
{
cout << "KP" << endl;
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();

   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      listener->keyPressed(keyMap(e));
   }
   delete iter;

   return true;
}

//callback
bool InputManager::keyReleased(const OIS::KeyEvent& e)
{
cout << "KR" << endl;
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      listener->keyReleased(keyMap(e));
   }
   delete iter;

   return true;
}

InputManager::InputManager(GameManager* game_manager)
{
   input_manager_ois = NULL;
   keyboard_ois = NULL;
   mouse_ois = NULL;
   joystick_ois = NULL;

   input_listeners = new ListArray<InputListener>();

   init(game_manager);

   addListener(game_manager);

   window_width = game_manager->getWindowWidth();
   window_height = game_manager->getWindowHeight();
}

InputManager::~InputManager()
{
   free();

   input_manager_ois = NULL;
   keyboard_ois = NULL;
   mouse_ois = NULL;
   joystick_ois = NULL;

   //the individual input listeners are not deleted here, just the ListArray
   delete input_listeners;
}

void InputManager::addListener(InputListener* listener)
{
   input_listeners->add(listener);
}

void InputManager::free()
{
   if (input_manager_ois)
   {
      input_manager_ois->destroyInputSystem(input_manager_ois);
   }
}

void InputManager::init(GameManager* game_manager)
{
   try
   {
      OIS::ParamList p1;
      std::ostringstream windowHndStr;
      size_t window_handle = game_manager->getWindowHandle();

      size_t handle = window_handle;
      windowHndStr << handle;
      p1.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

      input_manager_ois = OIS::InputManager::createInputSystem(p1);

      if (input_manager_ois->getNumberOfDevices(OIS::OISKeyboard) > 0)
      {
         keyboard_ois = static_cast<OIS::Keyboard*>(input_manager_ois->createInputObject(OIS::OISKeyboard, true));
         keyboard_ois->setEventCallback(this);  //forwards the information to all registered listeners
      }

      if (input_manager_ois->getNumberOfDevices(OIS::OISMouse) > 0)
      {
         mouse_ois = static_cast<OIS::Mouse*>(input_manager_ois->createInputObject(OIS::OISMouse, true));
         mouse_ois->setEventCallback(this);  //forwards the information to all registered listeners
      }

      if (input_manager_ois->getNumberOfDevices(OIS::OISJoyStick) > 0)
      {
         joystick_ois = static_cast<OIS::JoyStick*>(input_manager_ois->createInputObject(OIS::OISJoyStick, true));
         joystick_ois->setEventCallback(this);  //forwards the information to all registered listeners
      }
   }

   catch(std::exception& e)  //may be possible to get a good error message from ois this way
   {
      //THROW_EXCEPTION(1, e.what());  //error number 1 refers to an error that is too serious to continue
   }

   catch(...)  //catch any other type of exception thrown by OIS, won't be able to access the actual error message
   {
      //THROW_EXCEPTION(1, "Input Manager Initialization Error");  //error number 1 refers to an error that is too serious to continue
   }
}

std::string InputManager::keyMap(const OIS::KeyEvent& e)
{
   std::string game_key = "INVALID_KEY";
   OIS::KeyCode key_code = e.key;

   if (key_code == OIS::KC_ESCAPE)
   {
      game_key = "ESCAPE";
   }
   else if (key_code == OIS::KC_RIGHT)
   {
      game_key = "RIGHT_ARROW";
   }
   else if (key_code == OIS::KC_LEFT)
   {
      game_key = "LEFT_ARROW";
   }
   else if (key_code == OIS::KC_UP)
   {
      game_key = "UP_ARROW";
   }
   else if (key_code == OIS::KC_DOWN)
   {
      game_key = "DOWN_ARROW";
   }

   else if (key_code == OIS::KC_A)
   {
      game_key = "A";
   }
   else if (key_code == OIS::KC_B)
   {
      game_key = "B";
   }
   else if (key_code == OIS::KC_C)
   {
      game_key = "C";
   }
   else if (key_code == OIS::KC_D)
   {
      game_key = "D";
   }
   else if (key_code == OIS::KC_E)
   {
      game_key = "E";
   }
   else if (key_code == OIS::KC_F)
   {
      game_key = "F";
   }
   else if (key_code == OIS::KC_G)
   {
      game_key = "G";
   }
   else if (key_code == OIS::KC_H)
   {
      game_key = "H";
   }
   else if (key_code == OIS::KC_I)
   {
      game_key = "I";
   }
   else if (key_code == OIS::KC_J)
   {
      game_key = "J";
   }
   else if (key_code == OIS::KC_K)
   {
      game_key = "K";
   }
   else if (key_code == OIS::KC_L)
   {
      game_key = "L";
   }
   else if (key_code == OIS::KC_M)
   {
      game_key = "M";
   }
   else if (key_code == OIS::KC_N)
   {
      game_key = "N";
   }
   else if (key_code == OIS::KC_O)
   {
      game_key = "O";
   }
   else if (key_code == OIS::KC_P)
   {
      game_key = "P";
   }
   else if (key_code == OIS::KC_Q)
   {
      game_key = "Q";
   }
   else if (key_code == OIS::KC_R)
   {
      game_key = "R";
   }
   else if (key_code == OIS::KC_S)
   {
      game_key = "S";
   }
   else if (key_code == OIS::KC_T)
   {
      game_key = "T";
   }
   else if (key_code == OIS::KC_U)
   {
      game_key = "U";
   }
   else if (key_code == OIS::KC_V)
   {
      game_key = "V";
   }
   else if (key_code == OIS::KC_W)
   {
      game_key = "W";
   }
   else if (key_code == OIS::KC_X)
   {
      game_key = "X";
   }
   else if (key_code == OIS::KC_Y)
   {
      game_key = "Y";
   }
   else if (key_code == OIS::KC_Z)
   {
      game_key = "Z";
   }

   return game_key;
}

int InputManager::mouseMap(const OIS::MouseButtonID id)
{
   int game_mouse = -1;
   int mouse_code = id;

   if (mouse_code == OIS::MB_Left)
   {
      game_mouse = 0;
   }
   else if (mouse_code == OIS::MB_Right)
   {
      game_mouse = 1;
   }

   return game_mouse;
}

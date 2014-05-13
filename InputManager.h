#if !defined (INPUT_MANAGER)
#define INPUT_MANAGER

#include <OIS.h>
#include "CSC2110/ListArray.h"
#include "InputListener.h"
#include <iostream>

class GameManager;

class InputManager: public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener
{
   private:
      OIS::InputManager* input_manager_ois;

      OIS::Keyboard* keyboard_ois;
      OIS::Mouse* mouse_ois;
      OIS::JoyStick* joystick_ois;

      int window_width;
      int window_height;

      ListArray<InputListener>* input_listeners;
      std::string keyMap(const OIS::KeyEvent& e);
      int mouseMap(const OIS::MouseButtonID id);

      void init(GameManager* game_manager);
      void free();

   public:
      InputManager(GameManager* game_manager);
      virtual ~InputManager();

      void addListener(InputListener* listener);
      void checkInput();

      //key listener methods
      bool keyPressed(const OIS::KeyEvent& e);
      bool keyReleased(const OIS::KeyEvent& e);

      //mouse listener methods
      bool mouseMoved(const OIS::MouseEvent& e);
      bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
      bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);

      //joystick listener methods
      bool buttonPressed (const OIS::JoyStickEvent &arg, int button){std::cout << "works" << std::endl;};
      bool buttonReleased (const OIS::JoyStickEvent &arg, int button){};
      bool axisMoved (const OIS::JoyStickEvent &arg, int axis){};
      bool sliderMoved (const OIS::JoyStickEvent &, int index){};
      bool povMoved (const OIS::JoyStickEvent &arg, int index){};
      bool vector3Moved (const OIS::JoyStickEvent &arg, int index){};
};

#endif

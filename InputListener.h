#if !defined (INPUT_LISTENER)
#define INPUT_LISTENER

#include <string>

class InputListener
{
   private:

   public:
      virtual ~InputListener(){};
      virtual void keyPressed(std::string game_key) = 0;
      virtual void keyReleased(std::string game_key) = 0;

      virtual void mouseMoved(int mouse_x, int mouse_y, float mouse_x_rel, float mouse_y_rel) = 0;
      virtual void mousePressed(int mouse_x, int mouse_y, int game_mouse) = 0;
      virtual void mouseReleased(int mouse_x, int mouse_y, int game_mouse) = 0;
};

#endif
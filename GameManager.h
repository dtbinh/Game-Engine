#if !defined (GAME_MANAGER)
#define GAME_MANAGER

#include "InputListener.h"
#include "TinyXML.h"

class RenderManager;
class ResourceManager;
class InputManager;

//supplies communication between managers
class GameManager : public InputListener
{
   private:
      RenderManager* render_manager;
      InputManager* input_manager;
      ResourceManager* resource_manager;

      GameManager();
      void init();

   public:
      virtual ~GameManager();
      static GameManager* getGameManager();

      void checkInput();
      int getWindowWidth();
      int getWindowHeight();
      size_t getWindowHandle();

      std::string getConfigFilePath();

      void loadRenderResourceGroup(std::string group_name);
      void unloadRenderResourceGroup(std::string group_name);
      void initialiseRenderResourceGroup(std::string group_name);
      void addPathResource(std::string file_name, std::string path_type, std::string group_name);
      void addMeshResource(std::string file_name, std::string, std::string group_name);

      //GameManager acts as the listener for input
      void keyPressed(std::string game_key);
      void keyReleased(std::string game_key);

      void mouseMoved(int mouse_x, int mouse_y, float mouse_x_rel, float mouse_y_rel);
      void mousePressed(int mouse_x, int mouse_y, int game_mouse);
      void mouseReleased(int mouse_x, int mouse_y, int game_mouse);

      static int parseInt(std::string& str);
      static float parseFloat(std::string& str);
      static float* parseFloats(std::string& str, int num_tokens);
      static std::string textFromChildNode(TiXmlNode* parent_node, const char* child_element_name);
};

#endif
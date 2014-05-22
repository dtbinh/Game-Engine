#if !defined (RENDER_MANAGER)
#define RENDER_MANAGER

#include "Ogre.h"
#include "RenderListener.h"
#include "TinyXML.h"

class GameManager;
class GUIManager;
class ScriptManager;

class RenderManager
{
   private:
      Ogre::Root* root;
      Ogre::RenderWindow* window;
      Ogre::SceneManager* scene_manager;

      Ogre::Camera* camera;
      Ogre::Viewport* viewport;

      GameManager* game_manager;
      RenderListener* render_listener;
      GUIManager* gui_manager;
      ScriptManager* script_manager;

      void init();
      size_t window_handle;
      Ogre::Real time_since_last_frame;

      void addSceneNodeChildren(TiXmlNode* xml_node, Ogre::SceneNode* parent_node);

   public:
      RenderManager(GameManager* game_manager);
      virtual ~RenderManager();

      //double* getCameraPosition();
      //void setCameraPosition(double* camera_pos);

      void buildSceneFromXML(std::string file_name);

      void setTimeSinceLastFrame(Ogre::Real time_since_last_frame);
      void updateCameraRotation(float yaw, float pitch);
      void updateCameraPosition(std::string& game_key);

      size_t getWindowHandle();
      int getWindowWidth();
      int getWindowHeight();
      Ogre::RenderWindow* getRenderWindow();
      Ogre::SceneManager* getSceneManager();

      void mousePressed(int mouse_x, int mouse_y, int game_mouse);
      void mouseReleased(int mouse_x, int mouse_y, int game_mouse);
      void mouseMovedAbsolute(int mouse_x, int mouse_y);
      void mouseMovedRelative(float mouse_x, float mouse_y);

      void startRendering();
      void stopRendering();
      void checkInput();

      std::string getConfigFilePath();

      void loadResourceGroup(std::string section_name);
      void unloadResourceGroup(std::string group_name);
      void initialiseResourceGroup(std::string group_name);
      void addPathResource(std::string file_name, std::string path_type, std::string group_name);
      void addMeshResource(std::string file_name, std::string mesh_str, std::string group_name);

      void updateSceneNodeRotation(std::string& scene_node_name, float _pitch, float _yaw, float _roll);
      void executeScript(std::string& file_name, std::string& script_name, std::string& object_name);
};

#endif

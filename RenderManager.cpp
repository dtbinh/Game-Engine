#include "RenderManager.h"
#include "GameManager.h"
#include "GUIManager.h"
#include "ScriptManager.h"

#include <iostream>
using namespace Ogre;

/*

double* RenderManager::getCameraPosition()
{
   Ogre::Camera* camera = scene_manager->getCamera("Camera");
   Ogre::Vector3 camera_position = camera->getPosition();
   double* camera_pos = new double[3];
   camera_pos[0] = camera_position.x;
   camera_pos[1] = camera_position.y;
   camera_pos[2] = camera_position.z;
   return camera_pos;
}

void RenderManager::setCameraPosition(double* camera_pos)
{
   Ogre::Camera* camera = scene_manager->getCamera("Camera");
   Ogre::Vector3 camera_position = camera->getPosition();
   camera->setPosition(camera_pos[0], camera_pos[1], camera_pos[2]);
   delete[] camera_pos;
}

*/

void RenderManager::updateCameraRotation(float _yaw, float _pitch)
{
   string file_str("lua_scripts/camera_scripts.lua");
   string script_str("camera_rotate");

   double params[3];
   params[0] = _yaw;
   params[1] = _pitch;
   params[2] = time_since_last_frame;

   double results[2];
   
   script_manager->executeScript(file_str, script_str, 3, 2, params, results);

   camera->yaw(Degree(results[0]));
   camera->pitch(Degree(results[1]));
}

void RenderManager::updateCameraPosition(string& game_key)
{
   string file_str("lua_scripts/camera_scripts.lua");
   string script_str("camera_move");

   double params[2];
   params[0] = game_key.at(0);
   params[1] = time_since_last_frame;

   double results[3];
   
cout << params[0] << endl;
   script_manager->executeScript(file_str, script_str, 2, 3, params, results);
   camera->moveRelative(Vector3(results[0], results[1], results[2]));
}

void RenderManager::setTimeSinceLastFrame(Real tslf)
{
   time_since_last_frame = tslf;
}

void RenderManager::updateSceneNodeRotation(std::string& scene_node_name, float _pitch, float _yaw, float _roll)
{
   SceneNode* scene_node = scene_manager->getSceneNode(scene_node_name);
   scene_node->pitch(Degree(_pitch));
   scene_node->yaw(Degree(_yaw));
   scene_node->roll(Degree(_roll));
}

void RenderManager::executeScript(std::string& file_name, std::string& script_name, string& object_name)
{
   double* pyr = new double[3];
   pyr[0] = 0; pyr[1] = 0; pyr[2] = 0;
   script_manager->executeScript(file_name, script_name, 3, 3, pyr, pyr);
   updateSceneNodeRotation(object_name, (float) pyr[0], (float) pyr[1], (float) pyr[2]);
   delete[] pyr;
}

RenderWindow* RenderManager::getRenderWindow()
{
   return window;
}

SceneManager* RenderManager::getSceneManager()
{
   return scene_manager;
}
   
RenderManager::RenderManager(GameManager* gm)
{
   game_manager = gm;
   init();
   render_listener = new RenderListener(this);
   root->addFrameListener(render_listener);

   gui_manager = new GUIManager(this);
   script_manager = new ScriptManager(this);
}

RenderManager::~RenderManager()
{
   game_manager = NULL;

   stopRendering();
   delete render_listener;

   delete gui_manager;

   scene_manager->destroyAllCameras();
   scene_manager->clearScene();

   window->removeAllViewports();
   window->destroy();
   window = NULL;

   delete root;
   root = NULL;

   delete script_manager;
}

void RenderManager::mouseMovedAbsolute(int mouse_x, int mouse_y)
{
   gui_manager->injectMouseMoved(mouse_x, mouse_y);
}

void RenderManager::mouseMovedRelative(float mouse_x, float mouse_y)
{
   updateCameraRotation(mouse_x, mouse_y);
}

void RenderManager::mousePressed(int mouse_x, int mouse_y, int game_mouse)
{
   gui_manager->injectMousePressed(mouse_x, mouse_y, game_mouse);
}

void RenderManager::mouseReleased(int mouse_x, int mouse_y, int game_mouse)
{
   gui_manager->injectMouseReleased(mouse_x, mouse_y, game_mouse);
}

void RenderManager::stopRendering()
{
   render_listener->stopRendering();
}

void RenderManager::startRendering()
{
   root->startRendering();
}

void RenderManager::checkInput()
{
   game_manager->checkInput();
}

void RenderManager::init()
{
   root = NULL;
   window = NULL;
   scene_manager = NULL;
   render_listener = NULL;

   try
   {
      root = OGRE_NEW Root("","");  //resource/config files go here
      root->loadPlugin("RenderSystem_GL");  //prepares external dlls for later use

      RenderSystem* render_system = root->getRenderSystemByName("OpenGL Rendering Subsystem"); //just returns a pointer to an uninialized render system
      if (!render_system)
      {
         return;
      }

      root->setRenderSystem(render_system);
      //manually set configuration options
      render_system->setConfigOption("Full Screen", "No");
      render_system->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

      //initialize render system
      //automatically create the window and give it a title
      window = root->initialise(true, "Game Engine Programming");  

      //can create BSP or octree scene managers
      scene_manager = root->createSceneManager(ST_GENERIC, "Default Scene Manager");
      window->getCustomAttribute("WINDOW", &window_handle);

      //the Ogre viewport corresponds to a clipping region into which the contents of the camera view will be rendered in the window on each frame
      //by default, the size of the viewport matches the size of the window, but the viewport can be cropped
      //the camera represents a view into an existing scene and the viewport represents a region into which an existing camera will render its contents
      camera = scene_manager->createCamera("Camera");
      //z-order, left, top, width, height
      viewport = window->addViewport(camera, 0, 0, 0, 1.0, 1.0);  //assign a camera to a viewport (can have many cameras and viewports in a single window)
      viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
      viewport->setOverlaysEnabled(true);

      float actual_width = Ogre::Real(viewport->getActualWidth());
      float actual_height = Ogre::Real(viewport->getActualHeight());
      float aspect_ratio = actual_width/actual_height;
      camera->setAspectRatio(aspect_ratio);
   }

   catch(Ogre::Exception& e)  //an Ogre Exception, rethrow GameException
   {
      std::cout << e.what() << std::endl;
   }
}

void RenderManager::initialiseResourceGroup(std::string group_name)
{
   ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
   rgm.initialiseResourceGroup(group_name);  //pre-load the resources located in the specific paths (parse scripts)
}

void RenderManager::loadResourceGroup(std::string group_name)
{
   ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
   rgm.loadResourceGroup(group_name, true, true);  //load the resources in the specific paths

   gui_manager->loadResourceGroup(group_name);
}

void RenderManager::unloadResourceGroup(std::string group_name)
{
   ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
   rgm.destroyResourceGroup(group_name);   //completely remove resource information (could use clear instead of destroy)
}

void RenderManager::addPathResource(std::string file_name, std::string path_type, std::string group_name)
{
   ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
   rgm.addResourceLocation(file_name, path_type, group_name);  //"file_name" is the path
}
      
void RenderManager::addMeshResource(std::string file_name, std::string mesh_str, std::string group_name)
{
   ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();
   rgm.declareResource(file_name, mesh_str, group_name);
}

size_t RenderManager::getWindowHandle()
{
   return window_handle;
}

int RenderManager::getWindowWidth()
{
   return viewport->getActualWidth();
}

int RenderManager::getWindowHeight()
{
   return viewport->getActualHeight();
}

std::string RenderManager::getConfigFilePath()
{
   return game_manager->getConfigFilePath();
}

void RenderManager::buildSceneFromXML(std::string file_name)
{
   //ask GameManager (who asks ResourceManager) for config file directory for the currently loaded group
   std::string config_file_path = getConfigFilePath();

   TiXmlDocument doc((config_file_path + file_name).c_str());
   if (doc.LoadFile())
   {
      TiXmlNode* scene_node = doc.FirstChild("scene");
      if (scene_node)
      {
         TiXmlNode* camera_node = scene_node->FirstChild("camera");

         std::string camera_position_text = GameManager::textFromChildNode(camera_node, "position");
         float* result = game_manager->parseFloats(camera_position_text, 3);
         Vector3 camera_position(result[0], result[1], result[2]);
         camera->setPosition(camera_position);
         delete[] result;

         std::string camera_look_at_text = GameManager::textFromChildNode(camera_node, "look_at");
         result = game_manager->parseFloats(camera_look_at_text, 3);
         Vector3 camera_look(result[0], result[1], result[2]);
         camera->lookAt(camera_look);
         delete[] result;

         std::string camera_clip_distance_text = GameManager::textFromChildNode(camera_node, "clip_distance");
         result = game_manager->parseFloats(camera_clip_distance_text, 2);
         camera->setNearClipDistance(result[0]);
         camera->setFarClipDistance(result[1]);
         delete[] result;

         TiXmlNode* light_node = scene_node->FirstChild("light");

         std::string light_name_text = GameManager::textFromChildNode(light_node, "name");
         Light* light = scene_manager->createLight(light_name_text);

         std::string light_type_text = GameManager::textFromChildNode(light_node, "type");
         if (light_type_text == "directional")
         {
            light->setType(Light::LT_DIRECTIONAL);
         }
         else
         {
            light->setType(Light::LT_POINT);
         }

         std::string light_position_text = GameManager::textFromChildNode(light_node, "position");
         result = GameManager::parseFloats(light_position_text, 3);
         Vector3 light_pos(result[0], result[1], result[2]);
         if (light_type_text == "directional")
         {
            light->setDirection(light_pos);
         }
         else
         {
            light->setPosition(light_pos);
         }
         delete[] result;

         std::string light_color_text = GameManager::textFromChildNode(light_node, "color");
         result = GameManager::parseFloats(light_color_text, 3);
         light->setDiffuseColour(result[0], result[1], result[2]);
         delete[] result;

         TiXmlNode* scene_graph_node = scene_node->FirstChild("scene_graph");
         TiXmlNode* scene_graph_root_node = scene_graph_node->FirstChild("root");
         TiXmlNode* scene_graph_children = scene_graph_root_node->FirstChild("children");
         
         addSceneNodeChildren(scene_graph_children, scene_manager->getRootSceneNode());
      }
   }
   else
   {
      //set up a default camera and light source
   }

   gui_manager->buildGUIFromXML("gui.xml");
}

void RenderManager::addSceneNodeChildren(TiXmlNode* xml_node, SceneNode* parent_node)
{
   for(TiXmlNode* scene_graph_child_node = xml_node->FirstChild("child"); scene_graph_child_node; scene_graph_child_node = scene_graph_child_node->NextSibling())
   {  
      // Create the child node
      std::string child_name_text = GameManager::textFromChildNode(scene_graph_child_node, "name");
      SceneNode* curr_child_node = scene_manager->createSceneNode(child_name_text);
      parent_node->addChild(curr_child_node);
      
      //process the node entity
      TiXmlNode* scene_graph_child_entity = scene_graph_child_node->FirstChild("entity");
      if (scene_graph_child_entity)
      {
         std::string child_entity_name = GameManager::textFromChildNode(scene_graph_child_entity, "name");
         std::string child_entity_mesh = GameManager::textFromChildNode(scene_graph_child_entity, "mesh");
         std::string child_entity_material = GameManager::textFromChildNode(scene_graph_child_entity, "material");
         Ogre::Entity* child_entity = scene_manager->createEntity(child_entity_name, child_entity_mesh);
         child_entity->setMaterialName(child_entity_material); 
         curr_child_node->attachObject(child_entity);
      }

      // translate
      std::string child_translation_text = GameManager::textFromChildNode(scene_graph_child_node, "translation");
      float* result = GameManager::parseFloats(child_translation_text, 3);
      Vector3 child_translation(result[0], result[1], result[2]);
      curr_child_node->translate(child_translation, Node::TS_LOCAL);
      delete[] result;

      // rotate
      std::string child_rotation_text = GameManager::textFromChildNode(scene_graph_child_node, "rotation");
      result = GameManager::parseFloats(child_rotation_text, 4);

      Quaternion q(result[0], result[1], result[2], result[3]);
      curr_child_node->rotate(q, Node::TS_LOCAL);

      delete[] result;

      // scale
      std::string child_scale_text = GameManager::textFromChildNode(scene_graph_child_node, "scale");
      result = GameManager::parseFloats(child_scale_text, 3);
      Vector3 child_scale(result[0], result[1], result[2]);
      curr_child_node->scale(child_scale);
      delete[] result;

      addSceneNodeChildren(scene_graph_child_node->FirstChild("children"), curr_child_node);
   }
}

#include "GameManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include <iostream>
using namespace std;

//the problem is that in C++, it is not known when this initialization code will be called
//so we will use a function static variable that is initialized the first time the method is called
GameManager* GameManager::getGameManager()
{
   static GameManager game_manager;  //initialized when method is called the first time
   return &game_manager;  //won't go out of scope as game_manager is static
}

GameManager::GameManager()
{
   init();
   render_manager->startRendering();
}

GameManager::~GameManager()
{
   resource_manager->unloadResources();
   delete input_manager;
   delete resource_manager;
   delete render_manager;
}

void GameManager::init()
{
   render_manager = new RenderManager(this);

   resource_manager = new ResourceManager(this);
   resource_manager->loadFromXMLFile("resources.xml");
   resource_manager->loadResources("default");

   render_manager->buildSceneFromXML("scene.xml");

   input_manager = new InputManager(this);  //game_manager automatically set as an input listener
}

void GameManager::checkInput()
{
   input_manager->checkInput();
}

std::string GameManager::getConfigFilePath()
{
   return resource_manager->getConfigFilePath();
}

void GameManager::initialiseRenderResourceGroup(std::string group_name)
{
   render_manager->initialiseResourceGroup(group_name);
}

void GameManager::loadRenderResourceGroup(std::string group_name)
{
   render_manager->loadResourceGroup(group_name);
}

void GameManager::unloadRenderResourceGroup(std::string group_name)
{
   render_manager->unloadResourceGroup(group_name);
}

void GameManager::addPathResource(std::string file_name, std::string path_type, std::string group_name)
{
   render_manager->addPathResource(file_name, path_type, group_name);
}
      
void GameManager::addMeshResource(std::string file_name, std::string mesh_str, std::string group_name)
{
   render_manager->addMeshResource(file_name, mesh_str, group_name);
}

size_t GameManager::getWindowHandle()
{
   return render_manager->getWindowHandle();
}

int GameManager::getWindowWidth()
{
   return render_manager->getWindowWidth();
}

int GameManager::getWindowHeight()
{
   return render_manager->getWindowHeight();
}

void GameManager::keyPressed(std::string game_key) 
{
   if (game_key == "ESCAPE")
   {
      delete this;
      exit(0);
   }

   render_manager->updateCameraPosition(game_key);
}
void GameManager::keyReleased(std::string game_key) {}

void GameManager::mouseMoved(int mouse_x, int mouse_y, float mouse_x_rel, float mouse_y_rel)
{
   render_manager->mouseMovedAbsolute(mouse_x, mouse_y);
   render_manager->mouseMovedRelative(mouse_x_rel, mouse_y_rel);
}
void GameManager::mousePressed(int mouse_x, int mouse_y, int game_mouse)
{
   render_manager->mousePressed(mouse_x, mouse_y, game_mouse);
}
void GameManager::mouseReleased(int mouse_x, int mouse_y, int game_mouse)
{
   render_manager->mouseReleased(mouse_x, mouse_y, game_mouse);
}

float* GameManager::parseFloats(std::string& str, int num_tokens)
{
   int sz = str.size();
   float* values = new float[num_tokens];

   int loc = 0;
   int token_index = 0;
   while (loc < sz)
   {
      int index = str.find(',', loc);

      if (index == -1)
      {
         index = sz-1;
      }

      string sub = str.substr(loc, index);
      values[token_index] = parseFloat(sub);

      loc = index + 1;
      token_index++;
   }

   return values;
}

float GameManager::parseFloat(std::string& str)
{
   return atof(str.c_str());
}

int GameManager::parseInt(std::string& str)
{
   return atoi(str.c_str());
}

std::string GameManager::textFromChildNode(TiXmlNode* parent_node, const char* child_element_name)
{
   TiXmlNode* child_node = parent_node->FirstChild(child_element_name);
   if (child_node)
   {
      TiXmlElement* child_node_element = (TiXmlElement*) child_node->ToElement();
      std::string child_node_text = child_node_element->GetText();
      return child_node_text;
   }
   else
   {
      return "";
   }
}

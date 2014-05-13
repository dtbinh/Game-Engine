#include "GUIManager.h"
#include "RenderManager.h"
#include "GameManager.h"

#include <iostream>
using namespace std;

GUIManager::GUIManager(RenderManager* rm)
{
   render_manager = rm;
   ogre_platform = new MyGUI::OgrePlatform();
   my_gui = new MyGUI::Gui();

   all_widgets = new TableAVL<GUIWidgetScript, string >(&GUIWidgetScript::compare_items, &GUIWidgetScript::compare_keys);
}

GUIManager::~GUIManager()
{
   AVLTreeIterator<GUIWidgetScript>* all_widgets_iter = all_widgets->tableIterator();
   while(all_widgets_iter->hasNext())
   {
      GUIWidgetScript* widget = all_widgets_iter->next();
      delete widget;
   }
   delete all_widgets_iter;
   delete all_widgets;

   //unload resources here due to loading limitations
   MyGUI::ResourceManager& mygui_resource_manager = MyGUI::Singleton<MyGUI::ResourceManager>::getInstance();
   mygui_resource_manager.clear();

   //this used to crash!
   my_gui->destroyAllChildWidget();
   my_gui->shutdown();
   delete my_gui;
   my_gui = NULL;

   ogre_platform->shutdown();
   delete ogre_platform;
   ogre_platform = NULL;

   render_manager = NULL;
}

void GUIManager::loadResourceGroup(std::string resource_group_name)
{
   //can only be loaded once!! (otherwise, random crashes occur)
   //this may be a problem

   ogre_platform->initialise(render_manager->getRenderWindow(), render_manager->getSceneManager(), resource_group_name);
   my_gui->initialise();

   //ogre_platform->getRenderManagerPtr()->setSceneManager(render_manager->getSceneManager());
   //ogre_platform->getRenderManagerPtr()->setRenderWindow(render_manager->getRenderWindow());
   //ogre_platform->getRenderManagerPtr()->setActiveViewport(index);

   MyGUI::ResourceManager& mygui_resource_manager = MyGUI::Singleton<MyGUI::ResourceManager>::getInstance();
   mygui_resource_manager.load("my_gui_core.xml");
}

void GUIManager::genericGUIDelegate(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id) 
{
   const string& _name = _sender->getName();
   string name = _name;

   GUIWidgetScript* widget_script = all_widgets->tableRetrieve(&name);

   string file_name = widget_script->getFileName();
   string script_name = widget_script->getScriptName();
   string object_name = widget_script->getObjectName();

   render_manager->executeScript(file_name, script_name, object_name);
}

void GUIManager::injectMousePressed(int mouse_x, int mouse_y, int game_mouse)
{
   MyGUI::InputManager& mygui_input_manager = MyGUI::Singleton<MyGUI::InputManager>::getInstance();

   //left button is 0, right button is 1
   mygui_input_manager.injectMousePress(mouse_x, mouse_y, MyGUI::MouseButton::Enum(game_mouse));
}

void GUIManager::injectMouseReleased(int mouse_x, int mouse_y, int game_mouse)
{
   MyGUI::InputManager& mygui_input_manager = MyGUI::Singleton<MyGUI::InputManager>::getInstance();
   mygui_input_manager.injectMouseRelease(mouse_x, mouse_y, MyGUI::MouseButton::Enum(game_mouse));
}

void GUIManager::injectMouseMoved(int mouse_x, int mouse_y)
{
   MyGUI::InputManager& mygui_input_manager = MyGUI::Singleton<MyGUI::InputManager>::getInstance();
   mygui_input_manager.injectMouseMove(mouse_x, mouse_y, 0);
}

void GUIManager::buildGUIFromXML(std::string file_name)
{
   std::string config_file_path = render_manager->getConfigFilePath();

   TiXmlDocument doc((config_file_path + file_name).c_str());
   if (doc.LoadFile())
   {
      TiXmlNode* gui_node = doc.FirstChild("gui");
      if (gui_node)
      {
         TiXmlNode* buttons_node = gui_node->FirstChild("buttons");
         if (buttons_node)
         {
            addButtons(buttons_node);
         }
      }
   }
   else
   {
      //do nothing
   }
}

void GUIManager::addButtons(TiXmlNode* buttons_node)
{

   for(TiXmlNode* button_node = buttons_node->FirstChild("button"); button_node; button_node = button_node->NextSibling())
   {  
      std::string name_text = GameManager::textFromChildNode(button_node, "name");
      std::string caption_text = GameManager::textFromChildNode(button_node, "caption");
      std::string skin_text = GameManager::textFromChildNode(button_node, "skin");
      std::string position_text = GameManager::textFromChildNode(button_node, "position");
      float* position = GameManager::parseFloats(position_text, 2);
      std::string size_text = GameManager::textFromChildNode(button_node, "size");
      float* size = GameManager::parseFloats(size_text, 2);
      std::string align_text = GameManager::textFromChildNode(button_node, "align");
      std::string layer_text = GameManager::textFromChildNode(button_node, "layer");
      std::string object_name_text = GameManager::textFromChildNode(button_node, "object_name");
      std::string file_name_text = GameManager::textFromChildNode(button_node, "file_name");
      std::string script_name_text = GameManager::textFromChildNode(button_node, "script_name");
      std::string font_size_text = GameManager::textFromChildNode(button_node, "font");
      int font_size = (int) GameManager::parseFloat(font_size_text);

      MyGUI::Button* b = my_gui->createWidget<MyGUI::Button>(skin_text, position[0], position[1], size[0], size[1], MyGUI::Align::Default, layer_text, name_text);
      b->setCaption(caption_text);
      b->setFontHeight(font_size);
      b->setTextColour(MyGUI::Colour(0,0,0));
      delete[] position;
      delete[] size;
      b->eventMouseButtonPressed += newDelegate(this, &GUIManager::genericGUIDelegate);

      GUIWidgetScript* widget_script = new GUIWidgetScript(b, name_text);
      widget_script->setFileName(file_name_text);
      widget_script->setScriptName(script_name_text);
      widget_script->setObjectName(object_name_text);
      all_widgets->tableInsert(widget_script);
   }

}

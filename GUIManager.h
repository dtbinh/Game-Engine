#if !defined (GUI_MANAGER)
#define GUI_MANAGER

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "TinyXML.h"
#include "CSC2110/TableAVL.h"
#include "GUIWidgetScript.h"

class RenderManager;

class GUIManager
{

   private:
      MyGUI::Gui* my_gui;
      MyGUI::OgrePlatform* ogre_platform;
      TableAVL<GUIWidgetScript, string>* all_widgets;

      RenderManager* render_manager;

      void addButtons(TiXmlNode* buttons_node);
      void addComboBoxes(TiXmlNode* combo_boxes_node);

   public:
      GUIManager(RenderManager* render_manager);
      virtual ~GUIManager();

      void loadResourceGroup(std::string resource_group_name);
      void unloadResourceGroup();
      void buildGUIFromXML(std::string file_name);

      void injectMouseMoved(int mouse_x, int mouse_y);
      void injectMousePressed(int mouse_x, int mouse_y, int game_mouse);
      void injectMouseReleased(int mouse_x, int mouse_y, int game_mouse);

      void genericGUIDelegate(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

};

#endif

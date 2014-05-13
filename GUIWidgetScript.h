#if !defined (GUI_WIDGETSCRIPT)
#define GUI_WIDGETSCRIPT

#include "MyGUI.h"
#include <string>
using namespace std;

class GUIWidgetScript
{

   private:
      MyGUI::Widget* widget;
      string widget_name;

      string file_name;
      string script_name;
      string object_name;

   public:
      GUIWidgetScript(MyGUI::Widget* widget, string widget_name);
      virtual ~GUIWidgetScript();

      string& getFileName();
      string& getScriptName();
      string& getObjectName();
      void setFileName(string file_name);
      void setScriptName(string script_name);
      void setObjectName(string object_name);
      string* getKey();  //the name of the widget is the search key

      static int compare_items(GUIWidgetScript* widget_1, GUIWidgetScript* widget_2);
      static int compare_keys(string* key, GUIWidgetScript* widget);

};

#endif

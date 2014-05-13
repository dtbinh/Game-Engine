#include "GUIWidgetScript.h"

GUIWidgetScript::GUIWidgetScript(MyGUI::Widget* w, string wn)
{
   widget = w;
   widget_name = wn;
}

GUIWidgetScript::~GUIWidgetScript()
{
   widget = NULL;
}

string* GUIWidgetScript::getKey()
{
   return &widget_name;
}

string& GUIWidgetScript::getFileName()
{
   return file_name;
}

string& GUIWidgetScript::getScriptName()
{
   return script_name;
}

string& GUIWidgetScript::getObjectName()
{
   return object_name;
}

void GUIWidgetScript::setFileName(string fn)
{
   file_name = fn;
}

void GUIWidgetScript::setScriptName(string sn)
{
   script_name = sn;
}

void GUIWidgetScript::setObjectName(string on)
{
   object_name = on;
}

int GUIWidgetScript::compare_items(GUIWidgetScript* widget_1, GUIWidgetScript* widget_2)
{
   string* key_1 = widget_1->getKey();
   return compare_keys(key_1, widget_2);
}

int GUIWidgetScript::compare_keys(string* key, GUIWidgetScript* widget)
{
   const char* key_1 = key->c_str();
   const char* key_2 = widget->getKey()->c_str();
   return strcmp(key_1, key_2);
}

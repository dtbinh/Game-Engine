#include "ResourceManager.h"
#include "GameManager.h"

#include "MeshResource.h"
#include "PathResource.h"

#include "TinyXML.h"

std::string ResourceManager::getCurrentGroupName()
{
   return current_group;
}

ResourceManager::ResourceManager(GameManager* gm)
{
   game_manager = gm;
   current_group = "";
   all_resources = new TableAVL<KeyedListArray<GameResource>, string >(&KeyedListArray<GameResource>::compare_items, &KeyedListArray<GameResource>::compare_keys);
}

ResourceManager::~ResourceManager()
{
   AVLTreeIterator<KeyedListArray<GameResource> >* all_lists_iter = all_resources->tableIterator();

   while(all_lists_iter->hasNext())
   {
      KeyedListArray<GameResource>* list = all_lists_iter->next();
      ListArrayIterator<GameResource>* list_iter = list->iterator();

      while(list_iter->hasNext())
      {
         GameResource* resource = list_iter->next();
         delete resource;
      }

      delete list_iter;
      delete list;
   }

   delete all_lists_iter;
   delete all_resources;
}

void ResourceManager::loadResources(std::string group_name)
{
   if (current_group == group_name) return;
   if (current_group != "") unloadResources();

   KeyedListArray<GameResource>* list = all_resources->tableRetrieve(&group_name);

   //make sure that the PATHs are all loaded first
   ListArrayIterator<GameResource>* iter = list->iterator();
   while(iter->hasNext())
   {
      GameResource* resource = iter->next();
      if (resource->getResourceType() == PATH)
      {
         resource->load();
      }
   }
   delete iter;

   //load the rest
   iter = list->iterator();
   while(iter->hasNext())
   {
      GameResource* resource = iter->next();
      GameResourceType grt = resource->getResourceType();
      if (grt != PATH)
      {
         resource->load();
      }
   }
   delete iter;

   //initialize and load the Ogre group
   game_manager->initialiseRenderResourceGroup(group_name);
   game_manager->loadRenderResourceGroup(group_name);

cout << "meshes should all be loaded" << endl;
   current_group = group_name;
}

void ResourceManager::unloadResources()
{
   std::string group_name = current_group;

   KeyedListArray<GameResource>* list = all_resources->tableRetrieve(&group_name);  //old scope

   ListArrayIterator<GameResource>* iter = list->iterator();
   while(iter->hasNext())
   {
      GameResource* resource = iter->next();       
      resource->unload();
   }

   delete iter;

   game_manager->unloadRenderResourceGroup(group_name);
   current_group = "";
}

std::string ResourceManager::getConfigFilePath()
{
   GameResource* gr = findResourceByID(1);
   return gr->getResourceFileName();
}

//id 1 will be the config file path for the current resource group
GameResource* ResourceManager::findResourceByID(unsigned int resource_id)
{
   if (current_group == "") return NULL;

   KeyedListArray<GameResource>* current_list = all_resources->tableRetrieve(&current_group);
   ListArrayIterator<GameResource>* current_list_iter = current_list->iterator();

   while(current_list_iter->hasNext())
   {
      GameResource* game_resource = current_list_iter->next();
      int test_resource_id = game_resource->getResourceID();

      if (test_resource_id == resource_id)
      {
         delete current_list_iter;
         return game_resource;
      }
   }
   delete current_list_iter;

   return NULL;  //the requested resource_id was not found
}

void ResourceManager::loadFromXMLFile(std::string file_name)
{
   int resource_count = 1;

   TiXmlDocument doc(file_name.c_str());
   if (doc.LoadFile())
   {
      TiXmlNode* resource_tree = doc.FirstChild("resources");
      if (resource_tree)
      {
         //Enumerate resource objects (eventually, child will be false and loop will terminate)
         for(TiXmlNode* child = resource_tree->FirstChild(); child; child = child->NextSibling())
         {
            TiXmlElement* resource_element = child->ToElement();
            if(resource_element)
            {
               GameResource* game_resource = NULL;

               TiXmlElement* uid_element = (TiXmlElement*) resource_element->FirstChild("uid");
               unsigned int uid = atoi(uid_element->GetText());
               TiXmlElement* type_element = (TiXmlElement*) resource_element->FirstChild("resource_type");
               std::string resource_type = type_element->GetText();
               TiXmlElement* filename_element = (TiXmlElement*) resource_element->FirstChild("file_name");
               std::string file_name = filename_element->GetText();
               TiXmlElement* scope_element = (TiXmlElement*) resource_element->FirstChild("scope");
               std::string scope = scope_element->GetText();

               if (resource_type == "path")
               {
                  game_resource = new PathResource(uid, scope, file_name, PATH, game_manager);
               }
               else if (resource_type == "mesh")
               {
                  game_resource = new MeshResource(uid, scope, file_name, MESH, game_manager);
               }

               if (game_resource)
               {
                  KeyedListArray<GameResource>* list = all_resources->tableRetrieve(&scope);  //place in the appropriate list
                  if (list)
                  {
                     list->add(game_resource);
                  }
                  else
                  {
                     list = new KeyedListArray<GameResource>(scope);
                     list->add(game_resource);
                     all_resources->tableInsert(list);
                  }
                  resource_count++;
               }
            }  //individual resource element
            else 
            {
               std::string str = "Resource manager failed to load resource meta data ";
               str.append("" + resource_count);
               str.append(".");
               //game_manager->logException(str);
            }

         }  //resource elements for loop

      }  //resource tree
      else 
      {
         //THROW_EXCEPTION(1, "Resource manager failed to load any resources.");
      }

   }  //document
   else 
   {
      //THROW_EXCEPTION(1, "Resource manager failed to find the resource metadata file.");
   }
}

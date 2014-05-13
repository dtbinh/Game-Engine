#if !defined (RESOURCE_MANAGER)
#define RESOURCE_MANAGER

#include <iostream>
using namespace std;

#include "CSC2110/KeyedListArray.h"
#include "CSC2110/TableAVL.h"
#include "GameResource.h"

class GameManager;

class ResourceManager
{
   private:
      GameManager* game_manager;

      std::string current_group;
      TableAVL<KeyedListArray<GameResource>, string>* all_resources;

   public:
      ResourceManager(GameManager* gm);
      virtual ~ResourceManager();

      void loadFromXMLFile(std::string file_name);
      GameResource* findResourceByID(unsigned int resource_ID);
      std::string getConfigFilePath();
      
      void unloadResources();
      void loadResources(std::string group_name);
      std::string getCurrentGroupName();
};

#endif


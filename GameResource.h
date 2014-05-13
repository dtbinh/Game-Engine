#if !defined (GAME_RESOURCE)
#define GAME_RESOURCE

#include "GameResourceType.h"
#include <string>

class GameResource
{
   protected:
      unsigned int resource_id;  //unique identifier
      std::string resource_group_name;
      std::string resource_file_name;
      GameResourceType resource_type;

   public:
      GameResource(unsigned int id, std::string group_name, std::string file_name, GameResourceType type);
      virtual ~GameResource();

      virtual void load() {};
      virtual void unload() {};

      unsigned int getResourceID();
      std::string getResourceGroupName();
      std::string getResourceFileName();
      GameResourceType getResourceType();
};

#endif
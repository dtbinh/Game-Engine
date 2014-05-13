#if !defined (PATH_RESOURCE)
#define PATH_RESOURCE

#include "GameResource.h"

class GameManager;

class PathResource : public GameResource
{
   private:
      GameManager* game_manager;

   public:
      PathResource(unsigned int id, std::string group_name, std::string file_name, GameResourceType type, GameManager* gm);
      virtual ~PathResource();

      std::string getPath();

      virtual void load();
      virtual void unload();
};

#endif
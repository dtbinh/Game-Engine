#if !defined (MESH_RESOURCE)
#define MESH_RESOURCE

#include "GameResource.h"

class GameManager;

class MeshResource : public GameResource
{
   private:
      GameManager* game_manager;

   public:
      MeshResource(unsigned int id, std::string group_name, std::string file_name, GameResourceType type, GameManager* gm);
      virtual ~MeshResource();

      virtual void load();
      virtual void unload();
};

#endif
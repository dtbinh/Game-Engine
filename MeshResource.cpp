#include "MeshResource.h"
#include "GameManager.h"

MeshResource::MeshResource(unsigned int id, std::string group_name, std::string mesh_name, GameResourceType type, GameManager* gm) : GameResource(id, group_name, mesh_name, type)
{
   game_manager = gm;
}

MeshResource::~MeshResource()
{
   game_manager = NULL;
}

void MeshResource::load()
{
   game_manager->addMeshResource(getResourceFileName(), "Mesh", getResourceGroupName());
}

void MeshResource::unload()
{}
#include "PathResource.h"
#include "GameManager.h"

PathResource::PathResource(unsigned int id, std::string group_name, std::string path, GameResourceType type, GameManager* gm) : GameResource(id, group_name, path, type)
{
   game_manager = gm;
}

PathResource::~PathResource()
{
   game_manager = NULL;
}

void PathResource::load()
{
   game_manager->addPathResource(getResourceFileName(), "FileSystem", getResourceGroupName());
}

void PathResource::unload()
{}
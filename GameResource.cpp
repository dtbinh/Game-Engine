#include "GameResource.h"

GameResource::GameResource(unsigned int id, std::string group_name, std::string file_name, GameResourceType type)
{
   resource_id = id;
   resource_group_name = group_name;
   resource_file_name = file_name;
   resource_type = type;
}

GameResource::~GameResource()
{
}

unsigned int GameResource::getResourceID()
{
   return resource_id;
}

std::string GameResource::getResourceGroupName()
{
   return resource_group_name;
}

std::string GameResource::getResourceFileName()
{
   return resource_file_name;
}

GameResourceType GameResource::getResourceType()
{
   return resource_type;
}

#if !defined (SCRIPT_MANAGER)
#define SCRIPT_MANAGER

#include <string>
#include "luainc.h"

class RenderManager;

class ScriptManager
{

   private:
      lua_State* L;
      RenderManager* render_manager;

   public:
      ScriptManager(RenderManager* render_manager);
      virtual ~ScriptManager();
      void executeScript(std::string& script_name, std::string& function_name, int num_params, int num_results, double* params, double* results);

};

#endif

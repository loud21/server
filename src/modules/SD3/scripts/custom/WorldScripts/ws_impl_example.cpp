#include "WorldScript.h"

class ws_startup_example : public WorldScript
{
public:
    ws_startup_example() : WorldScript() {}

    void OnStartup() override
    {
        sLog.outString("World successfully started");
    }
};

class ws_config_load_example : public WorldScript
{
public:
    ws_config_load_example() : WorldScript() {}

    void OnConfigLoad(bool reload) override
    {
        if (reload)
            sLog.outString("Config settings successfully reloaded");
        else
            sLog.outString("Config setting loaded");
    }
};

void AddSC_ws_impl_example()
{
    RegisterWorldScript(WorldEvents::WORLD_EVENT_ON_STARTUP, ws_startup_example);
    RegisterWorldScript(WorldEvents::WORLD_EVENT_ON_CONFIG_LOAD, ws_config_load_example);
}
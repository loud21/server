/*
Single file PlayerScript class with it's own registrar
Written by Luka/Loud21 : https://github.com/loud21 for MangosThree
*/

#include <map>
#include <string>
#include <functional>

class World;

enum WorldEvents
{
    WORLD_EVENT_ON_STARTUP = 0,
    WORLD_EVENT_ON_CONFIG_LOAD,
};

class WorldScript
{
public:
    virtual ~WorldScript() {}

    virtual void OnStartup() {}
    virtual void OnConfigLoad(bool /*reload*/) { }

    virtual void OnEvent(WorldEvents events, void* args = nullptr)
    {
        switch (events)
        {
        case WORLD_EVENT_ON_STARTUP:
            OnStartup();
            break;

        case WORLD_EVENT_ON_CONFIG_LOAD:
        {
            bool reload = static_cast<bool>(args);
            if (reload)
                OnConfigLoad(true);
            else
                OnConfigLoad(false);
            break;
        }
        }
    }

protected:
};

typedef std::function<WorldScript*()> WorldScriptCreator;
typedef std::multimap<WorldEvents, WorldScriptCreator> WorldScriptRegistry;

class WorldScriptMgr
{
public:
    static WorldScriptMgr* instance()
    {
        static WorldScriptMgr instance;
        return &instance;
    }
    WorldScriptRegistry& GetRegistry() { return m_registry; }

    void Execute(WorldEvents events, void* args = nullptr)
    {
        auto result = m_registry.equal_range(events);
        for (auto itr = result.first; itr != result.second; itr++)
        {
            WorldScriptCreator creator = itr->second;
            WorldScript* script = creator();
            script->OnEvent(events, args);
        }
    }

private:
    WorldScriptRegistry m_registry;
};

#define sWorldScriptMgr WorldScriptMgr::instance()

template<class T>
class WorldScriptRegistrar
{
public:
    WorldScriptRegistrar(WorldEvents events, std::string const& name)
    {
        WorldScriptRegistry& registry = WorldScriptMgr::instance()->GetRegistry();
        registry.insert({ events, &WorldScriptRegistrar<T>::Create });
    }

    static WorldScript* Create()
    {
        return new T();
    }
};

#define RegisterWorldScript(E, T) \
    static WorldScriptRegistrar<T> T##Registrar(E, #T);
/*
Single file PlayerScript class with it's own registrar
Written by Luka/Loud21 : https://github.com/loud21 for MangosThree
*/

#include <map>
#include <string>
#include <functional>

class Player;

enum PlayerEvents
{
    PLAYER_EVENT_ON_LOGIN = 0,
    PLAYER_EVENT_ON_LOGOUT = 1,
    PLAYER_EVENT_ON_KILL_CREATURE = 2,
};

class PlayerScript
{
public:
    virtual ~PlayerScript() {}

    virtual void OnLogin(Player* player) {}
    virtual void OnLogout(Player* player) {}
    virtual void OnCreatureKill(Player* player, Creature* creature) {}

    virtual void OnEvent(PlayerEvents events, Player* player, void* args = nullptr)
    {
        switch (events)
        {
        case PLAYER_EVENT_ON_LOGIN:
            OnLogin(player);
            break;
        case PLAYER_EVENT_ON_LOGOUT:
            OnLogout(player);
            break;
        case PLAYER_EVENT_ON_KILL_CREATURE:
        {
            Creature* creature = static_cast<Creature*>(args);

            if (creature)
                OnCreatureKill(player, creature);
        }
        break;
        }
    }
};

typedef std::function<PlayerScript*()> PlayerScriptCreator;
typedef std::multimap<PlayerEvents, PlayerScriptCreator> PlayerScriptRegistry;

class PlayerScriptMgr
{
private:
    PlayerScriptMgr() = default;

public:
    PlayerScriptMgr(const PlayerScriptMgr&) = delete;
    PlayerScriptMgr(PlayerScriptMgr&&) = delete;

    static PlayerScriptMgr* instance()
    {
        static PlayerScriptMgr instance;
        return &instance;
    }
    PlayerScriptRegistry& GetRegistry() { return m_registry; }

    void Execute(PlayerEvents events, Player* player,void* args = nullptr)
    {
        auto result = m_registry.equal_range(events);
        for (auto itr = result.first; itr != result.second; itr++)
        {
            PlayerScriptCreator creator = itr->second;
            PlayerScript* script = creator();
            script->OnEvent(events, player, args);
        }
    }

private:
    PlayerScriptRegistry m_registry;
};

#define sPlayerScriptMgr PlayerScriptMgr::instance()

template<class T>
class PlayerScriptRegistrar
{
public:
    PlayerScriptRegistrar(PlayerEvents events, std::string const& name)
    {
        PlayerScriptRegistry& registry = PlayerScriptMgr::instance()->GetRegistry();
        registry.insert({ events, &PlayerScriptRegistrar<T>::Create });
    }

    static PlayerScript* Create()
    {
        return new T();
    }
};

#define RegisterPlayerScript(E, T) \
    static PlayerScriptRegistrar<T> T##Registrar(E, #T);
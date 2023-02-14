#include "PlayerScript.h"

class ps_login_example : public PlayerScript
{
public:
    ps_login_example() : PlayerScript() {}

    void OnLogin(Player* player) override
    {
        player->GetSession()->SendAreaTriggerMessage("Welcome back!");
    }
};

class ps_logout_example : public PlayerScript
{
public:
    ps_logout_example() : PlayerScript() {}

    void OnLogout(Player* player) override
    {
        player->GetSession()->SendAreaTriggerMessage("Goodbye!");
    }
};

class ps_creature_kill_example : public PlayerScript
{
public:
    ps_creature_kill_example() : PlayerScript() {}

    void OnCreatureKill(Player* player, Creature* creature) override
    {
        std::string msg{ "Congratulations you've killed " + std::string(creature->GetName()) };
        player->GetSession()->SendAreaTriggerMessage(msg.c_str());
    }
};

void AddSC_ps_impl_example()
{
    RegisterPlayerScript(PlayerEvents::PLAYER_EVENT_ON_LOGIN, ps_login_example);
    RegisterPlayerScript(PlayerEvents::PLAYER_EVENT_ON_LOGOUT, ps_logout_example);
    RegisterPlayerScript(PlayerEvents::PLAYER_EVENT_ON_KILL_CREATURE, ps_creature_kill_example);
}
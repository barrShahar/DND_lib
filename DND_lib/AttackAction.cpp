#include "Action.h"
#include "SubjectRoom_mt.h"
#include "StringActionResponse.h"
#include "AttackProcessor.h"
#include "NotificationService.h"

#include <cassert>
#include <optional>

namespace dnd_game {

    using AttackResponsePtr = std::unique_ptr<AttackResponse>;
    bool AttackAndNotifyVictim(Player& a_attacker, Player& a_victim)
    {
        bool didAttack = false;
        a_victim.WithLock([&]()
            {
                if (a_victim.IsAlive())
                {
                    const Number dmgPoints = a_attacker.GetDmgPoints();
                    const Number victimHP = a_victim.GetHealthPoints();
                    Number currvictimHP = std::max(0, victimHP - dmgPoints);

                    a_victim.SetHealthPoints(currvictimHP);

                    const std::string victimMessage = "You've been attack by " + a_attacker.GetName() + "! " +
                        "HP decreased from " + std::to_string(victimHP) + " to " + std::to_string(currvictimHP) + ".";
                    a_victim.NotifyPlayer(victimMessage);

                    didAttack = true;
                }
            });

        return didAttack;
    }

    Number InflictedDamage(const AttackResponsePtr& a_attackResponse)
    {
        return a_attackResponse->originalHp - a_attackResponse->currentHp;
    }

    std::string const RoomAttackedMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        return a_attackerName + " attacked " + a_attackResponse->adressedNameMsg + " for " + std::to_string(InflictedDamage(a_attackResponse)) + " damage.";
    }

    std::string const AtackableLostHpMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        return a_targetName + " HP decreased from " + std::to_string(a_attackResponse->originalHp) + " to " + std::to_string(a_attackResponse->currentHp) + ".";
    }

    std::string const YourLostHpMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        return "Your HP decreased from " + std::to_string(a_attackResponse->originalHp) + " to " + std::to_string(a_attackResponse->currentHp) + " damage.";
    }

    std::string const YourHpMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        return "Your HP decreased from " + std::to_string(a_attackResponse->originalHp) + " to " + std::to_string(a_attackResponse->currentHp) + ".";
    }

    std::string const YouAttackedMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        Number const inflictedDamage = a_attackResponse->originalHp - a_attackResponse->currentHp;
        return "You've attacked " + a_attackResponse->adressedNameMsg + " for " + std::to_string(inflictedDamage) + " damage.";
    }

    std::string const YouWereAttackedBackMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        return "You've been attacked back by " + a_attackResponse->adressedNameMsg + " for " + std::to_string(a_attackResponse->returnedDmg) + " damage.";
    }

    std::string const YouWereAttackedMsg(const std::string& a_attackerName, const std::string& a_attackerAdressName, const AttackResponsePtr& a_attackResponse)
    {
        Number inflictedDamage = InflictedDamage(a_attackResponse);
        return "You've been attacked by " + a_attackerAdressName + " for " + std::to_string(inflictedDamage) + " damage.";
    }

    std::string const YouBeenKilledMsg(const std::string& a_attackerName, const AttackResponsePtr& a_attackResponse)
    {
        return "You've been killed by " + a_attackResponse->adressedNameMsg + "!";
    }

    std::string const YouKilledMsg(const std::string& a_attackerName, const AttackResponsePtr& a_attackResponse)
    {
        return "You've killed " + a_attackResponse->adressedNameMsg + "!";
    }

    std::string const RoomKillMsg(const std::string& a_attackerName, const std::string& a_targetName, const AttackResponsePtr& a_attackResponse)
    {
        return a_attackerName + " killed " + a_attackResponse->adressedNameMsg + "!";
    }

    std::unique_ptr<ActionResponse> AttackAction::Act(Dungeon_mt& a_dungeon, Player& a_player) const
    {
        // Get the room number of the player
        const Number playerRoomNumber = a_player.GetRoomNumber();

        // Get the room from the dungeon
        const Room& room = a_dungeon.GetRoom(playerRoomNumber);

        // Lock room via dungeon (encapsulation):
        std::unique_ptr<ActionResponse> result = std::make_unique<StringActionResponse>("This is a test.");;
        a_dungeon.ExecuteWithRoomLock(playerRoomNumber, [&a_player, &result](Room& room)
        {
            // find the Attackable object in the room:
            std::string const& attackable_name = a_player.GetArguments();
            std::optional<std::reference_wrapper<IAttackable>> attackable_opt = room.GetAttackable_NoLock(attackable_name);
            if (!attackable_opt.has_value())
            {
                result = std::make_unique<StringActionResponse>("Target '" + attackable_name + "' was not found in this room."); 
                return;
            }

            // attack the attackable object:
            IAttackable& attackable = attackable_opt.value();
            AttackResponsePtr const attackableResponse = attackable.TakeDamage(a_player.GetDmgPoints());
            
            Number const returnedDamage = attackableResponse->returnedDmg;
            AttackResponsePtr playerAttackResponse = a_player.TakeDamage(returnedDamage);

            // Notify room, attcker, and attackable:
            std::string const attackableMessageName = attackable_name;

            std::vector<std::string> const PlayersNames = room.GetNamesVec();
            for (auto playerName: PlayersNames)
            {
                if (playerName == a_player.GetName())
                {
                    // send attackable message via method that I will do in the next task
                    a_player.NotifyPlayer(YouAttackedMsg(a_player.GetName(), attackable_name, attackableResponse));
                    a_player.NotifyPlayer(AtackableLostHpMsg(a_player.GetName(), attackable_name, attackableResponse));

                    if (playerAttackResponse->returnedDmg > 0)
                    {
                        a_player.NotifyPlayer(YouWereAttackedMsg(a_player.GetName(), attackable_name, attackableResponse));
                        a_player.NotifyPlayer(AtackableLostHpMsg(a_player.GetName(), attackable_name, playerAttackResponse));
                    }

                    if (!attackableResponse->isInPlay)
                    {
                        a_player.NotifyPlayer(YouKilledMsg(a_player.GetName(), attackableResponse));
                    }

                    if (!playerAttackResponse->isInPlay)
                    {
                        a_player.NotifyPlayer(YouBeenKilledMsg(a_player.GetName(), playerAttackResponse));
                        room.Unregister_NoLock(a_player);
                    }
                }
                else if (playerName == attackable_name)
                {
                    
                    room.NotifyPlayer_NoLock(playerName, YouWereAttackedMsg(a_player.GetName(), attackableMessageName, attackableResponse));
                    room.NotifyPlayer_NoLock(playerName, YourLostHpMsg(a_player.GetName(), attackableMessageName, attackableResponse));
                    
                   if (attackableResponse->returnedDmg > 0)
                   {
                    room.NotifyPlayer_NoLock(playerName, YouAttackedMsg(playerName, a_player.GetName(), playerAttackResponse));
                    room.NotifyPlayer_NoLock(playerName, AtackableLostHpMsg(playerName, a_player.GetName(), playerAttackResponse));
                   }

                    if (!playerAttackResponse->isInPlay)
                    {
                        room.NotifyPlayer_NoLock(playerName, YouKilledMsg(a_player.GetName(), playerAttackResponse));
                    }
                    if (!attackableResponse->isInPlay)
                    {
                        room.NotifyPlayer_NoLock(playerName, YouBeenKilledMsg(a_player.GetName(), attackableResponse));
                        room.Unregister_NoLock(attackable_name);
                    }
                }
                else
                {
                    room.NotifyPlayer_NoLock(playerName, RoomAttackedMsg(a_player.GetName(), attackable_name, attackableResponse));
                    room.NotifyPlayer_NoLock(playerName, AtackableLostHpMsg(a_player.GetName(), attackable_name, attackableResponse));

                    if (attackableResponse->returnedDmg > 0)
                    {
                        room.NotifyPlayer_NoLock(playerName, RoomAttackedMsg(playerName, a_player.GetName(), attackableResponse));
                        room.NotifyPlayer_NoLock(playerName, AtackableLostHpMsg(a_player.GetName(), attackable_name, playerAttackResponse));
                    }

                    if (!attackableResponse->isInPlay)
                    {
                        room.NotifyPlayer_NoLock(playerName, RoomKillMsg(a_player.GetName(),attackable_name, attackableResponse));
                    }

                    if (!playerAttackResponse->isInPlay)
                    {
                        room.NotifyPlayer_NoLock(playerName, YouBeenKilledMsg(a_player.GetName(), attackableResponse));
                        room.Unregister_NoLock(a_player);
                    }
                }
            } 
        });
        return result;
    } 

} // namespace dnd_game

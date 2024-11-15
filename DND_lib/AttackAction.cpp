#include "Action.h"
#include "SubjectRoom_mt.h"
#include "StringActionResponse.h"
#include "AttackService.h"

namespace dnd_game {

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

    std::unique_ptr<ActionResponse> AttackAction::Act(Dungeon_mt& a_dungeon, Player& a_player) const
    {
        const std::string& nameToAttack = a_player.GetArguments();

        std::string roomNotificationMessage;
        std::string actionResponseMessage;

        // Lock the room and perform the attack logic
        a_dungeon.ExecuteWithRoomLock(
            a_player.GetRoomNumber(),
            [&nameToAttack, &a_player, &roomNotificationMessage, &actionResponseMessage]
            (std::shared_ptr<Monster>& a_monsterPtr, std::shared_ptr<SubjectRoom_mt>& a_subject)
            {
                // If there is a monster in the room
                if (a_monsterPtr && a_monsterPtr->GetName() == nameToAttack)
                {
                    Number monsterHp = a_monsterPtr->GetHP();
                    monsterHp -= a_player.GetDmgPoints();

                    if (monsterHp <= 0)
                    {
                        // Monster defeated
                        a_monsterPtr.reset();  // Reset the shared pointer, monster is defeated
                        a_subject->NotifyAllExcept_mt(a_player, a_player.GetName() + " has defeated the " + nameToAttack + "!");
                        actionResponseMessage = "You have defeated the " + nameToAttack + "!";
                    }
                    else
                    {
                        // Monster is still alive
                        a_monsterPtr->SetHP(monsterHp);
                        a_subject->NotifyAllExcept_mt(a_player, a_player.GetName() + " attacked " + nameToAttack + " and dealt " + std::to_string(a_player.GetDmgPoints()) + " damage.");
                        actionResponseMessage = "You attacked " + nameToAttack + " and dealt " + std::to_string(a_player.GetDmgPoints()) + " damage.";
                    }

                    return;
                }

                // If there is no monster, check for other players to attack
                for (const std::string& name : a_subject->GetNamesVec())
                {
                    if (name == nameToAttack)
                    {
                        std::optional<std::reference_wrapper<Player>> other_opt = a_subject.get()->GetPlayer(nameToAttack);
                        if (other_opt.has_value())
                        {
                            Player& other = other_opt.value();
                            // Attacking another player
                            bool DidAttack = AttackAndNotifyVictim(a_player, other);

                            if (DidAttack)
                            {
                                const std::string notifyRoom = a_player.GetName() + " attacked " + other.GetName();
                                a_subject.get()->NotifyAllExcept_mt({ a_player, other }, notifyRoom);
                                actionResponseMessage = "You dealth " + std::to_string(a_player.GetDmgPoints()) + " damage points to " + nameToAttack + "! ";
                                if (!other.IsAlive())
                                {
                                    a_subject.get()->Unregister_mt(other);
                                    a_subject.get()->NotifyAllExcept_mt(a_player, a_player.GetName() + " killed " + other.GetName() + "!!");
                                    actionResponseMessage = "You've killed " + other.GetName() + "!";
                                    other.NotifyPlayer(ENDL + ENDL + ENDL + "****** Your'e Dead!! ******" + ENDL + "****** End Of Game ******");
                                }
                            }
                        }
                     }
                }
            }
        );

        // If no valid target was found in the room
        if (actionResponseMessage.empty())
        {
            return std::make_unique<StringActionResponse>(nameToAttack + " is not in the room");
        }

        // Return the action response message
        return std::make_unique<StringActionResponse>(actionResponseMessage);

    } // dnd_game

} // namespace dnd_game

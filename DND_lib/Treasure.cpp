#include "Treasure.h"
#include <string>

namespace dnd_game {

// Initialize the unordered_map for the treasure factory without `static`
std::unordered_map<TREASURE_TYPE, std::function<std::unique_ptr<Treasure>()>> treasureMap = {
    {TREASURE_TYPE::GUN, []() { return std::make_unique<TreasureGun>(); }},
    {TREASURE_TYPE::SWORD, []() { return std::make_unique<TreasureSword>(); }},
    {TREASURE_TYPE::MAGIC_YOUTH_POTION, []() { return std::make_unique<TreasureHealingPotion>(); }},
    {TREASURE_TYPE::GUARDED, []() { return std::make_unique<GuardedTreasure>(); }}

};

Treasure::Treasure(std::string a_name)
    : m_name { a_name }
    , m_reply {}
{
    // CTOR
}

TreasureSword::TreasureSword()
    : Treasure("Sword")
{
    // CTOR
}

TreasureHealingPotion::TreasureHealingPotion()
    : Treasure("Healing Potion")
{
    // CTOR
}

GuardedTreasure::GuardedTreasure()
    : Treasure { "Guarded Treasre" }
{
    // CTOR
}

void TreasureHealingPotion::operator()(Player & a_player)
{
    std::string answer = DeclareMessage() + " ";

    Number curr_hp = a_player.GetHealthPoints();
    a_player.SetHealthPoints(curr_hp + m_hp);

    answer += HpIncreased(curr_hp, a_player.GetHealthPoints());
    m_reply = answer;
}

void TreasureSword::operator()(Player& player)
{
    // Define the effect of the Sword on the Player here
    // For example:
    std::string message = DeclareMessage() + " ";
    // Assuming some effect like increasing attack points
    player.SetDmgPoints(player.GetDmgPoints() + 10);
    message += AttackPointsIncreaed(player.GetDmgPoints() - 10, player.GetDmgPoints());
    m_reply = message;
}

void TreasureGun::operator()(Player& player)
{
    // Define the effect of the Gun on the Player here
    std::string message = DeclareMessage() + " ";
    // Assuming some effect, e.g., increasing attack points
    player.SetDmgPoints(player.GetDmgPoints() + 20);
    message += AttackPointsIncreaed(player.GetDmgPoints() - 20, player.GetDmgPoints());
    m_reply = message;
}


void GuardedTreasure::operator()(Player & player)
{
    m_reply = "Treasure is gaurded and could not be obtained";
}

std::unique_ptr<Treasure> Treasure::TreasureFactory(TREASURE_TYPE type)
{
    auto it = treasureMap.find(type);
    if (it != treasureMap.end())
    {
        return it->second();  // Call the factory function for the type
    }
    return nullptr;  // Return nullptr if type not found
}

std::string Treasure::DeclareMessage() const
{
    return "You've found a " + m_name + "!";
}

// Convert integers to strings for concatenation
std::string Treasure::HpIncreased(int a_curr_hp, int a_hp) const
{
    std::string message = "Your health points increased from " + std::to_string(a_curr_hp);
    message += " to " + std::to_string(a_hp);
    return message;
}

std::string Treasure::AttackPointsIncreaed(int a_currAttackPoints, int a_attackPoints) const
{
    std::string message = "Your attack points increased from " + std::to_string(a_currAttackPoints);
    message += " to " + std::to_string(a_attackPoints);
    return message;
}

std::string Treasure::Get_Reply() const
{
    return m_reply;
}

std::string Treasure::GetName() const
{
    return m_name;
}

TreasureGun::TreasureGun()
    : Treasure("Gun")
{
    // CTOR
}

} // namespace dnd_game

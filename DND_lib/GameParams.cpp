#include "GameParams.h"

namespace dnd_game
{

AttackPlayerResponse::AttackPlayerResponse(bool a_isValid)
    : m_isValid { a_isValid }
{

}

Number AttackPlayerResponse::GetOtherPlayerLifePoints()
{
    return m_DamageToOtherPlayer;
}

Number AttackPlayerResponse::GetDamageToThisPlayer() const
{
    return m_DamageToPlayer;
}
}   // dnd_game
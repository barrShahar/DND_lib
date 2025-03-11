#include "AttackResponse.h"


namespace dnd_game
{
AttackResponse::AttackResponse(const std::string& a_adressedName, bool a_isInPlay, Number a_originalHp, Number a_currentHp, Number a_returnedDmg)
	: adressedNameMsg { a_adressedName }
	, isInPlay { a_isInPlay }
	, originalHp { a_originalHp }
	, currentHp { a_currentHp }
	, returnedDmg { a_returnedDmg  }
{
	// CTOR
}


} // namespace dnd_game
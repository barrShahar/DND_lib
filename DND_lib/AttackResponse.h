#pragma once
#include <string>
#include "GameParams.h"

namespace dnd_game
{
class AttackResponse
{
public:
	AttackResponse(const std::string& a_adressedName, bool a_isDead, Number a_originalHp, Number a_currentHp, Number a_returnedDmg);
	AttackResponse(const AttackResponse& a_other) = delete;
	const AttackResponse& operator=(const AttackResponse& a_other) = delete;
	virtual ~AttackResponse() = default;

	std::string const adressedNameMsg;
	bool isInPlay; 
	Number originalHp; 
	Number currentHp; 
	Number returnedDmg;
	
};

} // namespace dnd_game
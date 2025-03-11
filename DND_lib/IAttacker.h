#pragma once
#include "IAttackable.h"
#include "GameParams.h"
namespace dnd_game
{
class IAttacker : public IAttackable
{
public:
	IAttacker() = default;
	IAttacker(const IAttacker& a_other) = delete;
	IAttacker& operator=(const IAttacker& a_other) = delete;
	virtual ~IAttacker() = default;
	virtual Number GetDamage() const = 0;
private:

};

} // namespace dnd_game
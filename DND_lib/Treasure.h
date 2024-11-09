#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <unordered_map>
#include <functional>

#include "Player.h"
#include "GameParams.h"
#include "Writer.h"
namespace dnd_game
{


class Treasure  
{
public:
    explicit Treasure(std::string a_name);
    Treasure(const Treasure& a_other) = delete;
    Treasure& operator=(const Treasure& a_other) = delete; 
    virtual ~Treasure() = default;
    //virtual std::string ApplyEffect(Player& player) const = 0;
    virtual void operator()(Player& player) = 0;

    static std::unique_ptr<Treasure> TreasureFactory(TREASURE_TYPE type);

    std::string DeclareMessage() const;
    std::string HpIncreased(int a_curr_hp, int a_hp) const;
    std::string AttackPointsIncreaed(int a_currAttackPoints, int a_attackPoints) const;
    std::string Get_Reply() const;
    std::string GetName() const;

protected:
    std::string m_reply;

private:
    std::string m_name;

};


class TreasureSword: public Treasure
{
public:
    explicit TreasureSword();
    virtual void operator()(Player& player) override;
};

class TreasureHealingPotion: public Treasure
{
public:
    explicit TreasureHealingPotion();
    virtual void operator()(Player& player) override;


private:
    static const Number m_hp = 50;
};

class TreasureGun: public Treasure
{
public:
    explicit TreasureGun();
    virtual void operator()(Player& player) override;
};

class GuardedTreasure : public Treasure
{
public:
    explicit GuardedTreasure();
    virtual void operator()(Player& player) override;
};

}	// dnd_game
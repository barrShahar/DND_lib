#pragma once
#include <memory>
#include "Player.h"
#include "ActionResponse.h"
#include "Dungeon_mt.h"
#include "TextActionResponse.h"
// This header file defines Action base class
// and all the Actions classes derived from

namespace dnd_game
{
class Action    // Base class
{
public:
    explicit Action() = default;
    Action(const Action& a_other) = delete;
    Action& operator=(const Action& a_other) = delete;
    virtual ~Action() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const = 0;

private:

};

class AttackAction : public Action
{
public:
    explicit AttackAction() = default;
    AttackAction(const AttackAction& a_other) = delete;
    AttackAction& operator=(const AttackAction& a_other) = delete;
    ~AttackAction() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const override;

private:
};

class AttackDragon : public Action
{
public:
    explicit AttackDragon() = default;
    AttackDragon(const AttackDragon& a_other) = delete;
    AttackDragon& operator=(const AttackDragon& a_other) = delete;
    ~AttackDragon() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const override;

private:
};

class LeftAction : public Action
{
public:
    explicit LeftAction() = default;
    LeftAction(const LeftAction& a_other) = delete;
    LeftAction& operator=(const LeftAction& a_other) = delete;
    ~LeftAction() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const override;

private:
};

class LookAction : public Action
{
public:
    explicit LookAction() = default;
    LookAction(const LookAction& a_other) = delete;
    LookAction& operator=(const LookAction& a_other) = delete;
    ~LookAction() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const override;

private:
};

class RightAction : public Action
{
public:
    explicit RightAction() = default;
    RightAction(const RightAction& a_other) = delete;
    RightAction& operator=(const RightAction& a_other) = delete;
    ~RightAction() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const override;

private:
};

class WalkAction : public Action
{
public:
    explicit WalkAction() = default;
    WalkAction(const WalkAction& a_other) = delete;
    WalkAction& operator=(const WalkAction& a_other) = delete;
    ~WalkAction() = default;

    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt& a_dungeon, Player& a_player) const override;

private:
};

// Sends a messages to all players in the room
class TalkAction : public Action
{
public:
    explicit TalkAction() = default;
    TalkAction(const TalkAction& a_other) = delete;
    TalkAction& operator=(const TalkAction& a_other) = delete;
    ~TalkAction() = default;
    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt &a_dungeon, Player& a_player) const override;

private:
};

class ShoutAction : public Action
{
public:
    explicit ShoutAction() = default;
    ShoutAction(const ShoutAction& a_other) = delete;
    ShoutAction& operator=(const ShoutAction& a_other) = delete;
    ~ShoutAction() = default;
    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt &a_dungeon, Player& a_player) const override;

private:
};

class TakeAction : public Action
{
public:
    explicit TakeAction() = default;
    TakeAction(const TakeAction& a_other) = delete;
    TakeAction& operator=(const TakeAction& a_other) = delete;
    ~TakeAction() = default;
    virtual std::unique_ptr<ActionResponse> Act(Dungeon_mt &a_dungeon, Player& a_player) const override;

private:
};

}  // namespace dnd_game
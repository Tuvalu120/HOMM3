/**
 * @file PlayerController.h
 * @author Bartosz Han
 * @brief This file contains PlayerController class and other related to it classes.
 * Objects of these classes are special objects, associated with players,
 * which are used as an interface between player's game decisions and implementing these decisions into game.
 *
 */

#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "GameObjects/BaseObjects/BattleElement.h"
#include <utility>

class Battle;

/// @enum ControlType
/// @brief Enum to represent different types of players, who can play this game.
/// Currently, there are two types of players: human, and AI.
enum ControlType{
    HUMAN,
    AI
};

/// @class Controller
/// @brief This class is base class for all controller classes.
/// It contains decision vars for all other controllers.
class Controller{

protected:

    int decision_var_1_;
    int decision_var_2_;

    /// @brief Set the values of both decision vars to -1, indicating that no decision has been made yet.
    void resetDecisionVars();

public:

    Controller();
    /**
     * @brief Check, if decision has been made by player, which checking, if decision vars has benn already set.
     *
     * This method will check one or both decision vars has values indicating that decision has been made.
     * Currently, this method will check, if variables have values different than -1 (-1 is indicator of awaiting the input from player).
     *
     * @param n_of_vars Number of decision variables to check.
     * @return True if all checked decision variables have values, which are not -1, false otherwise.
     *
     * @note Even if only one decision variable is checked, second decision variable should not have any valid positive value.
     */
    bool checkDecisionVars(int n_of_vars) const;

    /// @brief Set the values of decision variables of this object.
    void setDecisionVars(const int new_decision_var_1 = -1, const int new_decision_var_2 = -1);

    /// @brief Set the values of decision variables of this object, by providing pair of new values.
    void setDecisionVars(const std::pair<int, int> new_decisions_vars);
};


/**
 * @class BattleController
 * @brief Objects of this are used to
 * make a decision about BattleObject, which is currently taking its turn during battle, to do something.
 */
class BattleController: virtual public Controller, public BattleElement{

public:

    /**
     * @brief Make a decision about BattleObject, which is currently taking its turn during battle, to do something.
     *
     * This method will await input from the player, and will call BattleManager.performBattleAction() based on decision vars.
     *
     * In this type of decision, decision vars are absolute coordinates of the tile, which is BattleObject's either target of destination.
     *
     */
    virtual void makeBattleDecision();

    /**
     * @brief Make a decision, about Hero, which is currently taking its turn during battle, to choose, which spell to cast next.
     *
     * This method will await input from the player, and will call Hero.chooseSpell() of current hero.
     *
     * This method DOES NOT cast chosen spell, casting spell is managed by makeBattleDecision().
     *
     * In this type of decision, only one decision var is needed. Decision var is index of Spell at list of spells of current hero.
     *
     */
    virtual void makeSpellDecision();
};


/**
 * @class WorldMoveController
 * @brief Objects of this are used to
 * make a decision about army, which is currently taking its turn on world map, to move to another tile.
 *
 */
class WorldMoveController: virtual public Controller{

public:
    /**
     * @brief Make a decision about army, which is currently taking its turn on world map, to move to another tile.
     *
     * This method will await input from the player, and will call WorldManager.moveCurrentArmyToTile() based on decision vars.
     *
     * In this type of decision, decision vars are absolute coordinates of the tile, where current army should go to.
     *
     */
    virtual void makeWorldMoveDecision();
};


/**
 * @class
 * @brief Objects of this are used to
 * make a decision about army, which is currently taking its turn on world map, to do some decision regarding some object,
 * which player is interacting now, or decision regarding army's hero leveling up.
 *
 */
class WorldDecisionController: virtual public Controller{

public:

    /**
     * @brief Make a decision about army, which is currently taking its turn on world map, to do some decision regarding some object,
     * which player is interacting now, or decision regarding army's hero leveling up.
     *
     * This method is associated with DecisionManager, and Decisions made by them. This method will await input from the player,
     * and will call DecisionManager.decide() based on decision var.
     *
     * In this type of decision, only one decision var is needed. Decision var is index of DecisionOption at list of options in currently Decision
     * managed by DecisionManager.
     *
     */
    virtual void makeWorldDecision();
};



/**
 * @class
 * @brief Objects of this are used to
 * make a decision about player, which is currently taking its turn on world map and is visiting friendly town,
 * to choose some decision regarding visited town.
 *
 */
class TownController: virtual public Controller{

public:
    /**
     * @brief Make a decision about player, which is currently taking its turn on world map and is visiting friendly town,
     * to choose some decision regarding visited town.
     *
     * This method will await input from the player, and will call Town.attemptToDo() of currently visited town object.
     *
     * In this type of decision, only one decision var is needed. Decision variable is value of TownDecision enum, which is associated with given decision.
     *
     */
    virtual void makeTownDecision();

};

/**
 * @class PlayerController
 * @brief This class is used by the players as an interface between player's game decisions and implementing these decisions into game.
 *
 * Whenever player are taking their turns, they have to make several decisions about steering their resources.
 * This class is used to get the player's input and do action in game based on decision.
 *
 * There are several types of decisions to make during the game: moving and interacting with objects on world map, controlling resources on battle
 * and buying resources in town.
 *
 * Each decision is identified with set of two intergers. The 'decision variables' are used to set the decision player is making. There variables
 * have different meaning based on type of current decision, and game state.
 */
class PlayerController: public BattleController, public WorldMoveController,
    public WorldDecisionController, public TownController{

protected:
    const ControlType control_type_;

public:
    /**
     * @brief Construct a new PlayerController object.
     *
     * @param control ControlType of this player controller.
     *
     */
    PlayerController(const ControlType control);

    /// @brief Get ControlType of this controller.
    ControlType getControlType() const;



    PlayerController(const PlayerController&) = delete;
    PlayerController& operator=(const PlayerController&) = delete;
};

#endif
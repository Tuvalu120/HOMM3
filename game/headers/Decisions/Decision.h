/**
 * @file Decision.h
 * @author Bartosz Han
 * @brief This file constains Decision class and several classes, which inherits from this class.
 *
 * Decision objects are created during the game, when player, while exploring world map, is interacting with another objects.
 * During these decisions, player can choose one of several DecisionOptions to perform, which each one affects the state of the game in different way.
 *
 */

#ifndef DECISION_H
#define DECISION_H

#include <memory>

#include "Decisions/DecisionOption.h"

class Army;

/**
 * @class Decision
 * @brief This class is used for making several special types of decisions while exploring the world map.
 * During these decisions, player can choose one of several DecisionOptions to perform, which each one affects the state of the game in different way.
 *
 * When exploring the world map, player can interact with several objects, or can gain experience points, which way cause level up hero player's army.
 * Theses situations are creating decisions, in which player can choose one of few options to do. Decision object contains few DecisionOptions, which
 * affects the game in different way. Player can only choose one option per Decision.
 *
 * There should only be one decision active per game. Decisions are managed by DecisionManager.
 *
 */
class Decision{

private:
    const DecisionType decision_type_;
protected:
    std::vector<std::unique_ptr<DecisionOption>> decision_options_;
    std::string decision_name_;

public:

    /**
     * @brief Construct a new Decision object
     *
     * @param decision_type DecisionType of new Decision object.
     */
    Decision(const DecisionType decision_type);


    /// @brief Call choose() method of DecisionOption at provided index of list of options.
    virtual void chooseDecisionOption(const int option_id);

    /// @brief Get DecisionType of this Decision.
    DecisionType getDecisionType() const;

    /// @brief Get number of decision options of this Decision to choose from.
    int getNOfDecisionOptions() const;

    /// @brief Get reference to DecisionOption, which is iat provided index in options list.
    DecisionOption& getDecisionOptionAt(const int index) const;

    /// @brief Get reference to the vector of all DecisionOptions available in this decision.
    const std::vector<std::unique_ptr<DecisionOption>>& getDecisionOptions() const;

    /// @brief Get name of this Decision. Used in GUI.
    std::string getDecisionName();

    Decision(const Decision&) = delete;
    Decision& operator=(const Decision&) = delete;
};

//////

/**
 * @class NeutralTroopsDecision
 * @brief This Decision is initialized when player interacts with WorldNeutralTroops on world map.
 *
 * Player, upon interacting can choose between:
 * - Initaliasing battle between its current army, and army of this object.
 * - Buying troops of this army into their world army.
 * - Doing noting, and skipping decision.
 *
 */
class NeutralTroopsDecision : public Decision{

private:
    Army& army_;

public:
    /**
     * @brief Construct a new NeutralTroopsDecision object.
     *
     * @param decision_army Army& of WorldNeutralTroops world object, which this decision is associated with.
     */
    explicit NeutralTroopsDecision(Army& decision_army);
    void chooseDecisionOption(const int option_id) override;
};

//////


/**
 * @class ScholarDecision
 * @brief This Decision is initialized when player interacts with Scholar on world map.
 *
 * Player, upon interacting can choose between:
 * - Buying new spell for current army's hero.
 * - Upgrading one of current army's hero spells.
 * - Increasing one of current army hero's statistics.
 *
 */
class ScholarDecision : public Decision{
public:
    ScholarDecision();
};

//////

/**
 * @class LevelUpDecision
 * @brief This Decision is initialized when player gets enough experience points, and levels up.
 *
 * Player, upon leveling up, can choose one between two statistics of current army's hero to increase.
 *
 */
class LevelUpDecision : public Decision{
public:
    LevelUpDecision();
};

//////

/**
 * @class ChestDecision
 * @brief This Decision is initialized when player interacts with TreasureChest on world map.
 *
 * Player, upon interacting can choose between:
 * - Adding some amount of gold to their resources.
 * - Adding some amount of experience points for current army's hero.
 */
class ChestDecision : public Decision{
public:
    ChestDecision();
};

#endif
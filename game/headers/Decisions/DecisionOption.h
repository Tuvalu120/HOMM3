/**
 * @file DecisionOption.h
 * @author Bartosz Han
 * @brief This file contains DecisionOption class and classes, which inherits from this class.
 * DecisionOptions are options, which player can choose when making a Decision. each decision do something different and
 * changes game state when selected.
 *
 */

#ifndef DECISION_OPTION_H
#define DECISION_OPTION_H

#include <string>
#include <vector>

#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/BaseObjects/DrawableObject.h"

class Army;

/**
 * @class DecisionOption
 * @brief This class is used for implementing decision made by players during making Decision.
 *
 * DecisionOptions are options, which player can choose when making a Decision. each decision do something different and
 * changes game state when selected.
 *
 * Decision objects consists of several DecisionOptions, which player can choose from. Only one DecisionOption can be choose in one Decision.
 */
class DecisionOption: public DrawableObject{

private:
    const DecisionOptionType decision_option_;

public:
    /**
     * @brief Construct a new DecisionOption object.
     * @param decision_option DecisionOptionType of new decision option.
     */
    DecisionOption(const DecisionOptionType decision_option);

    /**
     * @brief Choose this decision option.
     *
     * By calling this method, it is considered that player has made decision to choose this option.
     * This method will do some logic, related to choosing this decision option (such as adding resource to the player).
     */
    virtual void choose()=0;

    /// @brief Get name of this decision option. Used in UI.
    virtual const std::string getOptionName() const=0;

    /// @brief Get pair of vectos of string, which consists of names of various variables of the decision option,
    /// and values of these options. Used to be displayed by UI.
    virtual const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const=0;

    /// @brief Get DecisionOptionType of this decision option.
    DecisionOptionType getDecisionOptionType() const;

    DecisionOption(const DecisionOption&) = delete;
    DecisionOption& operator=(const DecisionOption&) = delete;
};
//////

/**
 * @class AddTroopDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, player will receive additional troops of one type to their army, but at a cost of some of their gold.
 *
 * Adding troops to army should cost some amount of gold.
 * This decision option should only be generated when player associated with this decision
 * has enough gold to buy these troops.
 */
class AddTroopsDecisionOption : public DecisionOption{

private:
    const TroopType troop_type_;
    const int troops_quantity_;
    const int cost_;

public:
    /**
     * @brief Construct a new Add TroopDecisionOption object
     *
     * @param troop TroopType of troops which will be added to player when choosing this decision option.
     * @param quantity Quantity of troops which will be added to player when choosing this decision option.
     * @param troops_cost Cost of buying troops (in gold) upon choosing this decision option.
     */
    AddTroopsDecisionOption(const TroopType troop, const int quantity, const int troops_cost);
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;

    /// @brief Get TroopType of troops which will be added to player when choosing this decision option.
    TroopType getTroopType() const;

    /// @brief Get quantity of troops which will be added to player when choosing this decision option.
    int getTroopsQuantity() const;

    /// @brief Get cost of buying troops (in gold) upon choosing this decision option.
    int getCost() const;
};
//////

/**
 * @class AddResourceDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, player will receive additional Resources in given quantity.
 *
 */
class AddResourceDecisionOption : public DecisionOption{

private:
    const Resource resource_type_;
    const int resource_quantity_;

public:
    /**
     * @brief Construct a new Add Resource Decision Option object
     *
     * @param resource ResourceType, which will be added to player when choosing this option.
     * @param quantity Quantity of resources, will be added to player when choosing this option.
     */
    AddResourceDecisionOption(const Resource resource, const int quantity);
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;

    /// @brief Get ResourceType, which will be added to player when choosing this option.
    Resource getResourceType() const;

    /// @brief Get quantity of resources, will be added to player when choosing this option.
    int getResourceQuantity() const;
};
//////

/**
 * @class AddExperiencePointsDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, hero of army, which is currently taking its turn on world map, receives additional experience points.
 *
 */
class AddExperiencePointsDecisionOption : public DecisionOption{

private:
    const int quantity_;

public:

    /**
     * @brief Construct a new AddExperiencePointsDecisionOption object.
     *
     * @param quantity Quantity of experience points awarded to player when choosing this decision option.
     */
    explicit AddExperiencePointsDecisionOption(const int quantity);
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;

    /// @brief Get quantity of experience points awarded to player when choosing this decision option.
    int getExperiencePoints() const;
};
//////

/**
 * @class AddSpellDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, hero of army, which is currently taking its turn on world map, will receive new spell at given level,
 * or, if player already has this spell, will level up this spell.
 *
 */
class AddSpellDecisionOption : public DecisionOption{

private:
    const SpellType spell_type_;
    const int spell_level_;

public:
    /**
     * @brief Construct a new AddSpellDecisionOption object.
     *
     * @param spell SpellType of spell to be added or upgraded to current army's hero when choosing this decision option.
     * @param level Level of spell to be added or upgraded to current army's hero when choosing this decision option.
     */
    AddSpellDecisionOption(const SpellType spell, const int level);
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;

    /// @brief Get SpellType of spell to be added or upgraded to current army's hero when choosing this decision option.
    SpellType getSpellType() const;

    /// @brief Get level of spell to be added or upgraded to current army's hero when choosing this decision option.
    int getSpellLevel() const;
};
//////

/**
 * @class AddHeroStatisticDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, hero of army, which is currently taking its turn on world map,
 * will increase one of their statistic by given quantity.
 *
 */
class AddHeroStatisticDecisionOption : public DecisionOption{

private:
    const HeroStat hero_stat_;
    const int quantity_;

public:

    /**
     * @brief Construct a new Add Hero Statistic Decision Option object
     *
     * @param hero_stat HeroStat to be increased for the hero of current army, when choosing this decision option.
     * @param quantity Value by which one of statistics of hero of current army will increased, when choosing this decision option.
     */
    AddHeroStatisticDecisionOption(const HeroStat hero_stat, const int quantity);
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;

    /// @brief Get HeroStat to be increased for the hero of current army, when choosing this decision option.
    HeroStat getHeroStat() const;

    /// @brief Get value by which one of statistics of hero of current army will increased, when choosing this decision option.
    int getQuantity() const;
};
//////

/**
 * @class BattleDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, player will initialize battle, between one of their army, which is currently taking its turn on world map,
 * of army associated with this object.
 *
 * Army of this object should consist of one type of troop.
 */
class BattleDecisionOption : public DecisionOption{

private:
    Army& army_;
    const TroopType troop_type_;
    const int troop_quantity_;

public:
    /**
     * @brief Construct a new BattleDecisionOption object.
     *
     * @param decision_army Army&, with which player will battle with, when choosing this decision option.
     */
    explicit BattleDecisionOption(Army& decision_army);
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;

    /// @brief Get TroopType of troop, which army of this decision option consist of.
    TroopType getTroopType() const;

    /// @brief get quantity of troop, which army of this decision option consist of.
    int getTroopQuantity() const;
};
//////

/**
 * @class DoNothingDecisionOption
 * @brief This class is one of DecisionOptions.
 * By choosing object of this class, nothing will happen, and current Decision should end.
 */
class DoNothingDecisionOption : public DecisionOption{

public:
    DoNothingDecisionOption();
    void choose() override;
    const std::string getOptionName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getOptionDescription() const override;
};
#endif
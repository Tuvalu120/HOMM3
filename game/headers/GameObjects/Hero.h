/**
 * @file Hero.h
 * @author Bartosz Han
 * @brief This file contains Hero class. Object of this class are components of armies. Objects of this class enhances troops of armies, and also contains and provide
 * interface to various spells which can be casted during the battle.
 *
 */
#ifndef HERO_H
#define HERO_H

#include "GameObjects/BaseObjects/BattleElement.h"
#include "GameObjects/BaseObjects/DrawableObject.h"
#include "GameObjects/BaseObjects/BattleObject.h"
#include "Data/Constants.hpp"
#include "GameObjects/Spell.h"

/**
 * @class Hero
 * @brief Object of this class are components of armies. Objects of this class enhances stacks of armies, and also contains and provide
 * interface to various spells which can be casted during the battle.
 *
 * This class contains vector fo various statistics, which enhances stacks of army during battle, and also enhances WorldArmy while exploring world map.
 *
 * This class contains vector of spells, which army can use during the battle, when hero is taking its turn.
 * This class also manages casting spells, and also has mana point system,
 * which prevents player from casting spells, if it does not have enough resources to do so.
 *
 * Heroes during the game, can level up by getting enough experience points. When leveling up,
 * player can choose between few attributes of this player to increase.
 *
 * Hero cannot be attacked or destroyed during the battle.
 * Only WorldArmy objects should have hero associated with them.
 *
 */
class Hero: public DrawableObject, public BattleObject, public BattleElement{

private:
    const HeroType hero_type_;
    const std::string hero_name_;
    std::array<int, N_HERO_STATS> statistics_;
    std::array<std::unique_ptr<Spell>, constants::MAX_NUMBER_OF_SPELLS> spells_;

    int current_level_;
    int experience_points_;
    int spell_chosen_;
    int mana_points_;

    /// @brief Increment the level of this hero, and do logic related to leveling up hero.
    /// This method will create LevelUpDecision upon calling.
    void levelUp();


public:

    /**
     * @brief Construct a new Hero object.
     *
     * All associated data of this hero will be extracted from game_data::getHeroData() function.
     *
     * @param hero_type HeroType (or ID) of this hero.
     * @param player_id ID of player controlling this hero.
     *
     * @note There cannot be neutral heroes.
     */
    Hero(const HeroType hero_type, const int player_id);

    bool attack(Tile& event_tile) override;

    /**
     * @brief Add new spell to the array of spells, or increase level of spell which hero currently have.
     *
     * This method will check, if player already have spell with provided spell ID.
     * If yes, it will attempt increase the level of spell.
     * If no, it will attempt to add new spell to the array.
     *
     * @param spell_type SpellType (or ID) of spell to add.
     * @param spell_level Level of spell to be added.
     * @return Ture, if spell was either successfully added to the array, or its level was increased,
     * false if something went wrong, and state of spells was not changed.
     */
    bool addSpell(SpellType spell_type, int spell_level=1);

    /// @brief Returns true, if player has any spell at provided index at spell array, false if spell at this index is nullptr.
    bool hasSpellAt(const int index_to_check) const;

    /// @brief Get reference to Spell object at provided index at spell array. Throws NullSpellError if player does not have any spell at this index.
    Spell& getSpellAt(const int index) const;

    /// @brief Get reference of array of spells of this hero.
    /// @note This array might contain nullptrs.
    const std::array<std::unique_ptr<Spell>, constants::MAX_NUMBER_OF_SPELLS>& getHeroSpells() const;

    /// @brief Get number of spells, which hero currently have.
    int getNOfSpells() const;

    /**
     * @brief Get the vector of pairs of data of hero's spells, in which first element is ID of spell, and second is level of this spell.
     *
     * @return const std::vector<std::pair<SpellType, int>> Vector of pair contains data about hero;s spells.
     */
    const std::vector<std::pair<SpellType, int>> getHeroSpellsData() const;

    /// @brief Set the index of spell to be casted upon player selecting target of spell.
    void chooseSpell(const int index);

    /// @brief Get reference to the vector of numerical statistics of hero.
    const std::array<int, N_HERO_STATS>& getStatistics() const;

    /// @brief Get shared pointer to the vector of numerical statistics of hero.
    std::shared_ptr<std::array<int, N_HERO_STATS>> const getStatisticsPtr() const;

    /// @brief Get of value of provided statistic of this hero.
    int getStatistic(HeroStat stat) const;

    /// @brief Increase the value of provided statistic by given amount.
    void addStatistic(HeroStat stat_to_add, const unsigned int amount);

    const std::string getHeroName() const;
    HeroType getHeroType() const;
    const std::string getUITextureName() const override;
    const std::pair<std::vector<std::string>, std::vector<std::string>> getDataText() const override;

    /// @brief Get hero's current level.
    int getHeroLevel() const;

    /// @brief get current amount of experience points of hero.
    /// @note Whenever player levels up, its value of current experience points is set to zero.
    int getHeroExperiencePoints() const;

    /// @brief Increase the value of current experience points of hero.
    /// If hero reaches the needed amount of points to level up, this method will also call levelUp() method.
    void addExperiencePoints(const unsigned int value);

    /// @brief Get text data about hero's current level and experience points. Used in UI.
    std::string getHeroLevelDescription() const;

    /// @brief Retruns ture, if there is sopt in spells's array to add new spell, false otherwise.
    bool canAddNewSpell();

    /**
     * @brief Get the current value of mana points of hero.
     * During the battle, hero can cast spells. whenever it does so, the casting cost of mana points.
     * Player cannot cast spells, which would cost him too much mana points.
     *
     * Maximum man points of hero depends of KNOWLEDGE statistic of hero.
     */
    int getManaPoints() const;

    /// @brief set the current mana points to be the currently maximum possible value.
    void resetManaPoints();

    /// @brief Decrement the value of current mana points by provided value.
    void reduceManaPoints(const int value);

    /// @brief returns true, if there is spell at given index, and hero has enough mana points to cast it.
    bool canCastSpellAt(const int spell_id) const;

    /// @brief Returns true, if hero has any spell. which hero can cast based on its current mana points.
    bool canCastAnySpell() const;

    /// Get index of spell chosen to be casted next by the player.
    int getSpellChosenIndex() const;

    const std::string toString() const override;

};

#endif
/**
 * @file Spell.h
 * @author Bartosz Han
 * @brief this file contains Spell class and classes, which inherits from it.
 * Spells are object used by the heros. During battle, hero, when taking its turn, can cast one of spells, to get the advantage on the battlefield.
 *
 */

#ifndef SPELL_H
#define SPELL_H

#include "GameObjects/BaseObjects/BattleElement.h"
#include "GameObjects/TileMaps/Tile.h"
#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/BaseObjects/DrawableObject.h"
#include "Data/GameData.hpp"
#include "Miscellaneous/IPainter.h"
#include <memory>
#include <utility>

class Stack;
class Army;


/**
 * @class Spell
 * @brief This class is used by the players and heroes. During battle, hero, when taking its turn, can cast one of spells,
 * to get the advantage on the battlefield.
 *
 * There are different types of spells, which can affect teh battlefield in any way.
 * All of them should have defined cast() method, which does something on provided tile.
 * Spells can be upgraded to further increase their power.
 *
 */
class Spell: public DrawableObject, public BattleElement{

private:
    const std::string spell_name_;
    const SpellType spell_type_;
    const SpellFamily spell_family_;
    const std::string spell_description_;
    const std::array<SpellDataArray, N_SPELL_STATS> statistics_;
    int current_level_of_spell_;
    const int maximum_level_of_spell_;


public:
    /**
     * @brief Construct a new Spell object.
     *
     * The data needed to create the spell will be further extracted from game_data::getSpellData() function.
     *
     * @param spell_type SpellType of this spell, base on which additional data will be generated.
     * @param init_level_of_spell Initial level of the spell. Level determins current data and power of spell.
     * @param maximum_level_of_spell Maximum possible level of the spell.
     */
    Spell(const SpellType spell_type, const int init_level_of_spell=1,
    const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);

    /**
     * @brief Do some action, with target declared at provided tile.
     *
     * @param event_tile Tile, on which some action should be performed.
     * @return True if action was performed successfully, false otherwise(for example, there was invalid target on tile).
     */
    virtual bool cast(Tile& event_tile)=0;

    /// @brief get SpellType of this spell.
    SpellType getSpellType() const;

    /// @brief get current level fo spell.
    int getLevelOfSpell() const;

    /// @brief  Set the level fo the to given value. New level cannot be lower than previous one, or level won't be changed.
    void setLevelOfSpell(const int new_spell_level);

    /// @brief Get value of given statistic of spell.
    int getStatistic(SpellStat stat) const;

    /// @brief Get name of spell.
    const std::string getSpellName() const;

    /// @brief Get short description string of a spell, used in UI.
    const std::string getSpellDescription() const;

    /// @brief Get spell family of spell. Spells, which are of same class, should have same spell family.
    SpellFamily getSpellFamily() const;

    /**
     * @brief Draw additional elements into texture on GUI.
     *
     * This method will draw level fo spell into spell texture.
     */
    void drawAdditionalData(IPainter& painter) const;

    /**
     * @brief Calculate heuristic quality of spell, if it was cast on provided
     * coordinates. The way this value is calculated is based on type of spell.
     */
    virtual float calculateCastingScore(int x_pos, int y_pos) const = 0;

    /**
     * @brief Calculate heuristic quality of spell, if it was cast on provided
     * coordinates. The way this value is calculated is based on type of spell.
     */
    float calculateCastingScore(std::pair<int, int> pos) const;

    /**
     * @brief Generate new spell, with data based on provided spell_type.
     *
     * New spell will be of level 1, and will not belong to any player.
     *
     * @param spell_type Spell type, of which will spell will be created.
     * @return std::unique_ptr<Spell> Pointer to the new spell.
     */
    static std::unique_ptr<Spell> generateSpell(const SpellType spell_type);

    Spell(const Spell&) = delete;
    Spell& operator=(const Spell&) = delete;

};
///////

/**
 * @class BuffSpell
 * @brief This class is used by the spell, which are supposed to buff friendly stacks, by giving them buffs to their statistics.
 *
 */
class BuffSpell: public Spell{
protected:
    const std::vector<TroopStat> stats_;


public:
    BuffSpell(const SpellType spell_type, const int init_level_of_spell=1,
        const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);
};
///////

class DamageSpell: public Spell{

public:
    DamageSpell(const SpellType spell_type, const int init_level_of_spell=1,
        const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);

protected:
    /// @brief Get number of damage points this spell will deal.
    int calculateDamage() const;
};


///////
/**
 * @class AreaOfEffectBuffSpell
 * @brief This class is used by the spell, which are supposed to buff friendly stacks on given area,
 * by giving them buffs to their statistics.
 *
 * When casting area of effect spell, all tiles are valid as a target of the spell. All friendly troops in range of target tile are affected by this spell.
 *
 */
class AreaOfEffectBuffSpell: public BuffSpell{

protected:
    std::vector<Stack*> getStacksOfFriendlyArmyWithinRange(int x_pos, int y_pos) const;
public:
        /**
     * @brief Construct a new AreaOfEffectBuffSpell object.
     *
     * The data needed to create the spell will be further extracted from game_data::getSpellData() function.
     *
     * @param spell_type SpellType of this spell, base on which additional data will be generated.
     * @param init_level_of_spell Initial level of the spell. Level determins current data and power of spell.
     * @param maximum_level_of_spell Maximum possible level of the spell.
     */
    AreaOfEffectBuffSpell(const SpellType spell_type, const int init_level_of_spell=1,
        const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);
    bool cast(Tile& event_tile) override;
    virtual float calculateCastingScore(int x_pos, int y_pos) const override;
};
///////

/**
 * @class AreaOfEffectDamageSpell
 * @brief This class is used by the spell, which are supposed to deal damage to enemy stacks on given area.
 *
 * When casting area of effect spell, all tiles are valid as a target of the spell. All enemy stacks in range of target tile are affected by this spell.
 *
 */
class AreaOfEffectDamageSpell: public DamageSpell{
protected:

    std::vector<Stack*> getStacksOfEnemyArmyWithinRange(int x_pos, int y_pos) const;

public:
    /**
     * @brief Construct a new AreaOfEffectDamageSpell object.
     *
     * The data needed to create the spell will be further extracted from game_data::getSpellData() function.
     *
     * @param spell_type SpellType of this spell, base on which additional data will be generated.
     * @param init_level_of_spell Initial level of the spell. Level determins current data and power of spell.
     * @param maximum_level_of_spell Maximum possible level of the spell.
     */
    AreaOfEffectDamageSpell(const SpellType spell_type, const int init_level_of_spell=1,
        const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);
    bool cast(Tile& event_tile) override;
    virtual float calculateCastingScore(int x_pos, int y_pos) const override;
};
///////

/**
 * @class SingleTargetBuffSpell
 * @brief This class is used by the spell, which are supposed to buff friendly stacks on given tile,
 * by giving them buffs to their statistics.
 *
 * When casting single target spell, only one tile is considered during casting - the provided one.
 * If there is no friendly unit at this tile, the spell won't be cast.
 *
 */
class SingleTargetBuffSpell: public BuffSpell{
public:
    /**
     * @brief Construct a new SingleTargetBuffSpell object.
     *
     * The data needed to create the spell will be further extracted from game_data::getSpellData() function.
     *
     * @param spell_type SpellType of this spell, base on which additional data will be generated.
     * @param init_level_of_spell Initial level of the spell. Level determins current data and power of spell.
     * @param maximum_level_of_spell Maximum possible level of the spell.
     */
    SingleTargetBuffSpell(const SpellType spell_type, const int init_level_of_spell=1,
        const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);
    bool cast(Tile& event_tile) override;
    virtual float calculateCastingScore(int x_pos, int y_pos) const override;
};
///////

/**
 * @class SingleTargetBuffSpell
 * @brief This class is used by the spell, which are supposed to deal damage to enemy stacks on given tile.
 *
 * When casting single target spell, only one tile is considered during casting - the provided one.
 * If there is no enemy unit at this tile, the spell won't be cast.
 *
 */
class SingleTargetDamageSpell: public DamageSpell{
public:
    /**
     * @brief Construct a new SingleTargetDamageSpell object.
     *
     * The data needed to create the spell will be further extracted from game_data::getSpellData() function.
     *
     * @param spell_type SpellType of this spell, base on which additional data will be generated.
     * @param init_level_of_spell Initial level of the spell. Level determins current data and power of spell.
     * @param maximum_level_of_spell Maximum possible level of the spell.
     */
    SingleTargetDamageSpell(const SpellType spell_type, const int init_level_of_spell=1,
        const int maximum_level_of_spell = constants::MAX_LEVEL_OF_SPELL);
    bool cast(Tile& event_tile) override;
    virtual float calculateCastingScore(int x_pos, int y_pos) const override;
};

#endif
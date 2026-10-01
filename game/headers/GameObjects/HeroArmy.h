/**
 * @file HeroArmy.h
 * @author Bartosz Han
 * @brief Tile file contains HeroArmy class,
 * which is a type of Army with contains hero.
 *
 */

#ifndef HERO_ARMY_H
#define HERO_ARMY_H

#include "GameObjects/Army.h"


/**
 * @class HeroArmy
 * @brief This class is a type of Army with contains hero
 *
 */
class HeroArmy: public Army{

protected:
    std::shared_ptr<Hero> const hero_;

public:

    /**
     * @brief Construct a new Hero Army object
     *
     * @param army_hero Hero of army
     * @param init_pos_x Initial X pos of army.
     * @param init_pos_y Initial Y pos of army.
     * @param is_world_army Flag, describing whether or not this army can move on world map.
     */
    HeroArmy(std::shared_ptr<Hero> const army_hero,
        const int init_pos_x, const int init_pos_y, const bool is_world_army = true);

    /// @brief Construct a new HeroArmy object, with army_hero as its hero.
    HeroArmy(std::shared_ptr<Hero> const army_hero);

/// @brief Get reference to the Hero object of this army.
    Hero& getArmyHero() const;

    std::vector<std::shared_ptr<BattleObject>> getBattleObjectsOfArmy() const override;
    std::shared_ptr<Hero> getArmyHeroPtr() const override;


};

#endif

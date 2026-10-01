/**
 * @file WorldArmy.h
 * @author Bartosz Han
 * @brief This file contains WorldArmy class. Objects of this class are InteractiveObjects on the world map, which player can control,
 * and with which player can interact with world map, and another players and their world armies.
 *
 */

#ifndef WORLD_ARMY_H
#define WORLD_ARMY_H

#include "GameObjects/HeroArmy.h"
#include "GameObjects/BaseObjects/MovableObject.h"

/**
 * @class WorldArmy
 * @brief Objects of this class are InteractiveObjects on the world map, which player can control and move,
 * and with which player can interact with world map, and another players and their world armies.
 *
 * WorldArmy are objects, with which player can take its turn on world map. Players can use them to explore world map and interact with other objects.
 * Like Army, this object can battle with another Armies and WorldArmies to get advantage on the game.
 */
class WorldArmy: public HeroArmy, public MovableObject{

public:


    /**
     * @brief Construct the WorldArmy object.
     *
     * @param army_hero Pointer to the Hero object representing the army's leader.
     * @param init_pos_x Initial X coordinate of the army's position on the world map.
     * @param init_pos_y Initial Y coordinate of the army's position on the world map.
     *
     * @note This constructor does not add any stacks to the army.
     * Owner of this army is based on the owner of the hero of this army.
     */
    WorldArmy(std::shared_ptr<Hero> const army_hero, const int init_pos_x, const int init_pos_y);

    void interact() override;

    /// @brief Get the radius, within which this army is marking tiles on the world as explored by its player.
    int getDiscoveryRadius() const;

    /**
     * @brief Generate new WorldArmy object.
     *
     * This method will generate new army, which will belong to given player, and will have provided hero.
     * This method adds initial stacks to the army, based on game_data::getHeroData().
     * this method DOES NOT add new army to the vector of player's armies.
     *
     * @param player_id ID of player owning this army.
     * @param hero_id HeroType of hero which will lead this army.
     * @param init_x_pos Initial X coordinate of the army's position on the world map.
     * @param init_y_pos Initial Y coordinate of the army's position on the world map.
     * @return std::shared_ptr<WorldArmy> Shared pointer to the new army.
     */
    static std::shared_ptr<WorldArmy> generateArmy(const int player_id, const HeroType hero_id,
        const int init_x_pos, const int init_y_pos);

    int getPriority(WorldArmy& army_to_check) const override;

};

#endif
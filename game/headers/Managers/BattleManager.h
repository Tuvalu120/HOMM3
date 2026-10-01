/**
 * @file BattleManager.h
 * @author Bartosz Han
 * @brief This file contains class BattleManager, which is responsible for managing battles and their resources.
 *
 */
#ifndef BATTLE_MANAGER_H
#define BATTLE_MANAGER_H

#include <memory>

#include "Data/ObjectDataEnums.hpp"
#include "Managers/SubManager.h"
#include "Singleton.hpp"
#include "GameObjects/TileMaps/Battlefield.h"
#include "GameObjects/Battle.h"


class Hero;
class Stack;
class BattleObject;
class BattleController;
class Army;
class Player;

/**
 * @class BattleManager
 * @brief This class is responsible for managing battles and their resources.
 *
 * BattleManager is used whenever battle is supposed to happen. It generates and gives access to necessary resources,
 * gives interface to communicate with other objects, and gives access to information about course of the battles.
 *
 * Battle occurs during the game. In battle two armies fight against each other.
 * Army wins, if it manages to eliminate all stacks from enemy army. Defeated army is removed from the game.
 */
class BattleManager: public Singleton<BattleManager>, public SubManager{

    friend class Singleton<BattleManager>;

private:
    BattleManager();

    std::unique_ptr<Battle> current_battle_;

    int attacking_army_id_, defending_army_id_;

    void initializeBattle(Army& attacker, Army& defender);


    /**
     * @brief Remove resources created for the battle, and do the logic
     * related to the result of the battle.
     *
     * This method will delete current battlefield, and will do logic related
     * to the result od the battle, including calling other managers to remove losing army
     * and player if necessary.
     *
     * @param result Result of the battle: 1 if attacker won, -1 of defender won.
     */
    void endBattle(const int result);


    /**
     * @brief Set the number of current battle turn to 0.
     *
     */
    // void resetNumerOfBattleTurns();


public:
    /**
     * @brief Perfrom battle between two armies.
     *
     * This method is called whenever battle should happen. It initializes battle,
     * controls battle loop, checks for conditions to end battle and performs actions
     * related to finishing the battle.
     *
     * @param attacker Reference to attacking army. Attacking army is the army which initialized battle.
     * @param defender Reference to attacking army. Defending army is the army attacked by attacking army.
     *
     * @note WorldManager::getCurrentPlayer().getCurrentArmy() is always attacking army.
     * Attacking army cannot have player ID of -1.
     * @return int Result of battle. 1 if attacking army won battle, -1 if defending army won battle.
     */
    int battle(Army& attacker, Army& defender);

    /**
     * @brief Perfrom battle between two armies.
     *
     * This method is called whenever battle should happen. It initializes battle,
     * controls battle loop, checks for conditions to end battle and performs actions
     * related to finishing the battle.
     *
     * @param attacker Reference to attacking army. Attacking army is the army which initialized battle.
     * @param defender Reference to attacking army. Defending army is the army attacked by attacking army.
     * @param attacker_controller Reference to BattleController to control attacking army.
     * @param defender_controller Reference to BattleController to control defending army.
     * @param battlefield_style TileType of Battlefield, based on it will be generated.
     *
     * @note WorldManager::getCurrentPlayer().getCurrentArmy() is always attacking army.
     * Attacking army cannot have player ID of -1.
     * @return int Result of battle. 1 if attacking army won battle, -1 if defending army won battle.
     */
    int battle(Army& attacker, Army& defender, BattleController& attacker_controller, BattleController& defender_controller, TileType battlefield_style);

    /// @brief Returns reference to Battle object of current army. Battle must be currently happening to access this object,
    /// otherwise exception is thrown.
    Battle& getCurrentBattle() const;

    /// @Returns true, if there is currently battle happening. This method is based in Battle::isBattleInProgress() method.
    /// Also returns false if no Battle object has been created.
    bool isBattleInProgress() const;

        /**
     * @brief Get reference to attacking army.
     * Attacking army is army, which initialized battle.
     *
     * @return Army& reference to attacking army.
    */
    Army& getAttackingArmy() const;

    /**
     * @brief Get reference to defending army.
     * Defending army is army, which was attacked by attacking army.
     *
     * @return Army& reference to defending army.
    */
    Army& getDefendingArmy() const;



    void resetManager() override;

};

#endif
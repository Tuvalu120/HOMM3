/**
 * @file Battle.h
 * @author Bartosz Han
 * @brief This file contains declaration of Battle class.
 * This class is an representation of battle in the game.
 * It manages battlefield, as well as armies and its stacks during the battle.
 *
 */

#ifndef BATTLE_H
#define BATTLE_H

#include <vector>
#include "GameObjects/Hero.h"
#include "PlayerControllers/PlayerController.h"
#include "GameObjects/TileMaps/Battlefield.h"
#include "Data/ObjectDataEnums.hpp"

class Battlefield;

/**
 * @class Battle
 * @brief This class is an representation of battle in the game.
 * It manages battlefield, as well as armies and its stacks during the battle.
 */
class Battle{

private:

    Army& attacking_army_;
    Army& defending_army_;

    BattleController& attacker_battle_controller_;
    BattleController& defender_battle_controller_;

    std::shared_ptr<Hero> attacker_hero_;
    std::shared_ptr<Hero> defender_hero_;

    int battle_turn_;
    std::vector<std::shared_ptr<BattleObject>> battle_order_;

    int attacking_player_id_, defending_player_id_;
    int current_player_id_;

    std::unique_ptr<Battlefield> battlefield_;
    bool flag_is_player_turn_finished_;

        /**
     * @brief Check if given battle object is Hero of any army
     *
     * @param battle_object_to_check battle object to check
     *
     * @return bool True if battle object is hero of any army, false otherwise.
     */
    bool isBattleObjectHero(const BattleObject& battle_object_to_check) const;

        /**
     * @brief Set and generate resources needed from the battle.
     *
     * This method creates resources needed for battle (mainly battlefield),
     * sets attacking and defending army and player ID of them,
     * and spawns stacks on the battlefield. This method must be called before
     * beginning of the battle.
     * @param attacker reference to army, which initialized battle.
     * @param defender reference to army, which was attacked by army initializing battle.
     *
     */
    void initializeBattle(Army& attacker, Army& defender);


        /**
     * @brief Increment the number of current battle turn.
     *
     * This value is incremented after each battle object
     * made any action related to the battle.
     *
     */
    void incrementBattleTurnNumber();


    /**
     * @brief Check if stacks of given army were defeated in the battle,
     * and remove them from the game.
     *
     * This method will check, if stacks should be destroyed, by checking their flag
     * setFlagDestroyAfterInteraction. If so, remove the stack from the game, by removing them
     * from army, battlefield, and battle order vector.
     *
     * @param army_to_check reference to army to check stacks.
     */
    void checkForDestroyedStacks(Army& army_to_check);


    /**
     * @brief Create the battle order vector, which indicates the order of
     * battle objects to take their turns.
     *
     * This method takes all battle objects of both armies, and create the order of objects to make actions.
     * Order is determined by BattlePriority of the objects, with attacking objects having higher priority in case
     * of same priority of objects from both armies.
     *
     * @param attacker reference to attacking army
     * @param defender reference to defending army
     */
    void generateBattleOrder(Army& attacker, Army& defender);


    /**
     * @brief Remove single stack from the game.
     *
     * This method removes stack from the battlefield, battle order and
     * army of this stack.
     *
     * @param stack_to_remove Reference to stack to remove.
     * @param army_of_stack Reference to army of stack to remove.
     * @return std::vector<std::shared_ptr<Stack>>::iterator Iterator
     * to the vector of the stacks of Army, indicating where stack was
     * removed in the army's stacks vector.
     */
    std::vector<std::shared_ptr<Stack>>::iterator removeStackFromGame(Stack& stack_to_remove, Army& army_of_stack);

    /// @brief Register this object to all objects related to the battle.
    void registerThisToBattleElements();

    /// @brief Deregister this object to all objects related to the battle.
    void deregisterThisFromBattleElements();

public:


    /**
     * @brief Construct a new Battle object
     *
     * @param attacking_army Attacking army of the battle.
     * @param defending_army Defending army of the battle.
     * @param attacking_army_controller  BattleController of attacking army (player).
     * @param defending_army_controller BattleController of defending army (player).
     * @param battlefield_type TileType of battlefield. The Battlefield object will be
     * created based on this object.
     */
    Battle(Army& attacking_army, Army& defending_army,
        BattleController& attacking_army_controller,
        BattleController& defending_army_controller,
        TileType battlefield_type);

    /**
    * @brief Runs the main battle loop, handling turns and determining the battle outcome.
    *
    * This function executes the turn-based battle logic between two armies.
    * Each iteration represents a turn, where the current battle object (hero or stack)
    * performs its actions via its associated BattleController.
    *
    *
    * @return int The result of the battle: 1 if attacker wins, -1 if defender wins.
    */
    int performBattleLoop();

    /**
     * @brief Get the battlefield object
     *
     * @return Battlefield& reference to battlefield.
     *
     * @note The must be battle occurring in order to access battlefield, otherwise this method causes exception.
     */
    Battlefield& getBattlefield() const;

        /**
     * @brief Check if Battlefield object currently exists.
     *
     * @return true if battlefield object in initialized, false if battlefield object is nullptr.
     *
     * @note This method can be used as indicator if battle is in progress.
     */
    bool isBattlefieldInitialized() const;


    /**
     * @brief Check if battle is currently occurring
     *
     * Check is being performed by checking, if battlefield was initialized.
     * @return true if battle is in progress, false otherwise.
     */
    bool isBattleInProgress() const;


    /**
     * @brief Get the current player ID.
     *
     * This method will return the ID of the current player object, which is currently taking its turn.
     *
     * @note There can be two players, which are currently taking their turns:
     * one for the turns of the world map, and one for the turns of the battle.
     * This method will return the ID of the player, which is currently taking its turn during the battle.
     *
     * @return int ID of the current player
     */
    Player& getCurrentPlayer() const;

        /**
     * @brief Get the player controller object of current player
     *
     * Get the player controller of current player, or get neutral player
     * controller in case when current army does not belong to any player.
     *
     * @return PlayerController&  reference to current player controller.
     */
    BattleController& getCurrentBattleController();

    /**
     * @brief Get ID of current player.
     *
     * @return int ID of current player.
     */
    int getCurrentPlayerID() const;


    /**
     * @brief Get battle object, which is currently performing actions
     *
     * @return BattleObject& current battle object
     */
    BattleObject& getCurrentBattleObject() const;

    /**
    * @brief Get vector of battle objects, which currently take part in the battle.
    *
    * @return std::vector<std::shared_ptr<BattleObject>>& reference to vector of battle objects
    *
    * @note battle order vector changes during the battle and is also used as a order of objects to perform.
    * First object of the vector is also the object, which is currently performing actions.
    *
     */
    const std::vector<std::shared_ptr<BattleObject>>& getBattleOrder() const;

    /**
     * @brief Make current battle object perform action regarging tile at given coordinates.
     *
     * Battle objects can perform actions based on the selected tile. This method will cause battle object to
     * do something on given tile (either walk to, or attack enemy which is on this tile) and, if successful
     * action was performed, will rotate battle order vector, which will indicate, that new object should take its turns.
     *
     * @param tile_x_pos x coordinate of tile to perform action on.
     * @param tile_y_pos y coordinate of tile to perform action on.
     */
    void performBattleAction(const int tile_x_pos, int const tile_y_pos);

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

    Army& getOppositeArmy() const;

    /**
     * @brief Get army, which is hostile to player with given ID.
     *
     * @param player_id ID of player, which does not control returned army.
     * @return Army& reference to enemy army.
     */
    Army& getOppositeArmy(const int player_id) const;

    /**
     * @brief Get army, which controls battle object, which is currently
     * performing battle actions.
     *
     * @return Army& reference to army, which is currently taking turns.
     */
    Army& getCurrentArmy() const;

    /**
     * @brief Get stack, which is currently taking its turn.
     *
     * This function must be called only when it is certain that current
     * battle object is a stack, otherwise this causes InvalidBattleObjectError.
     *
     * @return Stack& stack, which is currently taking its turn.
     */
    Stack& getCurrentStack() const;

    /**
     * @brief Skip (without doing any actions)
     * current battle object turn, and go to
     * next battle object.
     *
     * This method rotate the battle object vector
     * and sets flag flag_player_turn_finished to make sure that
     * battle object does exactly nothing and there
     * are no side effects of this decision.
     *
     */
    void endTurnPrematurely();

    /**
     * @brief Get the flag is player turn finished.
     *
     * This flag is mainly used in battle() to indicate that
     * current battle object turn was finished prematurely.
     */
    bool getFlagIsPlayerTurnFinished() const;

    ~Battle();
};

#endif
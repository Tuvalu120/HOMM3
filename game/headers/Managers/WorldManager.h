/**
 * @file WorldManager.h
 * @author Bartosz Han
 * @brief This file contains the definition of the WorldManager class.
 * The WorldManager class is responsible for managing the game world, including
 * the world map, player interactions and movements, and the game state.
 */


#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H


#include <memory>

#include "GameObjects/BaseObjects/InteractiveObject.h"
#include "Managers/GameManager.h"
#include "GameObjects/TileMaps/WorldMap.h"
#include "GameObjects/Player.h"
#include "Miscellaneous/BaseObjectGenerator.hpp"
#include "Managers/SubManager.h"

class WorldCanvasTileMapLabel;
class BaseObjectGenerator;

/**
 * @class WorldManager
 * @brief This class manages the game world, including the world map, player interactions and movement, and game state.
 *
 * The WorldManager is a manager responsible for creating and managing the world map, object which are on the map,
 * and player interactions with the world. It handles the game state, including player turns and moves, day and week transitions,
 * and the generation of world objects.
 */
class WorldManager: public Singleton<WorldManager>, public SubManager{

    friend class Singleton<WorldManager>;

private:
    WorldManager();
    std::unique_ptr<WorldMap> world_map_;
    std::vector<std::unique_ptr<BaseObjectGenerator>> world_dynamic_objects_generators_;
    std::vector<std::unique_ptr<BaseObjectGenerator>> world_static_objects_generators_;

    int current_day_;
    int current_player_id_;
    bool flag_was_army_eliminated_during_its_turn_;

    /// @brief This method is called at the end of the day to update the game state, mainly player turns and moves.
    void endDay();

    /// @brief This method is called at beginning of the day, to call all objects, which should be updated everyday, to update themselves.
    void onNewDay();

    /// @brief This method is called at beginning of the week, to call all objects, which should be updated every week, to update themselves.
    void onNewWeek();

    /**
     * @brief Remove the interactive object at the specified position of the world map.
     *
     * This method will remove the interactive object at the specified position (x_pos, y_pos)
     * on the world map, by removing its pointer from the tile at this position.
     *
     * @param x_pos x coordinate of the tile on the world map
     * @param y_pos y coordinate of the tile on the world map
     */
    void removeInteractiveObject(int x_pos, int y_pos);

    /**
     * @brief Attempts to interact with the interactive objects near the current player's army tile.
     *
     * This method will check for interactive objects on the tile bordering the tile where the current player's army is located,
     * and attempt to interact with them. If there are multiple objects, it will prioritize them based on their
     * interaction priority. Only one object with a higher priority will be interacted with, (such as towns or other armies),
     * while all other objects with 'low-priority' (such as mines, resources, etc.) will be interacted with.
     *
     * @note 'low-priority' objects are those with a priority of 1 (constants::OTHER_OBJECT_PRIORITY).
     *
     * @param only_low_priority If true, only low-priority objects will be interacted with.
     */
    void attemptToInteract(const bool only_low_priority = false);

    /**
     * @brief Generates world objects on the map.
     *
     * This method will call the updateObjects() method of each object generator
     * to generate the world objects (such as mines, resources, etc.) on the map.
     */
    void generateWorldObjects();

    /**
     * @brief Resets the visit flag of the neutral troops.
     *
     * Neutral troops are not allowed to be visited by the player army more then once pre turn.
     * This method will reset the flag 'was_army_visited_this_turn' of neutral troops
     * to allow them to be visited again during next army turn.
     */
    void resetNeutralTroopsVisitFlag();

public:

    /**
     * @brief ends the turn of the current army.
     *
     * This method will do the logic of ending the turn of the current army,
     * and prepare the game and resources for the next player's turn.
     */
    void endTurnOfArmy();

    /**
     * @brief Changes the current player to the next one.
     *
     * This method will change the current player to the next one in the list of players,
     * and will also update the game state and resources accordingly.
     */
    void goToNextPlayer();

    /**
     * @brief Get the World Map object
     *
     * @return WorldMap& Reference to the WorldMap object
     */
    WorldMap& getWorldMap();

    /**
     * @brief Creates the world map and initializes it.
     *
     * This method will create the world map and initialize it with the data from the map text file.
     *
     */
    void createWorldMap();

    /**
     * @brief Moves the current army to the specified tile on the world map.
     *
     * This method will move the current army, which is takes its turns,
     * to the specified tile (new_x_pos, new_y_pos). This method also checks if the move is valid,
     * and is also attempting to interact with the objects on the way of the army.
     *
     * @param new_x_pos x coordinate of the tile on the world map
     * @param new_y_pos y coordinate of the tile on the world map
     */
    void moveCurrentArmyToTile(const int new_x_pos, const int new_y_pos);

    /**
     * @overload
     *
     * @brief Moves the current army to the tile next to the current position of the army,
     * depending on the direction of the move.
     *
     * This method will move the current army, which is takes its turns,
     * to the specified tile next to current position of the army. This method also checks if the move is valid,
     * and is also attempting to interact with the objects on the way of the army.
     *
     * @param move direction, in which the army should be moved.
     *
     */
    void moveCurrentArmyToTile(const WorldMove move);

    /**
     * @brief Get the current player object.
     *
     * This method will return the current player object, which is currently taking its turn.
     *
     * @return Player& Reference to the current player object
     */
    Player& getCurrentPlayer() const;

    /**
     * @brief Get the current player ID.
     *
     * This method will return the ID of the current player object, which is currently taking its turn.
     *
     * @note There can be two players, which are currently taking their turns:
     * one for the turns of the world map, and one for the turns of the battle.
     * This method will return the ID of the player, which is currently taking its turn on the world map.
     *
     * @return int ID of the current player
     */
    int getCurrentPlayerID() const;

    /**
     * @brief Set the current player ID.
     *
     *
     * @param new_current_player ID of the new current player
     */
    void setCurrentPlayer(const int new_current_player);

    /**
     * @brief Get the current day of the game.
     *
     * @return int Current day of the game
     *
     * @note Day is a time, in which each player takes one turn. When all players take their turns, day counter is incremented.
     */
    int getCurrentDay() const;

    /**
     * @brief Get the current week of the game.
     *
     * @return int Current week of the game
     *
     * @note Week is a time, in which seven days pass and each player takes seven turns.
     */
    int getCurrentWeek() const;

    /**
     * @brief Create the world objects generators.
     *
     * This method will create the world objects generators, which are responsible for generating
     * WorldGeneratedInteractiveObjects on the world map.
     *
     */
    void initializeGenerators();

    /**
     * @brief Get the value of the power of the neutral objects.
     *
     * game_progress_value is a value, which is used to determine the power of the neutral objects,
     * such as neutral armies, armies of mines, etc. This value is increasing the further the game is going.
     *
     * @return int The game progress value
     */
    int getGameProgressValue() const;

    /**
     * @brief Set the player ID of all mines on the map belonging to the given player to -1.
     *
     * @param player_id ID of player whose mines should be reset.
     */
    void removeMineOwnershipOfPlayer(const int player_id);

    /**
     * @brief Set the flag indicating that army was removed from the game while it was
     * taking its world turns to true.
     *
     * This flag is used mainly to communicate, that army was permanently removed from the the game,
     * and it cannot perform more actions.
     *
     */
    void setFlagWasArmyEliminatedDuringItsTurn();

    /**
     * @brief Get the TileType of the tile, on which there is army, which is currently taking its turns.
     *
     * @return TileType type of the tile on which there is current army.
     */
    TileType getTileTypeOfCurrentArmyTile();

    /**
     * @brief Get the Object Generator object, based on the type of the object
     * which this generator is creating
     *
     * @param object_type_generated_by_generator ObjectType of the object
     * generated by the object generator to get access of.
     *
     * @return BaseObjectGenerator& the object generator which is generating given objects.
     */
    BaseObjectGenerator& getObjectGenerator(const ObjectType object_type_generated_by_generator) const;

    void resetManager() override;

};

#endif
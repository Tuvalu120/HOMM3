/**
 * @file Player.h
 * @author Bartosz Han
 * @brief this file contains Player class, which is abstraction of one of the users of the game. It contains data and interface to user's resources, as
 * well as methods to manage them.
 *
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "GameObjects/WorldArmy.h"
#include "GameObjects/BaseObjects/OwnedByPlayer.h"
#include "PlayerControllers/PlayerController.h"
#include "Data/Constants.hpp"
#include "Data/ObjectDataEnums.hpp"

class PlayerController;

/**
 * @class Player
 * @brief This class is an abstraction of one of users of the game. It contains data and interface to user's resources, as
 * well as methods to manage them.
 *
 * Mainly, this class contains vector of the world armies of the player. World armies are objects, which player can control to explore world map.
 * This class is responsible for managing Resources (currency-like data) of the player.
 * This class is also responsible for various other aspects of game related to the player, including managing the tiles of world map, which are visible for the player.
 *
 * Player are considered active, while they control at least one world army and at least one town. Eliminated players are removed from game.
 *
 */
class Player: public OwnedByPlayer{
private:
    std::shared_ptr<PlayerController> player_controller_;
    std::array<std::shared_ptr<WorldArmy>, constants::MAX_NUMBER_OF_ARMIES> armies_;
    int current_army_id_;
    ResourceArray player_resources_;
    std::vector<std::pair<int,int>> explored_world_tiles_;

    /**
     * @brief Move armies in army, in such way that the are not null pointers between valid armies. If needed, set current army id to valid one.
     *
     * This function is used to ensure, that armies can take its turns correctly, when one of them was eliminated from the game.
     *
     */
    void moveArmiesInArray();

    /// @brief Get current ID of provided army with player's army vector.
    /// Armies do not have information about its IDs in player's army vector, as this ID might change during the game.
    int getArmyID(const Army* army_to_check) const;

    std::shared_ptr<WorldArmy> attemptToAddArmy(std::shared_ptr<WorldArmy> army_to_add);

public:

    /**
     * @brief Construct a new Player object.
     *
     * model_file_name controls, which type of player will this object be. If no file path will be given - this player will be
     * controlled by user. If there is path declared, this player will be controlled by AI.
     *
     * @param player_id ID of player.
     * @param model_file_name File path to the AI model for the player to control it. If no path given, then this player will be controlled by user.
     */
    explicit Player(int player_id, std::string model_file_name = "");

    /// @brief Get array with pointers to all world armies controlled by this player.
    /// @note There might be nullptrs in this array.
    const std::array<std::shared_ptr<WorldArmy>, constants::MAX_NUMBER_OF_ARMIES>& getArmies() const;

    /**
     * @brief Create new army for the player.
     *
     * This method will create new army, with initial data based on the hero of army and extracted from game_data::getHeroData() function.
     *
     * This method does not add new army into world map.
     * This method will return nullptr, if it was not possible to create army for the player.
     *
     * @param hero_id HeroType of hero of new army.
     * @param init_x_pos Initial X position of army on world map.
     * @param init_y_pos Initial Y position of army on world map.
     * @return std::shared_ptr<WorldArmy> Pointer to newly created army, or nullptr if new army was nto created.
     */
    std::shared_ptr<WorldArmy> addArmyToPlayer(const HeroType hero_id, const int init_x_pos, const int init_y_pos);

    /// @brief Get armies array ID of army, which is currently taking its truns on world map.
    /// If this player does not currently take its turn, this value should be ignored.
    int getCurrentArmyID() const;

    /// @brief Get maximum number of armies, which this player can have in this game.
    int getNMaxArmies() const;

    /// @brief Get current number of armies controlled by this player.
    int getNArmies() const;

    /// @brief Set the value, indicating which army of this player will be taking turns on world map now, to a new value.
    void setCurrentArmyID(const int army_id);

    /// @brief Check, if at provided index, in player's object vector, there is a pointer to any army, or nullptr.
    bool doesArmyExist(const int army_to_check_index) const;

    /// @brief Get reference to the WorldArmy, which is currently taking its turns on world map.
    WorldArmy& getCurrentArmy() const;
    int goToNextArmy();

    /**
     * @brief Get the ID of provided army.
     *
     * Armies does not have control over its ID within Player's army vector.
     * This method provides the way to get army's ID.
     *
     * @param army_to_check Army to get ID.
     * @return int Id of army.
     */
    int getArmyID(const Army& army_to_check) const;

    /// @brief Get the reference to world army at provided index.
    WorldArmy& getArmyByID(int army_id);

    /// Get the array representing player's current values of all Resources (currencies).
    const ResourceArray& getPlayerResources() const;

    /**
     * @brief Check, if player has enough resources to spend the amount of resources as provided in resources_quantity array.
     *
     * This method does not change the quantities of resource.
     *
     * @param resources_quantity Amount to resources expected to spend.
     * @return True, if player have enough resources to spent on every resource in resources_quantity array, false otherwise.
     */
    bool canAfford(const ResourceArray& resources_quantity) const;

    /**
     * @brief Attempt to decrease the amount of player's Resources, based on the amounts provided in resources_quantity array.
     *
     * If player does not have enough resources, this method does nothing.
     *
     * @return True, if resources were successfully decreased, false otherwise.
     */
    bool spendResources(const ResourceArray& resources_quantity);

    /**
     * @brief Attempt to decrease the amount of player's Resource of type and by quantity provided in this method.
     *
     * If player does not have enough resources, this method does nothing.
     *
     * @param resource_type ResourceType to decrease from player.
     * @param resource_quantity Quantity of resources to decrease.
     * @return True, if resources were successfully decreased, false otherwise.
     */
    bool spendResources(const Resource resource_type, const unsigned int resource_quantity);

    /// @brief Add new resources to the player, by the amounts provided in resource quantity.
    void addResources(const ResourceArray& resources_quantity);

    /// @brief Increase the amount of resource_type resource of player by resource_quantity amount.
    void addResources(const Resource resource_type, const unsigned int resource_quantity);

    /**
     * @brief Get the vector of explored by this player tiles of world map.
     *
     * Player does not have access to whole world map at the beginning of the game - they have to discover it, by either visiting new tiles, or being
     * close enough to them.
     * this vector stores the information about all tiles visited and discovered by the player.
     *
     * @return const std::vector<std::pair<int,int>>& Reference to the vector of pairs <int, int>, which indicates to coordinates of tiles discovered by this player.
     */
    const std::vector<std::pair<int,int>>& getExploredWorldTiles() const;

    /**
     * @brief Check, if tile at provided position has already been discovered by the player.
     *
     * @param tile_pos Position (as a pair of integer coordinates) of the tile to check.
     * @return True if player has already discovered the Tile. false otherwise.
     */
    bool hasTileBeenVisited(const std::pair<int,int> tile_pos) const;

    /// @brief Mark given coordinates as visited, simply by adding them to the vector fo visited tiles.
    void markTileAsVisited(const std::pair<int,int> tile_pos);

    /**
     * @brief Get the PlayerController object of this player.
     *
     * PlayerController object is a object responsible for controlling AI player during the game.
     *
     * This method will throw an error, if called from non-AI player.
     *
     * @return PlayerController&
     */
    PlayerController& getPlayerController() const;

    std::shared_ptr<PlayerController> getPlayerControllerPtr() const;


    /// @brief returns true, if new army can be added to this player.
    bool canAddNewArmy();

    /**
     * @brief Retruns true, if player is active and can still take turns. False otherwise.
     *
     * Player are considered active, while they control at least one world army and at least one town.
     *
     */
    bool isPlayerActive();

    /// Remove army both from the player's armies vector, and from the world map. Also, do the logic related to this event.
    void removeArmy(const int army_id);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

};

#endif
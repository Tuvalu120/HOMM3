/**
 * @file PlayerManager.h
 * @author Bartosz Han
 * @brief This file contains PlayerManager class, which is used for managing players and their resources.
 *
 */
#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <memory>

#include "Managers/SubManager.h"
#include "Managers/GameManager.h"
#include "GameObjects/Player.h"
#include "PlayerControllers/PlayerController.h"

/**
 * @class PlayerManager
 * @brief This class is used for managing players and their resources.
 *
 * This class contains data about players and their state. It also provides inteface to get access to data of players.
 * This class is also responsible to have control over neutral player controller.
 *
 */
class PlayerManager : public Singleton<PlayerManager>, public SubManager{

    friend class Singleton<PlayerManager>;

private:
    PlayerManager();
    std::vector<std::unique_ptr<Player>> players_;
    std::vector<int> player_results_;
    std::shared_ptr<BattleController> neutral_player_controller_;

    /**
     * @brief Add new Player to the gave with given ID.
     *
     * @param player_id ID of new player.
     */
    void addNewPlayer(const int player_id);

    public:
    /**
     * @brief Return the number of players at the beginning of the game.
     *
     * This method returns number of all players, which were in game, regardless of their current status.
     *
     * @return int Number of players at the beginning of the game.
     */
    int getNOfPlayers() const;

    /**
     * @brief Return the number of currently active players.
     *
     * This method does not take in consideration players which were eliminated during the game.
     *
     * @return int
     */
    int getNOfActivePlayers() const;

    /**
     * @brief Check, if player with given ID is currently active.
     *
     * Check is being performed by checking, if pointer to the player is nullptr, or not.
     *
     * @param player_id Id of player to check.
     * @return True if player is active (pointer is not nullptr), false otherwise.
     */
    bool isPlayerActive(const int player_id) const;

    /**
     * @brief Get the Player object by its ID.
     *
     * @param player_id ID of player object to get.
     * @return Player& Reference to the player with given ID.
     *
     */
    Player& getPlayerByID(const int player_id) const;

    /**
     * @brief Attempt to add new army to player with given ID. The initial position of army is at given coordinates.
     *
     * New army wil ob created by Player::addArmyTOPlayer() method. If army was created properly, it will be added to the
     * WorldMap and other actions related to the world map and new army will be performed.
     *
     * @param player_id ID of player to add new army.
     * @param hero_id HeroType of new army. Data about this army will be generated via game_data::getHeroData() function.
     * @param init_x_pos Init x position of the army.
     * @param init_y_pos Init y position of the army.
     *
     * @return True if new army was successfully added to the player and map, false otherwise.
     */
    bool addArmyToPlayer(const int player_id, const HeroType hero_id, const int init_x_pos, const int init_y_pos);

    /**
     * @brief Removed player and its resources from the game.
     *
     * This method will remove Player object from the vector (by setting pointer to it to nullptr),
     * and will either remove resources (such as armies of player) of set them to become neutral again (such as town, mines, etc.).
     *
     * @param player_id ID of player to remove.
     */
    void removePlayerFromGame(const int player_id);

    /**
     * @brief Get the vector of player ID, in which indexes of players' ID correspond to their position at the end of game.
     *
     * For example, ID at index 0 will correspond to player, which won the game.
     * This method should only be used when game is finished, but I can be used earlier to determine order of which all removed players were eliminated.
     *
     * @return const std::vector<int>&
     */
    const std::vector<int>& getGameResults() const;

    /**
     * @brief Get the vector of players' ID, which contains only players, which are still active.
     *
     * @return const std::vector<int> Vector of active players' ID.
     */
    const std::vector<int> getRemainingPlayers() const;

    /**
     * @brief Check if player meet conditions to be eliminated from the game, and if so, remove it from game.
     *
     * Player, which should be eliminated, should either lost all of their armies, or lost all of their towns.
     *
     * @param player_id ID of player to check, if was eliminated.
     * @param check_during_player_turn flag indicating wether or not player was eliminated, when one of their army was currently taking
     * turns in world map. If so, the procedure to remove player is slightly changed.
     *
     */
    void checkForEliminatedPlayer(int player_id, bool check_during_player_turn);

    /**
     * @brief Create a given number of players.
     *
     * This method will not only create Players objects, but also will give them necessary resources (one army and one town)
     * to begin the game.
     * This method should be called at the beginning of the game, and should only be called once.
     *
     * @param n_players Number of players to generate.
     *
     */
    void initializePlayers(int n_players);

    /**
     * @brief Check, if given player ID is valid player ID.
     *
     * This is validation method for objects, which should belong to player.
     * This method checks, if such object belong to existing player, or throws InvalidPlayerIDError if not.
     *
     * @param player_id Player ID of checked object.
     * @param object_name Name of the object, used to construct exception.
     * @param can_be_neutral Flag whether or not player_id of -1 is also valid player ID for this object.
     * Object with ID of -1 represent neutral objects.
     */
    void validatePlayerID(const int player_id, const std::string object_name="", const bool can_be_neutral=false) const;

    /**
     * @brief Get neutral player controller.
     *
     * Neutral player controller is responsible for controlling resources, which should be controlled by players,
     * but these resource do not have player assigned to them and are neutral.
     *
     * @return PlayerController& reference to neutral player controller.
     */
    BattleController& getNeutralPlayerController();

    /**
     * @brief Get pointer to Neutral PlayerController.
     * This controller is used by neural armies.
     */
    std::shared_ptr<BattleController> getNeutralPlayerControllerPtr() const;

    /**
     * @brief Set pointer to Neutral PlayerController.
     * This controller will be used used by neural armies.
     */
    void setNeutralPlayerController(std::shared_ptr<BattleController> controller);

    void resetManager() override;

};

#endif
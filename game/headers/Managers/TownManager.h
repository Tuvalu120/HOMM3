/**
 * @file TownManager.h
 * @author Bartosz Han
 * @brief This file constains the TownManager class, which is responsible for managing
 * towns and their resources and interactions.
 *
 */
#ifndef TOWN_MANAGER_H
#define TOWN_MANAGER_H

#include "Managers/SubManager.h"
#include "Managers/GameManager.h"
#include "GameObjects/Town.h"
#include "Data/Constants.hpp"

/**
 * @class TownManager
 * @brief This class is for managing towns and their resources and interactions
 *
 * This class provides access to towns, controls their behaviour and
 * provides interactions of the towns with other components.
 */
class TownManager: public Singleton<TownManager>,  public SubManager{

    friend class Singleton<TownManager>;

private:
    TownManager();
    std::array<std::unique_ptr<Town>, constants::NUMBER_OF_TOWNS> towns_;
    int current_town_id_;

public:
    /**
     * @brief Get the town object with given ID
     *
     * @param id ID of town to get access to
     *
     * @return Town& reference to the town with given ID
     */
    Town& getTownWithID(const int id) const;

    /**
     * @brief Generate the towns on the world map, and generate their initial resources
     *
     * This method will generate the towns, by creating n_towns towns on coordinates selected randomly from valid coordinates.
     *
     * @param possible_town_coords Vector of coordinates, where towns can be generated. If number of valid coordinates is greater than
     * number of town to generate, coordinates will be selected randomly from this vector.
     * @param n_towns Number of towns to generate.
     *
     */
    void generateTowns(const std::vector<std::pair<int, int>> possible_town_coords, const int n_towns=constants::NUMBER_OF_TOWNS);

    /**
     * @brief Get the ID of the Town which is being currently visited by any player.
     *
     * @return int ID of the being currently visited.
     */
    int getCurrentTownID() const;

    /**
     * @brief Get the Town which is being currently visited by any player.
     *
     * @return Town& Reference to the Town currently being visited.
     *
     * @note If no Town is being currently visited, this method will throw TownLogicException.
     */
    Town& getCurrentTown() const;

    /**
     * @brief Set current town ID to -1.
     *
     */
    void resetCurrentTownID();

    /**
     * @brief Set current town ID to new value
     *
     * @param new_town_id ID of town being currently visited
     */
    void setCurrentTownID(const int new_town_id);

    /**
     * @brief Make all towns do logic related to the change of the day to a new one.
     *
     */
    void onNewDay();

    /**
     * @brief Make all towns do logic related to the change of the week to a new one.
     *
     */
    void onNewWeek();

    /**
     * @brief Reset visit flags of each town.
     *
     * Enemy Town can be visited only once during army turn. This method will reset visit flag of each
     * town after the end of turn fo army.
     */
    void resetFlagsWasTownVisitedThisTurn();

    /**
     * @brief Get the number of towns, which currently belong to given player.
     *
     * @param player_id Id of player to check number of towns.
     * @return int Number of towns belonging to given player.
     */
    int getNOfTownsOfPlayer(const int player_id) const;

    /**
     * @brief Set the ID of towns, which belonged to given player, to -1
     *
     * @param player_id ID of player whose towns should be reset.
     */
    void removeTownOwnershipOfPlayer(const int player_id);

    void resetManager() override;

};

#endif
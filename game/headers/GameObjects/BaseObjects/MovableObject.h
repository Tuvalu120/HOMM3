/**
 * @file MovableObject.h
 * @author Bartosz Han
 * @brief This file contains MovableObject class, which is used to create objects, which can change their position on the tile maps.
 *
 */
#ifndef MOVABLE_OBJECT_H
#define MOVABLE_OBJECT_H

#include "GameObjects/BaseObjects/InteractiveObject.h"

/**
 * @class MovableObject
 * @brief This class is used to create objects, which can freely change their position on the tile maps.
 *
 * Normally, instances of InteractiveObject should not change their postion on the world map.
 * This class is used by the game resources to make moving objects easily and safely within the map.
 * In addition, this class contains and manages information number of possible tiles, which this object can travel pass.
 */
class MovableObject : virtual public InteractiveObject{
private:

    int max_moves_;
    int moves_left_;

    /**
     * @brief Set the new x position of this object.
     *
     * This method only changes the value of variable x_pos_ of this class,
     * and does not have any effect on the state of the tile map.
     * @param new_x_pos New x position of the tile.
     */
    void setXPos(const int new_x_pos);

    /**
     * @brief Set the new y position of this object.
     *
     * This method only changes the value of variable y_pos_ of this class,
     * and does not have any effect on the state of the tile map.
     * @param new_x_pos New x position of the tile.
     */
    void setYPos(const int new_y_pos);

public:

    /**
     * @brief Construct a new MovableObject.
     *
     * @param initial_texture_name Path to texture of this object.
     * @param object_type Type of the object.
     * @param x_pos X coordinate of the object.
     * @param y_pos Y coordinate of the object.
     * @param max_moves Number of tiles, which this object can travel by during its turn.
     * @param priority Integer representing priority of the object, when other object might attempt to interact with several objects at a time.
     * @param destroy_after_interaction Flag, informing whther or not this object should be destroyed after one interaction with other object.
     *
     */
    MovableObject(const std::string initial_texture_name, const ObjectType object_type, const int x_pos, const int y_pos, const int max_moves,
        const int priority = 1, const bool destroy_after_interaction = false);

    /**
     * @brief Construct a new MovableObject.
     *
     * This constructor should be used in situations, when object has not been, or cannot be added into any of world map.
     * The initial coordinates of this object will be (-1, -1).
     *
     * @param initial_texture_name Path to texture of this object.
     * @param object_type Type of the object.
     * @param max_moves Number of tiles, which this object can travel by during its turn.
     *
     */
    MovableObject(const std::string initial_texture_name, const ObjectType object_type, const int max_moves);

    /**
     * @brief Get the maximum number of moves, which this object can travel on any single turn.
     *
     * This method does not return number of moves, which object can currently take - only maximum number of moves, which object can take
     * on one turn through the game.
     *
     * One move is defined as going into one of neighbouring tiles.
     */
    int getMaxMoves() const;

    /**
     * @brief Set the maximum number of moves, which this object can travel on any single turn.
     *
     * This method will also overwrite number of moves left on current turn, if it be greater then new maximum number of moves.
     *
     * @param new_max_moves New number of maximum number of moves on single turn.
     */
    void setMaxMoves(const int new_max_moves);

    /**
     * @brief Get the number of moves, which object can still take during this turn.
     * One move is defined as going into one of neighbouring tiles.
     */
    int getMovesLeft() const;

    /**
     * @brief Check, if object can make a move, which would require him to move given number of tiles.
     *
     * @param moves_to_go Number of tiles, which player must travel by when making move.
     * @return true if player can perform the action (has enough moves to go), false otherwise.
     */
    bool canMove(const int moves_to_go) const;

    /**
     * @brief Check, if player can make any move.
     * @return true, if player has number of moves left grater than 0, false otherwise.
     */
    bool canMove() const;

    /**
     * @brief Add additional moves to the current pool of moves, but not more than maximum number of moves.
     *
     * @param number_to_add Number of moves to add.
     */
    void addMoves(int number_to_add);

    /**
     * @brief Reduce the number of moves of object in current turn.
     *
     * @param number_to_remove Number of moves to reduce.
     */
    void removeMoves(int number_to_remove);

    ///@brief Set the current number of moves to become the maximum number of moves currently allowed.
    void resetMoves();

    template <typename T>
    friend class TileMap;
};

#endif
/**
 * @file OwnedByPlayer.h
 * @author Bartosz Han
 * @brief This file contains declaration of OwnedByPlayer class, which is an interface used by the several classes, whose
 * instances should be associated with players.
 *
 */
#ifndef OWNED_BY_PLAYER_H
#define OWNED_BY_PLAYER_H

/**
 * @class OwnedByPlayer
 * @brief This interface is used by several classes, whose instances should be associated with oen of players.
 *
 * This class contain one variable - player_id_, which is used to determine owner of this object. The value of this variable can be any of
 * player's ID, and, if this is possible, it can also have value of -1, indicating, that this object does not currently have owner.
 */
class OwnedByPlayer{
private:
    int player_id_;

protected:
    /**
     * @brief Set new player ID.
     *
     * This method, unlike setPlayerID(), can override the ownership of another player.
     *
     * @param new_player_id New player ID.
     */
    void replacePlayerID(int new_player_id);

public:
    /**
     * @brief Construct a new OwnedByPlayer object.
     *
     * @param player_id Initial ID of player, who own this object.
     */
    explicit OwnedByPlayer(int player_id);

    /**
     * @brief Construct a new OwnedByPlayer object, by passing ownership of another object into this object.
     *
     * @param parent Another OwnedByPlayer object, whose owner player's ID should be passed into this object.
     */
    explicit OwnedByPlayer(const OwnedByPlayer& parent);

    /// @brief Construct a new OwnedByPlayer object with no owner (ID will be set to -1).
    OwnedByPlayer();

    /// @brief Get Player ID of owner of this object.
    virtual int getPlayerID() const;

    /**
     * @brief Set the player ID.
     *
     * This method can only be used, if this object does not have an owner yet.
     *
     * @param new_player_id New player ID.
     */
    virtual void setPlayerID(int new_player_id);

};

#endif
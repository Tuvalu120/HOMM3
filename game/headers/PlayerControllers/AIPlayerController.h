/**
 * @file AIPlayerController.h
 * @author Bartosz Han
 * @brief This file contains AIPlayerController, which is used by AI player to make decisions about the game.
 *
 */

#ifndef AI_PLAYER_CONTROLLER_H
#define AI_PLAYER_CONTROLLER_H

#include "PlayerControllers/PlayerController.h"

/**
 * @class AIPlayerController
 *
 * @brief This class is used by Players controlled by AI to make decisions about the game.
 */
class AIPlayerController : public PlayerController{

public:
    AIPlayerController();

    void makeWorldMoveDecision() override;
    void makeWorldDecision() override;
    void makeBattleDecision() override;
    void makeSpellDecision() override;
    void makeTownDecision() override;

};

#endif
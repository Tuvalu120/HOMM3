/**
 * @file HumanPlayerController.h
 * @author Bartosz Han
 * @brief This file contains HumanPlayerController, which is used by users as an input to make decisions about the game.
 *
 */

#ifndef HUMAN_PLAYER_CONTROLLER_H
#define HUMAN_PLAYER_CONTROLLER_H

#include "PlayerControllers/PlayerController.h"

/**
 * @class HumanPlayerController
 * @brief This class, which inherits from PlayerController, is used by users as an input to make decisions about the game.
 *
 * When making decisions using objects of this class, theses objects are blocking the loop of the game, and are awaiting the player's
 * input from GUI.
 *
 * Objects of this class should only used by Players, which are users of the game and thus, should only be used only in GUI version of the game.
 *
 */
class HumanPlayerController : public PlayerController{

    private:

        /**
         * @brief Block the game loop, abd await in loop for the decision vars to be set.
         *
         * This method will be check in loop for decision vars to be set. Decision vars should be set via GUI, as this method is also
         * collecting event from GUI to process.
         *
         * This method will unblock once the decision variables are set.
         *
         * @param n_of_vars Number of decision variables to be checked.
         */
        void waitForDecisionVars(const int n_of_vars) const;

    public:
        HumanPlayerController();

        void makeWorldMoveDecision() override;
        void makeWorldDecision() override;
        void makeBattleDecision() override;
        void makeSpellDecision() override;
        void makeTownDecision() override;

};

#endif
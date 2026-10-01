/**
 * @file AIPlayerController.cc
 * @author Bartosz Han
 * @brief This file contains definition of class AIPlayerController.
 *
 */

#include "PlayerControllers/AIPlayerController.h"

AIPlayerController::AIPlayerController():PlayerController(AI){};

void AIPlayerController::makeWorldMoveDecision(){};
void AIPlayerController::makeWorldDecision(){};
void AIPlayerController::makeBattleDecision(){};
void AIPlayerController::makeSpellDecision(){};
void AIPlayerController::makeTownDecision(){};

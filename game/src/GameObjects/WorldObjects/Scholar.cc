/**
 * @file Scholar.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Scholar.
 *
 */


#include "GameObjects/WorldObjects/Scholar.h"
#include "Decisions/Decision.h"
#include "Managers/DecisionManager.h"
#include "Data/GameData.hpp"

using namespace std;
using namespace constants;
using namespace game_data;

Scholar::Scholar():WorldGeneratedInteractiveObject(SCHOLAR_IMAGE, SCHOLAR_OBJECT_TYPE,
    priority_=OTHER_OBJECT_PRIORITY, destroy_after_interaction_=false){};

void Scholar::interact(){
    unique_ptr<ScholarDecision> decision = make_unique<ScholarDecision>();
    DecisionManager::getInstance().makeDecision(std::move(decision));
    setFlagDestroyAfterInteraction();
}

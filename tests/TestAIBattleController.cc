 /**
 * @file TestAIBattleController.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class AIBattleController and VCMIBattleController.
 **/


#include "AIBattleControllers/BattleAction.hpp"
#include "CatchConfig.hpp"
#include "AIBattleControllers/AIBattleController.h"
#include "AIBattleControllers/VCMIBattleController.h"
#include "Data/GameData.hpp"
#include "Data/ObjectDataEnums.hpp"
#include "GameObjects/Battle.h"
#include "PlayerControllers/PlayerController.h"
#include "TestUtilities.hpp"
#include <memory>
#include <utility>


class MockAIBattleController : public AIBattleController{
    private:
    const BattleAction action_;
    public:
    MockAIBattleController(BattleAction action_to_perform):action_(action_to_perform){};

    std::pair<int, int> selectBattleAction() override{
        return battleActionToCoordinates(action_);
    }
};




TEST_CASE("Attack Closest Enemy Test", "[AIBattleController]"){

    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_CLOSEST_ENEMY);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 17);
    CHECK(target.second == 3);

}

TEST_CASE("Attack Furthest Enemy Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_FURTHEST_ENEMY);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 2);
}

TEST_CASE("Attack Enemy With Lowest Health Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WITH_LOWEST_HEALTH);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 2);
}

TEST_CASE("Attack Enemy With Biggest Health Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WITH_BIGGEST_HEALTH);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 17);
    CHECK(target.second == 3);
}

TEST_CASE("Attack Enemy With Lowest Attack Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WITH_LOWEST_ATTACK);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 2);
}

TEST_CASE("Attack Enemy With Biggest Attack Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WITH_BIGGEST_ATTACK);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 17);
    CHECK(target.second == 3);
}

TEST_CASE("Go Forward Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(GO_FORWARD);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 1);
    CHECK(target.second == 1);
}

TEST_CASE("Hide Stack Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(HIDE_STACK);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 1);
    CHECK(target.second == 3);
}

TEST_CASE("Go To Friendly Stacks Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(GO_TO_FRIENDLY_STACKS);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 2);
    CHECK(target.second == 5);
}

TEST_CASE("Spread From Friendly Stacks Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(SPREAD_FROM_FRIENDLY_STACKS);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 5);
    CHECK(target.second == 1);
}

TEST_CASE("Attack Enemy For Biggest Attack Loss Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_FOR_BIGGEST_ATTACK_LOSS);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 7);
}

TEST_CASE("Execute Enemy With Biggest Attack Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(EXECUTE_ENEMY_WITH_BIGGEST_ATTACK);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 2);
}

TEST_CASE("Attack Enemy With Biggest Range Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WITH_BIGGEST_RANGE);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 7);
}

TEST_CASE("Attack Enemy Which Can Kill My Stack Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WHICH_CAN_KILL_MY_STACK);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 17);
    CHECK(target.second == 3);
}

TEST_CASE("Attack Enemy Which Can Reduce My Attack By Most Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WHICH_CAN_REDUCE_MY_ATTACK_BY_MOST);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 17);
    CHECK(target.second == 3);
}

TEST_CASE("Attack Enemy Which Is Next In Order Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(ATTACK_ENEMY_WHICH_IS_NEXT_IN_ORDER);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 17);
    CHECK(target.second == 3);
}

TEST_CASE("Protect Strongest Friendly Stack Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(PROTECT_STRONGEST_FRIENDLY_STACK);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 5);
    CHECK(target.second == 5);
}

TEST_CASE("Do Nothing Battle Action Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<MockAIBattleController> controller = std::make_shared<MockAIBattleController>(DO_NOTHING_BATTLE_ACTION);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == -2);
    CHECK(target.second == -2);
}

TEST_CASE("VCMI Battle Controller Test", "[AIBattleController]") {
    initializeMockGame(1234);
    std::shared_ptr<Army> attacker = Army::generatePatternArmy(7, CASTLE_TROOPS);
    std::shared_ptr<Army> defender = Army::generatePatternArmy(7, CASTLE_TROOPS);
    attacker->setPlayerID(0);
    defender->setPlayerID(1);

    std::shared_ptr<VCMIBattleController> controller = std::make_shared<VCMIBattleController>(10);

    Battle battle(*attacker, *defender, *controller, *controller, GRASS_TILE);

    std::pair<int, int> target = controller->selectBattleAction();
    CHECK(target.first == 18);
    CHECK(target.second == 7);
}
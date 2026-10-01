/**
 * @file battle_example/main.cc
 * @author Bartosz Han
 * @brief This file contains main function of the battle example program.
 */

#include "BattleScenario.hpp"
#include "GameObjects/HeroArmy.h"
#include "Managers/BattleManager.h"
#include "Managers/GameManager.h"
#include "Managers/GraphicsManager.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include "RealGraphicsManager.h"
#include "PlayerControllers/HumanPlayerController.h"
#include "AIBattleControllers/NEATBattleController.h"
#include "TrainerUtilities.hpp"
#include "Hyperparams.hpp"
#include "AIBattleControllers/AIBattleControllerConstants.hpp"
#include "NEAT.h"
#include "Utilities.hpp"
#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>

#include <QTimer>
#include <string>


int main(int argc, char** argv){

    GraphicsManager::registerInstance(new RealGraphicsManager());
    GraphicsManager::getInstance().setUpGameWindow();
    GraphicsManager::getInstance().showWindow();

    if(argc <= 1){
        throw std::invalid_argument("No model file specified");
    }

    if(argc > 2){
        size_t seed = std::stoul(argv[2]);
        setRandomNumberGeneratorSeed(seed);
    }

    else{
        setRandomNumberGeneratorSeed();
    }

    std::string model_filename(argv[1]);
    std::shared_ptr<HumanPlayerController> human_pc = std::make_shared<HumanPlayerController>();

    NEAT::getInstance().loadNodeQuantityInfo(
        ai_battle_controller_constants::EXPECTED_N_IN_NODES,
        ai_battle_controller_constants::EXPECTED_N_OUT_NODES);

    NeuralNetwork ai_model = NeuralNetwork::fromFile(model_filename);
    std::shared_ptr<NEATBattleController> ai_pc = std::make_shared<NEATBattleController>(ai_model);
    GraphicsManager::getInstance().setFlagIsAIVerbose(true);
    GameManager::getInstance().setFlagGameManagerFinishesGame(false);
    GraphicsManager::getInstance().startDrawing();

    QTimer::singleShot(0, [&](){

        BattleScenario scenario = BattleScenario::createRandomScenario();
        HeroArmy army_1 = createArmyFromScenario(scenario, 0);
        HeroArmy army_2 = createArmyFromScenario(scenario, 1);
        int result = BattleManager::getInstance().battle(army_1, army_2, *human_pc, *ai_pc, scenario.battlefield_type);
        std::cout << "#########" << std::endl;
        if(result == 1){
            std::cout << "PLAYER WON" << std::endl;
        }
        else if (result == -1){
            std::cout << "AI WON" << std::endl;
        }
        GraphicsManager::getInstance().quitApplication();
    });

    return GraphicsManager::getInstance().execApp();
}
/**
 * @file GameManager.cc
 * @author Bartosz Han
 * @brief This file contains definition of class GameManager.
 *
 */


#include "Managers/GameManager.h"
#include "Data/GameData.hpp"
#include "Managers/IGraphicsManager.h"
#include "Managers/SubManager.h"
#include "Data/Constants.hpp"
#include "Managers/GraphicsManager.h"
#include "Managers/TownManager.h"
#include "Managers/WorldManager.h"
#include "Managers/PlayerManager.h"
#include "AIBattleControllers/NEATBattleController.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>


using namespace std;
using namespace constants;

GameManager::GameManager(){}

void GameManager::initialize(int argc, char *argv[]){

    argc_ = argc;
    argv_ = argv;

    int n_players = MAX_NUMBER_OF_PLAYERS;
    std::string model_filename = game_data::AI_MODEL_FILEPATH;

    if(argc > 1){
        int new_n_players = stoi(argv[1]);
        //minimum 2 players and maximum MAX_NUMBER_OF_PLAYERS may play the game.
        if(new_n_players <= MAX_NUMBER_OF_PLAYERS && new_n_players >= 2){
            n_players = stoi(argv[1]);
        }
        else{
            throw invalid_argument("Invalid number of players: " + string(argv[1]));
        }
    }


    if(argc > 2){
        model_filename = argv[2];
    }


    size_t seed;
    bool was_seed_set = false;
    if(argc > 3){
        seed = stoul(argv[3]);
        was_seed_set = true;
    }
    if(was_seed_set){
        setRandomNumberGeneratorSeed(seed);
    }
    else{
        setRandomNumberGeneratorSeed();
    }

    const NeuralNetwork network = NeuralNetwork::fromFile(model_filename);
    PlayerManager::getInstance().setNeutralPlayerController(make_shared<NEATBattleController>(network));
    GraphicsManager::getInstance().setUpGameWindow();

    WorldManager::getInstance().createWorldMap();
    TownManager::getInstance().generateTowns(WorldManager::getInstance().getWorldMap().getTownPossibleCoords());

    PlayerManager::getInstance().initializePlayers(n_players);
    WorldManager::getInstance().initializeGenerators();
    GraphicsManager::getInstance().startDrawing();
    GraphicsManager::getInstance().showWindow();
}

void GameManager::play(){
    while(!isGameFinished()){
        WorldManager::getInstance().getCurrentPlayer().getPlayerController().makeWorldMoveDecision();
    }
}

void GameManager::registerSubManager(SubManager* sub_manager){
    sub_managers_.push_back(sub_manager);
}

void GameManager::resetGame(){
    for(auto& sub_manager: sub_managers_){
        sub_manager->resetManager();
    }
}

bool GameManager::isGameFinished() const{
    return PlayerManager::getInstance().getNOfActivePlayers() <= 1
    && flag_game_manager_finishes_game_;
}

int GameManager::execApp(){
    return GraphicsManager::getInstance().execApp();
}

int& GameManager::getArgc(){
    return argc_;
}

char** GameManager::getArgv() const{
    return argv_;
}

void GameManager::setFlagGameManagerFinishesGame(bool value){
    flag_game_manager_finishes_game_ = value;
}

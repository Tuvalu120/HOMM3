/**
 * @file trainer/main.cc
 * @author Bartosz Han
 * @brief This file contains main function of the trainer.
 */


#include <memory>
#include <random>
#include <string>

#include "NEAT.h"
#include "Logger.h"
#include "TrainerConstants.hpp"

using namespace trainer_constants;

int main(int argc, char** argv){

    std::random_device rd;
    size_t seed = rd();
    setRandomNumberGeneratorSeed(seed);

    std::string hyperparams_path;
    std::string config_name = UNNAMED_CONFIG;
    int n_workers = 1;

    if(argc > 3){
        n_workers = std::stoi(argv[3]);
    }
    if(argc > 2){
        config_name = argv[2];
    }
    if(argc > 1){
        hyperparams_path = argv[1];
    }

    NEAT::getInstance().registerTrainer(std::make_unique<HOMM3BattleTrainer>(n_workers));
    NEAT::getInstance().run(hyperparams_path, config_name);

    Logger::getInstance().logData(SEED, seed);

    return 0;

}

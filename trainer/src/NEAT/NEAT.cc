/**
 * @file NEAT.cc
 * @author Bartosz Han
 * @brief This file contains definition of class NEAT.
 *
 */

#include "NEAT.h"
#include "Logger.h"
#include "Population.h"
#include "Hyperparams.hpp"
#include "Trainer.h"
#include "TrainerConstants.hpp"

#include <memory>
#include <stdexcept>
#include <string>

using namespace std;
using namespace trainer_constants;

void NEAT::initialize(string hyperparams_path, std::string config_name){

    if(!config_name.empty()){
        Logger::getInstance().initialize(config_name, hyperparams_path);
    }

    if(!hyperparams_path.empty()){
        hyperparams_.loadHyperparams(hyperparams_path);
    }


    if(trainer_ == nullptr){
        throw std::runtime_error("No trainer has been registered.");
    }

    if(hyperparams_.N_IN_NODES == 0 || hyperparams_.N_OUT_NODES == 0){
        throw std::runtime_error("Number of in nodes or number of out nodes has not been provided.");
    }


    current_generation_number_ = 0;
    innovation_tracker_.initialize();
    population_.initialize();
}

InnovationTracker& NEAT::getInnovationTracker(){
    return innovation_tracker_;
}

const InnovationTracker& NEAT::getInnovationTracker() const{
    return innovation_tracker_;
}

Population& NEAT::getPopulation(){
    return population_;
}

const Population& NEAT::getPopulation() const{
    return population_;
}

unsigned int NEAT::getCurrentGenerationNumber() const{
    return current_generation_number_;
}

const Hyperparams& NEAT::getHyperparams() const{
    return hyperparams_;
}

void NEAT::loadNodeQuantityInfo(const int n_in_nodes, const int n_out_nodes){
    hyperparams_.N_IN_NODES = n_in_nodes;
    hyperparams_.N_OUT_NODES = n_out_nodes;
}


void NEAT::run(string hyperparams_path, std::string config_name){

    initialize(hyperparams_path, config_name);

    Logger::getInstance().savePopulationIntoDirectory(to_string(current_generation_number_));

    while(current_generation_number_ < getHyperparams().N_GENERATIONS){

        trainer_->evaluate();
        population_.specialize();
        population_.createNewPopulation();
        ++current_generation_number_;

        Logger::getInstance().savePopulationIntoDirectory(to_string(current_generation_number_));

    }
    trainer_->evaluate();
    population_.getSpecimenWithID(population_.getBestSpecimenID()).getGenome().getNetwork().saveIntoFile(
        Logger::getInstance().getMainDirectory() + BEST_MODEL_SUFFIX);



}

void NEAT::registerTrainer(std::unique_ptr<Trainer> trainer){
    trainer_ = std::move(trainer);
}

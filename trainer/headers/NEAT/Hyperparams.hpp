/**
 * @file Hyperparams.hpp
 * @author Bartosz Han
 * @brief This file contains Hyperparams struct, which stores data about hyperparams in NEAT algorithm.
 *
 */

#ifndef HYPERPARAMS_HPP
#define HYPERPARAMS_HPP

#include <cstddef>
#include <fstream>
#include <string>
#include <map>
#include "TrainerUtilities.hpp"
#include "NeuralNetwork/Activations.hpp"

/**
 * @struct Hyperparams
 * @brief This struct stores data about hyperparams in NEAT algorithm.
 *
 */
struct Hyperparams{

    unsigned int N_IN_NODES;
    unsigned int N_OUT_NODES;

    unsigned int N_SPECIMEN = 20;
    unsigned int N_GENERATIONS = 100;
    float C1 = 1.0f;
    float C2 = 1.0f;
    float C3 = 0.4f;
    float COMPATIBILITY_THRESHOLD = 3;
    unsigned int SMALL_GENOME_THRESHOLD = 20;
    float MUTATION_CHANGE_CONNECTION_WEIGHT_PROBABILITY = 0.1f;
    float MUTATION_ADD_CONNECTION_PROBABILITY = 0.5f;
    float MUTATION_ADD_NODE_PROBABILITY = 0.05f;
    float MUTATION_ENABLE_NODE_PROBABILITY = 0.2f;
    float MUTATION_DISABLE_NODE_PROBABILITY = 0.9f;
    float MUTATION_WEIGHT_ASSIGNING_NEW_VALUE_PROBABILITY = 0.1f;
    float MUTATION_WEIGHT_UNIFORM_PERTURBATION_MAX_CHANGE = 0.5f;
    float CHILD_CONNECTION_INHERITING_BEING_DISABLED_FROM_ANY_PARENT_PROBABILITY = 0.75f;
    unsigned int MAX_STALENESS = 15;
    float PERCENT_OF_BEST_SPECIMEN_IN_SPECIES_TO_SURVIVE = 0.2f;
    unsigned int MIN_SIZE_OF_SPECIES_TO_STORE_ELITE = 5;
    float SPECIMEN_GOING_TO_NEXT_GENERATION_PROBABILITY = 0.25f;
    unsigned int GLOBAL_ELITE_SIZE = 1;
    float INTERSPECIES_CROSSOVER_PROBABILITY = 0.001f;
    ActivationType ACTIVATION = ActivationType::X;

    /**
     * @brief Load hyperparams data from text file into this object.
     *
     * @param file_path
     */
    void loadHyperparams(std::string file_path){

        std::ifstream file(file_path);
        std::map<std::string, std::string> hyperparams_;
        std::string line;
        while(std::getline(file, line)){
            size_t separator_pos = line.find('=');
            std::string name_part = line.substr(0, separator_pos);
            std::string value_part = line.substr(separator_pos+1);
            std::string name = trim(name_part);
            std::string value = trim(value_part);
            hyperparams_[name] = value;
        }

        auto load_int = [&](const std::string& key, unsigned int& value){
            if (hyperparams_.count(key)){
                value = std::stoi(hyperparams_.at(key));
            };
        };

        auto load_float = [&](const std::string& key, float& value){
            if (hyperparams_.count(key)){
                value = std::stof(hyperparams_.at(key));
            };
        };

        auto load_activation = [&](const std::string& key, ActivationType& activation_type){
            if (hyperparams_.count(key)){
                std::string activation = hyperparams_.at(key);
                activation_type = getActivationType(activation);
            };
        };

        load_int("N_SPECIMEN", N_SPECIMEN);
        load_int("N_GENERATIONS", N_GENERATIONS);
        load_float("C1", C1);
        load_float("C2", C2);
        load_float("C3", C3);
        load_float("COMPATIBILITY_THRESHOLD", COMPATIBILITY_THRESHOLD);
        load_int("SMALL_GENOME_THRESHOLD", SMALL_GENOME_THRESHOLD);
        load_float("MUTATION_CHANGE_CONNECTION_WEIGHT_PROBABILITY", MUTATION_CHANGE_CONNECTION_WEIGHT_PROBABILITY);
        load_float("MUTATION_ADD_CONNECTION_PROBABILITY", MUTATION_ADD_CONNECTION_PROBABILITY);
        load_float("MUTATION_ADD_NODE_PROBABILITY", MUTATION_ADD_NODE_PROBABILITY);
        load_float("MUTATION_ENABLE_NODE_PROBABILITY", MUTATION_ENABLE_NODE_PROBABILITY);
        load_float("MUTATION_DISABLE_NODE_PROBABILITY", MUTATION_DISABLE_NODE_PROBABILITY);
        load_float("MUTATION_WEIGHT_ASSIGNING_NEW_VALUE_PROBABILITY", MUTATION_WEIGHT_ASSIGNING_NEW_VALUE_PROBABILITY);
        load_float("MUTATION_WEIGHT_UNIFORM_PERTURBATION_MAX_CHANGE", MUTATION_WEIGHT_UNIFORM_PERTURBATION_MAX_CHANGE);
        load_float("CHILD_CONNECTION_INHERITING_BEING_DISABLED_FROM_ANY_PARENT_PROBABILITY", CHILD_CONNECTION_INHERITING_BEING_DISABLED_FROM_ANY_PARENT_PROBABILITY);
        load_int("MAX_STALENESS", MAX_STALENESS);
        load_float("PERCENT_OF_BEST_SPECIMEN_IN_SPECIES_TO_SURVIVE", PERCENT_OF_BEST_SPECIMEN_IN_SPECIES_TO_SURVIVE);
        load_int("MIN_SIZE_OF_SPECIES_TO_STORE_ELITE", MIN_SIZE_OF_SPECIES_TO_STORE_ELITE);
        load_float("SPECIMEN_GOING_TO_NEXT_GENERATION_PROBABILITY", SPECIMEN_GOING_TO_NEXT_GENERATION_PROBABILITY);
        load_int("GLOBAL_ELITE_SIZE", GLOBAL_ELITE_SIZE);
        load_float("INTERSPECIES_CROSSOVER_PROBABILITY", INTERSPECIES_CROSSOVER_PROBABILITY);
        load_activation("ACTIVATION", ACTIVATION);

    }

};

#endif
/**
 * @file Trainer.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Trainer and HOMM3BattleTrainer.
 *
 */

#include "Trainer.h"
#include "Logger.h"
#include "BattleScenario.hpp"
#include "NeuralNetwork/NeuralNetwork.h"
#include "PlayerControllers/PlayerController.h"
#include "AIBattleControllers/RandomBattleController.h"
#include "Specimen.h"
#include "AIBattleControllers/NEATBattleController.h"
#include "Managers/BattleManager.h"
#include "Hyperparams.hpp"
#include "ThreadPool.h"
#include "TrainerConstants.hpp"
#include "NEAT.h"
#include "GameObjects/HeroArmy.h"
#include "AIBattleControllers/VCMIBattleController.h"
#include "TrainerUtilities.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <chrono>


using namespace std;
using namespace game_data;
using namespace trainer_constants;

HOMM3BattleTrainer::HOMM3BattleTrainer(int n_workers):
    pool_(n_workers){
    NEAT::getInstance().loadNodeQuantityInfo(
        ai_battle_controller_constants::EXPECTED_N_IN_NODES,
        ai_battle_controller_constants::EXPECTED_N_OUT_NODES);

    Stack::registerStackDamageFunction(Stack::avarageStackDamageFunction());

}

void HOMM3BattleTrainer::validateBestSpecimen(){
    vector<Specimen> population = NEAT::getInstance().getPopulation().getSpecimens();
    Specimen& best_specimen = *max_element(population.begin(), population.end());
    Logger::getInstance().logData(BEST_FITNESS, best_specimen.getFitness());
    NeuralNetwork best_model = best_specimen.getGenome().getNetwork();

    float random_score = 0.0f;
    mutex random_score_mutex;
    float random_attacking_score = 0.0f;
    mutex random_attacking_score_mutex;
    float vcmi_10_score = 0.0f;
    mutex vcmi_10_score_mutex;
    int n_scenarios = N_BATTLES_IN_ONGOING_VALIDATION / 2;
    vector<BattleScenario> validation_scenarios;

    for(int i=0; i<n_scenarios; ++i){
        validation_scenarios.push_back(BattleScenario::createRandomScenario());
    }

    auto start = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < n_scenarios; ++i){

        BattleScenario scenario = validation_scenarios.at(i);

        function<shared_ptr<BattleController>()> random_generator = [](){ return std::make_shared<RandomBattleController>(
            ai_battle_controller_constants::AI_BATTLE_ACTIONS);};

        enqueueBattle(best_model, random_generator, scenario, 1, random_score, random_score_mutex);
        enqueueBattle(best_model, random_generator, scenario, -1, random_score, random_score_mutex);

        function<shared_ptr<BattleController>()> random_attacking_generator = [](){ return std::make_shared<RandomBattleController>(ai_battle_controller_constants::ATTACKING_ACTIONS);};

        enqueueBattle(best_model, random_attacking_generator, scenario, 1, random_attacking_score, random_attacking_score_mutex);
        enqueueBattle(best_model, random_attacking_generator, scenario, -1, random_attacking_score, random_attacking_score_mutex);

        function<shared_ptr<BattleController>()> vcmi_10_generator = [](){ return std::make_shared<VCMIBattleController>(
            ai_battle_controller_constants::VCMI_N_EVALUATION_TURNS_HARD);};

        enqueueBattle(best_model, vcmi_10_generator, scenario, 1, vcmi_10_score, vcmi_10_score_mutex);
        enqueueBattle(best_model, vcmi_10_generator, scenario, -1, vcmi_10_score, vcmi_10_score_mutex);

    }

    pool_.waitForAll();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    Logger::getInstance().logData(ONGOING_TESTING_TIME, static_cast<float>(duration.count()));

    Logger::getInstance().logData(RANDOM_VALIDATION_SCORE, random_score);
    Logger::getInstance().logData(RANDOM_ATTACKING_VALIDATION_SCORE, random_attacking_score);
    Logger::getInstance().logData(VCMI_10_VALIDATION_SCORE, vcmi_10_score);


    if(NEAT::getInstance().getCurrentGenerationNumber() == NEAT::getInstance().getHyperparams().N_GENERATIONS){
        start = std::chrono::high_resolution_clock::now();
        performFinalTest(best_model);
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        Logger::getInstance().logData(FINAL_TEST_TIME, static_cast<float>(duration.count()));
    }
}


std::vector<std::pair< unsigned int, unsigned int>> HOMM3BattleTrainer::generateBattlePairs(std::vector<unsigned int>& population_ids) const{

    std::vector<std::pair<unsigned int, unsigned int>> battle_pairs;

    for(size_t i=0; i<population_ids.size(); ++i){
        for(size_t j= i+1; j<population_ids.size(); ++j){
            battle_pairs.push_back({population_ids.at(i), population_ids.at(j)});
        }

    }

    int half_total_battles = battle_pairs.size();

    for(int i=0; i<half_total_battles; ++i){
        std::pair<unsigned int, unsigned int>& pair = battle_pairs.at(i);
        battle_pairs.push_back({pair.second, pair.first});
    }

    return battle_pairs;
}

void HOMM3BattleTrainer::enqueueBattle(NeuralNetwork& best_network, function<shared_ptr<BattleController>()> opponent_generator,
        BattleScenario& scenario, int expected_value, float& score, std::mutex& score_mutex){


        pool_.enqueue([scenario, best_network, opponent_generator, &score, &score_mutex, expected_value](){
        NeuralNetwork network = best_network;

        shared_ptr<BattleController> attacking_controller;
        shared_ptr<BattleController> defending_controller;

        if(expected_value == 1){
            attacking_controller = std::make_shared<NEATBattleController>(network);
            defending_controller = opponent_generator();
        }
        else{
            attacking_controller = opponent_generator();
            defending_controller = std::make_shared<NEATBattleController>(network);
        }

        HeroArmy attacking_army = createArmyFromScenario(scenario, 0);
        HeroArmy defending_army = createArmyFromScenario(scenario, 1);

        Battle battle(attacking_army, defending_army, *attacking_controller, *defending_controller, scenario.battlefield_type);
        int result = battle.performBattleLoop();

        if (result == expected_value){
            std::lock_guard<std::mutex> lock(score_mutex);
            ++score;
        }
    });
}


void HOMM3BattleTrainer::enqueueBattle(Specimen& specimen_1, Specimen& specimen_2,
        BattleScenario& scenario, float& model_1_score, float& model_2_score,
        mutex& model_1_score_mutex, mutex& model_2_score_mutex){

        NeuralNetwork& network_1 = specimen_1.getGenome().getNetwork();
        NeuralNetwork& network_2 = specimen_2.getGenome().getNetwork();

        int specimen_1_id = specimen_1.getID();
        int specimen_2_id = specimen_2.getID();

        pool_.enqueue([this, scenario, &network_1, &network_2, &model_1_score, &model_2_score,
            &model_1_score_mutex, &model_2_score_mutex, specimen_1_id, specimen_2_id](){

        NeuralNetwork attacker = network_1;
        NeuralNetwork defender = network_2;

        shared_ptr<NEATBattleController> attacking_controller = make_shared<NEATBattleController>(attacker);
        shared_ptr<NEATBattleController> defending_controller = make_shared<NEATBattleController>(defender);

        HeroArmy attacking_army = createArmyFromScenario(scenario, 0);
        HeroArmy defending_army = createArmyFromScenario(scenario, 1);

        Battle battle(attacking_army, defending_army, *attacking_controller, *defending_controller, scenario.battlefield_type);
        int result = battle.performBattleLoop();


        if(result == 1){
            {
                lock_guard<mutex> lock(model_1_score_mutex);
                ++model_1_score;
            }
        }

        else if(result == -1){
            {
                lock_guard<mutex> lock(model_2_score_mutex);
                ++model_2_score;
            }
        }

        {
            lock_guard<mutex> lock(performed_actions_mutexes_.at(specimen_1_id));
            vector<BattleAction> model_actions = attacking_controller->getPerformedActionsVector();
            performed_actions_.at(specimen_1_id).insert(
                performed_actions_.at(specimen_1_id).end(),
                model_actions.begin(),
                model_actions.end()
            );
        }

        {
            lock_guard<mutex> lock(performed_actions_mutexes_.at(specimen_2_id));
            vector<BattleAction> model_actions = defending_controller->getPerformedActionsVector();
            performed_actions_.at(specimen_2_id).insert(
                performed_actions_.at(specimen_2_id).end(),
                model_actions.begin(),
                model_actions.end()
            );
        }

        });


    }

void HOMM3BattleTrainer::performFinalTest(NeuralNetwork& best_network){
    int n_scenarios = N_BATTLES_IN_FINAL_TEST / 2;
    function<shared_ptr<BattleController>()> vcmi_10_generator = [](){ return std::make_shared<VCMIBattleController>(
        ai_battle_controller_constants::VCMI_N_EVALUATION_TURNS_HARD);};

    float score_att = 0.0f;
    mutex score_mutex_att;
    float score_def = 0.0f;
    mutex score_mutex_def;


    for(int i=0; i<n_scenarios; ++i){
        BattleScenario scenario = BattleScenario::createRandomScenario();
        enqueueBattle(best_network, vcmi_10_generator, scenario, 1, score_att, score_mutex_att);
        enqueueBattle(best_network, vcmi_10_generator, scenario, -1, score_def, score_mutex_def);
    }

    pool_.waitForAll();
    float total_score = (score_att + score_def) / N_BATTLES_IN_FINAL_TEST;

    float staring_1st_score = score_att / n_scenarios;
    float staring_2nd_score = score_def / n_scenarios;

    Logger::getInstance().logData(FINAL_TEST_SCORE, "STARTING_1ST," + to_string(staring_1st_score));
    Logger::getInstance().logData(FINAL_TEST_SCORE, "STARTING_2ND," + to_string(staring_2nd_score));
    Logger::getInstance().logData(FINAL_TEST_SCORE, "TOTAL," + to_string(total_score));


}

void HOMM3BattleTrainer::evaluate(){


    vector<BattleScenario> scenarios;

    const int n_scenarios = N_BATTLES_IN_TOURNAMENT/2;
    for(int i=0 ; i<n_scenarios ; ++i){
        scenarios.push_back(BattleScenario::createRandomScenario());
    }

    Population& population = NEAT::getInstance().getPopulation();

    auto start = std::chrono::high_resolution_clock::now();


    for (Specimen& specimen: population.getSpecimens()){
        specimen.setFitness(0.0f);
    }

    unordered_map<int, float> points;
    unordered_map<int, mutex> score_mutexes;

    for (Specimen& specimen: population.getSpecimens()){
        points[specimen.getID()] = 0.0f;
        score_mutexes[specimen.getID()];
        performed_actions_[specimen.getID()];
        performed_actions_mutexes_[specimen.getID()];
    }

    vector<unsigned int> population_ids = population.getSpecimensIDs();

    const vector<pair<unsigned int, unsigned int>> battle_pairs = generateBattlePairs(population_ids);

    for(const auto& [specimen_1_id, specimen_2_id]: battle_pairs){

        Specimen& specimen_1 = population.getSpecimenWithID(specimen_1_id);
        Specimen& specimen_2 = population.getSpecimenWithID(specimen_2_id);
        float& model_1_score = points.at(specimen_1_id);
        float& model_2_score = points.at(specimen_2_id);
        mutex& model_1_mutex = score_mutexes.at(specimen_1_id);
        mutex& model_2_mutex = score_mutexes.at(specimen_2_id);


        for(BattleScenario& scenario: scenarios){
            enqueueBattle(specimen_1, specimen_2, scenario, model_1_score, model_2_score, model_1_mutex, model_2_mutex);
        };

    }

    pool_.waitForAll();

    for(const auto& id: population_ids){
        float wins = points[id];
        population.getSpecimenWithID(id).addFitness(wins);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    Logger::getInstance().logData(RUNNING_TIME, static_cast<float>(duration.count()));

    for (Specimen& specimen: population.getSpecimens()){
        if(specimen.getFitness() < 0.0f){
            specimen.setFitness(0.0f);
        }
    }

    cout << "Training of Generation " << NEAT::getInstance().getCurrentGenerationNumber() << "/" << NEAT::getInstance().getHyperparams().N_GENERATIONS << " completed" << endl;

    for (Specimen& specimen: population.getSpecimens()){
        string score_data = to_string(NEAT::getInstance().getCurrentGenerationNumber()) + "," + to_string(specimen.getID()) + "," + to_string(specimen.getFitness());
        Logger::getInstance().logData(ALL_FITNESSES, score_data);
    }


    validateBestSpecimen();

    dumpPerformedActionsIntoLogger();

};


void HOMM3BattleTrainer::dumpPerformedActionsIntoLogger(){
    int generation_number = NEAT::getInstance().getCurrentGenerationNumber();

    auto battle_action_to_char = [](const BattleAction& action){ return static_cast<char>('A' + static_cast<char>(action));};

    for(const auto& [id, actions]: performed_actions_){
        string specimen_data = to_string(generation_number) + "," + to_string(id) + ",";
        //Data of actions will be stored as strings, in which each char represents the action performed
        string performed_actions_string;
        vector<BattleAction> performed_actions = actions;
        transform(performed_actions.begin(), performed_actions.end(), back_inserter(performed_actions_string), battle_action_to_char);
        Logger::getInstance().logData(ALL_PERFORMED_ACTIONS, specimen_data+performed_actions_string);
    }

    performed_actions_.clear();
    performed_actions_mutexes_.clear();

}

/**
 * @file Trainer.h
 * @author Bartosz Han
 * @brief This file contains Trainer and HOMM3Trainer classes.
 * First class is an abstract class used to evaluate specimens in NEAT algorithm.
 * Second class is an implementation of Trainer class, which is used for training computer
 * players in created version of game HOMM3.
 *
 */

#include "BattleScenario.hpp"
#include "NeuralNetwork/NeuralNetwork.h"
#include "PlayerControllers/PlayerController.h"
#include "AIBattleControllers/BattleAction.hpp"
#include "Specimen.h"
#include "ThreadPool.h"
#include <mutex>
#include <vector>

#ifndef TRAINER_H
#define TRAINER_H

class BattleController;

/**
 * @class Trainer
 * @brief This is an abstract class used to evaluate specimens in NEAT algorithm.
 */
class Trainer{

public:

    /**
     * @brief Evaluate specimens in population.
     * This method should evaluate the population, as well as
     * store fitnesses of specimens in objects of class Specimen.
     *
     */
    virtual void evaluate() = 0;

};

///////

/**
 * @class HOMM3BattleTrainer
 * @brief This class is an implementation of Trainer class, which is used for training computer
 * players in created version of game HOMM3.
 * Specimens battles against each other in battles and scores points based on number
 * of wins.
 */
class HOMM3BattleTrainer : public Trainer{

private:


    ThreadPool pool_;

    /**
     * @brief Generate new battle scenarios used in
     * training and replace old battle scenarios with these.
     *
     * @param n_to_replace
     */
    void updateBattleScenarios(int n_to_replace);

    /**
     * @brief Perform set of tests for best specimen in given generation.
     * the tests include battling against players performing random and random attacking
     * heuristics and player using algorithm from VCMI project.
     * The scores are stored in text files.
     *
     */
    void validateBestSpecimen();

    /// @brief Generate every possible combination of specimens in population.
    std::vector<std::pair<unsigned int, unsigned int>> generateBattlePairs(std::vector<unsigned int>& population_ids) const;

    /**
     * @brief Enqueue new battle, in which one player using neural networks
     * is battling against another opponent.
     * Score of player is incremented if it wins against enemy.
     *
     * @param network NeuralNetwork to test.
     * @param opponent_generator Function toi generate opponent.
     * @param scenario BattleScenario in which battle will be performed.
     * @param expected_value Expected result of battle, if neural network player win.
     * @param score Reference to score of network.
     * @param scores_mutex Reference to mutex of score variable.
     */
    void enqueueBattle(NeuralNetwork& network, std::function<std::shared_ptr<BattleController>()> opponent_generator,
        BattleScenario& scenario, int expected_value, float& score, std::mutex& scores_mutex);

    /**
     * @brief Enqueue new battle, in which two players using neural networks
     * are battling against another opponent.
     * Score of winning player is incremented after the battle.
     *
     * @param model_1  Attacking NeuralNetwork.
     * @param model_2  Defending NeuralNetwork.
     * @param scenario BattleScenario in which battle will be performed.
     * @param model_1_score Reference to score of attacking network.
     * @param model_2_score Reference to score of defending network.
     * @param model_1_score_mutex Reference to mutex of attacking network's score variable.
     * @param model_2_score_mutex Reference to mutex of defending network's score variable.
     */
    void enqueueBattle(Specimen& model_1, Specimen& model_2,
        BattleScenario& scenario, float& model_1_score, float& model_2_score,
        std::mutex& model_1_score_mutex, std::mutex& model_2_score_mutex);

    void performFinalTest(NeuralNetwork& best_network);

    std::map<unsigned int, std::vector<BattleAction>> performed_actions_;
    std::map<unsigned int, std::mutex> performed_actions_mutexes_;

    /**
     * @brief Make all specimen save data about performed actions
     * into one file, provided by Logger.
     */
    void dumpPerformedActionsIntoLogger();

public:

    /**
     * @brief Construct a new HOMM3BattleTrainer object
     *
     * @param n_workers Number of threads (workers), where battles will be performed.
     */
    HOMM3BattleTrainer(int n_workers = std::thread::hardware_concurrency());

    void evaluate() override;

};

#endif
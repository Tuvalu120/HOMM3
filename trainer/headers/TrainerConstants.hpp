/**
 * @file TrainerConstants.hpp
 * @author Bartosz Han
 * @brief This file contains various constants
 * used in artificial intelligence training.
 *
 */

#ifndef TRAINER_CONSTANTS_HPP
#define TRAINER_CONSTANTS_HPP

#include <vector>
#include <string>

namespace trainer_constants{


    const int N_BATTLES_IN_ONGOING_VALIDATION = 100;
    const int N_BATTLES_IN_TOURNAMENT = 10;
    const int N_BATTLES_IN_FINAL_TEST = 10000;


    const std::string TRAINER_DIR = "trainer/";
    const std::string OUTPUT_DIR = TRAINER_DIR + "output/";
    const std::string MODELS_DIR = "models";
    const std::string HYPERPARAMS_FILE = "hyperparams.txt";
    const std::string BEST_MODEL_SUFFIX = "/best_model";

    const std::string UNNAMED_CONFIG = "unnamed";

    const std::string BEST_FITNESS = "best_fitness";
    const std::string ALL_FITNESSES = "all_fitnesses";
    const std::string RANDOM_VALIDATION_SCORE = "random_validation_score";
    const std::string RANDOM_ATTACKING_VALIDATION_SCORE = "random_attacking_validation_score";
    const std::string VCMI_10_VALIDATION_SCORE = "vcmi_10_validation_score";
    const std::string RUNNING_TIME = "running_time";
    const std::string ONGOING_TESTING_TIME = "ongoing_testing_time";
    const std::string FINAL_TEST_SCORE = "final_test_score";
    const std::string FINAL_TEST_TIME = "final_test_time";
    const std::string ALL_PERFORMED_ACTIONS = "all_performed_actions";
    const std::string SEED = "seed";

    /// @brief List of all data being logged while training.
    const std::vector<std::string> LOGGED_INFO={
        BEST_FITNESS,
        ALL_FITNESSES,
        RANDOM_VALIDATION_SCORE,
        RANDOM_ATTACKING_VALIDATION_SCORE,
        VCMI_10_VALIDATION_SCORE,
        RUNNING_TIME,
        ONGOING_TESTING_TIME,
        FINAL_TEST_SCORE,
        ALL_PERFORMED_ACTIONS,
        SEED
    };
}

#endif
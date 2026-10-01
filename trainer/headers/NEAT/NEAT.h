/**
 * @file NEAT.h
 * @author Bartosz Han
 * @brief This file contains NEAT class, which is main class of implemented NEAT algorithm and
 * which is responsible for initializing and running the algorithm.
 *
 */

#ifndef NEAT_ALGORITHM_H
#define NEAT_ALGORITHM_H

#include "Hyperparams.hpp"
#include "InnovationTracker.h"
#include "Population.h"
#include "Singleton.hpp"
#include <memory>
#include "Trainer.h"

/**
 * @class NEAT
 * @brief This class is main class of implemented NEAT algorithm.
 * It is responsible for initializing and running the algorithm.
 *
 */
class NEAT: public Singleton<NEAT>{

    friend class Singleton<NEAT>;

    private:

    InnovationTracker innovation_tracker_;
    Population population_;
    Hyperparams hyperparams_;
    unsigned int current_generation_number_;
    std::unique_ptr<Trainer> trainer_;


    public:

    /**
     * @brief Initialize NEAt algorithm.
     * This method will load hyperparams data into program from file,
     * and will initialize Population and InnovationTracker.
     *
     * @param hyperparams_path
     * @param config_name
     */
    void initialize(std::string hyperparams_path, std::string config_name="");

    /**
     * @brief Get InnovationTracker of algorithm
     */
    InnovationTracker& getInnovationTracker();

    /**
     * @brief Get InnovationTracker of algorithm
     */
    const InnovationTracker& getInnovationTracker() const;

    /**
     * @brief Get Hyperparams of algorithm
     */
    const Hyperparams& getHyperparams() const;

    /// @brief Add information about number of in nodes and out nodes into algorithm.
    /// This method should be called before initialize().
    void loadNodeQuantityInfo(const int n_in_nodes, const int n_out_nodes);

    /**
     * @brief Get Population of algorithm
     */
    Population& getPopulation();

    /**
     * @brief Get Population of algorithm
     */
    const Population& getPopulation() const;

    /// @brief Get number of current generation of algorithm.
    unsigned int getCurrentGenerationNumber() const;

    /// @brief Run algorithm, with provided hyperparams.
    /// this method will be training AI for given hyperparams number of generations.
    void run(std::string hyperparams_path = "", std::string config_name = "");

    /// @brief Register Trainer object into algorithm, which will be responsible in evaluation objects.
    /// This method should be called before initialize().
    void registerTrainer(std::unique_ptr<Trainer> trainer);
};

#endif
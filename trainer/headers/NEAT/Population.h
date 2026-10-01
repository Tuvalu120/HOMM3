/**
 * @file Population.h
 * @author Bartosz Han
 * @brief This file contains Population class,
 * which is a container for all Specimen and Species of NEAT algorithm.
 *
 */

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "Specimen.h"
#include "Species.h"

class Genome;

/**
 * @class Population
 * @brief This class is a container for all Specimen and Species of NEAT algorithm.
 *
 */
class Population{

private:

    std::vector<Specimen> specimens_;
    std::vector<Specimen> global_elite_;
    std::vector<unsigned int> survived_specimens_id_;
    std::vector<Species> species_list_;

public:

    /**
     * @brief Initialize population, by created given in hyperparams number
     * of specimen.
     *
     */
    void initialize();

    /// @brief Associate all specimens into species.
    void specialize();

    /// @brief Update all information about all species, including information about species fitness,
    /// specimen which will survive within species, and number of specimen this species will create in next generation.
    void updateAllSpecies();

    /// @brief Create new Specimen, based on two other Specimen.
    static Specimen crossoverSpecimen(const Specimen& parent_1, const Specimen& parent_2);

    /// @brief Calculate the number of specimen each species will create in next generation.
    void calculateNSpecimenToReproduceForSpecies();

    /// @brief Remove worst species, whose fitness did not improve in
    /// given in hyperparams number of generation.
    /// @note In HOMM3 battle trainer, species purge is not used.
    void purgeWorstSpecies();

    /// @brief Get best specimen ofa ll species which meet provided criteria.
    const std::vector<Specimen> getEliteOfSpecies();

    /// @Create new generation of specimen, using crossover in species and
    /// randomly adding old specimen in new generation.
    const std::vector<Specimen> selectFromSpecies();

    /// @brief Create new population of specimen, and remove
    /// all specimen of old population.
    void createNewPopulation();

    Specimen& getSpecimenWithID(const unsigned int id);
    const Specimen& getSpecimenWithID(const unsigned int id) const;

    Species& getSpeciesWithID(const unsigned int id);
    const Species& getSpeciesWithID(const unsigned int id) const;

    /// @brief Get reference to list of all specimens.
    std::vector<Specimen>& getSpecimens();

    ///@brief Get list of all specimens' IDs.
    std::vector<unsigned int> getSpecimensIDs() const;

    /// @brief Get best specimens in population.
    /// @brief Number of specimens depends on hyperparams.
    const std::vector<Specimen>& getGlobalElite() const;


    /// @brief Get ID of specimen, which survived purge in species and will
    /// appear in crossover stage of generation.
    std::vector<unsigned int>& getSurvivedSpecimensID();

    /// @brief Get reference to list of all species.
    const std::vector<Species>& getSpeciesList() const;

    void updateBestSpecimenInfo();

    unsigned int getBestSpecimenID() const;

    /// @brief Choose parent for first_parent, from list of all possible_parents.
    unsigned int chooseOtherParent(const std::vector<unsigned int>& possible_parents, const unsigned int first_parent_id) const;


};

#endif
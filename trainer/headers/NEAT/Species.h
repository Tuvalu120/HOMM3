/**
 * @file Species.h
 * @author Bartosz Han
 * @brief This file contains Species class, which represents one species in NEAT algorithm.
 *
 */

#ifndef SPECIES_H
#define SPECIES_H

#include <vector>
#include "Genome.h"

/**
 * @class Species
 * @brief This class represents one species in NEAT algorithm.
 */
class Species{

private:

    static unsigned int next_id_;

    unsigned int id_;
    float species_fitness_;
    unsigned int n_specimen_to_reproduce_;
    float best_fitness_ = 0.0f;
    unsigned int staleness_ = 0;
    Genome representative_;
    std::vector<unsigned int> specimens_id_;
    std::vector<float> specimens_fitnesses_;

    /// @brief Check, if species' fitness improved in this generation. Is yes, reset staleness.
    /// If no, increment staleness.
    void updateStaleness();

    /// @brief Add Genome as a representative of this species.
    /// Genome is chosen randomly from all Specimens which are in this species.
    void addRepresentativeToSpecies();

    /// @brief Sort specimen in list, in order of fitness descending.
    void sortSpecimenListWithinSpecies();

    /// @brief Get specimens' fitnesses into vector of fitnesses.
    void addFitnessDataForSpecies();

    /// @brief Calculate species fitness, as mean of all specimen which belong to this
    /// species.
    void calculateSpeciesFitness();

    /// @brief Remove worst performing specimens from list. The number of specimens to remove
    /// is based on hyperparams.
    void purgeWorstSpecimens();

public:

    Species(Genome representative);

    /**
     * @brief Update the information about species. This method will call, in this order:
     *
     *  addRepresentativeToSpecies()
     *  sortSpecimenListWithinSpecies()
     *  addFitnessDataForSpecies()
     *  calculateSpeciesFitness()
     *  updateStaleness()
     *  purgeWorstSpecimens()
     *
     */
    void update();

    /// @brief Get ID of best specimen in species.
    unsigned int getIDOfBestSpecimen() const;

    bool operator==(const Species& other) const;
    bool operator==(const unsigned int& id) const;

    /// @brief Get ID of species
    unsigned int getID() const;
    float getSpeciesFitness() const;

    /// @brief Get number of specimens this species will create in next generation.
    unsigned int getNSpecimenToReproduce() const;

    /// @brief Set number of specimens this species will create in next generation.
    void setNSpecimenToReproduce(const int new_value);

    /// @brief Add the number of specimens this species will create in next generation
    /// number of additional species.
    void addNSpecimenToReproduce(const int value_to_add);

    /// @brief Get fitness of best specimen in species.
    float getBestFitness() const;

    /// @brief Get current staleness of species.
    unsigned int getStaleness() const;

    /// @brief Increment  staleness of species by one.
    void incrementStaleness();

    /// @brief Set staleness of species to 0.
    void resetStaleness();

    /// @brief Get Genome-representative of this species.
    const Genome& getRepresentative() const;

    /// @brief Set Genome-representative of this species.
    void setRepresentative(Genome& new_representative);

    /// @brief Get vector of all specimens's id of this species.
    std::vector<unsigned int>& getSpecimensID();

    /// @brief Get vector of all specimens's fitnesses of this species.
    std::vector<float>& getSpecimensFitnesses();

    /// @brief Reset the ID counter of this class to 0.
    static void resetIDCounter();

};

#endif
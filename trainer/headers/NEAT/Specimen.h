/**
 * @file Specimen.h
 * @author Bartosz Han
 * @brief This file contains Specimen class, which represents one specimen in NEAT algorithm.
 *
 */


#ifndef SPECIMEN_H
#define SPECIMEN_H

#include "Genome.h"

/**
 * @class Species
 * @brief This class represents one specimen in NEAT algorithm.
 * Objects of this class consists of one Genome object.
 */
class Specimen{

private:
    static unsigned int next_id_;
    unsigned int id_;
    Genome genome_;
    float fitness_;

public:

    /**
     * @brief Construct a new Specimen object,
     * by specifying genome of this object.
     */
    Specimen(const Genome new_genome);

    /**
     * @brief Construct a new Specimen object,
     * by copying genome of other specimen.
     * This constructor will copy ID of other specimen as well.
     */
    Specimen(const Specimen& other);

    /**
     * @brief Construct a new Specimen object,
     * by creating empty genome object, and thus -
     * empty network with defined number of inputs and outputs.
     *
     */
    Specimen();

     /**
     * @brief Propagate the data specimen's in network
     *
     * @param input Vector of values, which represents input of network. This vector must have
     * the size the same as the number of input nodes.
     * @return const std::vector<float> The output of network, with size equal to the number of out nodes.
     */
    const std::vector<float> predict(const std::vector<float>& input);

    bool operator==(const unsigned int& id) const;
    bool operator==(const Specimen& other) const;
    bool operator<(const Specimen& other) const;
    bool operator>(const Specimen& other) const;

    unsigned int getID() const;

    /// @brief Get reference to Genome object of this specimen.
    Genome& getGenome();

    /// @brief Returns copy of Genome object of this specimen.
    Genome getGenomeCopy();

    /// @brief Get reference to Genome object of this specimen.
    const Genome& getGenome() const;

    /// @brief Get fitness of this object.
    float getFitness() const;

    /// @brief Set fitness of this object.
    void setFitness(const float new_value);

    /// @brief Increase fitness of this object by provided value.
    void addFitness(const float value_to_add);

    /// @brief Reset the ID counter of this class to 0.
    static void resetIDCounter();

};

#endif
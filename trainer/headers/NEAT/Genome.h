/**
 * @file Genome.h
 * @author Bartosz Han
 * @brief This file contains Genome class. This class contains data about neural network of one specimen in NEAT algorithm,
 * and is also responsible for modifying network in this algorithm.
 *
 */
#ifndef GENOME_H
#define GENOME_H

#include <vector>

#include "NeuralNetwork/NeuralNetwork.h"
#include "NeuralNetwork/Connection.hpp"
#include "InnovationTracker.h"


/**
 * @struct GenomeParentsData
 * @brief This struct contains information about genotypes of two parents:
 * their matching, disjoint and excess connections.
 */
struct GenomeParentsData{
    std::vector<unsigned int> disjoint_genes;
    std::vector<unsigned int> excess_genes;
    std::vector<unsigned int> matching_genes;
};


class Genome{

private:

    NeuralNetwork network_;

    /**
     * @brief Mutate connection, by changing value of its weight.
     * This method will randomly either add random value to the value of the weight,
     * or assign new value. The behaviour of this method depends on
     * hyperparams.
     *
     * @param connection Connection to change weight
     */
    void mutateWeight(Connection& connection);

    /**
     * @brief Add new connection to the network, which
     * will connect in_node and out_node.
     * This method does not validate new connection.
     *
     * @param in_node_id ID of input node of connection.
     * @param out_node_id ID of output node of connection.
     * @param tracker InnovationTracker to get ID of new connection.
     */
    void mutateAddConnection(const unsigned int in_node_id, const unsigned int out_node_id, InnovationTracker& tracker);

    /**
     * @brief Add new node to the network, which
     * will split provided connection.
     * This method will disable old connection, and add
     * new connections, which will connect old input and output nodes
     * with new node.
     * This method does not validate new node.
     *
     * @param connection_to_split Connection, on which new node will appear.
     * @param tracker InnovationTracker to get ID of new node and connections.
     */
    void mutateAddNode(Connection& connection_to_split, InnovationTracker& tracker);

    /**
     * @brief Enable provided connection.
     *
     * @param connection_to_enable Connection to enable.
     */
    void mutateEnableConnection(Connection& connection_to_enable);

    /**
     * @brief Disable provided connection.
     *
     * @param connection_to_enable Connection to disable.
     */
    void mutateDisableConnection(Connection& connection_to_disable);

    /**
     * @brief Check, if new potential connection can be added into
     * network, without creating cycle in it.
     * Returns true, if this connection can be added safely to network.
     *
     * @param in_node_id ID of intput node of new potential connection.
     * @param out_node_id ID of output node of new potential connection.
     */
    bool validateConnection(const unsigned int in_node_id, const unsigned int out_node_id) const;

    /**
     * @brief Check, if new potential connection can be added into
     * network, without creating cycle in it.
     * Returns true, if this connection can be added safely to network.
     *
     * @param connection_to_check New potential connection to check.
     */
    bool validateConnection(const Connection& connection_to_check) const;

public:

    /**
    * @brief Construct a new Genome object.
    * This constructor will create empty
    *  network with number input and output nodes
    * as provided in hyperparams.
    */
    Genome();

    /**
    * @brief Construct a new Genome object,
    * With provided network.
    * @param network Network of genome.
    */
    explicit Genome(NeuralNetwork network);

    /**
     * @brief Create densely connected
    *  network with input and output nodes.
     *
     */
    void createBasicNetwork();

    /**
     * @brief Get data about topological similarity
     * of this genome and another one.
     * Returns GenomeParentsData object.
     *
     * @param other Other genome to compare similarity.
     */
    const GenomeParentsData getDisjointAndExcessGenes(const Genome& other) const;

    /**
     * @brief Returns new NeuralNetwork object,
     * with exactly same topology as this object's network.
     *
     * @return NeuralNetwork
     */
    NeuralNetwork getNetworkCopy();

    /**
     * @brief Get reference to NeuralNetwork object.
     */
    NeuralNetwork& getNetwork();

    /**
     * @brief Get reference to NeuralNetwork object.
     */
    const NeuralNetwork& getNetwork() const;

    /**
     * @brief Add to this genome's network connections,
     * which are present in both parents (provided in matching genes)
     * vector. This method will add all of these connections to this
     * object, with weights randomly added from parents.
     *
     * @param matching_genes Vector of IDs of matching genes (connections).
     */
    void addMatchingConnections(const std::vector<unsigned int>& matching_genes, const Genome& parent_1, const Genome& parent_2);

    /**
     * @brief Add connections from one parent.
     * This method will add all connections from vector of IDs,
     * which appear in parent, as well as its weights.
     *
     * @param genes Vector of IDs of genes (connections). Not all connections
     * might be added.
     */
    void addConnectionsFromOneParent(const std::vector<unsigned int>& genes, const Genome& parent);

    /**
     * @brief Add disjoint and excess genes from both parents.
     * Each gene in genes vector has 50% chance to appear in child.
     * If it appears, it will get weight randomly from one of parents.
     * @param genes Vector of IDs of genes (connections).
     */
    void addDisjointConnectionsRandomly(const std::vector<unsigned int>& genes, const Genome& parent_1, const Genome& parent_2);

    /**
     * @brief Returns compatibility distance (topological similarity)
     * of two genomes.
     *
     */
    static float calculateCompatibilityDistance(const Genome& genome_1, const Genome& genome_2);

    /**
     * @brief Create new Genome, by matching
     * two other genomes (parents), assuming different fitness.
     * This method will add matching connections randomly
     * from both parents, and all excess and disjoint genes
     * from better parent.
     *
     * @param better_parent Genome with better fitness
     * @param worse_parent Genome with worse fitness
     */
    static Genome crossoverDifferentFitnesses(const Genome& better_parent, const Genome& worse_parent);

    /**
     * @brief Create new Genome, by matching
     * two other genomes (parents), assuming different fitness.
     * This method will add matching connections randomly
     * from both parents, and will randomly add each disjoint and excess connections
     * from one parent.
     *
     * @param better_parent Genome with better fitness
     * @param worse_parent Genome with worse fitness
     */
    static Genome crossoverSameFitnesses(const Genome& parent_1, const Genome& parent_2);

    /**
     * @brief Perform all types of mutations (weight changes, disabling and enabling
     connections, as well as adding connections and nodes) onto this genome.
     * This methods validates all new elements of network.
     *
     */
    void mutate();

};

#endif
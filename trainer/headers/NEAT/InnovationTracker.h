/**
 * @file InnovationTracker.h
 * @author Bartosz Han
 * @brief This file contains InnovationTracker class, which is responsible for tracking IDs
 * of all connections and nodes, which appeared during NEAT algorithm.
 *
 */

#ifndef INNOVATION_TRACKER_H
#define INNOVATION_TRACKER_H

#include <map>
#include "NodeGenerationRecord.hpp"

/**
 * @class InnovationTracker
 * @brief This class is responsible for tracking IDs
 * of all connections and nodes, which appeared during NEAt algorithm.
 *
 */
class InnovationTracker{

private:

    unsigned int next_node_id_;
    unsigned int next_innovation_number_;

    std::map<std::pair<unsigned int, unsigned int>, unsigned int> innovation_record_;
    std::map<unsigned int, NodeGenerationRecord> node_record_; //key: innovation of connection being split

public:

    /// @brief Reset nodes and connections data and set ID variables.
    void initialize();

    /**
     * @brief Get the NodeGenerationRecord from connection being split.
     * If connection didn't appear in algorithm, this method will create new record and will assign
     * IDs to all new elements.
     *
     * @param connection_split ID of connection being split.
     * @return const NodeGenerationRecord
     */
    const NodeGenerationRecord getNodeIdValueFromSplit(const unsigned int connection_split);

    /**
     * @brief Get the ID of connection from nodes, which this connection connect.
     * If input and output nodes didn't appear in algorithm, this method will create new record and will assign
     * IDs to all new elements.
     *
     * @param in_node Input node of searched connection.
     * @param out_node Output node of searched connection.
     * @return
     */
    unsigned int getInnovationValueFromNodes(const unsigned int in_node, const unsigned int out_node);

};

#endif
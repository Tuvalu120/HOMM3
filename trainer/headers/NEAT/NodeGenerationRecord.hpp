/**
 * @file NodeGenerationRecord.hpp
 * @author Bartosz Han
 * @brief This file contains NodeGenerationRecord struct,
 * which is used in InnovationTracker to track data about
 * new nodes.
 *
 */

#ifndef NODE_GENERATION_RECORD_HPP
#define NODE_GENERATION_RECORD_HPP

/**
 * @brief This struct is used in InnovationTracker to track data about
 * new nodes. This struct contains data about Node ID and connections
 * which are created when the node is created.
 *
 */
struct NodeGenerationRecord{
    unsigned int node_id;
    unsigned int innovation_in;
    unsigned int innovation_out;

    bool operator==(const NodeGenerationRecord& other) const{
        return node_id == other.node_id &&
        innovation_in == other.innovation_in &&
        innovation_out == other.innovation_out;
    }
};

#endif
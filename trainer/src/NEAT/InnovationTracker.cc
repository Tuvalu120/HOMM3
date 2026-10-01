/**
 * @file InnovationTracker.cc
 * @author  * @author Bartosz Han
 * @brief This file contains definition of class InnovationTracker.
 *
 */

#include "InnovationTracker.h"
#include "Hyperparams.hpp"
#include "NEAT.h"

using namespace std;

void InnovationTracker::initialize(){
    innovation_record_.clear();
    node_record_.clear();
    next_node_id_ = NEAT::getInstance().getHyperparams().N_IN_NODES + NEAT::getInstance().getHyperparams().N_OUT_NODES + 1;
    next_innovation_number_ = 0;
}


const NodeGenerationRecord InnovationTracker::getNodeIdValueFromSplit(const unsigned int connection_split){
    const auto it = node_record_.find(connection_split);
    if(it != node_record_.end()){
        return it->second;
    };

    const NodeGenerationRecord new_record{
        next_node_id_++,
        next_innovation_number_++,
        next_innovation_number_++
    };

    node_record_[connection_split] = new_record;
    return new_record;
}

unsigned int InnovationTracker::getInnovationValueFromNodes(const unsigned int in_node, const unsigned int out_node){
    const auto key = make_pair(in_node, out_node);
    const auto it = innovation_record_.find(key);
    if(it != innovation_record_.end()){
        return it->second;
    };
    const int new_innovation = next_innovation_number_++;
    innovation_record_[key] = new_innovation;
    return new_innovation;
}
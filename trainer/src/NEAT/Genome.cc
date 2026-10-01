/**
 * @file Genome.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Genome.
 *
 */
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_set>

#include "Hyperparams.hpp"
#include "Genome.h"
#include "InnovationTracker.h"
#include "NEAT.h"

#include "NeuralNetwork/NeuralNetwork.h"
#include "Utilities.hpp"

using namespace std;

Genome::Genome():network_(
    NEAT::getInstance().getHyperparams().N_IN_NODES,
    NEAT::getInstance().getHyperparams().N_OUT_NODES,
    NEAT::getInstance().getHyperparams().ACTIVATION){};

Genome::Genome(NeuralNetwork network):network_(network){};

NeuralNetwork& Genome::getNetwork(){
    return network_;
}

NeuralNetwork Genome::getNetworkCopy(){
    return network_;
}

const NeuralNetwork& Genome::getNetwork() const{
    return network_;
}

void Genome::createBasicNetwork(){

    network_.addInitialNodes();

    InnovationTracker& tracker = NEAT::getInstance().getInnovationTracker();

    // Adding initial connections

    for(const unsigned int& output_node_index: network_.getOutputNodes()){
        const Node& output_node = network_.getNodes().at(output_node_index);
        for(const unsigned int& input_node_index: network_.getInputNodes()){
            const Node& input_node = network_.getNodes().at(input_node_index);
            network_.addConnection(Connection{
                .id = tracker.getInnovationValueFromNodes(input_node.id, output_node.id),
                .in_node = input_node.id,
                .out_node = output_node.id,
                .weight = generateRandomFloatInRange(-1.0f, 1.0f)
            });
        }
    }

    network_.sortNodesTopologically();
}

float Genome::calculateCompatibilityDistance(const Genome& genome_1, const Genome& genome_2){
    const GenomeParentsData genome_data = genome_1.getDisjointAndExcessGenes(genome_2);

    const NeuralNetwork& network_1 = genome_1.getNetwork();
    const NeuralNetwork& network_2 = genome_2.getNetwork();

    unsigned int n_all_genes = max(network_1.getConnections().size(), network_2.getConnections().size());
    unsigned int n_genes_smaller_genome = min(network_1.getConnections().size(), network_2.getConnections().size());
    n_all_genes = n_genes_smaller_genome > NEAT::getInstance().getHyperparams().SMALL_GENOME_THRESHOLD ? n_all_genes : 1;

    float disjoint_genes_value = NEAT::getInstance().getHyperparams().C1 * genome_data.disjoint_genes.size() / n_all_genes;
    float excess_genes_value = NEAT::getInstance().getHyperparams().C2 * genome_data.excess_genes.size() / n_all_genes;

    float weights_difference = 0.0f;
    if(!genome_data.matching_genes.empty()){
        for(const unsigned int& id: genome_data.matching_genes){
            float weight_1 = network_1.getConnectionWithID(id).weight;
            float weight_2 = network_2.getConnectionWithID(id).weight;
            float difference = abs(weight_1 - weight_2);
            weights_difference += difference;
        }
        weights_difference /= genome_data.matching_genes.size();
        weights_difference *= NEAT::getInstance().getHyperparams().C3;
    }

    return disjoint_genes_value + excess_genes_value + weights_difference;

}

const GenomeParentsData Genome::getDisjointAndExcessGenes(const Genome& other) const{
    GenomeParentsData data;
    const std::vector<Connection>& this_connections = network_.getConnections();
    const std::vector<Connection>& other_connections = other.getNetwork().getConnections();
    const unsigned int this_max_innovation_number = max_element(this_connections.begin(), this_connections.end())->id;
    const unsigned int other_max_innovation_number = max_element(other_connections.begin(), other_connections.end())->id;
    const Genome& more_complex_genome = this_max_innovation_number >= other_max_innovation_number ? *this : other;
    const unsigned int smaller_max_innovation_number = min(this_max_innovation_number, other_max_innovation_number);

    for(const Connection& gene: this_connections){
        if(gene.id <= smaller_max_innovation_number){
            const auto it = find(other_connections.begin(), other_connections.end(), gene);
            if(it == other_connections.end()){
                data.disjoint_genes.push_back(gene.id);
            }
            else{
                data.matching_genes.push_back(gene.id);
            }
        }
    };

    for(const Connection& gene: other_connections){
        if(gene.id <= smaller_max_innovation_number &&
            find(this_connections.begin(), this_connections.end(), gene) == this_connections.end()){
                data.disjoint_genes.push_back(gene.id);
        }
    };

    for(const Connection& gene: more_complex_genome.getNetwork().getConnections()){
        if(gene.id > smaller_max_innovation_number){
            data.excess_genes.push_back(gene.id);

        }
    }
    return data;
};


void Genome::addMatchingConnections(const vector<unsigned int>& matching_genes, const Genome& parent_1, const Genome& parent_2){
    for(const unsigned int& matching_gene: matching_genes){
        const Connection& connection_of_parent_1 = parent_1.getNetwork().getConnectionWithID(matching_gene);

        if(!validateConnection(connection_of_parent_1)){
            continue;
        }

        const Connection& connection_of_parent_2 = parent_2.getNetwork().getConnectionWithID(matching_gene);
        const Connection& connection_to_copy = roll(0.5f) ? connection_of_parent_1 : connection_of_parent_2;
        Connection new_connection(connection_to_copy);

        bool disabled_in_either = !connection_of_parent_1.is_enabled || !connection_of_parent_2.is_enabled;
        if(disabled_in_either && roll(NEAT::getInstance().getHyperparams().CHILD_CONNECTION_INHERITING_BEING_DISABLED_FROM_ANY_PARENT_PROBABILITY)){
            new_connection.is_enabled = false;
        }
        else{
            new_connection.is_enabled = true;
        }
        if(!network_.hasConnectionWithID(new_connection.id)){
            network_.addConnection(std::move(new_connection));
        }
    }
}

void Genome::addConnectionsFromOneParent(const vector<unsigned int>& genes, const Genome& parent){
    for(const unsigned int& gene: genes){
        if(!parent.getNetwork().hasConnectionWithID(gene)){
            continue;
        }
        const Connection& connection_of_parent = parent.getNetwork().getConnectionWithID(gene);
        if(!validateConnection(connection_of_parent)){
            continue;
        }
        Connection new_connection(connection_of_parent);
        if(!new_connection.is_enabled && roll(NEAT::getInstance().getHyperparams().CHILD_CONNECTION_INHERITING_BEING_DISABLED_FROM_ANY_PARENT_PROBABILITY)){
            new_connection.is_enabled = false;
        }
        else{
            new_connection.is_enabled = true;
        }
        if(!network_.hasConnectionWithID(new_connection.id)){
            network_.addConnection(std::move(new_connection));
        }
    }
}

void Genome::addDisjointConnectionsRandomly(const vector<unsigned int>& genes, const Genome& parent_1, const Genome& parent_2){
    for(const unsigned int& gene: genes){
        if(roll(0.5f)){
            continue;
        }

        const Genome& parent_of_connection = parent_1.getNetwork().hasConnectionWithID(gene) ? parent_1 : parent_2;
        const Connection& connection_to_copy = parent_of_connection.getNetwork().getConnectionWithID(gene);
        if(!validateConnection(connection_to_copy)){
            continue;
        }

        Connection new_connection(connection_to_copy);
        if(!new_connection.is_enabled && roll(NEAT::getInstance().getHyperparams().CHILD_CONNECTION_INHERITING_BEING_DISABLED_FROM_ANY_PARENT_PROBABILITY)){
            new_connection.is_enabled = false;
        }

        else{
            new_connection.is_enabled = true;
        }

        if(!network_.hasConnectionWithID(new_connection.id)){
            network_.addConnection(std::move(new_connection));
        }
    }
}

Genome Genome::crossoverDifferentFitnesses(const Genome& better_parent, const Genome& worse_parent){
    GenomeParentsData data = better_parent.getDisjointAndExcessGenes(worse_parent);
    Genome child;
    child.addMatchingConnections(data.matching_genes, better_parent, worse_parent);
    child.addConnectionsFromOneParent(data.disjoint_genes, better_parent);
    child.addConnectionsFromOneParent(data.excess_genes, better_parent);

    return child;
}

Genome Genome::crossoverSameFitnesses(const Genome& parent_1, const Genome& parent_2){
    GenomeParentsData data = parent_1.getDisjointAndExcessGenes(parent_2);
    Genome child;

    child.addMatchingConnections(data.matching_genes, parent_1, parent_2);

    child.addDisjointConnectionsRandomly(data.disjoint_genes, parent_1, parent_2);
    child.addDisjointConnectionsRandomly(data.excess_genes, parent_1, parent_2);

    return child;

}

bool Genome::validateConnection(const unsigned int in_node_id, const unsigned int out_node_id) const{

    unordered_set<int> visited_nodes;
    stack<unsigned int> stack;
    stack.push(out_node_id);
    while(!stack.empty()){
        unsigned int current_node = stack.top();
        stack.pop();

        if(current_node == in_node_id){
            return false; //Creates cycle
        }

        if (visited_nodes.count(current_node)){
            continue;
        }
        visited_nodes.insert(current_node);

        for (const Connection& connection: network_.getConnections()){
            if(connection.in_node == current_node){
                stack.push(connection.out_node);
            }
        }
    }

    return true;

};

bool Genome::validateConnection(const Connection& connection_to_check) const{
    return validateConnection(connection_to_check.in_node, connection_to_check.out_node);
}


void Genome::mutateWeight(Connection& connection){
    if(roll(NEAT::getInstance().getHyperparams().MUTATION_WEIGHT_ASSIGNING_NEW_VALUE_PROBABILITY)){
        connection.weight = generateRandomGaussianFloat();
    }
    else{ //perturbation
        const float max_perturbation = NEAT::getInstance().getHyperparams().MUTATION_WEIGHT_UNIFORM_PERTURBATION_MAX_CHANGE;
        connection.weight += generateRandomFloatInRange(-max_perturbation, max_perturbation);
    }
};

void Genome::mutateAddConnection(const unsigned int in_node_id, const unsigned int out_node_id, InnovationTracker& tracker){
    const unsigned int connection_id = tracker.getInnovationValueFromNodes(in_node_id, out_node_id);
    if(!network_.hasConnectionWithID(connection_id)){
        const Connection connection{
            connection_id,
            in_node_id,
            out_node_id,
        generateRandomFloatInRange(-1.0f, 1.0f)
        };
        network_.addConnection(std::move(connection));
    }

}

void Genome::mutateAddNode(Connection& connection_to_split, InnovationTracker& tracker){
    const NodeGenerationRecord record = tracker.getNodeIdValueFromSplit(connection_to_split.id);
    const Node new_node{
        record.node_id,
        HIDDEN
    };

    network_.addNode(std::move(new_node));

    const unsigned int conn_split_in_node = connection_to_split.in_node;
    const unsigned int conn_split_out_node = connection_to_split.out_node;
    const float conn_split_weight = connection_to_split.weight;
    connection_to_split.is_enabled = false;

    const Connection in_connection{
        record.innovation_in,
        conn_split_in_node,
        record.node_id,
        conn_split_weight
    };

    network_.addConnection(std::move(in_connection));

    const Connection out_connection{
        record.innovation_out,
        record.node_id,
        conn_split_out_node,
        1.0f
    };
    network_.addConnection(std::move(out_connection));

};

void Genome::mutateEnableConnection(Connection& connection_to_enable){
    connection_to_enable.is_enabled = true;
};

void Genome::mutateDisableConnection(Connection& connection_to_disable){
    connection_to_disable.is_enabled = false;
};

void Genome::mutate(){

    InnovationTracker& tracker = NEAT::getInstance().getInnovationTracker();

    //mutation change weights
    for(Connection& gene: network_.getConnections()){
        if(roll(NEAT::getInstance().getHyperparams().MUTATION_CHANGE_CONNECTION_WEIGHT_PROBABILITY)){
            mutateWeight(gene);
        }
    }

    //mutation add connection
    if(roll(NEAT::getInstance().getHyperparams().MUTATION_ADD_CONNECTION_PROBABILITY)){
        vector<pair<int, int>> valid_nodes;
        for(const Node& in_node: network_.getNodes()){
            for(const Node& out_node: network_.getNodes()){
                if(in_node.node_type == OUTPUT || out_node.node_type == INPUT || in_node.id == out_node.id){
                    continue;
                }
                auto possible_connection = make_pair(in_node.id, out_node.id);
                auto& connections = network_.getConnections();
                if(find(connections.begin(), connections.end(), possible_connection) == network_.getConnections().end() &&
                    validateConnection(possible_connection.first, possible_connection.second)){
                    valid_nodes.push_back(possible_connection);
                }
            }
        }
        if(!valid_nodes.empty()){
            auto node_to_add = chooseFromContainer(valid_nodes);
            mutateAddConnection(node_to_add.first, node_to_add.second, tracker);
        }
    }

    //mutation add node
    if (roll(NEAT::getInstance().getHyperparams().MUTATION_ADD_NODE_PROBABILITY)){
        auto valid_connections = getElementsMeetingCondition(this->network_.getConnections(), [](Connection& connection){return connection.is_enabled;});
        if(!valid_connections.empty()){
            Connection& connection_to_split = chooseFromContainer(valid_connections);
            mutateAddNode(connection_to_split, tracker);
        }
    };

    //mutation enable node
    if(roll(NEAT::getInstance().getHyperparams().MUTATION_ENABLE_NODE_PROBABILITY)){
        auto valid_connections = getElementsMeetingCondition(this->network_.getConnections(), [](Connection& connection){return !connection.is_enabled;});
        if(!valid_connections.empty()){
            Connection& connection_to_enable = chooseFromContainer(valid_connections);
            mutateEnableConnection(connection_to_enable);
        }
    };

    //mutation disable node
    if(roll(NEAT::getInstance().getHyperparams().MUTATION_DISABLE_NODE_PROBABILITY)){
        auto valid_connections = getElementsMeetingCondition(this->network_.getConnections(), [](Connection& connection){return connection.is_enabled;});
        if(!valid_connections.empty()){
            Connection& connection_to_disable = chooseFromContainer(valid_connections);
            mutateDisableConnection(connection_to_disable);
        }
    }

    network_.sortNodesTopologically();

};

/**
 * @file NeuralNetwork.cc
 * @author Bartosz Han
 * @brief This file contains definition of class NeuralNetwork.
 *
 * @note This file and class uses Kahn's Topological Sorting Algorithm implementation from
 * https://mohammad-imran.medium.com/understanding-topological-sorting-with-kahns-algo-8af5a588dd0e .
 */

#include <filesystem>
#include <functional>
#include <stdexcept>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>

#include "NeuralNetwork/NeuralNetwork.h"
#include "NeuralNetwork/Activations.hpp"

#include "Exceptions.hpp"

using namespace std;

NeuralNetwork::NeuralNetwork(const unsigned int n_in_nodes, const unsigned int n_out_nodes, const ActivationType activation_type):
    n_in_nodes_(n_in_nodes),n_out_nodes_(n_out_nodes),
    activation_type_(activation_type),activation_(getActivation(activation_type)){};


void NeuralNetwork::addInitialNodes(){
    //Adding input nodes
    for(unsigned int i=0; i<n_in_nodes_; ++i){
        nodes_.push_back(Node{i, INPUT});
        input_nodes_.push_back(i);
    }

    //Adding bias node
    unsigned int bias_id = n_in_nodes_;
    nodes_.push_back(Node{bias_id, INPUT, 1.0f});
    input_nodes_.push_back(bias_id);

    //Adding output nodes
    for(unsigned int i=n_in_nodes_+1 ; i<=n_in_nodes_+n_out_nodes_; ++i){
        nodes_.push_back(Node{i, OUTPUT});
        output_nodes_.push_back(i);
    }

}

void NeuralNetwork::addNodesFromConnections(){

    addInitialNodes();

    for(const Connection& connection: connections_){
        unsigned int in_node_id = connection.in_node;
        unsigned int out_node_id = connection.out_node;

        if(find(nodes_.begin(), nodes_.end(), in_node_id) == nodes_.end()){
            nodes_.push_back(Node{
                in_node_id,
                HIDDEN
            });
        }
        if(find(nodes_.begin(), nodes_.end(), out_node_id) == nodes_.end()){
            nodes_.push_back(Node{
                out_node_id,
                HIDDEN
            });
        }
    }
    sort(nodes_.begin(), nodes_.end());
    sort(connections_.begin(), connections_.end());

    sortNodesTopologically();

}


unsigned int NeuralNetwork::getNInNodes() const{
    return n_in_nodes_;
}

unsigned int NeuralNetwork::getNOutNodes() const{
    return n_out_nodes_;
}

std::function<float(const float)> NeuralNetwork::getActivationFunction() const{
    return activation_;
}

void NeuralNetwork::addNode(Node new_node){
    nodes_.push_back(new_node);
}

void NeuralNetwork::addConnection(Connection new_connection){
    connections_.push_back(new_connection);
}

std::vector<Connection>& NeuralNetwork::getConnections(){
    return connections_;
}

const std::vector<Node>& NeuralNetwork::getNodes() const{
    return nodes_;
}

const std::vector<Connection>& NeuralNetwork::getConnections() const{
    return connections_;
}

const std::vector<unsigned int>& NeuralNetwork::getInputNodes() const{
    return input_nodes_;
}

const std::vector<unsigned int>& NeuralNetwork::getOutputNodes() const{
    return output_nodes_;
}

const std::vector<unsigned int>& NeuralNetwork::getNodeOrder() const{
    return node_order_;
}

Node& NeuralNetwork::getNodeWithID(const int unsigned id){
    auto it = find(nodes_.begin(), nodes_.end(), id);
    if(it != nodes_.end()){
        return nodes_.at(it-nodes_.begin());
    }
    throw InvalidIDError("Node", id);
};

const Node& NeuralNetwork::getNodeWithID(const unsigned int id) const{
    auto it = find(nodes_.begin(), nodes_.end(), id);
    if(it != nodes_.end()){
        return nodes_.at(it-nodes_.begin());
    }

    throw InvalidIDError("Node", id);

};

Connection& NeuralNetwork::getConnectionWithID(const unsigned int id){
    auto it = find(connections_.begin(), connections_.end(), id);
    if(it != connections_.end()){
        return connections_.at(it-connections_.begin());
    }
    throw InvalidIDError("Connection", id);
};

const Connection& NeuralNetwork::getConnectionWithID(const unsigned int id) const{
    auto it = find(connections_.begin(), connections_.end(), id);
    if(it != connections_.end()){
        return connections_.at(it-connections_.begin());
    }
    throw InvalidIDError("Connection", id);
};

bool NeuralNetwork::hasNodeWithID(const unsigned int id) const{
    return find(nodes_.begin(), nodes_.end(), id) != nodes_.end();
};

bool NeuralNetwork::hasConnectionWithID(const unsigned int id) const{
    return find(connections_.begin(), connections_.end(), id) != connections_.end();
};

void NeuralNetwork::sortNodesTopologically(){
    unordered_map<unsigned int, vector<unsigned int>> graph;
    unordered_map<unsigned int, unsigned int> degrees;
    for(const Node& node: nodes_){
        degrees[node.id] = 0;
    }

    for(const Connection& connection: connections_){
        if(!connection.is_enabled){
            continue;
        }
        graph[connection.in_node].push_back(connection.out_node);
        ++degrees[connection.out_node];
    }

    vector<unsigned int> sorted_nodes;

    queue<unsigned int> queue;
    for(const auto& degree: degrees){
        if(degree.second == 0){
            queue.push(degree.first);
        }
    }

        while(!queue.empty()){
            unsigned int current_node = queue.front();
            queue.pop();
            sorted_nodes.push_back(current_node);

            for(unsigned int neighbour: graph[current_node]){
                --degrees[neighbour];
                if(degrees[neighbour] == 0){
                    queue.push(neighbour);
                }
            }

        }
    node_order_ = sorted_nodes;
}

const vector<float> NeuralNetwork::propagate(const vector<float>& input){

    // Order of nodes: INPUT -> BIAS -> OUTPUT -> HIDDEN

    for (auto& node : nodes_) {
        node.value = 0.0f;
    }

    const int input_size = input.size();
    for(int i=0; i<input_size; ++i){
        getNodeWithID(input_nodes_.at(i)).value = input.at(i);
    }

    // bias
    // This part code is assuming that last input node is always bias node
    // assert(getNodeGeneWithID(input_nodes.back()).node_type == BIAS);
    getNodeWithID(input_nodes_.back()).value = 1.0f;

    for(const unsigned int& node_id: node_order_){
        Node& node = getNodeWithID(node_id);
        if(node.node_type == INPUT){
            continue;
        }

        float value = 0.0f;

        for(const Connection& connection: connections_){
            if(!connection.is_enabled){
                continue;
            }
            if(connection.out_node == node_id){
                value += getNodeWithID(connection.in_node).value * connection.weight;
            }
        }
        node.value = activation_(value);
    }

    vector<float> output;
    for(const int output_node: output_nodes_){
        output.push_back(getNodeWithID(output_node).value);
    }
    return output;
};

void NeuralNetwork::saveIntoFile(const string& file_path) const{


    /*
    n_in_nodes
    n_out_nodes
    activation
    connection 0
    connection 1
    ...
    connection n
    */



    ofstream handle(file_path);

    handle << n_in_nodes_ << endl;
    handle << n_out_nodes_ << endl;
    handle << getActivationName(activation_type_) << endl;

    for(const Connection& connection: connections_){
        handle << connection << endl;
    };

};

NeuralNetwork NeuralNetwork::fromFile(const string& file_path){

    if(!filesystem::exists(file_path)){
        throw invalid_argument("File containing AI data: " + file_path + " was not found.");
    }

    /*
    n_in_nodes
    n_out_nodes
    activation
    connection 0
    connection 1
    ...
    connection n
    */

    ifstream file(file_path);
    std::string line;
    int n_in_nodes;
    int n_out_nodes;
    ActivationType activation;

    if(std::getline(file, line)){
        n_in_nodes = stoi(line);
    }
    else{
        throw invalid_argument("Expected number of input nodes at line 0 of AI data file");
    }

    if(std::getline(file, line)){
        n_out_nodes = stoi(line);
    }
    else{
        throw invalid_argument("Expected number of output nodes at line 1 of AI data file");
    }

    if(std::getline(file, line)){
        activation = getActivationType(line);
    }
    else{
        throw invalid_argument("Expected name of activation function at line 2 of AI data file");
    }

    NeuralNetwork network(n_in_nodes, n_out_nodes, activation);

    while(std::getline(file, line)){
        network.connections_.push_back(Connection::fromString(line));
    }

    network.addNodesFromConnections();

    return network;
}
/**
 * @file Node.hpp
 * @author Bartosz Han
 * @brief This file contains Connection class, which represents node in neural network.
 */
#ifndef NODE_HPP
#define NODE_HPP

/**
 * @enum NodeType
 * @brief This file represents different types of nodes: input, output, hidden and bias.
 */
enum NodeType{
    INPUT,
    OUTPUT,
    HIDDEN,
    BIAS
};

/**
 * @struct Node
 * @brief This struct represents node in neural network. It consists of
 * ID, node type and value of node, used in data propagation.
 */
struct Node{
    unsigned int id;
    NodeType node_type;
    float value = 0.0f;

    bool operator==(const Node& other) const{
        return id == other.id;
    };

    bool operator==(const unsigned int& id) const{
        return this->id == id;
    };

    bool operator<(const Node& other) const{
        return id < other.id;
    }
};

#endif
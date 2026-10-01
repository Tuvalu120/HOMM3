/**
 * @file NeuralNetwork.h
 * @author Bartosz Han
 * @brief This file contains class NeuralNetwork, which is a class representing artificial neural networks.
 *
 *
 */
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <functional>
#include <vector>

#include "NeuralNetwork/Activations.hpp"
#include "Node.hpp"
#include "Connection.hpp"

/**
 * @class Neural Network
 *
 * This class is representing artificial neural networks.
 * Objects of this class consists of Connections and Nodes.
 * Neural Networks implemented in this class can only do forward propagation.
 */
class NeuralNetwork{

private:
    unsigned int n_in_nodes_;
    unsigned int n_out_nodes_;
    ActivationType activation_type_;
    std::function<float(const float)> activation_;

    std::vector<Node> nodes_;
    std::vector<unsigned int> node_order_;
    std::vector<Connection> connections_;
    std::vector<unsigned int> input_nodes_;
    std::vector<unsigned int> output_nodes_;


    public:

    /**
     * @brief Construct a new Neural Network object
     *
     * @param n_in_nodes Number of input nodes which this network will have
     * @param n_out_nodes Number of output nodes which this network will have
     * @param activation_type Activation function of this network
     */
    NeuralNetwork(const unsigned int n_in_nodes, const unsigned int n_out_nodes, const ActivationType activation_type);

    /// @brief Add input, bias and output Nodes into network.
    void addInitialNodes();


    /// @brief Add nodes into network, based on the connections in this node.
    /// This method will add all Nodes, which are present in connections, in either in node or out node of connections.
    void addNodesFromConnections();

    /// @brief Get number of input nodes of this network.
    unsigned int getNInNodes() const;

    /// @brief Get number of output nodes of this network.
    unsigned int getNOutNodes() const;

    /// @brief Get activation function of this network.
    std::function<float(const float)> getActivationFunction() const;

    /**
     * @brief Get the Node with provided ID.
     *
     * @param id ID of node.
     */
    Node& getNodeWithID(const unsigned int id);

    /**
     * @brief Get the Node with provided ID.
     *
     * @param id ID of node.
     */
    const Node& getNodeWithID(const unsigned int id) const;

    /**
     * @brief Get the Connection with provided ID.
     *
     * @param id ID of node.
     */
    Connection& getConnectionWithID(const unsigned int id);

    /**
     * @brief Get the Connection with provided ID.
     *
     * @param id ID of node.
     */
    const Connection& getConnectionWithID(const unsigned int id) const;

    /**
     * @brief Returns ture, is this network has Node with provided ID.
     *
     * @param id ID of Node to search.
     */
    bool hasNodeWithID(const unsigned int id) const;

    /**
     * @brief Returns ture, is this network has Connection with provided ID.
     *
     * @param id ID of Node to search.
     */
    bool hasConnectionWithID(const unsigned int id) const;

    /**
     * @brief Sort nodes in network, based on the order of
     * processing nodes. This method will sort IDs of network,
     * and will store them in node_order_.
     *
     */
    void sortNodesTopologically();

    /**
     * @brief Propagate the data in network
     *
     * @param input Vector of values, which represents input of network. This vector must have
     * the size the same as the number of input nodes.
     * @return const std::vector<float> The output of network, with size equal to the number of out nodes.
     */
    const std::vector<float> propagate(const std::vector<float>& input);

    /// @brief Return vector of nodes present in this network
    const std::vector<Node>& getNodes() const;

    /// @brief Return vector of connections present in this network
    const std::vector<Connection>& getConnections() const;

    /// @brief Return vector of connections present in this network
    std::vector<Connection>& getConnections();

    /// @brief Add new node to network. This method will not check, if new node is valid.
    void addNode(Node new_node);

    /// @brief Add new connection to network. This method will not check, if new connection is valid.
    void addConnection(Connection new_connection);

    /// @brief Get IDs of input nodes.
    const std::vector<unsigned int>& getInputNodes() const;

    /// @brief Get IDs of output nodes.
    const std::vector<unsigned int>& getOutputNodes() const;

    /// @brief Get order of nodes (as vector of IDs), in which nodes will be processed in forward propagation.
    const std::vector<unsigned int>& getNodeOrder() const;

    /**
     * @brief Write data about this network into provided new file.
     *
     * @param file_path Path of file in which data of network will be stored.
     */
    void saveIntoFile(const std::string& file_path) const;

    /**
     * @brief Create new Neural Network, with data from path file.
     *
     * @param file_path File path to the file, in which data
     * about network is stored.
     */
    static NeuralNetwork fromFile(const std::string& file_path);

};

#endif
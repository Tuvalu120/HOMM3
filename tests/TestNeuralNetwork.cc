/**
 * @file TestNeuralNetwork.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class NeuralNetwork.
 */
#include "CatchConfig.hpp"
#include "NeuralNetwork/NeuralNetwork.h"
#include "NeuralNetwork/Activations.hpp"

TEST_CASE("Constructor and propagate test", "[NeuralNetwork]") {
    NeuralNetwork network(1, 1, ActivationType::X);
    network.addConnection(Connection{0, 0, 2, 1.0f, true});
    network.addNodesFromConnections();

    REQUIRE(network.getNInNodes() == 1);
    REQUIRE(network.getNOutNodes() == 1);

    std::vector<float> input = {2.0f};
    auto output = network.propagate(input);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == Approx(2.0f));
}

TEST_CASE("Propagate with bias test", "[NeuralNetwork]") {
    NeuralNetwork network(1, 1, ActivationType::X);
    network.addConnection(Connection{0, 0, 2, 1.0f, true});
    network.addConnection(Connection{1, 1, 2, 0.5f, true});
    network.addNodesFromConnections();

    std::vector<float> input = {3.0f};
    auto output = network.propagate(input);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == Approx(3.5f));
}

TEST_CASE("Propagate with hidden node test", "[NeuralNetwork]") {
    NeuralNetwork network(1, 1, ActivationType::X);
    network.addConnection(Connection{0, 0, 3, 0.5f, true});
    network.addConnection(Connection{1, 3, 2, 2.0f, true});
    network.addNodesFromConnections();

    std::vector<float> input = {4.0f};
    auto output = network.propagate(input);

    REQUIRE(output.size() == 1);
    const std::vector<unsigned int> expected_order = {1, 0, 3, 2};
    REQUIRE(network.getNodeOrder().size() == 4);
    REQUIRE(network.getNodeOrder()[2] == expected_order[2]);
    REQUIRE(network.getNodeOrder()[3] == expected_order[3]);
    REQUIRE(output[0] == Approx(4.0f));
}

TEST_CASE("Propagate with disabled connection test", "[NeuralNetwork]") {
    NeuralNetwork net(1, 1, ActivationType::X);
    net.addConnection(Connection{0, 0, 2 ,1.0f, false}); // false = disabled
    net.addNodesFromConnections();

    std::vector<float> input = {5.0f};
    auto output = net.propagate(input);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == Approx(0.0f));
}

TEST_CASE("Network ReLU test", "[NeuralNetwork]") {
    NeuralNetwork network(1, 1, ActivationType::RELU);
    network.addConnection(Connection{0, 0, 2, 1.0f, true});
    network.addNodesFromConnections();

    std::vector<float> input = {2.0f};
    auto output = network.propagate(input);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == Approx(2.0f));
}

TEST_CASE("Network Sigmoid test", "[NeuralNetwork]") {
    NeuralNetwork network(1, 1, ActivationType::SIGMOID);
    network.addConnection(Connection{0, 0, 2, 1.0f, true});
    network.addNodesFromConnections();

    std::vector<float> input = {0.0f};
    auto output = network.propagate(input);

    REQUIRE(output.size() == 1);
    REQUIRE(output[0] == Approx(0.5f));
}
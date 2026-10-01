/**
 * @file TestGenome.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Genome.
 */

#include <memory>


#include "CatchConfig.hpp"
#include "Genome.h"
#include "NeuralNetwork/Connection.hpp"
#include "NeuralNetwork/NeuralNetwork.h"
#include "include/catch.hpp"
#include "TestUtilities.hpp"


TEST_CASE("createBasicNetwork test", "[Genome]"){
    resetNEAT(9, 5);
    Genome genome;
    genome.createBasicNetwork();

    //Bias should be also created
    REQUIRE(genome.getNetwork().getConnections().size() == 50);
    REQUIRE(genome.getNetwork().getNodes().size() == 15);
    REQUIRE(genome.getNetwork().getInputNodes().back() == 9);
    REQUIRE(genome.getNetwork().getOutputNodes().back() == 14);

    Genome genome_2;
    genome.createBasicNetwork();
    REQUIRE(genome.getNetwork().getInputNodes().back() == 9);
    REQUIRE(genome.getNetwork().getOutputNodes().back() == 14);

}


TEST_CASE("Mutate change connection weights test", "[Genome]"){
    resetNEAT();
    Genome genome;
    genome.createBasicNetwork();
    auto& connections = genome.getNetwork().getConnections();
    REQUIRE(!connections.empty());
    float old_weight = connections[0].weight;

    genome.mutate();
    REQUIRE(connections[0].weight != old_weight);
}

TEST_CASE("Mutate add a connection test", "[Genome]"){
    resetNEAT();
    Genome genome;
    genome.createBasicNetwork();
    auto& network = genome.getNetwork();
    size_t old_size = network.getConnections().size();

    Node new_node{3, HIDDEN};
    network.addNode(new_node);

    genome.mutate();
    REQUIRE(network.getConnections().size() > old_size);
}

TEST_CASE("Mutate add a node test", "[Genome]"){
    resetNEAT();
    Genome genome;
    genome.createBasicNetwork();
    auto& network = genome.getNetwork();
    size_t old_nodes = network.getNodes().size();
    size_t old_connections = network.getConnections().size();

    genome.mutate();

    REQUIRE(network.getNodes().size() > old_nodes);
    REQUIRE(network.getConnections().size() == old_connections + 2);
    REQUIRE(network.getNodeOrder()[3] == 2);
    REQUIRE(network.getNodeOrder()[2] == 3);

}

TEST_CASE("Mutate enable and disable connections test", "[Genome]"){
    resetNEAT(1, 1, "HyperparamsEnable.txt");
    Genome genome;
    genome.createBasicNetwork();
    auto& network = genome.getNetwork();
    auto& connections = network.getConnections();

    for (auto& c : connections) c.is_enabled = false;
    genome.mutate();
    bool any_enabled = std::any_of(connections.begin(), connections.end(), [](const Connection& c){return c.is_enabled;});
    REQUIRE(any_enabled);


    resetNEAT(1, 1);

    for (auto& c : connections) c.is_enabled = true;
    genome.mutate();
    bool any_disabled = std::any_of(connections.begin(), connections.end(), [](const Connection& c){ return !c.is_enabled;});
    REQUIRE(any_disabled);
}

TEST_CASE("getDisjointAndExcessGenes test", "[Genome]"){

    resetNEAT(2, 2);
    Genome genome;
    genome.createBasicNetwork();
    genome.getNetwork().addNode(Node{5, HIDDEN});
    genome.getNetwork().addConnection(Connection{6, 1, 5, 0.5f, true});


    Genome genome_2;
    genome_2.createBasicNetwork();
    genome_2.getNetwork().addNode(Node{5, HIDDEN});
    genome_2.getNetwork().addConnection(Connection{7, 0, 5, 0.5f, true});
    genome_2.getNetwork().addConnection(Connection{8, 5, 3, 1.0f, true});

    GenomeParentsData data = genome.getDisjointAndExcessGenes(genome_2);

    REQUIRE(data.matching_genes == std::vector<unsigned int>{0,1,2,3,4,5});
    REQUIRE(data.disjoint_genes == std::vector<unsigned int>{6});
    REQUIRE(data.excess_genes == std::vector<unsigned int>{7, 8});

}

TEST_CASE("calculateCompatibilityDistance test", "[Genome]"){

    resetNEAT(2, 2);
    Genome genome;
    genome.createBasicNetwork();
    genome.getNetwork().addNode(Node{5, HIDDEN});
    genome.getNetwork().addConnection(Connection{6, 1, 5, 0.5f, true});

    Genome genome_2(genome);
    genome_2.getNetwork().addNode(Node{5, HIDDEN});
    genome_2.getNetwork().addConnection(Connection{7, 0, 5, 0.5f, true});
    genome_2.getNetwork().addConnection(Connection{8, 5, 3, 1.0f, true});

    float value = Genome::calculateCompatibilityDistance(genome, genome_2);

    REQUIRE(Approx(value) == 2.0f);

    value = Genome::calculateCompatibilityDistance(genome, genome);
    REQUIRE(value == 0.0f);


}

TEST_CASE("addConnections test", "[Genome]"){

    resetNEAT(2, 2);
    Genome genome;
    genome.createBasicNetwork();
    genome.getNetwork().addNode(Node{5, HIDDEN});
    genome.getNetwork().addConnection(Connection{6, 1, 5, 0.5f, true});

    Genome genome_2;
    genome_2.createBasicNetwork();
    genome_2.getNetwork().addNode(Node{5, HIDDEN});
    genome_2.getNetwork().addConnection(Connection{7, 0, 5, 0.5f, true});
    genome_2.getNetwork().addConnection(Connection{8, 5, 3, 1.0f, true});

    Genome child;
    GenomeParentsData parents_data = genome.getDisjointAndExcessGenes(genome_2);
    child.addMatchingConnections(parents_data.matching_genes, genome, genome_2);

    for(const auto conn: child.getNetwork().getConnections()){
        unsigned int id = conn.id;
        float weight = conn.weight;
        bool is_equal = weight == genome.getNetwork().getConnectionWithID(id).weight ||
            weight == genome_2.getNetwork().getConnectionWithID(id).weight;
        REQUIRE(is_equal);
    }


    child.addConnectionsFromOneParent(parents_data.excess_genes, genome_2);
    REQUIRE(child.getNetwork().getConnections().size() == 8);
    REQUIRE(child.getNetwork().getConnections().at(6).id == 7);
    REQUIRE(child.getNetwork().getConnections().at(7).id == 8);


}


TEST_CASE("Genome crossover test", "[Genome]"){

    resetNEAT(2, 2);
    Genome parent_1; //better
    parent_1.createBasicNetwork();
    parent_1.getNetwork().addNode(Node{5, HIDDEN});
    parent_1.getNetwork().addConnection(Connection{6, 1, 5, 0.5f, true});

    Genome parent_2; //worse
    parent_2.createBasicNetwork();
    parent_2.getNetwork().addNode(Node{5, HIDDEN});
    parent_2.getNetwork().addConnection(Connection{7, 0, 5, 0.5f, true});
    parent_2.getNetwork().addConnection(Connection{8, 5, 3, 1.0f, true});

    Genome child = Genome::crossoverDifferentFitnesses(parent_1, parent_2);

    std::vector<unsigned int> expected_ids = {0,1,2,3,4,5,6};
    auto& child_conns = child.getNetwork().getConnections();
    std::vector<unsigned int> child_ids;
    for(const auto& c : child_conns){
        child_ids.push_back(c.id);
    };

    for(unsigned int id : expected_ids) {
        REQUIRE(std::find(child_ids.begin(), child_ids.end(), id) != child_ids.end());
    }

    REQUIRE(std::find(child_ids.begin(), child_ids.end(), 7) == child_ids.end());
    REQUIRE(std::find(child_ids.begin(), child_ids.end(), 8) == child_ids.end());

    ///

    Genome child_2 = Genome::crossoverSameFitnesses(parent_1, parent_2);

    child_conns = child.getNetwork().getConnections();
    child_ids.clear();
    for(const auto& c : child_conns){
        child_ids.push_back(c.id);
    };

    for(unsigned int id = 0; id <= 5; ++id) {
        REQUIRE(std::find(child_ids.begin(), child_ids.end(), id) != child_ids.end());
    }

    bool has_6 = std::find(child_ids.begin(), child_ids.end(), 6) != child_ids.end();
    bool has_7 = std::find(child_ids.begin(), child_ids.end(), 7) != child_ids.end();
    bool has_8 = std::find(child_ids.begin(), child_ids.end(), 8) != child_ids.end();
    bool has_at_least_one = has_6 || (has_7 && has_8);
    REQUIRE(has_at_least_one);
}
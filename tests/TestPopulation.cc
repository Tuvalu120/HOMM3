/**
 * @file TestPopulation.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Population.
 */

#include "NeuralNetwork/Node.hpp"
#include "Population.h"
#include "Specimen.h"
#include "Species.h"
#include "TestUtilities.hpp"
#include "CatchConfig.hpp"
#include "include/catch.hpp"
#include "Exceptions.hpp"


TEST_CASE("Initialize test", "[Population]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();
    REQUIRE(pop.getSpecimens().size() == NEAT::getInstance().getHyperparams().N_SPECIMEN);
}

TEST_CASE("Specialization test", "[Population][Species]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    for(auto& specimen: pop.getSpecimens()){
        specimen.setFitness(1);
    }

    pop.specialize();
    REQUIRE(pop.getSpeciesList().size() == 1);

    Species s = pop.getSpeciesList().at(0);
    REQUIRE(s.getSpecimensID().size() == NEAT::getInstance().getHyperparams().N_SPECIMEN * NEAT::getInstance().getHyperparams().PERCENT_OF_BEST_SPECIMEN_IN_SPECIES_TO_SURVIVE);
}

TEST_CASE("crossoverSpecimen test", "[Population]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    for(auto& specimen: pop.getSpecimens()){
        specimen.setFitness(1);
    }

    auto& specimens = pop.getSpecimens();
    Specimen& s1 = specimens.at(0);
    Specimen& s2 = specimens.at(1);

    s1.addFitness(1);
    s1.getGenome().getNetwork().addNode(Node{3, HIDDEN, 0.0f});
    s1.getGenome().getNetwork().addConnection(Connection{2,0, 3, 0.0f, true});
    REQUIRE(s1.getGenome().getNetwork().getConnections().size() == 3);
    REQUIRE(Approx(s1.getFitness()) == 2.0f);
    REQUIRE(Approx(s2.getFitness()) == 1.0f);

    pop.specialize();

    Specimen child = pop.crossoverSpecimen(s1, s2);
    REQUIRE(child.getGenome().getNetwork().getNodes().size() == 4);
    REQUIRE(child.getGenome().getNetwork().getConnections().size() == 3);
}

TEST_CASE("getEliteOfSpecies test", "[Population]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    for(auto& specimen: pop.getSpecimens()){
        specimen.setFitness(1);
    }
    unsigned int best_id = 1;
    pop.getSpecimenWithID(best_id).addFitness(1);
    pop.specialize();


    std::vector<Specimen> elite = pop.getEliteOfSpecies();
    std::vector<Specimen> global_elite = pop.getGlobalElite();
    elite.insert(elite.end(), global_elite.begin(), global_elite.end());
    REQUIRE(elite.size() == 1);
    REQUIRE(elite.at(0).getID() == best_id);
}

TEST_CASE("Reproduce new generation test", "[Population]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();
    pop.initialize();
    auto old_ids = pop.getSpecimensIDs();

    for(auto& specimen: pop.getSpecimens()){
        specimen.setFitness(1);
    }
    unsigned int best_id = 1;
    pop.getSpecimenWithID(best_id).addFitness(1);

    pop.specialize();
    pop.createNewPopulation();

    auto new_ids = pop.getSpecimensIDs();

    REQUIRE(new_ids.size() == old_ids.size());

    bool any_diff = false;
    for (size_t i = 0; i < old_ids.size(); ++i)
        if (old_ids[i] != new_ids[i]){
            any_diff = true;
        }

    REQUIRE(any_diff);
    REQUIRE(std::find(pop.getSpecimensIDs().begin(), pop.getSpecimensIDs().end(), best_id) != pop.getSpecimensIDs().end());
}

TEST_CASE("Populations exceptions test", "[Population]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();
    pop.initialize();
    REQUIRE_THROWS_AS(pop.chooseOtherParent({}, 0), std::invalid_argument);

    REQUIRE_THROWS_AS(pop.getSpecimenWithID(10), InvalidIDError);
    REQUIRE_THROWS_AS(pop.getSpeciesWithID(0), InvalidIDError);

}
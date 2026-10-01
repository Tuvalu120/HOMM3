/**
 * @file TestSpecies.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class Species.
 */

#include "Hyperparams.hpp"
#include "Species.h"
#include "Genome.h"
#include "Population.h"
#include "Specimen.h"
#include "CatchConfig.hpp"
#include "TestUtilities.hpp"
#include <cstddef>


TEST_CASE("Species ID test", "[Species]"){
    resetNEAT();
    Genome g;
    Species s1(g);
    Species s2(g);
    REQUIRE(s1.getID() != s2.getID());
}

TEST_CASE("Species sort test", "[Species]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    for(size_t i=0; i<pop.getSpecimens().size(); ++i){
        pop.getSpecimenWithID(i).setFitness(i);
    }
    pop.specialize();
    Specimen elite = pop.getGlobalElite().at(0);
    Species& species = pop.getSpeciesWithID(0);
    REQUIRE(species.getIDOfBestSpecimen() == elite.getID());
    REQUIRE(species.getSpecimensID().at(0) == elite.getID());
}

TEST_CASE("Species fitness test", "[Species]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    for(size_t i=0; i<pop.getSpecimens().size(); ++i){
        pop.getSpecimenWithID(i).setFitness(i);
    }
    for(auto& specimen: pop.getSpecimens()){
        specimen.setFitness(1);
    };
    pop.specialize();

    REQUIRE(Approx(pop.getSpeciesWithID(0).getSpeciesFitness()) == 1.0f);
}

TEST_CASE("Species Fitness and specimen purge", "[Species]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    float total_fitness = 0.0f;
    for(size_t i=0; i<pop.getSpecimens().size(); ++i){
        pop.getSpecimenWithID(i).setFitness(i);
        total_fitness += i;
    }
    pop.specialize();

    const Hyperparams& hyperparams = NEAT::getInstance().getHyperparams();
    total_fitness /= hyperparams.N_SPECIMEN;
    Species& species = pop.getSpeciesWithID(0);
    REQUIRE(species.getSpeciesFitness() == total_fitness);
    size_t expected_n_specimen = hyperparams.N_SPECIMEN * hyperparams.PERCENT_OF_BEST_SPECIMEN_IN_SPECIES_TO_SURVIVE;
    REQUIRE(species.getSpecimensID().size() == expected_n_specimen);
    for(const size_t id: species.getSpecimensID()){
        REQUIRE(id >= expected_n_specimen);
    }

}

TEST_CASE("Species new species and N specimen to reproduce test", "[Species]"){
    resetNEAT();
    Population& pop = NEAT::getInstance().getPopulation();

    for(size_t i=0; i<pop.getSpecimens().size(); ++i){
        pop.getSpecimenWithID(i).setFitness(1);
    }
    unsigned int odd_specimen_id = 0;
    Specimen& odd_specimen = pop.getSpecimenWithID(odd_specimen_id);
    odd_specimen.getGenome().getNetwork().getConnectionWithID(0).weight = 1000.0f;
    odd_specimen.setFitness(8.0f);
    pop.specialize();
    REQUIRE(pop.getSpeciesList().size() == 2);
    const Species& odd_species = pop.getSpeciesList().at(0).getIDOfBestSpecimen() == odd_specimen_id ?
        pop.getSpeciesList().at(0) : pop.getSpeciesList().at(1);

    Genome representative = odd_species.getRepresentative();
    REQUIRE(representative.getNetwork().getConnections() == odd_specimen.getGenome().getNetwork().getConnections());
    //Global best specimen reduces the number of specimen to reproduce by one.
    //New species should get 8/9 * (10-1) = 8 specimens in next gen.
    REQUIRE(Approx(odd_species.getSpeciesFitness()) == 8.0f);
    REQUIRE(odd_species.getNSpecimenToReproduce() == 8);
    pop.createNewPopulation();
    int counter = 0;
    for(const auto& specimen: pop.getSpecimens()){
        float weight = specimen.getGenome().getNetwork().getConnections().at(0).weight;
        if(weight > 990.0f && weight < 1010.0f){
            ++counter;
        }
    }
    REQUIRE(counter == 9);

}

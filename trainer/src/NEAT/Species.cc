/**
 * @file Species.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Species.
 *
 */

#include <algorithm>
#include <numeric>
#include <stdexcept>

#include "Species.h"
#include "Population.h"
#include "Utilities.hpp"
#include "Hyperparams.hpp"
#include "NEAT.h"

using namespace std;

unsigned int Species::next_id_=0;

Species::Species(Genome representative_):id_(next_id_++),representative_(representative_){};

bool Species::operator==(const Species& other) const{
    return id_ == other.id_;
}

bool Species::operator==(const unsigned int& id) const{
    return this->id_ == id;
}

void Species::updateStaleness(){

    if(!specimens_id_.empty()){

        float current_best_fitness = specimens_fitnesses_.at(0);
        if(current_best_fitness >= best_fitness_ ){
            staleness_ = 0;
        }
        else{
            ++staleness_;
        }
    }
}

void Species::addRepresentativeToSpecies(){

    if(!specimens_id_.empty()){
        const int next_representative = chooseFromContainer(specimens_id_);
        representative_ = NEAT::getInstance().getPopulation().getSpecimenWithID(next_representative).getGenomeCopy();
    }
}

void Species::sortSpecimenListWithinSpecies(){
    const Population& population = NEAT::getInstance().getPopulation();
    sort(specimens_id_.begin(), specimens_id_.end(), [population](const int a, const int b){
        const Specimen& s_a = population.getSpecimenWithID(a);
        const Specimen& s_b = population.getSpecimenWithID(b);
        return s_a > s_b;
    });
}

void Species::addFitnessDataForSpecies(){
    specimens_fitnesses_.clear();
    for(int specimen: specimens_id_){
        specimens_fitnesses_.push_back(
            NEAT::getInstance().getPopulation().getSpecimenWithID(specimen).getFitness());
    };
}


void Species::calculateSpeciesFitness(){
    float score = std::accumulate(specimens_fitnesses_.begin(), specimens_fitnesses_.end(), 0.0f);
    score /= specimens_id_.size();
    species_fitness_ = score;
}

void Species::purgeWorstSpecimens(){

    Population& population = NEAT::getInstance().getPopulation();
    float percent_to_remain = NEAT::getInstance().getHyperparams().PERCENT_OF_BEST_SPECIMEN_IN_SPECIES_TO_SURVIVE;
    const int n_to_remain = ceil(specimens_id_.size() * percent_to_remain);
    specimens_id_.erase(specimens_id_.begin()+n_to_remain, specimens_id_.end());
    population.getSurvivedSpecimensID().insert(
        population.getSurvivedSpecimensID().end(),
        specimens_id_.begin(),
        specimens_id_.end()
    );
    specimens_fitnesses_.erase(specimens_fitnesses_.begin()+n_to_remain, (specimens_fitnesses_.end()));

}

void Species::update(){
    addRepresentativeToSpecies();
    sortSpecimenListWithinSpecies();
    addFitnessDataForSpecies();
    calculateSpeciesFitness();
    updateStaleness();
    purgeWorstSpecimens();
}

unsigned int Species::getIDOfBestSpecimen() const{
    if(!specimens_id_.empty()){
        return specimens_id_.at(0);
    }
    throw runtime_error("Tried to access best specimen of extinct species.");
}


unsigned int Species::getID() const{
    return id_;
}

float Species::getSpeciesFitness() const{
    return species_fitness_;
}

unsigned int Species::getNSpecimenToReproduce() const{
    return n_specimen_to_reproduce_;
}

void Species::setNSpecimenToReproduce(const int new_value){
    n_specimen_to_reproduce_ = new_value;
}

void Species::addNSpecimenToReproduce(const int value_to_add){
    n_specimen_to_reproduce_ += value_to_add;
}

float Species::getBestFitness() const{
    return best_fitness_;
}

unsigned int Species::getStaleness() const{
    return staleness_;
}

void Species::incrementStaleness(){
    ++staleness_;
}

void Species::resetStaleness(){
    staleness_ = 0;
}

const Genome& Species::getRepresentative() const{
    return representative_;
}

void Species::setRepresentative(Genome& new_representative){
    representative_ = new_representative;
}

std::vector<unsigned int>& Species::getSpecimensID(){
    return specimens_id_;
}

std::vector<float>& Species::getSpecimensFitnesses(){
    return specimens_fitnesses_;
}

void Species::resetIDCounter(){
    next_id_ = 0;
}

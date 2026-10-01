/**
 * @file Population.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Population.
 *
 */

#include "Population.h"
#include "Specimen.h"
#include "Species.h"
#include "NEAT.h"

#include "Hyperparams.hpp"
#include "Utilities.hpp"
#include "Exceptions.hpp"


using namespace std;

void Population::initialize(){

    Specimen::resetIDCounter();
    Species::resetIDCounter();
    species_list_.clear();
    specimens_.clear();
    survived_specimens_id_.clear();
    for(unsigned int i=0; i<NEAT::getInstance().getHyperparams().N_SPECIMEN; ++i){
        Specimen new_specimen;
        new_specimen.getGenome().createBasicNetwork();
        specimens_.push_back(std::move(new_specimen));
    }

}

void Population::specialize(){

    for(Species& species: species_list_){
        species.getSpecimensID().clear();
        species.getSpecimensFitnesses().clear();
    }
    survived_specimens_id_.clear();

    for(Specimen& specimen: specimens_){
        const Genome& genome = specimen.getGenome();
        bool flag_did_specimen_find_species = false;
        for(Species& species: species_list_){
            const Genome& representative = species.getRepresentative();
            if(Genome::calculateCompatibilityDistance(genome, representative) <= NEAT::getInstance().getHyperparams().COMPATIBILITY_THRESHOLD){
                species.getSpecimensID().push_back(specimen.getID());
                flag_did_specimen_find_species = true;
                break;
            }
        }
        if(!flag_did_specimen_find_species){
            Species new_species(genome);
            new_species.getSpecimensID().push_back(specimen.getID());
            species_list_.push_back(std::move(new_species));
        }

    }
    updateBestSpecimenInfo();
    purgeWorstSpecies();
    updateAllSpecies();
}

void Population::calculateNSpecimenToReproduceForSpecies(){
    map<int, int> n_species_to_reproduce;
    vector<pair<int, float>> remainders;
    const int n_specimen = NEAT::getInstance().getHyperparams().N_SPECIMEN - NEAT::getInstance().getHyperparams().GLOBAL_ELITE_SIZE;

    float sum_of_fitnesses = accumulate(species_list_.begin(), species_list_.end(), 0.0f,
    [](float acc, const Species& s){ return acc + s.getSpeciesFitness(); });

    int children_used=0;
    for(Species& species: species_list_){
        float init_children = n_specimen * (species.getSpeciesFitness() / sum_of_fitnesses);
        int whole_children = floor(init_children);
        species.setNSpecimenToReproduce(whole_children);
        remainders.push_back(make_pair(species.getID(),init_children - whole_children));
        children_used += whole_children;
    };

    int children_left = n_specimen - children_used;
    if(children_left > 0){
        sort(remainders.begin(), remainders.end(), [](const auto& a, const auto& b){return a.second > b.second;});
        for(int i=0; i<children_left; ++i){
            getSpeciesWithID(remainders.at(i).first).addNSpecimenToReproduce(1);
        }
    }
}

void Population::updateAllSpecies(){


    for(Species& species: species_list_){
        species.update();
    }
    calculateNSpecimenToReproduceForSpecies();

}

void Population::purgeWorstSpecies(){
    auto it = species_list_.begin();
    bool will_at_least_one_species_survive = false;
    while (it != species_list_.end()){
        if (it->getSpecimensID().empty() || (it->getStaleness() >= NEAT::getInstance().getHyperparams().MAX_STALENESS && will_at_least_one_species_survive)){
            it = species_list_.erase(it);
        }
        else{
            ++it;
            will_at_least_one_species_survive = true;
        }
    }
}

Specimen Population::crossoverSpecimen(const Specimen& parent_1, const Specimen& parent_2){
    Genome child;
    //if(parent_1.score != parent_2.score)
    if(abs(parent_1.getFitness() - parent_2.getFitness()) > numeric_limits<float>::epsilon()){
        if(parent_1.getFitness() > parent_2.getFitness()){
            child = Genome::crossoverDifferentFitnesses(parent_1.getGenome(), parent_2.getGenome());
        }
        else{
            child = Genome::crossoverDifferentFitnesses(parent_2.getGenome(), parent_1.getGenome());
        }
    }
    else{
        child = Genome::crossoverSameFitnesses(parent_1.getGenome(), parent_2.getGenome());
    }
    child.getNetwork().addNodesFromConnections();

    return Specimen(child);
}

const vector<Specimen> Population::getEliteOfSpecies(){

    vector<Specimen> local_elite;
    for(Species& species: species_list_){
        int best_specimen_id = species.getIDOfBestSpecimen();
        Specimen& best_specimen = getSpecimenWithID(best_specimen_id);
        if(species.getNSpecimenToReproduce() >= NEAT::getInstance().getHyperparams().MIN_SIZE_OF_SPECIES_TO_STORE_ELITE
        && species.getNSpecimenToReproduce() > 0
        && std::find(global_elite_.begin(), global_elite_.end(), best_specimen) == global_elite_.end()){
            local_elite.push_back(Specimen(best_specimen));
            species.addNSpecimenToReproduce(-1);
        }
    }
    return local_elite;
}

const vector<Specimen> Population::selectFromSpecies(){
    vector<Specimen> new_generation;
    for(Species& species: species_list_){

        int n_specimen_to_reproduce = species.getNSpecimenToReproduce();

        if(species.getSpecimensID().size() == 1){
            int specimen_id = species.getSpecimensID().at(0);
            Genome child_genome = getSpecimenWithID(specimen_id).getGenome();
            for(int i=0; i<n_specimen_to_reproduce; ++i){
                new_generation.push_back(Specimen(child_genome));
            }
        }
        else{
            auto& species_specimens = species.getSpecimensID();
            for(int i=0; i<n_specimen_to_reproduce; ++i){

                if(roll(NEAT::getInstance().getHyperparams().SPECIMEN_GOING_TO_NEXT_GENERATION_PROBABILITY)){
                    int id = chooseFromContainer(species_specimens);
                    new_generation.push_back(Specimen(getSpecimenWithID(id).getGenome()));
                }
                else{
                    unsigned int parent_1_id = chooseFromContainer(species_specimens);
                    Specimen& parent_1 = getSpecimenWithID(parent_1_id);

                    int parent_2_id;
                    if(roll(NEAT::getInstance().getHyperparams().INTERSPECIES_CROSSOVER_PROBABILITY)){
                        parent_2_id = chooseOtherParent(survived_specimens_id_, parent_1_id);
                    }
                    else{
                        parent_2_id = chooseOtherParent(species_specimens, parent_1_id);
                    }
                    Specimen& parent_2 = getSpecimenWithID(parent_2_id);
                    new_generation.push_back(crossoverSpecimen(parent_1, parent_2));
                }
            }
        }
    }
    return new_generation;
}

void Population::createNewPopulation(){

    survived_specimens_id_.erase(unique(survived_specimens_id_.begin(), survived_specimens_id_.end()));

    vector<Specimen> new_generation = global_elite_;
    vector<Specimen> local_elite = getEliteOfSpecies();

    new_generation.insert(new_generation.end(), local_elite.begin(), local_elite.end());

    vector<Specimen> offspring = selectFromSpecies();
    for(Specimen& specimen: offspring){
        specimen.getGenome().mutate();
    }
    new_generation.insert(new_generation.end(), offspring.begin(), offspring.end());
    specimens_ = new_generation;
}

Specimen& Population::getSpecimenWithID(const unsigned int id){
    auto it = find(specimens_.begin(), specimens_.end(), id);
    if(it != specimens_.end()){
        return specimens_.at(it-specimens_.begin());
    }
        throw InvalidIDError("Specimen", id);
}

const Specimen& Population::getSpecimenWithID(const unsigned int id) const{
    return const_cast<Population*>(this)->getSpecimenWithID(id);
}

Species& Population::getSpeciesWithID(const unsigned int id){
    auto it = find(species_list_.begin(), species_list_.end(), id);
    if(it != species_list_.end()){
        return species_list_.at(it-species_list_.begin());
    }
    throw InvalidIDError("Species", id);
}

const Species& Population::getSpeciesWithID(const unsigned int id) const{
    return const_cast<Population*>(this)->getSpeciesWithID(id);
}

std::vector<Specimen>& Population::getSpecimens(){
    return specimens_;
}

std::vector<unsigned int> Population::getSpecimensIDs() const{
    std::vector<unsigned int> specimens;
    for(const Specimen& specimen: specimens_){
        specimens.push_back(specimen.getID());
    }
    return specimens;
}

std::vector<unsigned int>& Population::getSurvivedSpecimensID(){
    return survived_specimens_id_;
}

const std::vector<Species>& Population::getSpeciesList() const{
    return species_list_;
}

void Population::updateBestSpecimenInfo(){

    global_elite_.clear();
    sort(specimens_.begin(), specimens_.end(), std::greater<Specimen>());

    for(unsigned int i=0; i<NEAT::getInstance().getHyperparams().GLOBAL_ELITE_SIZE; ++i){
        Specimen elite_specimen = specimens_.at(i);
        global_elite_.push_back(elite_specimen);
        survived_specimens_id_.push_back(elite_specimen.getID());
    }
}

const vector<Specimen>& Population::getGlobalElite() const{
    return global_elite_;
}

unsigned int Population::getBestSpecimenID() const{
    return max_element(specimens_.begin(), specimens_.end())->getID();
}

unsigned int Population::chooseOtherParent(const std::vector<unsigned int>& possible_parents, const unsigned int first_parent_id) const{

    if(possible_parents.empty()){
        throw std::invalid_argument("Tried to get element of empty container");
    }

    std::vector<unsigned int> copy_vec = possible_parents;
    std::shuffle(copy_vec.begin(), copy_vec.end(), getRandomNumberGenerator());
    for(const unsigned int id: copy_vec){
        if(id != first_parent_id){
            return id;
        };
    }
    throw std::runtime_error("No other parent found");
}
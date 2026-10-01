/**
 * @file Specimen.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Specimen.
 *
 */

#include "Specimen.h"

using namespace std;

unsigned int Specimen::next_id_ = 0;

Specimen::Specimen(Genome new_genome):id_(next_id_++),genome_(new_genome){};

Specimen::Specimen(const Specimen& other):id_(other.id_),genome_(other.genome_),fitness_(other.fitness_){};

Specimen::Specimen():id_(next_id_++),genome_(){};

const vector<float> Specimen::predict(const vector<float>& input){
    return genome_.getNetwork().propagate(input);
};

bool Specimen::operator==(const unsigned int& id) const{
    return this->id_ == id;
}

bool Specimen::operator==(const Specimen& other) const{
    return id_ == other.id_;
}

bool Specimen::operator<(const Specimen& other) const{
    return fitness_ < other.fitness_;
}

bool Specimen::operator>(const Specimen& other) const{
    return fitness_ > other.fitness_;
}

unsigned int Specimen::getID() const{
    return id_;
}

Genome& Specimen::getGenome(){
    return genome_;
}

const Genome& Specimen::getGenome() const{
    return const_cast<Specimen*>(this)->getGenome();
}

Genome Specimen::getGenomeCopy(){
    return genome_;
}

float Specimen::getFitness() const{
    return fitness_;
}

void Specimen::setFitness(const float new_value){
    fitness_ = new_value;
}

void Specimen::addFitness(const float value_to_add){
    fitness_ += value_to_add;
}

void Specimen::resetIDCounter(){
    next_id_ = 0;
}

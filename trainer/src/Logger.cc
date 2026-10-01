/**
 * @file Logger.cc
 * @author Bartosz Han
 * @brief This file contains definition of class Logger.
 *
 */

#include "Logger.h"
#include "TrainerConstants.hpp"
#include "NEAT/NEAT.h"
#include <cstddef>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;
using namespace trainer_constants;
namespace fs = filesystem;

void Logger::registerHandle(const std::string handle_name){
    handles_[handle_name] = ofstream(directory_name_ + "/" + handle_name);
}

void Logger::logData(std::string variable, std::string value){
    ofstream& stream = handles_[variable];
    stream << value << std::endl;
}

void Logger::logData(std::string variable, float value){
    logData(variable, std::to_string(value));
}

void Logger::logData(std::string variable, int value){
    logData(variable, std::to_string(value));
}

void Logger::logData(std::string variable, size_t value){
    logData(variable, std::to_string(value));
}

void Logger::initialize(std::string directory_name, std::string hyperparams_file){
    directory_name_ = OUTPUT_DIR + directory_name;
    models_dir_name_ = directory_name_ + "/" + MODELS_DIR;

    fs::create_directories(directory_name_);
    fs::create_directories(models_dir_name_);

    std::string hyperparams_path = hyperparams_file.substr(hyperparams_file.find_last_of("/")+1);

    if(!hyperparams_file.empty()){

        fs::path input_path = hyperparams_file;
        fs::path output_path = directory_name_ + "/" + hyperparams_path;
        fs::copy_file(input_path, output_path);
    }


    for(const string& info: LOGGED_INFO){
        registerHandle(info);
    }
    //TODO: Make loggers here.

}

std::string Logger::getMainDirectory() const{
    return directory_name_;
}

std::string Logger::getModelsDirectory() const{
    return models_dir_name_;
}

void Logger::savePopulationIntoDirectory(const std::string& directory_name){

    string prefix = models_dir_name_ + "/" + directory_name + "/";
    filesystem::create_directories(prefix);
    for(const Specimen& specimen: NEAT::getInstance().getPopulation().getSpecimens()){
        string file_name = prefix + to_string(specimen.getID());
        specimen.getGenome().getNetwork().saveIntoFile(file_name);
    }
}
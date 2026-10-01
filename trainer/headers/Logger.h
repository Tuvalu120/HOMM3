/**
 * @file Logger.h
 * @author Bartosz Han
 * @brief This file contains Logger class, which is a helper class for
 * logging various data about the training of the computer agents.
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "Singleton.hpp"
#include <fstream>
#include <map>

/**
 * @class Logger
 * @brief This class is a helper class for
 * logging various data about the training of the computer agents.
 * Object of this creates various file handles and write data in them,
 * as training of players progress.
 */
class Logger: public Singleton<Logger>{

    friend class Singleton<Logger>;

private:
    std::map<std::string, std::ofstream> handles_;

    /// @brief Create new file and store handle to this file.
    /// Program will be able to write data into this by using handle_name
    /// as ID of this file. handle_name is also name of file.
    void registerHandle(const std::string handle_name);
    std::string directory_name_;
    std::string models_dir_name_;

    public:

    /// @brief Initialize the Logger, by creating directory in which data will be stored.
    /// Save run's hyperparams file into this directory.
    void initialize(std::string directory_name, std::string hyperparams_file);

    /// @brief Write data <value> in file <variable>.
    void logData(std::string variable, std::string value);

    /// @brief Write data <value> in file <variable>.
    void logData(std::string variable, float value);

    /// @brief Write data <value> in file <variable>.
    void logData(std::string variable, int value);

    /// @brief Write data <value> in file <variable>.
    void logData(std::string variable, size_t value);

    /// @brief Create directory, in which all models in given generation will be stored.
    void savePopulationIntoDirectory(const std::string& directory_name);

    /// @brief Get directory in which run data is being stored.
    std::string getMainDirectory() const;

    /// @brief Get directory in which models data is being stored.
    std::string getModelsDirectory() const;

};

#endif
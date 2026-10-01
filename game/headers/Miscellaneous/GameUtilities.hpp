/**
 * @file Utilities.hpp
 * @author Bartosz Han
 * @brief This file contains various utility and helper functions,
 * used in various other componenets of the game.
 *
 */

#ifndef GAME_UTILITIES_HPP
#define GAME_UTILITIES_HPP

#include <random>
#include <string>
#include <stdexcept>
#include <cassert>
#include <algorithm>

#include "Utilities.hpp"

/**
 * @brief Generates a vector of n ascending integers starting from a given value.
 *
 * @param n The number of integers to generate.
 * @param starting_value The value to start from (default is 1).
 * @return A vector containing n ascending integers.
 */
inline std::vector<int> generateAscendingIntVector(const int n, const int starting_value=1){
    std::vector<int> vector;
    for(int i=starting_value; i<starting_value+n; ++i){
        vector.push_back(i);
    }
    return vector;
}

/**
 * @brief Generates a vector of n descending integers starting from a given value.
 *
 * @param n The number of integers to generate.
 * @param starting_value The value to start from.
 * @return A vector containing n descending integers.
 */
inline std::vector<int> generateDescendingIntVector(const int n, const int starting_value){
    std::vector<int> vector;
    for(int i=starting_value; i>starting_value-n; --i){
        vector.push_back(i);
    }
    return vector;
}


/**
 * @brief Generates a vector of incrementing strings with a given prefix, suffix, and starting value.
 *
 * The number will be from [starting_value; starting_value+n_next_values], meaning that there will be
 * n_next_values - start_value + 2 numbers.
 *
 * For example: generateIncrementingStrings("A", 3, "_B", 1) yields {"A1_B", "A2_B", "A3_B", "A4_B"}.
 *
 * @param prefix The prefix for each string.
 * @param n The number of strings to generate.
 * @param suffix The suffix for each string (default is empty).
 * @param starting_value The starting integer value (default is 0).
 * @return A vector of incrementing strings.
 */
inline std::vector<std::string> generateIncrementingStrings(const std::string prefix, const int n_next_values,
    const std::string suffix = "", const int starting_value = 0){

    std::vector<std::string> names;
    for (int i=starting_value; i<=n_next_values+starting_value; ++i){
        std::string name = prefix + std::to_string(i) + suffix;
        names.push_back(name);
    }
    return names;
};

#endif
/**
 * @file Utilities.hpp
 * @author Bartosz Han
 * @brief This file contains various utility and helper functions,
 * used in various other componenets of programs.
 *
 */


#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <random>
#include <stdexcept>
#include <cassert>
#include <algorithm>
#include <ctime>

/// @brief Returns a reference to a static random number generator.
inline std::mt19937& getRandomNumberGenerator(){
    static std::mt19937 generator;
    return generator;
}

/**
 * @brief Sets the seed for the static random number generator.
 * The seed can only be set once per program execution.
 * @param new_seed The seed value to use.
 */
inline void setRandomNumberGeneratorSeed(uint32_t new_seed){
    static bool is_seed_set = false;
    if(!is_seed_set){
        std::mt19937& generator = getRandomNumberGenerator();
        generator.seed(new_seed);
        is_seed_set = true;
    }
}

inline void setRandomNumberGeneratorSeed(){
    std::random_device rd;
    setRandomNumberGeneratorSeed(rd());
}

/**
 * @brief Generates a random integer in the range [min, max].
 *
 * @param min The minimum value.
 * @param max The maximum value.
 * @return A random integer between min and max.
 */
inline int generateRandomIntInRange(int min, int max){
    std::mt19937& generator = getRandomNumberGenerator();
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
};

inline float generateRandomFloatInRange(float min, float max){
    std::mt19937& generator = getRandomNumberGenerator();
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
};

inline float generateRandomGaussianFloat(float mean=0, float stddev=1){
    std::mt19937& generator = getRandomNumberGenerator();
    std::normal_distribution<float> distribution(mean, stddev);
    return distribution(generator);
};

/**
 * @brief Randomly selects n unique elements from a container.
 *
 * @tparam Container The container type (must support .size() and .at()).
 * @param container The container to select elements from.
 * @param n The number of elements to select.
 * @return A vector containing n randomly chosen elements from the container.
 */
template <typename Container>
inline std::vector<typename Container::value_type> chooseNFromContainer(const Container& container, const unsigned int n){

    if (n > container.size()){
        throw std::invalid_argument("n cannot be greater than size of container");
    };

    std::vector<int> indexes(container.size());
    for (std::size_t i = 0; i < container.size(); ++i){
        indexes.at(i) = i;
    };

    std::mt19937& generator = getRandomNumberGenerator();
    std::shuffle(indexes.begin(), indexes.end(), generator);

    std::vector<typename Container::value_type> elements;
    for (unsigned int i=0; i<n; ++i){
        elements.push_back(container.at(indexes.at(i)));
    }

    return elements;

}

/**
 * @brief Randomly selects a single element from a container.
 *
 * @tparam Container The container type (must support .size() and .at()).
 * @param container The container to select an element from.
 *
 * @return A reference to a randomly chosen element from the container.
 */
template <typename Container>
inline typename Container::value_type& chooseFromContainer(Container& container){

    if(container.empty()){
        throw std::invalid_argument("Tried to get element of empty container");
    }

    return container.at(generateRandomIntInRange(0, container.size()-1));
}

template <typename Container, typename Condition>
inline std::vector<std::reference_wrapper<typename Container::value_type>>
    getElementsMeetingCondition(Container& container, Condition condition){

    std::vector<std::reference_wrapper<typename Container::value_type>> result;

    for(auto& element: container){
        if (condition(element)){
            result.push_back(element);
        }
    }

    return result;
}

/**
 * @brief Randomly selects a single element from a container using a custom distribution.
 *
 * Each element's probability of being chosen is proportional to
 * its corresponding value in the distribution container.
 *
 * @tparam Container The container type (must support .size() and operator[]).
 * @tparam IntContainer The container type for the distribution
 * (must support .size() and operator[]).
 * @param container The container to select an element from.
 * @param distribution The distribution weights for each element.
 *
 * @return A reference to the randomly chosen element.
 */
template <typename Container, typename IntContainer>
inline const typename Container::value_type& chooseFromContainerWithDistribution(
    const Container& container, const IntContainer& distribution){

    if(container.empty()){
        throw std::invalid_argument("Tried to get element of empty container");
    }

    if (container.size() != distribution.size()){
        throw std::invalid_argument("Container and distribution sizes must be the same");
    }

    int sum = std::accumulate(distribution.begin(), distribution.end(), 0);
    if (sum == 0){
        throw std::invalid_argument("At least one element in distribution must not be 0");
    }

    int number_chosen = generateRandomIntInRange(0, sum - 1);
    int current_sum = 0;

    for (std::size_t i = 0; i < container.size(); ++i){
        current_sum += distribution[i];
        if (number_chosen < current_sum){
            return container[i];
        }
    }

    assert(false && "This point should never be reached");

}

/**
 * @brief Selects n unique random integers from a given range [begin, end].
 *
 * @param begin The starting value of the range.
 * @param end The ending value of the range.
 * @param n The number of unique integers to select.
 *
 * @return A vector containing n unique random integers from the range.
 *
 */
inline std::vector<int> chooseNNumbersFromRange(const int begin, const int end, const int n){
    std::vector<int> all_numbers;
    for( int i=begin; i<=end; ++i){
        all_numbers.push_back(i);
    }
    return chooseNFromContainer(all_numbers, n);
}

/**
 * @brief Randomly selects n unique elements from a container using a custom distribution.
 *
 * Each element's probability of being chosen is proportional to its corresponding value in the distribution container.
 * Once an element is chosen, it cannot be selected again.
 *
 * @tparam Container The container type (must support .size() and .at()).
 * @tparam IntContainer The container type for the distribution (must support .size() and .at()).
 * @param container The container to select elements from.
 * @param n The number of unique elements to select.
 * @param distribution The distribution weights for each element.
 *
 * @return A vector containing n unique randomly chosen elements from the container.
 *
 */
template <typename Container, typename IntContainer>
inline std::vector<typename Container::value_type> chooseNUniqueElementsFromContainerWithDistribution(
    Container& container, const size_t n, const IntContainer& distribution){

    if (n > container.size()){
        throw std::invalid_argument("n cannot be greater than size of container");
    }
    if (container.size() != distribution.size()){
        throw std::invalid_argument("Container and distribution sizes must be the same");
    }
    int sum = std::accumulate(distribution.begin(), distribution.end(), 0);
    if (sum == 0){
        std::invalid_argument("At least one element in distribution must not be 0");
    }

    std::vector<typename Container::value_type> chosen_elements;
    std::vector<bool> chosen_flags(container.size(), false);

    for(size_t i=0; i<n; ++i){
        int number_chosen = generateRandomIntInRange(0, sum - 1);
        int current_sum = 0;
        for (size_t j=0; j<container.size(); ++j){
            if (chosen_flags[j]){
                continue;
            }
            current_sum += distribution.at(j);
            if (number_chosen < current_sum){
                chosen_elements.push_back(container.at(j));
                chosen_flags.at(j) = true;
                sum -= distribution.at(j);
                break;
            }
        }
    }
    return chosen_elements;
}


/**
 * @brief Converts the elements of a container to a single string, separated by a specified delimiter.
 *
 * @tparam Container The type of the container whose elements will be joined into a string.
 * @param container The container whose elements are to be joined.
 * @param separator The string to insert between elements.
 */
template <typename Container>
inline std::string stringFromContainer(const Container& container, const std::string& separator = ","){

    if (container.empty()){
        return "";
    }

    std::string result;
    for (const auto& element : container){
        result += element + separator;
    }

    result.erase(result.size() - separator.size());

    return result;
}

/**
 * @brief Converts a delimited string into a vector of floats.
 *
 * @param string The input string containing float values separated by the separator.
 * @param separator The delimiter used to split the string.
 */
inline std::vector<float> containerFromString(const std::string& string, const std::string& separator = ","){

    std::vector<float> result;
    std::size_t start = 0;
    std::size_t end = string.find(separator);

    while (end != std::string::npos){
        result.push_back(std::stof(string.substr(start, end - start)));
        start = end + separator.size();
        end = string.find(separator, start);
    }

    result.push_back(std::stof(string.substr(start, end)));
    return result;

}

/**
 * @brief generates a random number, and returns true, it is less than provided threshold.
 *
 * @param threshold The probability threshold.

 */
inline bool roll(const float threshold){
    return generateRandomFloatInRange(0.0f, 1.0f) < threshold;
}



/**
 * @brief Flattens a two-dimensional vector into a one-dimensional vector.
 *
 * @tparam T The type of elements stored in the vectors.
 * @param vector The two-dimensional vector to flatten.
 */
template<typename T>
inline std::vector<T> flatten(const std::vector<std::vector<T>>& vector){
    std::vector<T> flat_vector;
    for(const auto& sub_vector: vector){
        flat_vector.insert(flat_vector.end(), sub_vector.begin(), sub_vector.end());
    }
    return flat_vector;
}

#endif
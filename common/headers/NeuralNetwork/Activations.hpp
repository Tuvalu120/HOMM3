/**
 * @file Activations.hpp
 * @author Bartosz Han
 * @brief This file contains data about activation functions used in neural networks.
 *
 */

#ifndef ACTIVATIONS_HPP
#define ACTIVATIONS_HPP

#include <algorithm>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <string>


/**
 * @namespace activations
 * @brief This namespace contains activation functions.
 */
namespace activations{

    inline float sigmoid(const float value){
        return 1.0f / (1.0f + std::exp(1.0f*-value));
    }

    inline float ReLU(const float value){
        return std::max(0.0f, value);
    }

    inline float x(const float value){
        return value;
    }

};

/**
 * @enum ActivationType
 * @brief Enum to represent activation functions.
 */
enum ActivationType{
    SIGMOID,
    RELU,
    X
};

/**
 * @brief Get the activation function object from enumeration
 *
 * @param type ActivationType value of function.
 */
inline std::function<float(const float)> getActivation(ActivationType type){
    switch (type) {
        case ActivationType::SIGMOID: return activations::sigmoid;
        case ActivationType::RELU: return activations::ReLU;
        case ActivationType::X: return activations::x;
        default: throw std::invalid_argument("Unknown activation function type");
    }
}

/**
 * @brief Get the ActivationType of fuction, from name (string) of function.
 *
 * @param key Name (string) of function.
 */
inline ActivationType getActivationType(const std::string& key){
    if (key == "SIGMOID") return ActivationType::SIGMOID;
    else if (key == "RELU") return ActivationType::RELU;
    else if (key == "X") return ActivationType::X;
    else throw std::invalid_argument("Unknown activation function name: " + key);
}

/**
 * @brief Get the activation function Name from ActivationType
 *
 * @param type ActivationType of activation function
 */
inline std::string getActivationName(ActivationType type){
    switch (type) {
        case ActivationType::SIGMOID: return "SIGMOID";
        case ActivationType::RELU: return "RELU";
        case ActivationType::X: return "X";
        default: throw std::invalid_argument("Unknown activation function type");
    }
}

#endif
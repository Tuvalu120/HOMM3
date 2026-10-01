/**
 * @file Exceptions.hpp
 * @author Bartosz Han
 * @brief This file contains custom exceptions used in programs.
 *
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

/**
 * @class InvalidIDError
 * @brief Exception thrown when tried to get some type of object, which is using some kind of type list or unique ID for
 * different types of this object, indicating that object of such ID doesn't or cannot exist.
 *
 * @param object_type A string describing the object type of invalid ID.
 * @param id ID of invalid object.
 *
 */
class InvalidIDError: public std::runtime_error{
public:
    InvalidIDError(const std::string object_type, const int id):
            std::runtime_error(object_type + " with ID " + std::to_string(id) + " does not exist"){};
    };

#endif
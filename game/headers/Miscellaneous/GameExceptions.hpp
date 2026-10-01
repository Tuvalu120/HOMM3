/**
 * @file GameExceptions.hpp
 * @author Bartosz Han
 * @brief This file contains various custom exceptions used in game.
 *
 */
#ifndef GAME_EXCEPTIONS_HPP
#define GAME_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>
#include "Exceptions.hpp"

/**
 * @class InvalidStackError
 * @brief Exception thrown when tried to access stack, which shouldn't be accessed in
 * current circumstances, or when tried to accesss to troop which does not exist.
 *
 * @param stack_description A string describing the invalid stack.
 * @param description Optional additional information about the error.
 *
 */
class InvalidStackError: public std::runtime_error{
public:
    explicit InvalidStackError(const std::string stack_description, const std::string& description = ""):
        std::runtime_error(stack_description + " " + description){};
};

/**
 * @class InvalidBattleObjectError
 * @brief Exception thrown when tried to access battle object, which shouldn't be accessed in
 * current circumstances, or when tried to accesss to battle object which does not exist.
 *
 * @param battle_object_description A string describing the invalid battle object.
 * @param description Optional additional information about the error.
 *
 */
class InvalidBattleObjectError: public std::runtime_error{
public:
    explicit InvalidBattleObjectError(const std::string battle_object_description, const std::string& description = ""):
            std::runtime_error(battle_object_description + " " + description){};
    };

/**
 * @class UnknownMapCharError
 * @brief Exception thrown when, during building map from data from file, program cannot decode char representing tile,
 * or there is no valid logic to do when encountering such char.
 *
 * Every instance of TileMap class has is own sets of valid chars.
 *
 * @param unknown_char Invalid char representing tile in map.
 * @param file Optional file path to the text file with invalid data/
 *
 */
class UnknownMapCharError: public std::logic_error{
public:
    explicit UnknownMapCharError(const char unknown_char, const std::string& file = ""):
        std::logic_error("Invalid char representing map tile '" + std::string(1, unknown_char) + "' in file " + file){};
};

/**
 * @class MapStructureError
 * @brief General exception thrown during building map from data from file, or when generating objects on it,
 * indicating that map does not have required properties, which required in file describing tile map.
 *
 * For example: tile map is to small to generate all required objects into the map.
 *
 * @param description String description of the error.
 *
 */
class MapStructureError: public std::logic_error{
public:
    explicit MapStructureError(const std::string& description):
        std::logic_error(description){};
};

/**
 * @class BattleLogicError
 * @brief General exception thrown during battles, when something unexpected happened during one of them,
 * which shouldn't ever happen, and which cannot be handled properly.
 *
 * @param description String description of the error.
 *
 */
class BattleLogicError: public std::logic_error{
public:
    explicit BattleLogicError(const std::string& description):
        std::logic_error(description){};
};

/**
 * @class TownLogicError
 * @brief General exception thrown when something unexpected happened regarding towns and getting access to them.
 *
 * @param description String description of the error.
 *
 */
class TownLogicError: public std::logic_error{
public:
    explicit TownLogicError(const std::string& description):
        std::logic_error(description){};
};

/**
 * @class WorldLogicError
 * @brief General exception thrown when something unexpected happened during taking turns in world map,
 * which shouldn't ever happen, and which cannot be handled properly.
 *
 * @param description String description of the error.
 *
 */
class WorldLogicError: public std::logic_error{
public:
    explicit WorldLogicError(const std::string& description):
        std::logic_error(description){};
};

/**
 * @class DecisionLogicError
 * @brief General exception thrown when something unexpected happened while working with DecisionManager,
 * which shouldn't ever happen, and which cannot be handled properly.
 *
 * @param description String description of the error.
 *
 */
class DecisionLogicError: public std::logic_error{
public:
    explicit DecisionLogicError(const std::string& description):
        std::logic_error(description){};
};

/**
 * @class UninitializedObjectError
 * @brief Exception thrown when tried to access object, which does not exist, or
 * shouldn't be accessed yet.
 *
 * @param object_name Name the object, or class of the object.
 *
 */
class UninitializedObjectError: public std::logic_error{
public:
    explicit UninitializedObjectError(const std::string object_name):
        std::logic_error("Tried to get access to uninitialized " + object_name + " object"){};
};

/**
 * @class RemovedObjectError
 * @brief Exception thrown when tried to access object, which before accessing was removed from
 * resources, and now cannot be accessed anymore.
 *
 * @param object_name Name the object, or class of the object.
 *
 */
class RemovedObjectError: public std::logic_error{
    public:
        explicit RemovedObjectError(const std::string object_name):
            std::logic_error("Tried to get access to removed " + object_name + " object"){};
    };

/**
 * @class TextureNotFoundError
 * @brief Exception thrown when trying to access texture on provided
 * file path, which does not exists.
 *
 * @param file_path Invalid file path to the texture.
 */
class TextureNotFoundError: public std::runtime_error{
public:
    explicit TextureNotFoundError(const std::string& file_path):
        std::runtime_error("Texture not found: " + file_path){};
};

/**
 * @class TileLogicError
 * @brief General exception thrown, when some unexpected behaviour happened with logic of the Tile object,
 * which shouldn't ever happen, and which cannot be handled properly.
 *
 * @param description String description of the error.
 *
 */
class TileLogicError: public std::logic_error{
public:
    explicit TileLogicError(const std::string& description):
        std::logic_error(description){};
};

/**
 * @bclass InvalidPlayerIDError
 * @brief Exception thrown when tried to access or assign invalid player ID to a object.
 *
 * This exception can mean that index of player ID of out of range of possible indexes, but also
 * can indicate that object cannot have such player ID, despite the fact that other object can have it (for example,
 * some objects cannot be associated with neutral player with index of -1).
 *
 * @param object_name Name of the object or class, which cannot have such ID.
 * @param invalid_id Invalid player Id of object.
 */
class InvalidPlayerIDError: public std::invalid_argument{
public:
    explicit InvalidPlayerIDError(const std::string& object_name, const int invalid_id):
        std::invalid_argument("Player ID of " + std::to_string(invalid_id) + " is not a valid Player ID for object: " + object_name){};
};

#endif
/**
 * @file Connection.hpp
 * @author Bartosz Han
 * @brief This file contains Connection class, which represents connection in neural network.
 *
 */

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <ostream>
#include <utility>

#include "Utilities.hpp"

/**
 * @struct Connection
 * @brief This class represents connection in neural network.
 * Connection contains information about: Id of connection,
 * input and output node of connection, weight of connection and
 * flag, representing information about this connection beging enabled.
 */
struct Connection{
    unsigned int id;
    unsigned int in_node;
    unsigned int out_node;
    float weight;
    bool is_enabled=true;

    bool operator==(const Connection& other) const{
        return id == other.id;
    };

    bool operator==(const unsigned int& other_id) const{
        return this->id == other_id;
    };

    bool operator==(const std::pair<unsigned int, unsigned int>& nodes) const{
        return in_node == nodes.first && out_node == nodes.second;
    };

    bool operator<(const Connection& other) const{
        return id < other.id;
    };

    friend std::ostream& operator<<(std::ostream& os, const Connection& connection);

    /**
     * @brief Create Connection from string describing it.
     */
    static Connection fromString(std::string& connection_data_string){
        std::vector<float> data = containerFromString(connection_data_string);
        if (data.size() < 5){
            throw std::invalid_argument("Invalid Connection string");
        };
        return Connection{
            static_cast<unsigned int>(data[0]),
            static_cast<unsigned int>(data[1]),
            static_cast<unsigned int>(data[2]),
            data[3],
            static_cast<bool>(static_cast<int>(data[4]))
        };
    };
};

inline std::ostream& operator<<(std::ostream& os, const Connection& connection){
    os << connection.id << "," << connection.in_node << "," << connection.out_node << "," <<
    connection.weight << "," << static_cast<int>(connection.is_enabled);
    return os;
}

#endif

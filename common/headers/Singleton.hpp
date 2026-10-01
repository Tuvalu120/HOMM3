/**
 * @file Singleton.hpp
 * @author Bartosz Han
 * @brief This file contains Singleton template,
 * which implements Singleton design pattern.
 */

#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * @class Singleton
 * @brief Generic singleton base class template.
 */
template <typename T>
class Singleton{

protected:
    Singleton() = default;

public:

    /// @brief Returns the singleton instance of the class.
    static T& getInstance(){
        static T instance;
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

};

#endif
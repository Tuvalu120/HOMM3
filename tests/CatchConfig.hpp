/**
 * @file CatchConfig.hpp
 * @author Bartosz Han
 * @brief This file contains configuration for tests.
 *
 */

#ifndef CATCH_CONFIG_HPP
#define CATCH_CONFIG_HPP

#ifdef DEFINE_CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_RUNNER
#endif

#include "include/catch.hpp"

inline std::string executable_path_;
inline int argc_;
inline char** argv_;

#endif
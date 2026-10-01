/**
 * @file TestDrawableObject.cc
 * @author Bartosz Han
 * @brief Tile file contains tests for class DrawableObject.
 *
 */

#include "CatchConfig.hpp"

#include "GameObjects/BaseObjects/DrawableObject.h"

TEST_CASE("DrawableObject Constructor Test", "[DrawableObject]"){
    DrawableObject obj("initial_texture");
    REQUIRE(obj.getCurrentTextureName() == "initial_texture");
}

TEST_CASE("DrawableObject setCurrentTextureName Test", "[DrawableObject]"){
    DrawableObject obj("initial_texture");
    obj.setCurrentTextureName("new_texture");
    REQUIRE(obj.getCurrentTextureName() == "new_texture");
}

TEST_CASE("DrawableObject getCurrentTextureName Test", "[DrawableObject]"){
    DrawableObject obj("initial_texture");
    REQUIRE(obj.getCurrentTextureName() == "initial_texture");
    obj.setCurrentTextureName("new_texture");
    REQUIRE(obj.getCurrentTextureName() == "new_texture");
}

TEST_CASE("DrawableObject setCurrentTextureName with empty string", "[DrawableObject]"){
    DrawableObject obj("initial_texture");
    obj.setCurrentTextureName("");
    REQUIRE(obj.getCurrentTextureName() == "");
}

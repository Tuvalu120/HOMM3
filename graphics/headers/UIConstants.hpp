/**
 * @file UIConstants.hpp
 * @author Bartosz Han
 * @brief This file contains ui_constants namespace, which contains data about various constants related to GUI and displaying it.
 *
 */

#ifndef UI_CONSTANTS_H
#define UI_CONSTANTS_H


#include <string>
#include <array>
#include <QColor>


#include "Data/ObjectDataEnums.hpp"
#include "Data/GameData.hpp"

/**
 * @namespace ui_constants
 * @brief This namespace is for constants related to displaying game with GUI
 *
 * @note All values regarding sizes are in pixels.
 *
 */
namespace ui_constants{

    const int MAIN_WINDOW_WIDTH = 1311;
    const int MAIN_WINDOW_HEIGHT = 830;

    const std::string GAME_TEXT_MAIN_FONT = "Times New Roman";

    /// @brief Font size of the text to display into the tiles.
    constexpr int TILE_TEXT_SIZE = 12;

    /// @brief Size in pixels of square indicator of battle object's owner image.
    constexpr int BATTLE_OBJECT_PLAYER_INDICATOR_SQUARE_SIZE = 8;

    /// @brief Size of QPixmaps images being displayed at the final resource array QPixmap. It is also standard height of resource array QPixmap height.
    constexpr int RESOURCE_ARRAY_QPIXMAP_SIZE = 32;

    /// @brief Font is of the text used in resource arrays.
    constexpr int RESOURCE_ARRAY_FONT_HEIGHT = 10;

    /// @brief Text margin while generating resource array QPixmaps.
    constexpr int RESOURCE_ARRAY_MARGIN = 5;

    /// @brief Max number of digits of quantity of single resource in resource array QPixmaps.
    constexpr int RESOURCE_ARRAY_MAX_N_DIGITS = 5;

    /// @brief Width of hero statictic image displayed at hero statistic layout.
    constexpr int HERO_STATISTIC_ICON_WIDTH = 58;

    /// @brief Height of hero statictic image displayed at hero statistic layout.
    constexpr int HERO_STATISTIC_ICON_HEIGHT = 64;

    /// @brief Font size of hero statistic values at hero statictic layout.
    constexpr int HERO_STATISTIC_TEXT_HEIGHT = 20;

    /// @brief Size of single tile at display.
    constexpr int TILE_SIZE = 64;

    /// @brief Aspect ratio of widget in which world map is being displayed.
    constexpr double WORLD_MAP_WINDOW_SIZE_RATIO = 0.875;

    /// @brief Number of battle order objects to display at UI
    constexpr int BATTLE_ORDER_UI_DISPLAY_COUNT = 8;

    /// @brief Number of troop's statictic to display when display troop data.
    constexpr int TROOP_TEXT_DATA_COUNT = 7;

    /// @brief Time between collecting all events and inputs from user by the game.
    constexpr int FRAME_PROCESSING_TIME_MS = 20;

    /// @brief Time between redrawing while game window.
    constexpr int FRAME_REDRAWING_TIME = 50;

    /// @brief Vector of paths to resources icons.
    const std::array<std::string, N_RESOURCES> RESOURCES_IMAGE = { "Icons/ResourceGold.png",
        "Icons/ResourceWood.png", "Icons/ResourceOre.png", "Icons/ResourceCrystal.png", "Icons/ResourceJewel.png"};

    const std::string MOVES_LEFT_ICON = "Icons/IconMovesLeft.png";

    /// @brief Number of widgets of battle statistic layout.
    const int BATTLE_STAT_TEXT_LAYOUT_SIZE = std::max(game_data::TROOP_DISPLAY_STAT.size(), game_data::HERO_DISPLAY_STAT.size()) + 1; // "+ 1" for battle object name.
    const std::string TOWN_TROOP_AVAILABLE_INFO = "Ilość jednostek w mieście: ";
    const std::string TOWN_TROOP_CAN_BUY_BUTTON_TEXT = "Kup jednostkę: ";
    const std::string TOWN_TROOP_CANNOT_BUY_BUTTON_TEXT = "Nie można kupić tej jednostki";
    const std::string TOWN_HERO_CAN_BUY_BUTTON_TEXT = "Kup bohatera: ";
    const std::string TOWN_SPELL_CAN_BUY_BUTTON_TEXT = "Kup zaklęcie: ";
    const std::string TOWN_SPELL_CANNOT_BUY_BUTTON_TEXT = "Nie można kupić tego zaklęcia";
    const std::string CANNOT_DO_BUTTON_TEXT = "Nie można wykonać tej akcji";
    const std::string DECISION_BUTTON_TEXT = "Podejmij tą akcję";
    const std::string TOWN_MAX_LEVEL_OF_BUILDING_TEXT = "Maksymalny poziom budynku - nie można dalej ulepszać";
    const std::string TOWN_BUILDING_BUTTON_TEXT = "Ulepsz ten budynek";

    const std::string FOG_OF_WAR_IMAGE = "Tiles/FogOfWar.png";

    /// @brief Main font size of UI elements.
    const int UI_NAME_FONT_SIZE = 24;

    /// @brief Multiplier by which non-walkable tiles are being darkened during battlefield.
    const float TILE_DARKEN_FACTOR = 0.5f;

    /// @brief Color of pixels on textures in files, which should be replaced when creating QPixmap
    /// by a color of player owning object associated with this texture.
    const QColor TEXTURE_PLAYER_COLOR_PLACEHOLDER(255, 255, 0);

    /// @brief Main color of game, including game's background
    const QColor GAME_MAIN_COLOR(134, 8, 11);

}

#endif
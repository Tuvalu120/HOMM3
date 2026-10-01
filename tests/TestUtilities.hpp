/**
 * @file TestUtilities.hpp
 * @author Bartosz Han
 * @brief This file contains several functions, used in various test cases.
 *
 */
#include <filesystem>
#include <thread>
#include <chrono>

#include "CatchConfig.hpp"
#include "Managers/WorldManager.h"
#include "Managers/GraphicsManager.h"
#include "Managers/TownManager.h"
#include "Managers/PlayerManager.h"
#include "Managers/BattleManager.h"
#include "Managers/DecisionManager.h"
#include "GameObjects/BaseObjects/WorldGeneratedInteractiveObject.h"
#include "GameObjects/WorldObjects/WorldNeutralTroops.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include "AIBattleControllers/NEATBattleController.h"
#include "NEAT.h"


/// @brief Generate path, from which tests are run.
inline std::string getExecutablePath(const char* argv0){
    return std::filesystem::canonical(std::filesystem::path(argv0)).parent_path().string();
}

/// @brief Force game to stop for given amount of milliseconds
inline void pauseGame(int milliseconds){
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

/// @brief Returns ture, if provided tile is on the edge of the world map.
inline bool isTileBorderOfTheWorldMap(Tile& tile){
    return tile.getXPos() == 0 || tile.getXPos() == WorldManager::getInstance().getWorldMap().getTileMapWidth()-1 ||
           tile.getYPos() == 0 || tile.getYPos() == WorldManager::getInstance().getWorldMap().getTileMapHeight()-1;
}

/// @brief Set all tiles, marked as walls to become walkable, by setting their wall flag to false,
/// and by setting their type to grass.
inline void removeWallsFromWorldMap(){
    WorldMap& world_map = WorldManager::getInstance().getWorldMap();
    for(auto& tile: world_map.getTileMap()){
        if(!isTileBorderOfTheWorldMap(*tile)){
            tile->setFlagIsTileWall(false);
            tile->setTileType(GRASS_TILE);
        }
    }
}

/// @brief Mock initializing game, with provided seed.
/// This method does almost the sam,e thing, as GameManager::initialize(), but with slightly more control
/// over what is being generated and how it is being generated.
inline void initializeMockGame(const int seed = -1){

    int rng_seed = seed != -1 ? seed : 0; //Default seed for testing

    std::mt19937& generator = getRandomNumberGenerator();
    generator.seed(rng_seed);

    //BattleManager's reset, for tests purposes, must be first.
    BattleManager::getInstance().resetManager();
    GameManager::getInstance().resetGame();
    std::filesystem::current_path(executable_path_ + "/../..");

    //From GameManager::initialize()

    const NeuralNetwork network = NeuralNetwork::fromFile((game_data::AI_MODEL_FILEPATH));
    PlayerManager::getInstance().setNeutralPlayerController(std::make_shared<NEATBattleController>(network));

    WorldManager::getInstance().createWorldMap();
    removeWallsFromWorldMap();
    TownManager::getInstance().generateTowns(WorldManager::getInstance().getWorldMap().getTownPossibleCoords());

    PlayerManager::getInstance().initializePlayers(constants::MAX_NUMBER_OF_PLAYERS);
    WorldManager::getInstance().initializeGenerators();
    GraphicsManager::getInstance().startDrawing();
    GraphicsManager::getInstance().showWindow();
}

/// @brief Generate std::thread, in which there will be instance of game being played.
/// @return std::thread with game loop initialized within this thread.
inline std::thread playMockGame(int seed = -1){
    initializeMockGame(seed);
    std::thread game_thread([&](){
        GameManager::getInstance().play();
    });
    pauseGame(100);
    return game_thread;
}

/// @brief Set decision vars of player, which is currently taking its battle turn, to given values.
inline void setBattleDecisionVars(int decision_var_1 = -1, int decision_var_2 = -1){
    BattleManager::getInstance().getCurrentBattle().getCurrentPlayer().getPlayerController().setDecisionVars(decision_var_1, decision_var_2);
    pauseGame(100);
}

/// @brief Set decision vars of player, which is currently taking its turn on world map, to given values.
inline void setWorldDecisionVars(int decision_var_1 = -1, int decision_var_2 = -1){
    WorldManager::getInstance().getCurrentPlayer().getPlayerController().setDecisionVars(decision_var_1, decision_var_2);
    pauseGame(100);
}

/// @brief Set decision vars of player, which is currently taking its turn on world map, to given pair of values.
inline void setWorldDecisionVars(const std::pair<int, int>& decision_vars){
    setWorldDecisionVars(decision_vars.first, decision_vars.second);
}


/**
 * @brief Sets up a battle scenario for testing spells.
 *
 * This function prepares a battle between two armies, assigns a spell to the attacking hero,
 * and starts the battle in a separate thread. It then waits until the attacking hero's turn
 * in the battle, ensuring the test is ready for spell-related actions.
 *
 * @param attacker Reference to the attacking WorldArmy.
 * @param defender Reference to the defending WorldArmy.
 * @param spell_type The type of spell to be added to the attacking hero.
 * @param spell_level The level of the spell to be added.
 * @return std::thread The thread running the battle.
 *
 */
inline std::thread setUpBattleForSpellTesting(WorldArmy& attacker, WorldArmy& defender, SpellType spell_type, int spell_level){
    Hero* attacking_hero = attacker.getArmyHeroPtr().get();
    attacker.getArmyHero().addSpell(spell_type, spell_level);

    std::thread battle_thread([&](){
        BattleManager::getInstance().battle(attacker, defender);
    });
    pauseGame(100);

    BattleObject* current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    while (current_battle_object != attacking_hero) {
        setBattleDecisionVars(-2, -2);
        current_battle_object = &BattleManager::getInstance().getCurrentBattle().getCurrentBattleObject();
    }

    REQUIRE(current_battle_object == attacking_hero);
    return battle_thread;
}

/**
 * @brief Moves two troops to positions adjacent to a target location for testing purposes.
 *
 * This function moves one (friendly) troop to the left of the target coordinates (target_x - 1, target_y)
 * and another (enemy) troop to the right of the target coordinates (target_x + 1, target_y) on the battlefield.
 *
 * @param friendly_troop Pointer to the friendly Troop object to be moved.
 * @param enemy_troop Pointer to the enemy Troop object to be moved.
 * @param target_x The x-coordinate of the target location.
 * @param target_y The y-coordinate of the target location.
 *
 * This method is used for testing AreaOfEffect spells.
 */
inline void moveTroopsForTest(Stack* friendly_troop, Stack* enemy_troop, int target_x, int target_y){
    BattleManager::getInstance().getCurrentBattle().getBattlefield().moveObject(*friendly_troop, target_x - 1, target_y);
    BattleManager::getInstance().getCurrentBattle().getBattlefield().moveObject(*enemy_troop, target_x + 1, target_y);
}


/**
 * @brief Moves the attacker and defender armies to positions adjacent to the target coordinates for testing purposes.
 *
 * This function moves the attacker army to (target_x - 1, target_y) and the defender army to (target_x + 1, target_y)
 * on the world map.
 * It is used for test cases where two armies are positioned next to a specific location.
 *
 * @param attacker Reference to the attacking WorldArmy object to be moved.
 * @param defender Reference to the defending WorldArmy object to be moved.
 * @param target_x The x-coordinate of the target position.
 * @param target_y The y-coordinate of the target position.
 */
inline void moveArmiesForTest(WorldArmy& attacker, WorldArmy& defender, int target_x, int target_y){
    WorldManager::getInstance().getWorldMap().moveObject(attacker, target_x - 1, target_y);
    WorldManager::getInstance().getWorldMap().moveObject(defender, target_x + 1, target_y);
}

/**
 * @brief End battle, by safely removing troops from army opposite of winning_player_id,
 * and setting decision vars, to end battle.
 *
 *  BattleManager::battle() is a function which works in loop and waits for user input.
 *  This causes problems in tests, especially because battle() must be called in different thread.
 *  This function is to be called at the end of each battle test, so that each battle will end in proper way, and other tests can be performed.
 *
 * This method should not be used when no battle is happening at the time of calling this function.
 *
 * @param winning_player_id ID of player which should win current battle.
 */
inline void endBattle(int winning_player_id){

    for (const auto& troop : BattleManager::getInstance().getCurrentBattle().getOppositeArmy(winning_player_id).getArmyStacks()){
        troop->setFlagDestroyAfterInteraction();
    }

    setBattleDecisionVars(-2, -2);
}

/**
 * @brief End battle, by safely removing troops from army opposite of winning_player_id, and setting decision vars, to end battle.
 *
 * This will is taking thread with initialized battle as an argument and is calling .join() on this thread, after ending battle.
 *
 * @param battle_thread
 * @param winning_player_id
 */
inline void endBattleThread(std::thread& battle_thread, int winning_player_id){

    endBattle(winning_player_id);
    if(DecisionManager::getInstance().isDecisionInProgress()){
        setWorldDecisionVars(0);
    }
    battle_thread.join();

}

/// @brief set decision vars to end turn of current army.
inline void mockEndTurnOfCurrentWorldArmy(){
    setWorldDecisionVars(-2, -2);
}

/// @brief Set position of provided WorldGeneratedInteractiveObject to provided coordinates.
inline void setPositionOfWorldGeneratedInteractiveObject(std::shared_ptr<WorldGeneratedInteractiveObject> object, int x_pos, int y_pos){
    object->setPos({x_pos, y_pos});
    WorldManager::getInstance().getWorldMap().getTileAt(x_pos, y_pos).addObjectOnTile(object);
}

/**
 * @brief End game and game thread, by safely removing all players, except current one (which is taking turns on world map)
 * and setting decision vars, to end turn of player, and ending game.
 *
 *  This game is working in such way that it waits in loop and waits for user input.
 *  This causes problems in tests, especially because GameManager::play() must be called in different thread.
 *  This function is to be called at the end of each test with needs game thread to work,
 *  so that each game will end in proper way, and other tests can be performed.
 *
 * this method also calls join() on provided game_thread.
 */
inline void endGame(std::thread& game_thread){
    int current_player_id = WorldManager::getInstance().getCurrentPlayerID();
    for(const int player : PlayerManager::getInstance().getRemainingPlayers()){
        if(player != current_player_id){
                PlayerManager::getInstance().removePlayerFromGame(player);
        }
    }
    if(DecisionManager::getInstance().isDecisionInProgress()){
        setWorldDecisionVars(0);
    };
    setWorldDecisionVars(-2, -2);
    game_thread.join();
}

/// @brief Mock entering to friendly town. Army must be at its initial position, and must be currently taking world map turns to make this function work.
inline std::pair<int, int> enterFriendlyTown(){

    WorldArmy& world_army_1 = WorldManager::getInstance().getCurrentPlayer().getArmyByID(0);

    int army_x_pos = world_army_1.getXPos();
    int army_y_pos = world_army_1.getYPos();

    setWorldDecisionVars(army_x_pos, army_y_pos+1);

    return {army_x_pos, army_y_pos};
}

/// @brief give player 9999 of each resoure available in game.
inline void givePlayerMaxResources(Player& player){
    for(int i=0; i<N_RESOURCES; ++i){
        player.addResources(static_cast<Resource>(i), 99999);
    }
}

/// @brief Generate <max_level+1-element< vector of strings, each of mocking texture of building.
inline std::vector<std::string> getMockBuildingSprites(const int max_level){
    std::vector<std::string> building_sprites;
    for(int i=0; i<=max_level; ++i){
        building_sprites.push_back("sprite_" + std::to_string(i));
    }
    return building_sprites;
}

/// @brief Generate town, which belongs to given player, is type of town_type and its location is at given coordinates.
inline std::unique_ptr<Town> generateMockTown(const int player_id, const TownType town_type, int town_x_pos, int town_y_pos){
    TownData town_data = game_data::getTownData(town_type);
    auto town = std::make_unique<Town>(town_data, town_type, 0, town_x_pos, town_y_pos);
    town->setPlayerID(player_id);
    return town;
}

/// @brief Calculate power level of provided Army&.
/// Army level is used in building new army, but it is not stored anywhere. this method allows to calculate level of provided army.
inline int calculateArmyLevel(const Army& army){
    int army_power = army.getArmyPower();
    int army_level = 0;
    for( const int& limit: game_data::PATTERN_ARMY_POWER){
        if(army_power > limit){
            ++army_level;
        }
        else{
            break;
        }
    }
    return army_level;
}


/// @brief Skip each player turn until there is new day in game.
inline void skipDay(){
    int current_day = WorldManager::getInstance().getCurrentDay();
    while(current_day == WorldManager::getInstance().getCurrentDay()){
        WorldManager::getInstance().endTurnOfArmy();
    }
}

/// @brief Skip each player turn until there is new week in game.
inline void skipWeek(){
    int current_week = WorldManager::getInstance().getCurrentWeek();
    while(current_week == WorldManager::getInstance().getCurrentWeek()){
        skipDay();
    }
}

/// @brief Find any object of provided ObjectType on world map, and return its coordinates as pair of integers.
/// this method iterates through all tiles, and return coordinates of first tile with has object with provided object type.
inline std::pair<int, int> findAnyObjectOfType(ObjectType object_type){
    for(const auto& tile: WorldManager::getInstance().getWorldMap().getTileMap()){
        if(tile->hasInteractiveObject() && tile->getObjectOnTile().getInteractiveObjectType() == object_type){
            return {tile->getXPos(), tile->getYPos()};
        }
    }
    throw std::runtime_error("No object of type " + std::to_string(object_type) + " found on the map");
}

/// @brief Get TownType, which is associated with provided TroopType.
/// Each town has army, which can consist of troops of only one troop family.
/// This method allows to get TownType of town, in which given Troop might be present as troop in town army.
inline TownType getTroopFamilyTypeFromTroop(const Stack& stack){
    TroopType troop_type = stack.getTroopType();
    for(const auto& troop_family: game_data::TROOP_FAMILY_TYPES){
        if(std::find(game_data::TROOP_TYPES.at(troop_family).begin(), game_data::TROOP_TYPES.at(troop_family).end(), troop_type) != game_data::TROOP_TYPES.at(troop_family).end()){
            return troop_family;
        }
    }
    throw std::runtime_error("Troop type " + std::to_string(troop_type) + " not found in any troop family");
}


/// @brief Mock making Decision, when making decision with DecisionManager.
inline void makeDecision(DecisionOptionType decision_option_type){
    auto& decision_options = DecisionManager::getInstance().getCurrentDecision().getDecisionOptions();
    for(size_t i=0; i<decision_options.size(); ++i){
        if(decision_options.at(i)->getDecisionOptionType() == decision_option_type){
            setWorldDecisionVars(i);
            return;
        }
    }
    throw std::runtime_error("Decision option type " + std::to_string(decision_option_type) + " not found in current decision");
}

/// @brief Add new WorldNeutralTroops at provided coordinates on world map, and set up their army.
inline std::shared_ptr<WorldNeutralTroops> setUpWorldNeutralTroops(int troops_level, int x_pos, int y_pos){
    std::shared_ptr<WorldNeutralTroops> neutral_troops = std::make_shared<WorldNeutralTroops>();
    neutral_troops->setUpArmy(troops_level);
    REQUIRE(neutral_troops.use_count() == 1);
    setPositionOfWorldGeneratedInteractiveObject(neutral_troops, x_pos, y_pos);
    REQUIRE(neutral_troops.use_count() == 2);

    return neutral_troops;

}


/**
 * @brief Sets up a WorldGeneratedInteractiveObject at the specified position and moves the player's army nearby.
 *
 * This function positions the given interactive object at (x_pos, y_pos) on the world map,
 * verifies that the object is correctly placed, and then moves army, which is currently taking tis turns,
 * to the tile at (x_pos + 1, y_pos + 1).
 *
 * @param object Pointer to the WorldGeneratedInteractiveObject to be placed.
 * @param x_pos The x-coordinate where the object should be placed.
 * @param y_pos The y-coordinate where the object should be placed.
 * @return std::pair<int, int> The new position of the army after moving, as (x_pos + 1, y_pos).
 */
inline std::pair<int, int> setUpWorldGeneratedInteractiveObject(std::shared_ptr<WorldGeneratedInteractiveObject> object, int x_pos, int y_pos){
    setPositionOfWorldGeneratedInteractiveObject(object, x_pos, y_pos);
    REQUIRE(WorldManager::getInstance().getWorldMap().getTileAt(x_pos, y_pos).hasInteractiveObject() == true);
    WorldArmy& army = PlayerManager::getInstance().getPlayerByID(0).getArmyByID(0);

    WorldManager::getInstance().getWorldMap().moveObject(army, x_pos+1, y_pos+1);
    REQUIRE(DecisionManager::getInstance().isDecisionInProgress() == false);

    return {x_pos+1, y_pos};
}


/**
 * @brief Adds experience points to the given hero, simulating decision making during level-ups.
 *
 * This function adds the specified total experience points to the provided hero.
 * While experience is being added, a separate thread simulates decision making
 * by repeatedly calling makeDecision() until all experience points have been processed, in order
 * to safely add all needed experience points to the player, handling multiple level-ups if necessary.
 *
 * @param army_hero Hero& to which experience points will be added.
 * @param total_exp_points The total number of experience points to add to the hero.
 */
inline void mockAddExperiencePoints(Hero& army_hero, int total_exp_points){

    bool flag_is_adding_experience_finished = false;

    std::thread decision_maker_thread([&]{
        while(!flag_is_adding_experience_finished){
            makeDecision(ADD_HERO_STAT);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    while(total_exp_points > 0){

        int hero_exp_points = army_hero.getHeroExperiencePoints();
        int exp_to_level_up = game_data::EXPERIENCE_POINTS_TO_LEVEL_UP.at(army_hero.getHeroLevel());
        bool will_level_up = hero_exp_points + total_exp_points >= exp_to_level_up;
        int exp_points_to_add = will_level_up ? exp_to_level_up - hero_exp_points : total_exp_points;

        army_hero.addExperiencePoints(exp_points_to_add);
        total_exp_points -= exp_points_to_add;
    }

    flag_is_adding_experience_finished = true;
    decision_maker_thread.join();

}

/// @brief Get number of objects, which are present on world manager, and which have provided ObjectType.
inline int countObjectsOnMap(ObjectType object_type){

     int counter = 0;

    for (const auto& tile: WorldManager::getInstance().getWorldMap().getTileMap()){
        if(tile->hasInteractiveObject() && tile->getObjectOnTile().getInteractiveObjectType() == object_type){
            ++counter;
        }
    }

    return counter;

}

/**
 * @brief Reset NEAT algorithm to the state before running it.
 * In this function, number of in nodes, out nodes and hyperparams of the new run
 * of algorithm can be set.
 */
inline void resetNEAT(int n_in_nodes=1, int n_out_nodes=1, std::string hyperparams_path="Hyperparams.txt"){

    class DummyTrainer : public Trainer{
        void evaluate() override{};
    };

    std::unique_ptr<Trainer> trainer = std::make_unique<DummyTrainer>();
    NEAT::getInstance().registerTrainer(std::move(trainer));
    NEAT::getInstance().loadNodeQuantityInfo(n_in_nodes, n_out_nodes);
    NEAT::getInstance().initialize(executable_path_ + "/../../tests/data/" + hyperparams_path);

}
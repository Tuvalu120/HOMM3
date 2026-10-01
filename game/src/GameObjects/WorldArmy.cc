/**
 * @file WorldArmy.cc
 * @author Bartosz Han
 * @brief This file contains definition of class WorldArmy.
 *
 */



#include "GameObjects/WorldArmy.h"
#include "Managers/BattleManager.h"
#include "Managers/WorldManager.h"

using namespace std;
using namespace game_data;
using namespace constants;

WorldArmy::WorldArmy(shared_ptr<Hero> army_hero, int init_pos_x, int init_pos_y):
    InteractiveObject(army_hero->getCurrentTextureName(), ARMY, init_pos_x, init_pos_y, 3, false),
    HeroArmy(army_hero, init_pos_x, init_pos_y),
    MovableObject(army_hero->getCurrentTextureName(), ARMY, init_pos_x, init_pos_y, army_hero->getStatistic(MAX_MOVES), 3, false)
    {

        setBonusExperiencePoints(EXPERIENCE_BONUS_FROM_DEFEATING_ARMY_WITH_HERO);
}

void WorldArmy::interact(){

    WorldArmy& current_army = WorldManager::getInstance().getCurrentPlayer().getCurrentArmy();
    if(current_army.getPlayerID() != getPlayerID()){
        BattleManager::getInstance().battle(current_army, *this);
    }

}

int WorldArmy::getDiscoveryRadius() const{
    return getArmyHero().getStatistic(DISCOVERY_RADIUS);
}

shared_ptr<WorldArmy> WorldArmy::generateArmy(int player_id, HeroType hero_id, int init_x_pos, int init_y_pos){

    unique_ptr<Hero> hero;

    HeroData hero_init_data = getHeroData(hero_id);
    hero = make_unique<Hero>(hero_id, player_id);
    auto army = make_shared<WorldArmy>(std::move(hero), init_x_pos, init_y_pos);
    army->setPlayerID(player_id);


    for (const auto& troop_data: hero_init_data.init_army){
        army->addTroops(troop_data.first, troop_data.second);
    }

    for (const SpellType& spell: hero_init_data.init_spells){
        army->getArmyHero().addSpell(spell);
    }

    return army;
}

int WorldArmy::getPriority(WorldArmy& army_to_check) const{
    return army_to_check.getPlayerID() == getPlayerID() ? -1 : priority_;
}

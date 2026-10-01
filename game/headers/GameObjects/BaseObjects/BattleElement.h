/**
 * @file BattleElement.h
 * @author Bartosz Han
 * @brief This file contains BattleElement abstract class.
 * This class is used by objects, which are somehow related to the battle,
 * and they need information about it.
 * During battle, Battle object registers itself to all BattleObjects related to it,
 * and later deregisters itself from theses objects.
 *
 */

#ifndef BATTLE_ELEMENT_H
#define BATTLE_ELEMENT_H

class Battle;

class BattleElement{

private:

    Battle* battle_;

public:

    /// @brief Get reference to the battle this object is related to.
    Battle& getBattle() const;

    /// @brief Register Battle object to this object.
    virtual void registerBattle(Battle* battle);

    /// @brief Deregister Battle object from this object.
    virtual void deregisterBattle();
};

#endif
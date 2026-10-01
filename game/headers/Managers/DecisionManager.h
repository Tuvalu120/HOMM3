/**
 * @file DecisionManager.h
 * @author Bartosz Han
 * @brief This file constains DecisionManager class, which is a class for manage situation, when player
 * must make some kind of decision during its turn. It also creates and manages resources needed to perform this decision.
 *
 */
#ifndef DECISION_MANAGER_H
#define DECISION_MANAGER_H

#include "Managers/SubManager.h"
#include "Decisions/Decision.h"
#include "Singleton.hpp"

/**
 * @class DecisionManager
 * @brief This class is for manage situation, when player must make some kind of decision during its turn.
 * It also creates and manages resources needed to perform this decision.
 *
 * Decisions are situations, when player can choose one of several options to perform. Mainly, decisions are made when interacting with
 * objects at world map, but they can also be made in other situations, such as when player levels up.
 *
 * At any moment, there can only be up to one decision to perform. Also, only player, which can perform decision is player, which is currently
 * taking world map turns.
 *
 */
class DecisionManager: public Singleton<DecisionManager>, public SubManager{

    friend class Singleton<DecisionManager>;

private:
    DecisionManager();
    std::unique_ptr<Decision> current_decision_;

public:
    /**
     * @brief Set new current decision and await player to decide, which option to choose.
     *
     * @param decision Decision, which player should make.
     */
    void makeDecision(std::unique_ptr<Decision> decision);

    /**
     * @brief Choose option at given index for the current decision.
     *
     * This method also deletes current decision, making it the end of decision process.
     *
     * @param decision_option_index Index of option to choose for current decision.
     */
    void decide(const int decision_option_index);

    /**
     * @brief Get current decision object.
     *
     * There must be decision currently in progress in order to make this method work,
     * otherwise it throws UninitializedObjectError.
     *
     * @return Decision& reference to the current decision object.
    */
    Decision& getCurrentDecision();

    /**
     * @brief Check if decision is currently in progress.
     *
     * Check is being performed by checking, if current_decision_ is nullptr or not.
     *
     * @return true if decision is in progress (is initialized), false otherwise.
     */
    bool isDecisionInProgress() const;

    void resetManager() override;
};

#endif
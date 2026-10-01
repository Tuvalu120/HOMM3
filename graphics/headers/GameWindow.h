/**
 * @file GameWindow.h
 * @author Bartosz Han
 * @brief This file contains GameWindow data, which is used for displaying and managing game window.
 *
 */

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include <QLayoutItem>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

#include "Data/ObjectDataEnums.hpp"
#include "ui_gamewindow.h"

class Stack;
class BattleObject;
class Town;
class Hero;

/**
 * @class GameWindow
 * @brief This class is used for displaying and managing game window.
 *
 * This class contains pointer to Qt's game window and is used as a interface between manager and window elements.
 * This class also has several method for managing scenes and constins few method which are used in two or more scenes.
 *
 * GameWindow, and all of its components and scenes are only used in GUI version of the game.
 *
 */
class GameWindow : public QMainWindow
{
    Q_OBJECT

private:
    std::unique_ptr<Ui::GameWindow> ui_;


    /**
     * @brief Do the logic related to player choosing spell.
     *
     * This method is connected to all buttons, which are used for selecting spell during battle.
     * This method will also set current player's decision variables to a index of spell associated with button.
     *
     */
    void onSpellButtonClicked();

    /**
     * @brief Do the logic related to player making the decision on buying town's resources.
     *
     * This method is connected to all button, which are used for changing scene while visiting town
     * and selecting, which resources (spells, troops, heroes) player wants to check or buy.
     * This method will set new scene associated with button.
     */
    void onTownResourceBuyingButtonClicked();

    /**
     * @brief Do the logic related to player choosing one of decision options.
     *
     * This method is connected the all buttons, which are used for selecting decision option.
     * This method will set the decision variables of player, who is currently taking world turn, to index of associated decision option.
     */
    void onDecisionOptionButtonClicked();

    /**
     * @brief Connect the button in WorldDecisionMakingScene to a DecisionOption's index in current Decision and associated with this button.
     *
     * When clicked, button will now do action related to connected decision option.
     *
     * This method should be used when building scenes.
     *
     * @param button QPushButton* to connect DecisionOption with.
     * @param decision_option_index Index of DecisionOption in current Decision to connect with the button.
     */
    void prepareDecisionOptionButton(QPushButton* button, int decision_option_index);

    /**
     * @brief Add new empty QLabels to QGridLayout.
     *
     * The QGridLayout will contain QLabels creating rectangle with (width * height) QLabels.
     *
     * @param layout QGridLayout to create new QLabels.
     * @param width Number of QLabels in horizontal line of Layout.
     * @param height Number of QLabels in vertical line of Layout.
     */
    void addLabelsToGridLayout(QGridLayout* const layout, const int width, const int height);

    /**
     * @brief Connect the button in town scene to a TownDecision associated with this button.
     *
     * When clicked, button will now do action related to connected decision.
     *
     * This method should be used when building scenes.
     *
     * @param button QPushButton* to connect town decision with.
     * @param decision TownDecision to connect with the button.
     */
    void prepareTownDecisionButton(QPushButton* button, TownDecision decision);

    /**
     * @brief Do the logic related to player making town decision.
     *
     * This method is connected to all buttons, which are used for making town related decision in town scenes.
     * This method will set current player's decision variables to a value of selected town decision.
     *
     */
    void onTownDecisionButtonClicked();

    /**
     * @brief Do the logic related to the situation when player taking turn on world and want to skip it.
     *
     * This method is connected to button used for skipping turn.
     * This method will set decision variables of player, which is currently taking world turn, to values indicating end of turn.
     *
     */
    void onWorldEndTurnButtonClicked();

    /**
     * @brief Do the logic related to the situation when player taking turn on battlefield and want to skip it.
     *
     * This method is connected to button used for skipping turn.
     * This method will set decision variables of player, which is currently taking battle turn, to values indicating end of turn.
     *
     */
    void onBattleEndTurnButtonClicked();


protected:

    /**
     * @brief Do the logic related to situation when user closes application.
     *
     * This method ensures safely deleteing all resources allocated to the game, and also finishes the game prematurely.
     *
     * @param event
     */
    void closeEvent(QCloseEvent* event) override;

    /**
     * @brief Do the logic related to situation when user presses button.
     *
     * Currently, buttons can only be pressed during exploring world map. It will slightly move displayed chunk of map,
     * by changing values of displayed map offset.
     *
     */
    void keyPressEvent(QKeyEvent* event) override;


public:
    explicit GameWindow(QWidget *parent = nullptr);

    /// @brief Get pointer to Ui::GameWindow object, which constains all GUI componenets generated by Qt.
    Ui::GameWindow* getUI();

    /// @brief Disable layout used for showing list of spells.
    void disableSpellLayout();

    /// @brief Enable layout used for showing list of spells.
    void enableSpellLayout();

    /**
     * @brief Display the data text of BattleObject into layouts.
     *
     * Names of the statistics of battle object will be displayed at battle_object_text_data_names_layout,
     * and values of theses statistics will be displayed at battle_object_text_data_values_layout.
     * Text displayed at theses layouts depends of type of battle object.
     *
     * @param battle_object_text_data_names_layout Layout to display battle object's statistics names.
     * @param battle_object_text_data_values_layout Layout to display battle object's statistics value.
     * @param battle_object BattleObject& to display data into layouts.
     */
    void displayBattleObjectTextData(QLayout* battle_object_text_data_names_layout,
        QLayout* battle_object_text_data_values_layout, const BattleObject& battle_object);

    /**
     * @brief Add icons of spells of given hero into buttons, which are responsible for using theses spells.
     *
     * @param hero Hero&, whose spells should be displayed at buttons.
     *
     */
    void setSpellsImagesOnButtons(Hero& hero);

    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;

};

#endif
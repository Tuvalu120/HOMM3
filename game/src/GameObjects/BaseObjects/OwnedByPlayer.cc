/**
 * @file OwnedByPlayer.cc
 * @author Bartosz Han
 * @brief This file contains definition of class OwnedByPlayer.
 *
 */


#include <stdexcept>

#include "GameObjects/BaseObjects/OwnedByPlayer.h"

using namespace std;


OwnedByPlayer::OwnedByPlayer(int player_id):player_id_(player_id){};

OwnedByPlayer::OwnedByPlayer(const OwnedByPlayer& parent):player_id_(parent.getPlayerID()){};

OwnedByPlayer::OwnedByPlayer():player_id_(-1){};

int OwnedByPlayer::getPlayerID() const{
    return player_id_;
}

void OwnedByPlayer::setPlayerID(int new_player_id){
    if (player_id_ == -1){
        player_id_ = new_player_id;
    }
    else{
        throw invalid_argument("Tried to set this object's player_id, which already belongs to some player.");
    }
}

void OwnedByPlayer::replacePlayerID(int new_player_id){
    player_id_ = new_player_id;
}

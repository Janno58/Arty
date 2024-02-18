////////////////////////////////////////////////////////////////////////////////
#include "Players.h"

////////////////////////////////////////////////////////////////////////////////
std::optional<Player*> MarkDead(std::vector<Player>& players) {
    for(auto& player : players) {
        if(player.health < 0.0F && !player.dead) {
            player.dead = true;
            return {&player};
        }
    }

    return {};
}

////////////////////////////////////////////////////////////////////////////////
std::optional<Player*> GetLoneSurvivor(std::vector<Player>& players) {
    Player* res = nullptr;

    for(auto& player : players) {
        if(res == nullptr && !player.dead) {
            res = &player;
        } else if(res != nullptr && !player.dead) {
            return {};
        }
    }

    return res;
}

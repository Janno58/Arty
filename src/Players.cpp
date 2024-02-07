////////////////////////////////////////////////////////////////////////////////
#include "Players.h"
#include "CompPlayer.h"
#include "Unit.h"
#include "Vector.h"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <ranges>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
bool Player::IsComputer() const {
    return false;
}

////////////////////////////////////////////////////////////////////////////////
const Unit& Player::GetDrawable() const {
    return drawable;
}

////////////////////////////////////////////////////////////////////////////////
Unit& Player::GetDrawable() {
    return drawable;
}

////////////////////////////////////////////////////////////////////////////////
PlayerInfo Player::GetPlayerInfo() const {
    PlayerInfo info;
    
    info.index = index;
    info.health = GetDrawable().GetHealthAbs();
    info.position = GetDrawable().GetPosition();
    info.AI = IsComputer();

    return info;
}

////////////////////////////////////////////////////////////////////////////////
std::pair<sf::Vector2f, sf::Vector2f> Player::GetShellPos() const {
    const auto unit = GetDrawable();
    const auto posBase = unit.GetRotationPoint();
    const auto posTip = unit.GetMuzzlePos();
    const auto direction = Normalize(posTip - posBase);

    return std::make_pair(posTip, direction);
}

////////////////////////////////////////////////////////////////////////////////
void HumanPlayer::MouseMove(const sf::Vector2f& pos) {
    drawable.MouseMove(pos);
}

////////////////////////////////////////////////////////////////////////////////
std::optional<Player*> Players::GetLoneSurvivor() {
    Player* res = nullptr;

    for(auto& player : players) {
        if(res == nullptr && !player->dead) {
            res = player.get();
        } else if(res != nullptr && !player->dead) {
            return {};
        }
    }

    return res;
}

////////////////////////////////////////////////////////////////////////////////
Player* Players::GetActive() {
    assert(activeIndex < players.size());

    if(players[activeIndex]->dead) {
        Next();
    }

    return players[activeIndex].get();
}

////////////////////////////////////////////////////////////////////////////////
void Players::Next() {
    activeIndex++;

    while(activeIndex >= players.size() || players[activeIndex]->dead) {
        activeIndex++;

        if(activeIndex >= players.size()) {
            activeIndex = 0UL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector<PlayerInfo> Players::EnumeratePlayersData() const {
    std::vector<PlayerInfo> info;
    
    std::for_each(players.begin(), players.end(), [&info](auto &player){
        info.emplace_back(player->GetPlayerInfo());
    });

    return info;
}

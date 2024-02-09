#pragma once

////////////////////////////////////////////////////////////////////////////////
#include "Unit.h"

////////////////////////////////////////////////////////////////////////////////
struct PlayerInfo {
    size_t index{0}; // Id of player
    sf::Vector2f position{0.F, 0.F}; // Position of player
    float health{0}; // HP
    bool AI{false}; // is AI?
};

////////////////////////////////////////////////////////////////////////////////
class Player {
public:

    ////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    Player(size_t ind, std::string nm, Args&& ...args) : drawable(std::forward<Args>(args)...),
                                                         index(ind), name(std::move(nm)) { }

    ////////////////////////////////////////////////////////////////////////////
    virtual ~Player() = default;

    ////////////////////////////////////////////////////////////////////////////
    virtual void MouseMove(const sf::Vector2f& pos) = 0;

    ////////////////////////////////////////////////////////////////////////////
    virtual bool IsComputer() const;

    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    const Unit& GetDrawable() const;

    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    Unit& GetDrawable();

    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    PlayerInfo GetPlayerInfo() const;

    // pair.first is position
    // pair.second is direction
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    std::pair<sf::Vector2f, sf::Vector2f> GetShellPos() const;

    ////////////////////////////////////////////////////////////////////////////
    Unit drawable;

    ////////////////////////////////////////////////////////////////////////////
    size_t index;

    ////////////////////////////////////////////////////////////////////////////
    bool dead = false;

    ////////////////////////////////////////////////////////////////////////////
    std::string name;
};
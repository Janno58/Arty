#pragma once

////////////////////////////////////////////////////////////////////////////////
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <Unit.h>
#include <vector>
#include <memory>

////////////////////////////////////////////////////////////////////////////////
class Player {
public:

    ////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    Player(Args&& ...args) : drawable(std::forward<Args>(args)...) { }

    ////////////////////////////////////////////////////////////////////////////
    virtual ~Player() = default;

    ////////////////////////////////////////////////////////////////////////////
    virtual void MouseMove(const sf::Vector2f& pos) = 0;
    
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    virtual const Unit& GetDrawable() const;
    
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    virtual Unit& GetDrawable();
    
    // pair.first is position
    // pair.second is direction
    ////////////////////////////////////////////////////////////////////////////
    [[nodiscard]]
    std::pair<sf::Vector2f, sf::Vector2f> GetShellPos() const;
   
    ////////////////////////////////////////////////////////////////////////////
    Unit drawable;

};

////////////////////////////////////////////////////////////////////////////////
class HumanPlayer : public Player {
public:

    ////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    HumanPlayer(Args&& ...args) : Player(std::forward<Args>(args) ...) { }

    ////////////////////////////////////////////////////////////////////////////
    void MouseMove(const sf::Vector2f& pos) final;

};

////////////////////////////////////////////////////////////////////////////////
class Players {
public:

    ////////////////////////////////////////////////////////////////////////////
    template<typename... Args>
    void AddPlayer(Args&& ...args) {
        players.emplace_back(std::make_unique<HumanPlayer>(std::forward<Args>(args) ...));
    }

    ////////////////////////////////////////////////////////////////////////////
    Player* GetActive();

    ////////////////////////////////////////////////////////////////////////////
    bool PlayerChanged();

    ////////////////////////////////////////////////////////////////////////////
    void Next();

    ////////////////////////////////////////////////////////////////////////////
    std::vector<std::unique_ptr<Player>>::iterator begin() { return players.begin(); }

    ////////////////////////////////////////////////////////////////////////////
    std::vector<std::unique_ptr<Player>>::iterator end() { return players.end(); }

private:
    
    ////////////////////////////////////////////////////////////////////////////
    bool activePlayerChanged = false; 

    ////////////////////////////////////////////////////////////////////////////
    std::vector<std::unique_ptr<Player>> players;

    ////////////////////////////////////////////////////////////////////////////
    std::vector<std::unique_ptr<Player>>::size_type activeIndex = 0UL;
};
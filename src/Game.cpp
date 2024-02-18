#include "Config.h"
#include "Game.h"
#include "Players.h"
#include "TextureCache.h"
#include "platform/SFML_Input.h"
#include <ranges>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
SinglePlayer::SinglePlayer(sf::RenderWindow& win,
                           TextureCache& cache,
                           std::stack<std::unique_ptr<GameState>>& stack,
                           const std::string& levelName,
                           std::vector<std::string> playerNames)
: GameState(stack), activePlayer(playerNames.size()), window(win), level(levelName), scroll(level.GetSize(), win.getSize()) {

    for(auto index = 0U; index < playerNames.size(); index++) {
        switch(index) {
            case 0:
                players.emplace_back(playerNames[index], level.GetSpawn(index), DEFAULT_STARTING_HEALTH, 100.F, "tank_green.png", "tanks_turret1.png");
                break;
            case 1:
                players.emplace_back(playerNames[index], level.GetSpawn(index), DEFAULT_STARTING_HEALTH, 100.F, "navy_tank.png", "tanks_turret1.png");
                break;
            default:
                players.emplace_back(playerNames[index], level.GetSpawn(index), DEFAULT_STARTING_HEALTH, 100.F, "tank_green.png", "tanks_turret1.png");
                break;
        }
    }

    projectile.texture = "green_rocket.png";
    drawer.CreateProjectile(projectile);
    auto[shell_pixels, shell_size] = drawer.GetTexturePixels(projectile.texture);
    projectile.pixels = shell_pixels;
    projectile.size = shell_size;

    drawer.CreateTanks(players);
    for(auto& player : players) {
        auto [pixels, size] = drawer.GetTexturePixels(player.base_texture);
        player.physicsID = phys.AddDynamicBody(player.position, {0.F, 0.F}, 1.F/100.F, pixels, size);
    }

    phys.SetTerrainPixels(level.GetPixels());

    // @FIXME: Does not seem to actually work
    //scroll.Focus(players.GetActive()->GetDrawable().GetCenter());

    last_update = std::chrono::steady_clock::now();
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::ExecuteFrame() {
    if(!window.isOpen()) {
        Stop();
    }

    auto [pos, vel] = GetShellPosVel(players[activePlayer]);

    while(window.pollEvent(event)) {
        if( event.type == sf::Event::Closed ) {
            window.close();
        }

        else if( event.type == sf::Event::Resized ) {
            scroll.SetScreenSize( static_cast<float>(event.size.width), static_cast<float>(event.size.height));
        }

        else if( event.type == sf::Event::MouseButtonReleased &&
                 event.mouseButton.button == sf::Mouse::Left &&
                 projectile.has_exploded) {
            auto vel2 = vel * SHELL_VELOCITY;
            projectile.has_collided = false;
            projectile.has_exploded = false;
            projectile.position = pos + (vel * 50.F);
            projectile.velocity = vel2;
            phys.SetProjectile(&projectile);
        }
    }

    const auto gMousePos = sf::Mouse::getPosition(window);
    const auto mousePos  = window.mapPixelToCoords(gMousePos);

    players[activePlayer].turret_angle = AngleBetweenVectors(players[activePlayer].position + TURRET_OFFSET, mousePos);

    auto died = MarkDead(players);
    if(died) {
        std::string deathNotice = died.value()->name + " has died!";
        text.CreateTimedText(deathNotice, DEATH_NOTICE_SIZE, sf::seconds(3), PresetTxtPos::FirstQuarter, PresetTxtPos::Center);
    }

    auto winner = GetLoneSurvivor(players);
    if(winner) {
        text.Clear();
        std::string winNotice = winner.value()->name + " has WON!!!";
        text.CreateText(winNotice, WIN_NOTICE_SIZE, PresetTxtPos::FirstQuarter, PresetTxtPos::Center);
    }

    if(projectile.position.x > level.GetSize().x || projectile.position.x < 0) {
        projectile.has_collided = true;
    }

    drawer.UpdateTanks(players);
    drawer.UpdateProjectile(projectile);

    //
    // AI
    //
    /*
    if(players.GetActive()->IsComputer()) {
        auto* comp = dynamic_cast<AI::CompPlayer*>(players.GetActive());

        if(comp->Act()) {
            vel *= SHELL_VELOCITY;
            shell = Projectile(pos, vel);
        }
    }  */

    //
    // Physics and other frame time sensitive updates
    //
    auto now = std::chrono::steady_clock::now();
    accumulator += (now - last_update);
    last_update = now;
    while (accumulator >= FIXED_FRAME_TIME) {
        accumulator -= FIXED_FRAME_TIME;

        auto frame_time_secs = std::chrono::duration<float>(FIXED_FRAME_TIME);

        scroll.Scroll(Input::GetMapScroll(), frame_time_secs.count());
        doPhysics();
    }

    for (auto& player : players) {
        const auto& dynBody = phys.Get(player.physicsID);
        player.position = dynBody.position;
    }

    //
    // Text
    //
    text.Update();

    //
    // Draw
    //
    window.clear(level.GetBackgroundColor());
    window.setView(scroll.GetView());
    window.draw(level);

    window.draw(drawer);

    window.setView(text.View());
    window.draw(text);

    window.setView(scroll.GetView());
    window.display();


}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::doPhysics() {
    phys.Update();

    if(projectile.has_collided && !projectile.has_exploded) {
        shellExplode();
    }
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::shellExplode() {
    projectile.has_exploded = true;

    const int xPos = static_cast<int>(projectile.position.x);
    const int yPos = static_cast<int>(projectile.position.y);
    const auto pix = CreateCircle(xPos, yPos, 50);

    level.SetPixels(pix);
    phys.SetTerrainPixels(level.GetPixels());

    for(auto &player: players) {
        const auto overlap = Physics::CountOverlap(pix, phys.Get(player.physicsID));

        if(overlap > 0 && !player.dead) {
            const auto damage = static_cast<float>(overlap) / DAMAGE_REDUCTION;
            player.health -= damage;

            std::string turnNotice = players[activePlayer].name + " got hit for " + std::to_string(damage) + "damage!";
            text.CreateTimedText(turnNotice, TURN_NOTICE_SIZE, sf::seconds(2), PresetTxtPos::FirstQuarter, PresetTxtPos::Center);
        }
    }

    playerChange();
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::playerChange() {
    activePlayer++;

    while(players[activePlayer].health < 0.F) {
        activePlayer++;
    }

    scroll.Focus(players[activePlayer].position);
    std::string turnNotice = players[activePlayer].name + "'s turn!";
    text.CreateTimedText(turnNotice, TURN_NOTICE_SIZE, sf::seconds(2), PresetTxtPos::FirstFourth, PresetTxtPos::Center);

    /*
    if(player->IsComputer()) {
        auto* comp = dynamic_cast<AI::CompPlayer*>(player);
        const auto info = players.EnumeratePlayersData();

        comp->UpdateTargeting(info);
        comp->TurnStart();
    } */
}

////////////////////////////////////////////////////////////////////////////////
/// MainMenu class
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
MainMenu::MainMenu(sf::RenderWindow &win,
                   TextureCache &cache,
                   std::stack<std::unique_ptr<GameState>>& stack)
: GameState(stack), window(win), textureCache(cache) {
    if(!font.loadFromFile("SingleDay-Regular.ttf")) {
        throw std::runtime_error("MainMenu::MainMenu: Error loading font!");
    }

    const float buttonWidth = 300.F;
    const float buttonHeight = 60.F;
    const float buttonX = viewport.width / 2.F - buttonWidth / 2.F;
    const float newGameY = 300.F;
    const float quitY = 500.F;

    GUI::ButtonTextDescriptor desc;
    desc.text = sf::String("New Game");
    desc.texture1 = &textureCache.GetTexture("panel_woodPaper.png");
    desc.texture2 = &textureCache.GetTexture("panel_woodPaperDetail.png");
    desc.font = &font;

    buttons.emplace_back(std::make_unique<GUI::Button>("new game",
                                                       sf::Vector2f(buttonX, newGameY),
                                                       sf::Vector2f(buttonWidth, buttonHeight),
                                                       desc));

    desc.text = sf::String("QUIT");
    buttons.emplace_back(std::make_unique<GUI::Button>("quit",
                                                       sf::Vector2f(buttonX, quitY),
                                                       sf::Vector2f(buttonWidth, buttonHeight),
                                                       desc));
}

////////////////////////////////////////////////////////////////////////////////
void MainMenu::ExecuteFrame() {
    if(!window.isOpen()) {
        Stop();
    }

    while(window.pollEvent(event)) {
        if( event.type == sf::Event::Closed ) {
            GameState::Stop();
        }

        for(const auto& button : buttons) {
            button->DoEvent(event);
        }
    }

    const auto gMousePos = sf::Mouse::getPosition(window);
    const auto mousePos  = window.mapPixelToCoords(gMousePos);

    for(const auto& button : buttons) {
        button->MouseMove(mousePos);

        if(button->Clicked()) {
            if(button->ID() == "new game") {
                GetStack().push(std::make_unique<NewGameMenu>(window, textureCache, GetStack()));
                return;
            }

            if(button->ID() == "quit") {
                Stop();
                return;
            }
        }
    }

    window.clear(sf::Color::White);
    window.setView(view);

    for(const auto& button : buttons) {
        window.draw(*button);
    }

    window.display();
}

////////////////////////////////////////////////////////////////////////////////
/// NewGameMenu class
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
NewGameMenu::NewGameMenu(sf::RenderWindow &win,
                         TextureCache &cache,
                         std::stack<std::unique_ptr<GameState>> &stack)
: GameState(stack), window(win), textureCache(cache)  {
    levels = EnumerateLevels();
    if(levels.empty()) {
        throw std::runtime_error("Game corrupted? No levels found!");
    }

    if(!font.loadFromFile("SingleDay-Regular.ttf")) {
        throw std::runtime_error("MainMenu::MainMenu: Error loading font!");
    }

    const sf::Vector2f bigButton {300.F, 60.F};
    const sf::Vector2f smallButton {60.F, 60.F};

    const sf::Vector2f lvlBtnPos {810.F, 350.F};
    const sf::Vector2f preLvlPos {740.F, 350.F};
    const sf::Vector2f posLvlPos {1120.F, 350.F};

    const sf::Vector2f backPos {490.F, 800.F};
    const sf::Vector2f startPos {1130.F, 800.F};

    GUI::ButtonTextDescriptor desc;
    desc.text = sf::String(levels[levelIndex]);
    desc.texture1 = &textureCache.GetTexture("panel_woodPaper.png");
    desc.texture2 = &textureCache.GetTexture("panel_woodPaperDetail.png");
    desc.font = &font;

    buttons.emplace_back(std::make_unique<GUI::Button>("level", lvlBtnPos, bigButton, desc));

    desc.text = sf::String("<");
    buttons.emplace_back(std::make_unique<GUI::Button>("prevLevel", preLvlPos, smallButton, desc));

    desc.text = sf::String(">");
    buttons.emplace_back(std::make_unique<GUI::Button>("nextLevel", posLvlPos, smallButton, desc));

    desc.text = sf::String("back");
    buttons.emplace_back(std::make_unique<GUI::Button>("back", backPos, bigButton, desc));

    desc.text = sf::String("start");
    buttons.emplace_back(std::make_unique<GUI::Button>("start", startPos, bigButton, desc));

    /// Players settings
    createPlayersSettings(levels[levelIndex]);
}

////////////////////////////////////////////////////////////////////////////////
void NewGameMenu::ExecuteFrame() {
    if (!window.isOpen()) {
        Stop();
    }

    const auto gMousePos = sf::Mouse::getPosition(window);
    const auto mousePos  = window.mapPixelToCoords(gMousePos);

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            GameState::Stop();
        }

        if (event.type == sf::Event::TextEntered) {
            for (auto& fields : playerNameEdits) {
                fields.DoEvent(event);
            }
        }

        if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
            GetStack().pop();
            return;
        }

        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Button::Left) {
            for (auto& fields : playerNameEdits) {
                fields.SetActive( fields.GetRect().contains(mousePos) );
            }
        }

        for (const auto &button: buttons) {
            button->DoEvent(event);
        }
    }

    for(const auto& button : buttons) {
        button->MouseMove(mousePos);

        if(button->Clicked()) {
            if(button->ID() == "start") {
                GetStack().push(std::make_unique<SinglePlayer>(window, textureCache, GetStack(), levels[levelIndex], getPlayersOptions()));
                return;
            }

            if(button->ID() == "back") {
                GetStack().pop();
                return;
            }

            if(button->ID() == "prevLevel") { prevLevel(); }
            else if(button->ID() == "nextLevel" || button->ID() == "level") { nextLevel(); }
        }

        if(button->ID() == "level") {
            button->SetText(levels[levelIndex]);
        }
    }

    window.clear(sf::Color::White);
    window.setView(view);

    for(const auto& button : buttons) {
        window.draw(*button);
    }

    for(const auto& label : playerNameLabels) {
        window.draw(label);
    }

    for(const auto& textEdit: playerNameEdits) {
        window.draw(textEdit);
    }

    window.setView(text.View());
    window.draw(text);

    window.display();
}

////////////////////////////////////////////////////////////////////////////////
void NewGameMenu::prevLevel(){
    if(levelIndex == 0) {
        levelIndex = levels.size() - 1;
        return;
    }

    levelIndex--;
}

////////////////////////////////////////////////////////////////////////////////
void NewGameMenu::nextLevel(){
    levelIndex++;
    if(levelIndex >= levels.size()) {
        levelIndex = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
void NewGameMenu::createPlayersSettings(const std::string& levelName) {
    auto spawnsCount = GetMaxPlayers(levelName);

    playerNameLabels.clear();
    playerNameEdits.clear();

    const sf::Vector2f labelFirstPos = {700.F, 400.F};
    const sf::Vector2f editFirstPos = {900.F, 400.F};
    const float y_offset = 75.F;
    const auto labelTextSize = 50U;

    std::string str = "Player ";

    for(auto index = 0; index < spawnsCount; index++) {
        sf::Text label;
        label.setCharacterSize(labelTextSize);
        label.setPosition(labelFirstPos.x, labelFirstPos.y + (y_offset * static_cast<float>(index)));

        auto cnt = std::to_string(static_cast<int>(index));
        auto res = str + cnt + ':';

        label.setString(res);
        label.setFillColor(sf::Color::Black);
        label.setFont(font);

        playerNameLabels.push_back(label);
        // @FIXME: Add random names
        GUI::TextEdit editField("John", sf::Vector2f(editFirstPos.x, editFirstPos.y + (y_offset * static_cast<float>(index))), font);
        playerNameEdits.push_back(editField);
    }
}

////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> NewGameMenu::getPlayersOptions() {
    std::vector<std::string> results;

    for(auto field : playerNameEdits) {
        results.push_back(field.GetText());
    }

    assert(results.size() > 1);

    return results;
}
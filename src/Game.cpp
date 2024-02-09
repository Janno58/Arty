#include "Config.h"
#include "Game.h"
#include "Players.h"
#include "TextureCache.h"
#include "Input.h"
#include "Physics.h"
#include <ranges>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////
SinglePlayer::SinglePlayer(sf::RenderWindow& win,
                           TextureCache& cache,
                           std::stack<std::unique_ptr<GameState>>& stack,
                           const std::string& levelName,
                           std::vector<std::string> playerNames)
: GameState(stack), window(win), level(levelName), scroll(level.GetSize(), win.getSize()) {

    sf::Texture& turret = cache.GetTexture("tanks_turret1.png");

    for(auto index = 0U; index < playerNames.size(); index++) {
        switch(index) {
            case 0:
                players.AddPlayer(playerNames[index], cache.GetTexture("tanks_tankGreen_body3.png"), turret, level.GetSpawn(index));
                break;
            case 1:
                players.AddPlayer(playerNames[index], cache.GetTexture("tanks_tankNavy_body3.png"), turret, level.GetSpawn(index));
                break;
            default:
                players.AddPlayer(playerNames[index], cache.GetTexture("tanks_tankGreen_body3.png"), turret, level.GetSpawn(index));
                break;
        }
    }

    scroll.Focus(players.GetActive()->GetDrawable().GetCenter());
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::ExecuteFrame() {
    if(!window.isOpen()) {
        Stop();
    }

    auto [pos, vel] = players.GetActive()->GetShellPos();

    while(window.pollEvent(event)) {
        if( event.type == sf::Event::Closed ) {
            window.close();
        }

        else if( event.type == sf::Event::Resized ) {
            scroll.SetScreenSize( static_cast<float>(event.size.width), static_cast<float>(event.size.height));
        }

        else if( event.type == sf::Event::MouseButtonReleased &&
                 event.mouseButton.button == sf::Mouse::Left &&
                 shell.HasExploded()) {
            vel *= SHELL_VELOCITY;
            shell = Projectile(pos, vel);
        }
    }

    const auto gMousePos = sf::Mouse::getPosition(window);
    const auto mousePos  = window.mapPixelToCoords(gMousePos);

    players.GetActive()->MouseMove(mousePos);
    trajectoryDrawer.Update(pos, vel*SHELL_VELOCITY);

    for(const auto& player : players) {
        if(!player->dead && player->GetDrawable().GetHealthAbs() <= 0.0F ) {
            player->dead = true;
            std::string deathNotice = player->name + " has died!";
            text.CreateTimedText(deathNotice, DEATH_NOTICE_SIZE, sf::seconds(3), PresetTxtPos::FirstQuarter, PresetTxtPos::Center);
        }
    }

    //
    // Win
    //
    auto loneSurvivor = players.GetLoneSurvivor();
    if(loneSurvivor) {
        text.Clear();
        std::string winNotice = loneSurvivor.value()->name + " has WON!!!";
        text.CreateText(winNotice, WIN_NOTICE_SIZE, PresetTxtPos::FirstQuarter, PresetTxtPos::Center);
    }

    //
    // Projectile update
    //
    if(!shell.HasExploded() && IsOutsideLevel(LEVEL_WIDTH, LEVEL_HEIGHT, shell)) {
        shellExplode();
    }

    if(!shell.HasExploded()) {
        scroll.Focus(shell.GetTheTip());
    }

    //
    // AI
    //
    if(players.GetActive()->IsComputer()) {
        auto* comp = dynamic_cast<AI::CompPlayer*>(players.GetActive());

        if(comp->Act()) {
            vel *= SHELL_VELOCITY;
            shell = Projectile(pos, vel);
        }
    }

    //
    // Physics
    //
    frameTimeAccumulator += frameClock.restart();
    while( frameTimeAccumulator >= fixedFrameTime ) {
        scroll.Scroll(Input::GetMapScroll(), fixedFrameTime);
        doPhysics(fixedFrameTime);
        frameTimeAccumulator -= fixedFrameTime;
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

    if(shell.HasExploded()) {
        window.draw(trajectoryDrawer);
    }

    for(const auto& player : players) {
        window.draw(player->GetDrawable()); 
    }

    window.draw(shell);

    window.setView(text.View());
    window.draw(text);

    window.setView(scroll.GetView());
    window.display();
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::doPhysics(const sf::Time& deltaTime) {

    for(auto& tank : players) {
        tank->GetDrawable().StepPhysics(deltaTime.asSeconds());

        if(Physics::Collides(level, tank->GetDrawable())) {
            tank->GetDrawable().SetVelocity(-GRAVITY);
        }
    }

    shell.StepPhysics(fixedFrameTime.asSeconds());

    // TODO: Physics should probably try collisions
    // FIXME: Shell get exploded when check for collision but out of bounds;
    if (Physics::Collides(level, shell)) {
        shellExplode();
    }

    for (auto &tank: players) {
        if (Physics::Collides(tank->GetDrawable(), shell)) {
            shellExplode();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::shellExplode() {
    if(shell.HasExploded()) {
        return;
    }

    const auto pixels = shell.Explode();
    level.SetPixels(pixels);

    for(auto &player: players) {
        const auto overlap = CountOverlap(pixels, player);
        player->GetDrawable().Damage(static_cast<float>(overlap) / 100.F);
    }

    playerChange();
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::playerChange() {
    players.Next();
    auto* player = players.GetActive();

    scroll.Focus(player->GetDrawable().GetCenter());
    std::string turnNotice = player->name + "'s turn!";
    text.CreateTimedText(turnNotice, TURN_NOTICE_SIZE, sf::seconds(2), PresetTxtPos::FirstFourth, PresetTxtPos::Center);

    if(player->IsComputer()) {
        auto* comp = dynamic_cast<AI::CompPlayer*>(player);
        const auto info = players.EnumeratePlayersData();

        comp->UpdateTargeting(info);
        comp->TurnStart();
    }
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
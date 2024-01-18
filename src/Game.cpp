#include "Config.h"
#include "Game.h"
#include "TextureCache.h"
#include "Input.h"
#include "Vector.h"
#include "Physics.h"

////////////////////////////////////////////////////////////////////////////////
SinglePlayer::SinglePlayer(sf::RenderWindow& win, TextureCache& cache) : window(win), textureCache(cache), scroll(level.GetSize(), win.getSize()) {
    players.AddPlayer(cache.GetTexture("tanks_tankGreen_body3.png"),
                      cache.GetTexture("tanks_turret1.png"),
                      level.GetSpawn(0U));

    players.AddPlayer(cache.GetTexture("tanks_tankNavy_body3.png"),
                      cache.GetTexture("tanks_turret1.png"),
                      level.GetSpawn(1U));

}
////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::ExecuteFrame() {
    while(window.pollEvent(event)) {
        if( event.type == sf::Event::Closed ) {
            GameState::Stop(); 
        }

        else if( event.type == sf::Event::Resized ) {
            scroll.SetScreenSize( static_cast<float>(event.size.width), static_cast<float>(event.size.height));
        }

        else if( event.type == sf::Event::MouseButtonReleased &&
                 event.mouseButton.button == sf::Mouse::Left) {
            auto [pos, vel] = players.GetActive()->GetShellPos();
            vel *= SHELL_VELOCITY;

            shells.emplace_back(pos, vel);
            players.Next();
        }
    }
 
    const auto gMousePos = sf::Mouse::getPosition(window);
    const auto mousePos  = window.mapPixelToCoords(gMousePos);

    players.GetActive()->MouseMove(mousePos);

    if(shells.empty() && players.PlayerChanged()) {
        scroll.Focus(players.GetActive()->GetDrawable().GetCenter());
    } else if(!shells.empty()) {
        scroll.Focus(shells.back().GetTheTip());
    }

    frameTimeAccumulator += frameClock.restart();
    while( frameTimeAccumulator >= fixedFrameTime ) {
        scroll.Scroll(Input::GetMapScroll(), fixedFrameTime);
        doPhysics(fixedFrameTime);
        frameTimeAccumulator -= fixedFrameTime;
    }

    //
    // Draw
    //
    window.clear(level.GetBackgroundColor());
    window.setView(scroll.GetView());
    window.draw(level);

    for(const auto& player : players) {
        window.draw(player->GetDrawable()); 
    }

    for(const auto& shell : shells) {
        window.draw(shell);
    }


    window.display();

    //
    // Misc housekeeping
    //
    std::erase_if(shells, [](const auto& shell){ return shell.HasExploded(); });
    std::erase_if(shells, ShellOutsideLevel(LEVEL_WIDTH, LEVEL_HEIGHT));
}

////////////////////////////////////////////////////////////////////////////////
void SinglePlayer::doPhysics(const sf::Time& deltaTime) {

    for(auto& tank : players) {
        tank->GetDrawable().StepPhysics(deltaTime.asSeconds());

        if(Physics::Collides(level, tank->GetDrawable())) {
            tank->GetDrawable().SetVelocity(-GRAVITY);
        }
    }

    for (auto &shell: shells) {
        shell.StepPhysics(fixedFrameTime.asSeconds());

        // TODO: Physics should probably try collisions
        // FIXME: Shell get exploded when check for collision but out of bounds;
        if (Physics::Collides(level, shell)) {
            auto pixels = shell.Explode();
            level.SetPixels(pixels);
        }

        for (auto &tank: players) {
            if (Physics::Collides(tank->GetDrawable(), shell)) {
                auto pixels = shell.Explode();
                level.SetPixels(pixels);
            }
        }

        // TODO: Damage calculation code is max iffy
        if (shell.HasExploded()) {
            const auto pixels = shell.Explode();
            for(auto& player : players) {
                const auto overlap = CountOverlap(pixels, player);
                player->GetDrawable().Damage(static_cast<float>(overlap / 100));
            }
        }
    }
}

#include "CompPlayer.h"
#include "Vector.h"
#include <ranges>

////////////////////////////////////////////////////////////////////////////////
namespace AI {

////////////////////////////////////////////////////////////////////////////////
void CompPlayer::MouseMove([[maybe_unused]] const sf::Vector2f& pos) {

}

////////////////////////////////////////////////////////////////////////////////
bool CompPlayer::IsComputer() const {
    return true;
}

////////////////////////////////////////////////////////////////////////////////
void CompPlayer::UpdateTargeting(std::vector<PlayerInfo> info) {
    const auto pos = GetDrawable().GetPosition();
    const auto dedThreshold = 0.0001F;

    const auto isNotItself = [pos](const auto &player) {
        return player.position != pos;
    };

    const auto isNotDead = [dedThreshold](const auto &player) {
        return player.health > dedThreshold;
    };

    std::ranges::sort(info, [&pos](const auto& lhs, const auto& rhs){
        return Length(pos - lhs) < Length(pos - rhs);
    }, &PlayerInfo::position);

    auto view = info | std::views::filter(isNotItself) | std::views::filter(isNotDead);

    if(view.empty()) {
        return;
    }

    target = view.front().index;

    const auto vec1 = view.front().position;
    const auto vec2 = drawable.GetPosition();
    const auto angle = atan2(vec1.y - vec2.y, vec1.x - vec2.x) * 180.F / M_PI;

    drawable.SetTurretRotation(angle);
}

////////////////////////////////////////////////////////////////////////////////
void CompPlayer::TurnStart() {
    actionList.clear();
    actionList.emplace_back(AI::WaitAction(sf::seconds(3)));
    actionList.emplace_back(AI::ShootAction());
}

////////////////////////////////////////////////////////////////////////////////
bool CompPlayer::Act() {
    if(actionList.empty()) {
        return false;
    }

    auto action = actionList.front();

    switch(action.index()) {
        // Wait action
        case 0: {
            auto waitAction = std::get<AI::WaitAction>(action);

            if(waitAction.clock.getElapsedTime() > waitAction.timeToWait) {
                actionList.pop_front();
            }

            break;
        }

            // Rotate turret
        case 1: {
            auto rotateAction = std::get<AI::RotateTurretAction>(action);
            drawable.SetTurretRotation(rotateAction.degrees);
            actionList.pop_front();

            break;
        }

            // Shoot
        case 2: {
            actionList.pop_front();
            return true;
        }

            // ???
        default:
            break;
    }

    return false;
}

}

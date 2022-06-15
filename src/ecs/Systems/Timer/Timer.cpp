/*
** EPITECH PROJECT, 2022
** B-YEP-400-LIL-4-1-indiestudio-clement.chausson
** File description:
** Timer.cpp
*/


#include "Timer.hpp"
#include "raylib.hpp"
#include "../../Components/Timable/Timable.hpp"
#include "../../Components/Playable/Playable.hpp"
#include "../../Components/Breakable/Breakable.hpp"
#include "../../Components/Animable/Animable.hpp"


Timer::Timer(ECSManager *ecsManager, std::vector<Entity *> *mapEntities, std::vector<Entity *> *playerEntities)

{
    Raylib::Raylib_encap Raylib_encp;
    this->_ecsManager = ecsManager;
    this->_mapEntities = mapEntities;
    this->_playerEntities = playerEntities;
    this->_soundSize = {0.00025f, 0.00025f, 0.00025f};
    this->_soundTexture = Raylib_encp.LTexture("assets/materials/game/sounds.png");
}

Timer::~Timer()
{
    Raylib::Raylib_encap Raylib_encp;
    Raylib_encp.UnlTexture(this->_soundTexture);
}

void Timer::apply(std::vector<IComponent *> component)
{
    // Components [0] Timable [1] DrawableText [2] Placable [3] Animable [4] Soundable
    Timable *time = static_cast<Timable *>(component[0]);
    Placable *place = static_cast<Placable *>(component[2]);
    Animable *anim = static_cast<Animable *>(component[3]);
    Soundable *sound;
    DrawableText *text;
    TIMABLE_TYPE time_type = time->getTimeType();
    Playable *playable;
    Vector3 playerPos = {1.0f, 0.0f, 0.0f};

    if (time_type == GAME_CLOCK) {
        text = static_cast<DrawableText *>(component[1]);
        text->setText(std::to_string(time->getTimeLeft()) + "s");
    } else if (time_type == GAME_MANDRAKE) {
        if (time->isTimeOut()) {
            time->setTimer(0.2);
            time->RestartClock();
            time->setTimeType(GAME_MANDRAKE_SECOND);
            anim->setAnimationType(ANIMATION_TYPE::RUN);
        }
    } else if  (time_type == GAME_MANDRAKE_SECOND) {
        if (time->isTimeOut()) {
            sound = static_cast<Soundable *>(component[4]);
            if (sound != nullptr)
                sound->stopSound();
            time->setFinished(true);
            playable = static_cast<Playable *>(time->getPlayable());
            playable->setNbMandrake(playable->getNbMandrake() - 1);
            this->updatePlayer(place->getPosition(), time->getPlayable());
            this->deleteGnome(place->getPosition(), time->getPlayable());
            this->_ecsManager->deleteEntity(time->getIdEntity());

            int particles_id = this->_ecsManager->createEntity();
            this->_ecsManager->addComponent(particles_id, std::make_unique<Placable>(round(place->getX()) + 1, round(place->getY()), round(place->getZ()), playerPos, -90.0f, this->_soundSize));
            this->_ecsManager->addComponent(particles_id, std::make_unique<DrawableCubeTexture>(this->_soundTexture, CubeTextureType::SOUND));
        }
    } else if (time_type == GAME_GNOME) {
        if (time->isTimeOut()) {
            time->setFinished(true);
            this->deleteGnome(place->getPosition(), time->getPlayable());
        }
    } else if (time_type == GAME_PLAYER_FALL) {
        if (time->isTimeOut()) {
            time->setFinished(true);
            this->deletePlayer(place->getPosition(), time->getPlayable());
        }
    }
}

bool Timer::isInRange(Vector3 bomb_pos, Vector3 breakable_pos, float range)
{
    return (std::abs(breakable_pos.x - bomb_pos.x) < 0.5 && (std::abs(breakable_pos.z - bomb_pos.z)) < range
        || (std::abs(breakable_pos.z - bomb_pos.z) < 0.5 && (std::abs(breakable_pos.x - bomb_pos.x)) < range));
}

SYSTEM_TYPES Timer::getType()
{
    return (TIMER);
}

void Timer::deleteGnome(Vector3 position, void *play)
{
    Vector3 pos;
    Playable *playable = static_cast<Playable *>(play);
    Placable *place;
    Breakable *breakable;
    float range = playable->getRange();
    for (auto &entity : *this->_mapEntities) {
        if (entity->getComponents().size() != 0 && static_cast<Breakable *>(entity->getComponentsByType(BREAKABLE)) != nullptr) {
            place = static_cast<Placable *>(entity->getComponentsByType(PLACABLE));
            pos = place->getPosition();
            if (isInRange(position, pos, range)) {
                this->_ecsManager->getEntity(entity->getId())->clearComponent();
            }
        }
    }
}

void Timer::updateGnome(Vector3 position, void *play)
{
    Vector3 pos;
    Playable *playable = static_cast<Playable *>(play);
    Placable *place;
    Breakable *breakable;
    float range = playable->getRange();
    for (auto &entity : *this->_mapEntities) {
        if (entity->getComponents().size() != 0 && static_cast<Breakable *>(entity->getComponentsByType(BREAKABLE)) != nullptr) {
            place = static_cast<Placable *>(entity->getComponentsByType(PLACABLE));
            pos = place->getPosition();
            if (isInRange(position, pos, range)) {
                this->_ecsManager->addComponent(entity->getId(), std::make_unique<Animable>("assets/models/gnome/gnome.iqm", ANIMATION_TYPE::RUN, 40));
                this->_ecsManager->addComponent(entity->getId(),std::make_unique<Timable>(0.3, GAME_GNOME, -1, playable));
                this->_ecsManager->addComponent(entity->getId(), std::make_unique<Playable>(3));
                this->_ecsManager->addComponent(entity->getId(), std::make_unique<Movable>());
            }
        }
    }
}

void Timer::updatePlayer(Vector3 position, void *play)
{
    Vector3 pos;
    Playable *playable = static_cast<Playable *>(play);
    Placable *place;
    Breakable *breakable;
    float range = playable->getRange();
    for (auto &entity : *this->_playerEntities) {
        if (entity->getComponents().size() != 0 && static_cast<Breakable *>(entity->getComponentsByType(BREAKABLE)) != nullptr) {
            place = static_cast<Placable *>(entity->getComponentsByType(PLACABLE));
            pos = place->getPosition();
            if (isInRange(position, pos, range)) {
                static_cast<Animable *>(entity->getComponentsByType(ANIMABLE))->setAnimationType(ANIMATION_TYPE::FALL);
                static_cast<Animable *>(entity->getComponentsByType(ANIMABLE))->setAnimFrameCounter(0);

                entity->addComponent(std::make_unique<Timable>(1.5, GAME_PLAYER_FALL, -1, playable));
            }
        }
    }
}

void Timer::deletePlayer(Vector3 position, void *play)
{
    Vector3 pos;
    Playable *playable = static_cast<Playable *>(play);
    Placable *place;
    Breakable *breakable;
    float range = playable->getRange();
    for (auto &entity : *this->_playerEntities) {
        if (entity->getComponents().size() != 0 && static_cast<Breakable *>(entity->getComponentsByType(BREAKABLE)) != nullptr) {
            place = static_cast<Placable *>(entity->getComponentsByType(PLACABLE));
            pos = place->getPosition();
            if (isInRange(position, pos, range)) {
                this->_ecsManager->getEntity(entity->getId())->clearComponent();
            }
        }
    }
}

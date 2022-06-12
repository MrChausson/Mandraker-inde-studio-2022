/*
** EPITECH PROJECT, 2022
** B-YEP-400-LIL-4-1-indiestudio-clement.chausson
** File description:
** Player.cpp
*/


#include "Player.hpp"
#include "raylib.hpp"
#include "../../Components/Animable/Animable.hpp"
#include "../../Components/Timable/Timable.hpp"
#include "../../Components/Playable/Playable.hpp"


Player::Player(ECSManager *ecsManager)
{
    Raylib::Raylib_encap Raylib_encp;
    this->_ecsManager= ecsManager;
    this->_toWait = std::chrono::system_clock::now();
    
    // Creating Mandrake model
    this->_mandrakeModel = Raylib_encp.LModel("assets/models/mandrake/mandrake.iqm");
    this->_texturesMandrake = {
        Raylib_encp.LTexture("assets/models/mandrake/target_mandrake_d.png"),
        Raylib_encp.LTexture("assets/models/mandrake/target_plantpot_d.png"),
    };
    this->_meshOrderMandrake = {
       1, 2
    };
    this->_scaleMandrake = {0.0002f, 0.0002f, 0.0002f};
    this->_plantSound = Raylib_encp.LSound("assets/sounds/mandrake/potting.mp3");
    Raylib_encp.SetSVolume(this->_plantSound, soundVolume);
}

Player::~Player()
{
    Raylib::Raylib_encap Raylib_encp;
    UnloadModel(this->_mandrakeModel);
    Raylib_encp.UnlSound(this->_plantSound);
    Raylib_encp.UnlTexture(this->_texturesMandrake[0]);
    Raylib_encp.UnlTexture(this->_texturesMandrake[1]);

}

void Player::apply(std::vector<IComponent *> component)
{
    Raylib::Raylib_encap Raylib_encp;
    Placable *playerPlace = static_cast<Placable *>(component[0]);
    Movable *playerMove = static_cast<Movable *>(component[1]);
    Playable *playable = static_cast<Playable *>(component[2]);
    Vector3 playerPos = {1.0f, 0.0f, 0.0f};
    MOVABLE_TYPE type = playerMove->getMovableType();
    int bomb_id;
    if ( IsKeyPressed(KEY_SPACE) && type == MOVABLE_PLAYER  && playable->getNbMandrake( ) < playable->getNbMaxMandrake()) {
        playable->setNbMandrake(playable->getNbMandrake() + 1);
        bomb_id = this->_ecsManager->createEntity();
        Raylib_encp.PlayS(this->_plantSound);
        this->_ecsManager->addComponent(bomb_id, std::make_unique<Timable>(3, GAME_MANDRAKE, bomb_id, playable));
        this->_ecsManager->addComponent(bomb_id, std::make_unique<Placable>(playerPlace->getX(), playerPlace->getY(), playerPlace->getZ(), playerPos, -90.0f, this->_scaleMandrake));
        this->_ecsManager->addComponent(bomb_id, std::make_unique<DrawableModel>(this->_texturesMandrake, this->_mandrakeModel, this->_meshOrderMandrake));
        this->_ecsManager->addComponent(bomb_id, std::make_unique<Animable>("assets/models/mandrake/mandrake.iqm", ANIMATION_TYPE::IDLE));
    }
}


SYSTEM_TYPES Player::getType()
{
    return (PLAYER);
}
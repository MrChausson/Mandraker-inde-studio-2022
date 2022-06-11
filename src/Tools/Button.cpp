/*
** EPITECH PROJECT, 2022
** B-YEP-400-LIL-4-1-indiestudio-clement.chausson
** File description:
** Button
*/

#include "Button.hpp"


Button::Button(ECSManager *ecsManager, std::string text, int x, int y, Font font, Texture2D textures[3], SCENE_TYPE type, ClickableActionType actionType, Scene *current_scene)
{
    this->_idSprite = ecsManager->createEntity();
    this->_idText = ecsManager->createEntity();

    ecsManager->addComponent(this->_idSprite, std::make_unique<Placable>(x, y));
    ecsManager->addComponent(this->_idSprite, std::make_unique<DrawableSprite>(textures[1], 1));
    ecsManager->addComponent(this->_idSprite, std::make_unique<Clickable>(ecsManager->getEntity(this->_idSprite) ,textures[2], type, actionType, current_scene));
    ecsManager->addComponent(this->_idSprite, std::make_unique<Hoverable>(ecsManager->getEntity(this->_idSprite), textures[0]));
    if (text.compare("") != 0) {
        if (text.length() > 6)
            x = x - 90;
        ecsManager->addComponent(this->_idText, std::make_unique<Placable>(static_cast<float>(x - 10 + (textures[1].width / 2 - font.baseSize)),static_cast<float>(y + (textures[1].height / 2 - font.baseSize / 2))));
        ecsManager->addComponent(this->_idText, std::make_unique<DrawableText>(2 ,text, Color{255, 255, 255, 255}, font));
    }
}

Button::Button(ECSManager *ecsManager, int x, int y, Texture2D textures[3], ClickableActionType actionType, Scene *current_scene, Sound *sound)
{
    this->_idSprite = ecsManager->createEntity();

    ecsManager->addComponent(this->_idSprite, std::make_unique<Placable>(x, y));
    ecsManager->addComponent(this->_idSprite, std::make_unique<DrawableSprite>(textures[1], 1));
    ecsManager->addComponent(this->_idSprite, std::make_unique<Clickable>(ecsManager->getEntity(this->_idSprite) ,textures[2], SCENE_NONE, actionType, current_scene, sound));
    ecsManager->addComponent(this->_idSprite, std::make_unique<Hoverable>(ecsManager->getEntity(this->_idSprite), textures[0]));
}

Button::~Button()
{
}

int Button::getIdSprite()
{
    return this->_idSprite;
}

int Button::getIdText()
{
    return this->_idText;
}
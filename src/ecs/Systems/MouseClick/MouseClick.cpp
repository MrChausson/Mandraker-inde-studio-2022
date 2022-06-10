/*
** EPITECH PROJECT, 2022
** B-YEP-400-LIL-4-1-indiestudio-clement.chausson
** File description:
** MouseClick
*/

#include <iostream>
#include "raylib.hpp"
#include "MouseClick.hpp"
#include "../../Components/Clickable/Clickable.hpp"
#include "../../../Engine/Engine.hpp"
#include "../../../Scene/Game/Game.hpp"
#include "../../../Scene/Menu/Menu.hpp"
#include "../../../Scene/Settings/Settings.hpp" 
#include "../../../Scene/CharacterSelector/CharacterSelector.hpp"

MouseClick::MouseClick()
{
}

MouseClick::~MouseClick()
{
}


SYSTEM_TYPES MouseClick::getType()
{
    return (MOUSE_CLICK);
}

void MouseClick::clickAction(ClickableActionType actionType, IComponent *component)
{
    Clickable *click = static_cast<Clickable *>(component);
    ECSManager *ecs = click->getEcs();
    Scene *scene = click->getScene();
    CharacterSelector *charScene;
    Raylib::Raylib_encap r;

    switch (actionType)
    {
    case CLICKABLE_ACTION_CHANGE_ECS:
        if (click->_tmpEcs == SCENE_GAME) {
            if (scene == nullptr)
                throw std::runtime_error("Scene is null");
            charScene = static_cast<CharacterSelector *>(scene);
            Game *game = new Game(charScene->getModels());
            click->setEcs(game->getECS());
            delete (charScene);
        } else if (click->_tmpEcs == SCENE_MENU) {
            Menu *menu = new Menu();
            click->setEcs(menu->getECS());
            if (scene != nullptr)
                delete (static_cast<Settings *>(scene));
        } else if (click->_tmpEcs == SCENE_SETTINGS) {
            Settings *settings = new Settings();
            click->setEcs(settings->getECS());
            if (scene != nullptr)
                delete (static_cast<Menu *>(scene));
        } else if (click->_tmpEcs == SCENE_CHARACTER_SELECTOR) {
            CharacterSelector *characterSelector = new CharacterSelector();
            click->setEcs(characterSelector->getECS());
            if (scene != nullptr)
                delete (static_cast<Menu *>(scene));
        }
        break;
    case CLICKABLE_ACTION_QUIT_GAME:
        std::cout << "Goodbye!" << std::endl;
        loop_status = false;
        if (scene != nullptr)
            delete (scene);
        break;
    case CLICKABLE_ACTION_CHOOSE_CHARACTER:
        if (click->_sound != nullptr)
            r.PlayS(*click->_sound);
        break;
    }
}

void MouseClick::apply(std::vector<IComponent *> component)
{
    Raylib::Raylib_encap Raylib_encp;
    Clickable *click = static_cast<Clickable *>(component[1]);
    Vector2 mouse = Raylib_encp.GetMousePos();
    bool pressed = Raylib_encp.IsMouseBDown(MOUSE_LEFT_BUTTON);
    bool released = Raylib_encp.IsMouseBReleased(MOUSE_LEFT_BUTTON);

    //Drawable first, clickable second
    if (click->changedState(mouse, pressed))
        if (pressed) {
            DrawableSprite *draw = static_cast<DrawableSprite *>(component[0]);
            click->_textureSaved = draw->getTexture();
            draw->setTexture(click->getTexture());
        } else if (released && click->getActionType() != CLICKABLE_ACTION_NONE) {
            clickAction(click->getActionType(), component[1]);
        } else {
            DrawableSprite *draw = static_cast<DrawableSprite *>(component[0]);
            draw->setTexture(click->_textureSaved);
        }
}
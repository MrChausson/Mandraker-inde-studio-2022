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

    switch (actionType)
    {
    case CLICKABLE_ACTION_CHANGE_ECS:
        if (click->_tmpEcs != nullptr)
            click->setEcs(click->_tmpEcs);
        break;
    case CLICKABLE_ACTION_OPEN_OPTIONS:
        std::cout << "OPEN OPTIONS" << std::endl;
        //TODO: change ECS
        break;
    case CLICKABLE_ACTION_QUIT_GAME:
        std::cout << "Goodbye!" << std::endl;
        loop_status = false;
        break;
    default:
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
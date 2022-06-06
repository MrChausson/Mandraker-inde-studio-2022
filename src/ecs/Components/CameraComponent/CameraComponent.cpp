/*
** EPITECH PROJECT, 2022
** B-YEP-400-LIL-4-1-indiestudio-clement.chausson
** File description:
** CameraComponent
*/

#include "CameraComponent.hpp"

CameraComponent::CameraComponent(Vector3 position, Vector3 target, Vector3 up, float fov, int projection)
{
    this->_position = position;
    this->_target = target;
    this->_up = up;
    this->_fov = fov;
    this->_projection = projection;
}

CameraComponent::~CameraComponent()
{
}

COMPONENT_TYPES CameraComponent::getType()
{
    return CAMERA;
}

Camera3D CameraComponent::getCamera()
{
    Camera3D camera;

    camera.position = this->_position;
    camera.target = this->_target;
    camera.up = this->_up;
    camera.fovy = this->_fov;
    camera.projection = this->_projection;
    return camera;
}
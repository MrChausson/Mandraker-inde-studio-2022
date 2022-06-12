/*
** EPITECH PROJECT, 2022
** B-YEP-400-LIL-4-1-indiestudio-clement.chausson
** File description:
** Timer.hpp
*/

#ifndef Timer_HPP_
#define Timer_HPP_

#include "../../ISystem/ISystem.hpp"
#include "../../../ecs/ECSManager/ECSManager.hpp"
#include <chrono>

class Timer : public ISystem {
    public:
        Timer(ECSManager *ecsManager);
        ~Timer();
        void apply(std::vector<IComponent *> component) override;
        SYSTEM_TYPES getType() override;

    private:
        ECSManager *_ecsManager;
        bool _deleted = false;
};

#endif /* !Timer_HPP_ */

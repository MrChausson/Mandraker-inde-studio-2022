/*
** EPITECH PROJECT, 2022
** Visual Studio Live Share (Workspace)
** File description:
** Menu
*/

#ifndef MENU_HPP_
#define MENU_HPP_
    #include "../Scene.hpp"

class Menu : public Scene {
    public:
        Menu();
        ~Menu();

    protected:
    private:
        Texture2D _background_texture;
        Texture2D _btn_active_texture;
        Texture2D _btn_inactive_texture;
        Texture2D _btn_clicked_texture;
        Texture2D _title_texture;
        Font _btn_font;
        Font _title_font;
};

#endif /* !MENU_HPP_ */

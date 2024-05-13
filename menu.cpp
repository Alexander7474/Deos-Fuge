#include "menu.h"

Menu::Menu() : isPlaying(false)
{
    background = Sprite(Texture("img/menu/menu.png"));
    playButton = Sprite(Texture("img/menu/play_button/0.png"));
    playButton.setPosition(Vector2f(BBOP_WINDOW_SIZE.x / 3, BBOP_WINDOW_SIZE.y / 2.1));
}

Menu::~Menu()
{
}

void Menu::update()
{
    if (!isPlaying)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            playButton.setTexture(Texture("img/menu/play_button/1.png"));
        }

        defaultScene.Draw(background);
        defaultScene.Draw(playButton);
    }
    else
    {
        int x = game.update(window);
        if (x > 0)
        {
            game = Game(x + 1, SoundEngine, font);
        }
        defaultScene.Draw(game);
    }
}

void Menu::draw()
{
    // Rien à faire ici, car nous dessinons tout dans la fonction update()
}

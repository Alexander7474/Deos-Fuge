#ifndef MENU_H
#define MENU_H

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Menu : public BbopDrawable
{
public:
    Menu();
    virtual ~Menu();

    virtual void update();
    virtual void draw();

private:
    Sprite background;
    Sprite playButton;
    bool isPlaying;
};

#endif // MENU_H

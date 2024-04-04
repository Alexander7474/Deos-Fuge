#pragma once
#include <BBOP/Graphics.h>
#include <vector>
#include <iostream>
enum perso_state:int{stationary=0,run=1,dash=2};
class Personnage : public Sprite
{
    private:
        float percentage;
        float vitesse;
        Vector2f mouvement;
        float weight;
        std::vector<Texture> animation[3];
        perso_state state;
    public:
        Personnage(std::string sprite_folder,float percentage_, float vitesse_ , float weight_);
        void update();
        //virtual void attack(Personnage Ennemy[], GLFWwindow *);
        //virtual bool gettinghit();
        //virtual void Dash();
};
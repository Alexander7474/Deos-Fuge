#include "../include/personnage.h"


Personnage::Personnage(std::string sprite_folder,float percentage_ , float vitesse_ , float weight_):
    Sprite("img/default.png"),
    percentage(percentage_),
    vitesse(vitesse_),
    weight(weight_),
    state(stationary)
{
   std::string anim_folder[3] = {"walk","static","run"};
   int anim_frame_n[3] = {1,1,1};
   for (int i = 0; i < 3; i++)
   {
        
        std::string anim_folder_i=sprite_folder+anim_folder[i];
        for (int k = 0; k < anim_frame_n[i]; k++)
        {
            std::string anim_frame=anim_folder_i+"/"+ std::to_string(k)+".png";
            Texture new_frame(anim_frame.c_str());
            animation[i].push_back(new_frame);
        }
        
   }
    
    setPosition(Vector2f(100,100));
    setSize(Vector2f(50,50));
    
}
void Personnage::update()
{
    
    mouvement=Vector2f(0,0);
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (1 == present)
    {
        
        int axes;
        const float *axesc = glfwGetJoystickAxes(GLFW_JOYSTICK_1,&axes);
        float deplacement=vitesse*axesc[0];
        mouvement.x=deplacement;
        

        
    }
    
    setTexture(animation[state][0]);
    move(mouvement);
}
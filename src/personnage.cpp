#include "../include/personnage.h"


Personnage::Personnage(std::string sprite_folder,float percentage_ , float vitesse_ , float weight_):
    Sprite("img/default.png"),
    percentage(percentage_),
    vitesse(vitesse_),
    weight(weight_),
    state(stationary)
{
   std::string anim_folder[6] = {"walk","static","run","jump","fall","dash"};
   int anim_frame_n[6] = {2,2,2,2,2,2};
   for (int i = 0; i < 6; i++)
   {
        
        std::string anim_folder_i=sprite_folder+anim_folder[i];
        for (int k = 0; k < anim_frame_n[i]; k++)
        {
            std::string anim_frame=anim_folder_i+"/"+ std::to_string(k)+".png";
            Texture new_frame(anim_frame.c_str());
            animation[i].push_back(new_frame);
        }
        
   }
    pr_time=glfwGetTime();
    weight=5;
    vitesse=7;
    setPosition(Vector2f(100,100));
    setSize(Vector2f(50,50));
    
}
void Personnage::update()
{
    bool invincible=false;
    int cpt=0;
    int framecount=0;
    mouvement=Vector2f(0,0);
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (1 == present)
    {
        
        int axes;
        const float *axesc = glfwGetJoystickAxes(GLFW_JOYSTICK_1,&axes);
        if (axesc[0]>0.1 || axesc[0]<-0.1)
        {
            std::cout<<"c"<<std::endl;
            mouvement.x=vitesse*axesc[0];
        }
        if (getPosition().x<50 )
        {
            setPosition(Vector2f(50,getPosition().y));
        }
        if (getPosition().x>950 )
        {
            setPosition(Vector2f(950,getPosition().y));
        }
        
        
        if(axesc[5]>0.1)
        {
            float acjump_time=glfwGetTime();
            if (acjump_time-pr_time>3)
            {
                mouvement.y=mouvement.y-(vitesse/weight);
                pr_time=glfwGetTime();
                std::cout<<"kys"<<std::endl;
                cpt=0;
                state=jump;
            }
            
        }
        if(axesc[3]>0.1 && axesc[0]>0.1)
        {
            
            mouvement.y=mouvement.x+50;
            pr_time=glfwGetTime();
            std::cout<<"kys"<<std::endl;
            cpt=0;
            state=dash;        
            
        }
        if(axesc[3]>0.1 && axesc[0]<-0.1)
        {
            
            mouvement.y=mouvement.x-50;
            pr_time=glfwGetTime();
            std::cout<<"kys"<<std::endl;
            cpt=0;
            state=dash;        
            
        }
        
        if (getPosition().y<950 )
        {
            mouvement.y=mouvement.y+(weight/2);
            std::cout<<"ckk"<<std::endl;
            state=fall;
        }
        
        framecount++;
        
    }
    if (framecount==5)
    {
        framecount=0;
        cpt++;
    }

    std::cerr<<"beforz"<<std::endl;    
    setTexture(animation[state][1]);
    std::cerr<<"after"<<std::endl;
    move(mouvement);
    if (cpt==1)
    {
        cpt=0;
    }
}
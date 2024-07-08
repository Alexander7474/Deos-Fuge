#include "knight.h"
#include <GLFW/glfw3.h>

Knight::Knight()
        : Personnage()
{
        //attribut de base 
        //
        speed = 250.f;
        weight = 650.f;
        jump_force = 100.f;

        //attribut du sprite
        // 
        buildAnimCache(KNIGHT);
        setAutoUpdateCollision(true);
        setPosition(300.f,100.f);
        setSize(200.f,200.f); 
        setOrigin(100.f,200.0f);
        getCollisionBox().setOffsetY(Vector2f(110.f,0.f));
        rebuildCollisionBox();
        attack_box.setSize(50.f,50.f);
        attack_box.setOrigin(25.f,25.f);
        setRGBFilterState(true);
        setColor(255,255,255);
}

void Knight::Dash(double delta_time_)
{
        if(anim_t[dash] > glfwGetTime()-anim_start_t[dash]){
                mouvement.x=direction*speed*4*delta_time_;
        }else{
                state = fall;
        }
}

void Knight::Jump(double delta_time_)
{
        if(anim_t[jump] > glfwGetTime()-anim_start_t[jump]){
                mouvement.y= -jump_force*delta_time_;
        }else{
                state = fall;
        }
}

void Knight::Attack(double delta_time_)
{
        if(anim_t[attack] > glfwGetTime()-anim_start_t[attack]){
                //gestion de la box d'attack 
                attack_box.setPosition(getPosition().x+(17.5f*direction),getPosition().y-20.f);
                attack_box.setOffsetX(Vector2f(-25.f,-25.f));
                attack_box.setOffsetY(Vector2f(-35.f,5.f));
        }else{
                state = fall;
        }
}

void Knight::Light_attack(double delta_time_)
{
        if(anim_t[light_attack] > glfwGetTime()-anim_start_t[light_attack]){
                //gestion de la box d'attack 
                attack_box.setPosition(getPosition().x+(35.f*direction),getPosition().y-20.f);
                attack_box.setOffsetX(Vector2f(-10.f,-10.f));
                attack_box.setOffsetY(Vector2f(-55.f,5.f));
        }else{
                state = fall;
        }
}

void Knight::Hit(double delta_time_)
{ 
        if(anim_t[hit] > glfwGetTime()-anim_start_t[hit]){
                // hit frame en blanc
                if(int((anim_last_frame_t[hit]-glfwGetTime())*100) % 2 == 0){
                       setColor(255,255,255);
                }else{
                        setColor(255*3,255*3,255*3);
                }

                // mouvement 
                mouvement.x = -1*direction*percentage*5*delta_time_;
                mouvement.y = -1*percentage*0.5*delta_time_;
        }else{
                state = fall;
                setColor(255,255,255);
        }
}

void Knight::rebuildCollisionBox()
{
        if(direction == right){
                getCollisionBox().setOffsetX(Vector2f(80.f,95.f));
        }else{
                getCollisionBox().setOffsetX(Vector2f(95.f,80.f));
        }
}

float Knight::getPercentageToApply()
{
        switch(state){
                case attack: 
                        return 20.f;
                case light_attack:
                        return 15.f;
                default:
                        return 10.f;
        }
}

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
        buildAnimCache(DEFAULT_PERSO);
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
        if(dash_frame_cpt < anim_frame_n[dash]){
                mouvement.x=direction*speed*delta_time_;
                if(glfwGetTime()-last_frame_t[dash] > anim_frame_t[dash]){
                        dash_frame_cpt++;
                        last_frame_t[dash] = glfwGetTime();
                }
        }else{
                state = fall;
        }
        frame_cpt = dash_frame_cpt;
}

void Knight::Jump(double delta_time_)
{
        if(jump_frame_cpt < anim_frame_n[jump] && jump_cpt <= 2){
                mouvement.y= -jump_force*delta_time_;
                if(glfwGetTime()-last_frame_t[jump] > anim_frame_t[jump]){
                        jump_frame_cpt++;
                        last_frame_t[jump] = glfwGetTime();
                }
        }else{
                state = fall;
        }
        frame_cpt = jump_frame_cpt;
}

void Knight::Attack(double delta_time_)
{
        if(attack_frame_cpt < anim_frame_n[attack]){
                if(glfwGetTime()-last_frame_t[attack] > anim_frame_t[attack]){
                        attack_frame_cpt++;
                        last_frame_t[attack] = glfwGetTime();
                }
                //gestion de la box d'attack 
                attack_box.setPosition(getPosition().x+(17.5f*direction),getPosition().y-20.f);
                attack_box.setOffsetX(Vector2f(-25.f,-25.f));
                attack_box.setOffsetY(Vector2f(-35.f,5.f));
        }else{
                state = fall;
        }
        frame_cpt = attack_frame_cpt;
}

void Knight::Light_attack(double delta_time_)
{
        if(light_attack_frame_cpt < anim_frame_n[light_attack]){
                if(glfwGetTime()-last_frame_t[light_attack] > anim_frame_t[light_attack]){
                        light_attack_frame_cpt++;
                        last_frame_t[light_attack] = glfwGetTime();
                }
                //gestion de la box d'attack 
                attack_box.setPosition(getPosition().x+(35.f*direction),getPosition().y-20.f);
                attack_box.setOffsetX(Vector2f(-10.f,-10.f));
                attack_box.setOffsetY(Vector2f(-55.f,5.f));
        }else{
                state = fall;
        }
        frame_cpt = light_attack_frame_cpt;
}

void Knight::Hit(double delta_time_)
{ 
        if(hit_frame_cpt < anim_frame_n[hit]){
                if(int((last_frame_t[hit]-glfwGetTime())*100) % 2 == 0){
                        setColor(255,255,255);
                }else{
                        setColor(255*3,255*3,255*3);
                }
                mouvement.x=direction*(hit_frame_cpt)*delta_time_;
                if(glfwGetTime()-last_frame_t[hit] > anim_frame_t[hit]){
                        hit_frame_cpt++;
                        last_frame_t[hit] = glfwGetTime();
                }
        }else{
                state = fall;
        }
        frame_cpt = hit_frame_cpt;
}

void Knight::rebuildCollisionBox()
{
        if(direction == right){
                getCollisionBox().setOffsetX(Vector2f(80.f,95.f));
        }else{
                getCollisionBox().setOffsetX(Vector2f(95.f,80.f));
        }
}

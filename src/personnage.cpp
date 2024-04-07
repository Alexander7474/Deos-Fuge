#include "../include/personnage.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>


Personnage::Personnage(std::string sprite_folder,float percentage_ , float speed_ , float weight_):
    Sprite("img/default.png"),
    percentage(percentage_),
    speed(speed_),
    weight(weight_),
    state(fall)
{
  int anim_frame_n[5] = {1,1,1,1,1};
  for (int i = stationary; i <= fall; i++)
  {        
    std::string anim_folder_i=sprite_folder+std::to_string(i);
    for (int k = 0; k < anim_frame_n[i]; k++)
    {
      std::string anim_frame=anim_folder_i+"/"+ std::to_string(k)+".png";
      Texture new_frame(anim_frame.c_str());
      animation[i].push_back(new_frame);
    }
  }
  setPosition(100.f,100.f);
  setSize(50.f,50.f); 
  setOrigin(25.f,50.0f);
}

void Personnage::update(Map &map_)
{
  //gestion de la manette
  int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
  mouvement.x=0.f;
  if (1 == present)
  {
    int n_axes;
    int n_buttons;
    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1,&n_axes);
    const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &n_buttons);

    //perso axe y
      
    //application de la gravité
    if(state == fall){
      mouvement.y=mouvement.y+weight*0.00981;
      if(mouvement.y>10.0f)
        mouvement.y=10.0f;
    }
    //collision avec les plateforms
    for(long unsigned int i = 0; i < map_.getTiles().size() ; i++){
      if(map_.getTiles()[i].getCollisionBox().check(shapeCollisionBox)){
        mouvement.y=0;
        jump_cpt=0;
        if(state != dash)
          state=stationary;
        setPosition(getPosition().x,map_.getTiles()[i].getPosition().y);
      }
    }
    
    //perso axe x
    //
    //touche x de la manette
    if(buttons[2] == GLFW_PRESS){
      //std::cerr << "dash" << std::endl;
      if(dash_frame_cpt == 0)
        state = dash;
    }else{
      // si la touche de dash est relaché et que l'état est stationnaire(donc dash fini) on reset le compteur de frame de dash
      //std::cerr << "dash released" << std::endl;
      if(state == stationary){
        dash_frame_cpt = 0;
      }
    }

    //touche a de la manette
    if(buttons[0] == GLFW_PRESS){
      if(state!=jump && jump_frame_cpt == 0){
        jump_cpt++;
      }
      state = jump;
    }else{
      if(state != jump)
        jump_frame_cpt = 0;
      else
       state = fall;
    }

    // axes de la manette
    if(state != dash){
      if(axes[0] > 0.15){
        mouvement.x=speed*axes[0];
        direction=right;
        if(state==stationary)
          state=run;
      }
      if(axes[0] < -0.15){
        mouvement.x=speed*axes[0];
        direction=left;
        if(state==stationary)
          state=run;
      }
    }
    switch(state){
      case dash:
        // si le perso dash
        if(dash_frame_cpt <= 10){
          mouvement.x=direction*speed*4;
          dash_frame_cpt++;
        }else{
          state = fall;
        }
        break;
      case jump:
        //si le perso jump 
        if(jump_frame_cpt <= 10 && jump_cpt <= 2){
          mouvement.y=-5;
          jump_frame_cpt++;
        }else{
          state = fall;
        }
        break;
      default:
        break;
    }
  
    if(false){
      std::cerr << "0:  " << axes[0] << std::endl;
      std::cerr << "1:  " << axes[1] << std::endl;
      std::cerr << "2:  " << axes[2] << std::endl;
      std::cerr << "3:  " << axes[3] << std::endl;
      std::cerr << "4:  " << axes[4] << std::endl;
      std::cerr << "5:  " << axes[5] << std::endl;
    }
    if(false){
      for(int i = 0; i < n_buttons; i++){
        if (buttons[i] == GLFW_PRESS){
          std::cerr << i << std::endl;
        }
      }
    }
  }
 
  
  std::cerr << "state: " << state << std::endl;
  move(mouvement);
  setTexture(animation[0][0]);
}

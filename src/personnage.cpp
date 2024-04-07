#include "../include/personnage.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

Personnage::Personnage(int joystick_, std::string sprite_folder,float percentage_ , float speed_ , float weight_):
    Sprite("img/default.png"),
    percentage(percentage_),
    speed(speed_),
    weight(weight_),
    state(fall),
    direction(right),
    frame_cpt(0),
    dash_frame_cpt(0),
    jump_frame_cpt(0),
    jump_cpt(0),
    joystick(joystick_)
{
  anim_frame_n[stationary] = 3;anim_frame_n[run] = 4;anim_frame_n[dash] = 3;anim_frame_n[jump] = 3;anim_frame_n[fall] = 3;
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
  setPosition(300.f,100.f);
  setSize(100.f,100.f); 
  setOrigin(50.f,100.0f);
}

void Personnage::update(Map &map_)
{
  //gestion de la manette
  int present = glfwJoystickPresent(joystick);
  mouvement.x=0.f;
  if (1 == present)
  {
    int n_axes;
    int n_buttons;
    const float *axes = glfwGetJoystickAxes(joystick,&n_axes);
    const unsigned char *buttons = glfwGetJoystickButtons(joystick, &n_buttons);

    //y gravité et collision////////////////////////////////////////////////////////////////////////////////////////
    //application de la gravité si le personnage tombe
    if(state == fall){
      mouvement.y=mouvement.y+weight*0.00981;
      //limite de vitesse y
      if(mouvement.y>10.0f)
        mouvement.y=10.0f;
    }
    //collision avec les plateformes
    for(long unsigned int i = 0; i < map_.getTiles().size() ; i++){
      if(map_.getTiles()[i].getCollisionBox().check(shapeCollisionBox)){
        //reset du mouvement
        mouvement.y=0;
        //reset compteur de jump
        jump_cpt=0;
        // si le personngae ne dash pas il est considéré comme stationnaire
        if(state != dash)
          state=stationary;
        //on replace le personnage pour eviter les decalage avec la plateforme
        setPosition(getPosition().x,map_.getTiles()[i].getPosition().y);
      }
    }
  
    //touche de la manette////////////////////////////////////////////////////////////////////////////////////////////
    //touche x de la manette
    if(buttons[2] == GLFW_PRESS){
      //std::cerr << "dash" << std::endl;
      //si le personnage peut dash
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
      //si le personnage ne saute pas deja et que la touche et appuyé on considère qu'un saut est rajouté au compteur
      if(state!=jump && jump_frame_cpt == 0){
        jump_cpt++;
      }
      state = jump;
    }else{
      //si la touche de jump est relaché on reset le compteur de jump
      if(state != jump)
        jump_frame_cpt = 0;
      else
       state = fall;
    }

    // axes de la manette////////////////////////////////////////////////////////////////////////////////////////////////
    // le dash est le seule mouvement en x qui passe au dessus des deplacement en priorité donc on utilise les joystick uniquement si le personnage ne dash pas
    if(state != dash){
      if(axes[0] > 0.15){
        mouvement.x=speed*axes[0];
        if(direction==left){
          flipVertically();
          direction=right;
        }
        // si le personnage est stationnaire cela signifie q'il est au sol donc on peut changer son etat sur run
        if(state==stationary)
          state=run;
      }
      if(axes[0] < -0.15){
        mouvement.x=speed*axes[0];
        if(direction==right){
          flipVertically();
          direction=left;
        }
        // si le personnage est stationnaire cela signifie q'il est au sol donc on peut changer son etat sur run
        if(state==stationary)
          state=run;
      }
    }

    //gestion des jump et des dash///////////////////////////////////////////////////////////////////////////////////////
    switch(state){
      case dash:
        // si le perso dash
        if(dash_frame_cpt < anim_frame_n[dash]*frame_divisor){
          mouvement.x=direction*speed*4;
          dash_frame_cpt++;
        }else{
          state = fall;
        }
        frame_cpt = dash_frame_cpt;
        break;
      case jump:
        //si le perso jump 
        if(jump_frame_cpt < anim_frame_n[jump]*frame_divisor && jump_cpt <= 2){
          mouvement.y=-5;
          jump_frame_cpt++;
        }else{
          state = fall;
        }
        frame_cpt = jump_frame_cpt;
        break;
      default:
        break;
    }
  
    //debuging/////////////////////////////////////////////
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
    ////////////////////////////////////////////////////////
  }
  
  move(mouvement);
  // sécurité pour ne pas que frame_cpt dépasse le nombre de frame de l'anim qui va être joué
  if(frame_cpt/frame_divisor >= anim_frame_n[state])
    frame_cpt = 0;
  setTexture(animation[state][frame_cpt/5]);
  frame_cpt++;
}

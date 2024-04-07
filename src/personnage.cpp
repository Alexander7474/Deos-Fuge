#include "../include/personnage.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

Personnage::Personnage(perso_info personnage_info_):
    Sprite("img/default.png"),
    percentage(0),
    speed(personnage_info_.speed),
    weight(personnage_info_.weight),
    state(fall),
    direction(right),
    frame_cpt(0),
    dash_frame_cpt(0),
    jump_frame_cpt(0),
    jump_cpt(0)
{
  jump_call = false;
  std::string sprite_folder = personnage_info_.folder_path;
  anim_frame_n[stationary] = personnage_info_.anim_frame_n[stationary];
  anim_frame_n[run] = personnage_info_.anim_frame_n[run];
  anim_frame_n[dash] = personnage_info_.anim_frame_n[dash];
  anim_frame_n[jump] = personnage_info_.anim_frame_n[jump];
  anim_frame_n[fall] = personnage_info_.anim_frame_n[fall];
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
  if(jump_call){
    jump_call = false;
  }else{
    // si il n'y a pas de jump call le jump frame est reset
    if(state != jump)
      jump_frame_cpt = 0;
    else // pas de jump call ni de jump state en cours le perso tombe
      state = fall;
  }

  if(dash_call){
    dash_call = false;
  }else{
    // reset du dash uniquement quand le pero est au sol et sans dash call
    if(state==stationary || state == run)
      dash_frame_cpt = 0;
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

  //y gravité et collision////////////////////////////////////////////////////////////////////////////////////////
  //application de la gravité si le personnage tombe
  if(state == fall){
    mouvement.y=mouvement.y+weight*0.00981; // utilser les attribut de la map
    //limite de vitesse y
    if(mouvement.y>10.0f)
      mouvement.y=10.0f;
  }
  //collision avec les plateformes
  for(long unsigned int i = 0; i < map_.getTiles().size() ; i++){
    if(map_.getTiles()[i].getCollisionBox().check(shapeCollisionBox) && state != jump){
      //reset du mouvement 
      mouvement.y=0;
      //reset compteur de jump
      jump_cpt=0;
      // si le personngae ne dash pas il est considéré comme stationnaire
      if(state == fall)
        state=stationary;
      //on replace le personnage pour eviter les decalage avec la plateforme
      setPosition(getPosition().x,map_.getTiles()[i].getPosition().y);
    }
  }

  if(state == run && mouvement.x == 0.f)
    state = stationary;
 
  move(mouvement);
  // sécurité pour ne pas que frame_cpt dépasse le nombre de frame de l'anim qui va être joué
  if(frame_cpt/frame_divisor >= anim_frame_n[state])
    frame_cpt = 0;
  setTexture(animation[state][frame_cpt/5]);
  frame_cpt++;
  mouvement.x=0.f;

  std::cerr << state << std::endl;
}

void Personnage::goRight(float value)
{
  // le dash est le seule mouvement en x qui passe au dessus des deplacement en priorité donc on utilise les joystick uniquement si le personnage ne dash pas
  if(state != dash){
    mouvement.x=speed*value;
    if(direction==left){
      flipVertically();
      direction=right;
    }
    // si le personnage est stationnaire cela signifie q'il est au sol donc on peut changer son etat sur run
    if(state==stationary)
      state=run;
  }
}

void Personnage::goLeft(float value)
{
  // le dash est le seule mouvement en x qui passe au dessus des deplacement en priorité donc on utilise les joystick uniquement si le personnage ne dash pas
  if(state != dash){
    mouvement.x=speed*value;
    if(direction==right){
      flipVertically();
      direction=left;
    }
    // si le personnage est stationnaire cela signifie q'il est au sol donc on peut changer son etat sur run
    if(state==stationary)
      state=run;
  }
}

// appelle pour faire sauter le personnage
void Personnage::doJump()
{
  //si le personnage ne saute pas deja on considère qu'un saut est rajouté au compteur
  if(state!=jump && jump_frame_cpt == 0)
    jump_cpt++;
  // limite de deux jump 
  if(jump_cpt <= 2)
    state = jump;
  // utilisé dans update pour savoir si il y a un appelle de saut dans la frame
  jump_call = true;
}

// appelle pour faire dash le personnage
void Personnage::doDash()
{
  // dash seulement si le dash est reset
  if(dash_frame_cpt == 0)
    state = dash;
  // utilisé dasn update pour savoir si il y a un appelle de dash dans la frame
  dash_call = true;
}

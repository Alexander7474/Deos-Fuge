#include "personnage.h"
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>

Personnage::Personnage():
    Sprite("img/default.png"),
    percentage(0),
    state(fall),
    direction(right),
    fall_start_t(glfwGetTime()),
    frame_cpt(0),
    dash_frame_cpt(0),
    jump_frame_cpt(0),
    jump_cpt(0),
    hit_frame_cpt(0),
    attack_box(getCollisionBox())
{}

void Personnage::updatePersonnage(double delta_time_, Map *map_)
{
  // gestion des états entrants
  switch (calling_state) {
    case dash:
      // dash seulement si le dash est reset
      if(dash_frame_cpt == 0)
        state = dash;
      break;
    case jump:
      //si le personnage ne saute pas deja on considère qu'un saut est rajouté au compteur
      if(state!=jump && jump_frame_cpt == 0)
        jump_cpt++;
      // limite de deux jump 
      if(jump_cpt <= 2)
        state = jump;
      break;
    case light_attack:
      if(light_attack_frame_cpt == 0){
        state=light_attack;
      }
      break;
    case attack:
      if(attack_frame_cpt == 0){
        state=attack;
      }
      break;
    case hit:
      if(hit_frame_cpt == 0){
        state = hit;
      }
      break;
    case run:
      // si le personnage est stationnaire cela signifie q'il est au sol donc on peut changer son etat sur run
      if(state==stationary)
        state=run;
    default:
      // si il n'y a pas de jump call le jump frame est reset
      if(state != jump)
        jump_frame_cpt = 0;
      else // pas de jump call ni de jump state en cours le perso tombe
        state = fall;
      // reset du dash uniquement quand le pero est au sol et sans dash call
      if(state==stationary || state == run){
        dash_frame_cpt = 0;
        light_attack_frame_cpt = 0;
        attack_frame_cpt = 0;
      }
      if(state != hit)
        hit_frame_cpt = 0;
      break;
  }

  //gestion des états///////////////////////////////////////////////////////////////////////////////////////
  switch(state){
    case dash:
      // si le perso dash
      Dash(delta_time_);
      break;
    case jump:
      //si le perso jump 
      Jump(delta_time_);
      break;
    case light_attack:
      // si le perso fais une attaque légère
      Light_attack(delta_time_);
      break;
    case attack:
      // si le perso fais une attaque
      Attack(delta_time_);
      break;
    case hit:
      //si le perso est touché
      Hit(delta_time_);
      break;
    default:
      break;
  }

  //application de la gravité si le personnage tombe
  double fall_time = glfwGetTime() - fall_start_t;
  mouvement.y += fall_time * delta_time_ * weight * delta_time_; // utilser les attribut de la map
  
  if(mouvement.y > weight * delta_time_)
    mouvement.y= weight * delta_time_;
  
  move(mouvement);

  //collision avec les plateformes
  bool isInCollision = false;
  for(long unsigned int i = 0; i < map_->getTiles().size() ; i++){
    if(map_->getTiles()[i].getCollisionBox().check(shapeCollisionBox)){
      //reset du mouvement 
      mouvement.y=0;
      //reset compteur de jump
      jump_cpt=0;
      // si le personngae ne dash pas il est considéré comme stationnaire
      if(state == fall){
        state=stationary;
      }
      fall_start_t = glfwGetTime();
      //on replace le personnage pour eviter les decalage avec la plateforme
      setPosition(getPosition().x,map_->getTiles()[i].getPosition().y);
      isInCollision = true;
    }
  }

  // check pour éviter des situations anormale
  if(mouvement.x >= -0.1f && mouvement.x <= 0.1f) mouvement.x = 0.f;  
  if(!isInCollision && state == run) state = fall;
  if(state == run && mouvement.x == 0.f) state = stationary;
  
  // application du vecteur de mouvement final
  move(mouvement);

  // gestion des frames et de l'animation
  // sécurité pour ne pas que frame_cpt dépasse le nombre de frame de l'anim qui va être joué
  if(frame_cpt/frame_divisor >= anim_frame_n[state])
    frame_cpt = 0;
  setTexture(animation[state][frame_cpt/frame_divisor]);
  if(glfwGetTime()-last_frame_t[state] > anim_frame_t[state]){
    frame_cpt++;
    last_frame_t[state] = glfwGetTime();
  }
  
  // reset du mouvement
  mouvement.x = mouvement.x/20.0;

  calling_state = stationary;
}

void Personnage::goRight(double delta_time_, float value)
{
  // le dash est le seule mouvement en x qui passe au dessus des deplacement en priorité donc on utilise les joystick uniquement si le personnage ne dash pas
  if(state != dash){
    mouvement.x=speed*value*delta_time_;
    if(direction==left){
      flipVertically();
      direction=right;
      rebuildCollisionBox();
    }
  }
  calling_state=run;
}

void Personnage::goLeft(double delta_time_, float value)
{
  // le dash est le seule mouvement en x qui passe au dessus des deplacement en priorité donc on utilise les joystick uniquement si le personnage ne dash pas
  if(state != dash){
    mouvement.x=speed*value*delta_time_;
    if(direction==right){
      flipVertically();
      direction=left;
      rebuildCollisionBox();
    }
  }
  calling_state = run;
}

void Personnage::doHit(int dir)
{
  calling_state = hit;
  if(direction != static_cast<perso_direction>(dir))
    flipVertically();
  direction = static_cast<perso_direction>(dir);
  rebuildCollisionBox();
}

int Personnage::getState()
{
  return state;
}

int Personnage::getDirection()
{
  return direction;
}

const CollisionBox &Personnage::getAttackBox() const
{
  return attack_box;
}

void Personnage::rebuildCollisionBox()
{
  if(direction == right){
    getCollisionBox().setOffsetX(Vector2f(75.f,90.f));
  }else{
    getCollisionBox().setOffsetX(Vector2f(90.f,75.f));
  }
}

void Personnage::buildAnimCache(perso_info info_)
{
  std::string sprite_folder = info_.folder_path;
  for (int i = stationary; i <= hit; i++)
  {        
    std::string anim_file=sprite_folder+std::to_string(i)+".png";
    anim_frame_n[i] = info_.anim_frame_n[i];
    anim_frame_t[i] = info_.anim_frame_t[i];
    last_frame_t[i] = glfwGetTime();
    animation[i] = bbopLoadSpriteSheet(anim_file.c_str(), 1, anim_frame_n[i]);
  }

}

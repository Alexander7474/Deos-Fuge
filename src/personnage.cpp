#include "personnage.h"
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <GLFW/glfw3.h>
#include <cmath>
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
    jump_cpt(0),
    hit_frame_cpt(0),
    attack_box(getCollisionBox())
{
  std::string sprite_folder = personnage_info_.folder_path;
  for (int i = stationary; i <= hit; i++)
  {        
    std::string anim_file=sprite_folder+std::to_string(i)+".png";
    anim_frame_n[i] = personnage_info_.anim_frame_n[i];
    anim_frame_t[i] = personnage_info_.anim_frame_t[i];
    last_frame_t[i] = glfwGetTime();
    animation[i] = bbopLoadSpriteSheet(anim_file.c_str(), 1, anim_frame_n[i]);
  }
  setAutoUpdateCollision(true);
  setPosition(300.f,100.f);
  setSize(200.f,200.f); 
  setOrigin(100.f,200.0f);
  getCollisionBox().setOffsetY(Vector2f(100.f,0.f));
  rebuildCollisionBox();
  attack_box.follow(getCollisionBox());
  attack_box.setOffsetX(Vector2f(25.f,25.f));
  attack_box.setOffsetY(Vector2f(0.f,20.f));
}

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
      if(dash_frame_cpt < anim_frame_n[dash]*frame_divisor){
        mouvement.x=direction*speed*delta_time_;
        mouvement.y=0.01*weight*delta_time_;
        if(glfwGetTime()-last_frame_t[dash] > anim_frame_t[dash]){
          dash_frame_cpt++;
          last_frame_t[dash] = glfwGetTime();
        }
      }else{
        state = fall;
      }
      frame_cpt = dash_frame_cpt;
      break;
    case jump:
      //si le perso jump 
      if(jump_frame_cpt < anim_frame_n[jump]*frame_divisor && jump_cpt <= 2){
        mouvement.y=-500*delta_time_;
        if(glfwGetTime()-last_frame_t[jump] > anim_frame_t[jump]){
          jump_frame_cpt++;
          last_frame_t[jump] = glfwGetTime();
        }
      }else{
        state = fall;
      }
      frame_cpt = jump_frame_cpt;
      break;
    case light_attack:
      // si le perso fais une attaque légère
      if(light_attack_frame_cpt < anim_frame_n[light_attack]*frame_divisor){
        if(glfwGetTime()-last_frame_t[light_attack] > anim_frame_t[light_attack]){
          light_attack_frame_cpt++;
          last_frame_t[light_attack] = glfwGetTime();
        }
        attack_box.setPosition(getCollisionBox().getPosition().x+(direction*30.0f),getCollisionBox().getPosition().y);
      }else{
        state = fall;
      }
      frame_cpt = light_attack_frame_cpt;
      break;
    case attack:
      // si le perso fais une attaque
      if(attack_frame_cpt < anim_frame_n[attack]*frame_divisor){
        if(glfwGetTime()-last_frame_t[attack] > anim_frame_t[attack]){
          attack_frame_cpt++;
          last_frame_t[attack] = glfwGetTime();
        }
        attack_box.setPosition(getCollisionBox().getPosition().x+(direction*30.0f),getCollisionBox().getPosition().y);
      }else{
        state = fall;
      }
      frame_cpt = attack_frame_cpt;
      break;
    case hit:
      //si le perso est touché
      if(hit_frame_cpt < anim_frame_n[hit]*frame_divisor){
        mouvement.x=direction*(hit_frame_cpt)*delta_time_;
        if(glfwGetTime()-last_frame_t[hit] > anim_frame_t[hit]){
          hit_frame_cpt++;
          last_frame_t[hit] = glfwGetTime();
        }
      }else{
        state = fall;
      }
      frame_cpt = hit_frame_cpt;
      break;
    default:
      break;
  }

  //application de la gravité si le personnage tombe
  if(state != dash || state != jump){
    mouvement.y=mouvement.y+weight*0.00981*delta_time_; // utilser les attribut de la map
    //limite de vitesse y
    if(mouvement.y>0.1*weight)
      mouvement.y=0.1*weight;
  }

  //collision avec les plateformes
  bool isInCollision = false;
  for(long unsigned int i = 0; i < map_->getTiles().size() ; i++){
    if(map_->getTiles()[i].getCollisionBox().check(shapeCollisionBox) && state != jump){
      //reset du mouvement 
      mouvement.y=0;
      //reset compteur de jump
      jump_cpt=0;
      // si le personngae ne dash pas il est considéré comme stationnaire
      if(state == fall)
        state=stationary;
      //on replace le personnage pour eviter les decalage avec la plateforme
      setPosition(getPosition().x,map_->getTiles()[i].getPosition().y);
      isInCollision = true;
    }
  }

  // check pour éviter des situations anormale
  if(mouvement.x >= -0.1f && mouvement.x <= 0.1f) mouvement.x = 0.f;  
  if(!isInCollision && state == run) state = fall;
  if(state == run && mouvement.x == 0.f) state = stationary;
 
  // application du vecteur de mouvmeent final
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
  if(state == stationary || state == run || state == fall || state == jump){
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

// appelle pour faire sauter le personnage
void Personnage::doJump()
{
  calling_state = jump;
}

// appelle pour faire dash le personnage
void Personnage::doDash()
{
  calling_state = dash;
}

void Personnage::doLightAttack()
{
  calling_state = light_attack;
}

void Personnage::doAttack()
{
  calling_state = attack;
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

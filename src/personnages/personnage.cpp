#include "personnage.h"
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <climits>
#include <iostream>
#include <ostream>
#include <string>

Personnage::Personnage():
    Sprite("assets/default.png"),
    percentage(0),
    frame_cpt(0),
    state(fall),
    direction(right),
    fall_start_t(glfwGetTime()),
    attack_box(getCollisionBox())
{}

void Personnage::updatePersonnage(double delta_time_, Map *map_)
{
  // gestion des états entrants
  if(state != hit && !call_hit){
    switch (calling_state) {
      case dash:
        // dash seulement si le dash est reset
        if(anim_frame_cpt[dash] == 0 && state != dash){
          state = dash;
          anim_last_frame_t[dash] = glfwGetTime();
          anim_start_t[state] = glfwGetTime();
        }
        break;
      case jump:
        // limite de deux jump 
        if(state != jump && state != fall && anim_frame_cpt[jump] == 0) {
          state = jump;
          anim_last_frame_t[jump] = glfwGetTime();
          anim_start_t[state] = glfwGetTime();
        }
        break;
      case light_attack:
        if(anim_frame_cpt[light_attack] == 0 && state != light_attack){
          state=light_attack;
          anim_last_frame_t[light_attack] = glfwGetTime();
          anim_start_t[state] = glfwGetTime();
        }
        break;
      case attack:
        if(anim_frame_cpt[attack] == 0 && state != attack){
          state=attack;
          anim_last_frame_t[attack] = glfwGetTime();
          anim_start_t[state] = glfwGetTime();
        }
        break;
      case run:
        // si le personnage est stationnaire cela signifie q'il est au sol donc on peut changer son etat sur run
        if(state==stationary && state != run){
          state=run;
          anim_last_frame_t[run] = glfwGetTime();
        }
      default:
        // si il n'y a pas de jump call le jump frame est reset
        if(state != jump){
          anim_frame_cpt[jump] = 0;
        }else{ // pas de jump call ni de jump state en cours le perso tombe
          state = fall;
        }
        // reset du dash uniquement quand le pero est au sol et sans dash call
        if(state==stationary || state == run){
          anim_frame_cpt[dash] = 0;
          anim_frame_cpt[light_attack] = 0;
          anim_frame_cpt[attack] = 0;
        }
        if(state != hit)
          anim_frame_cpt[hit] = 0;
        if(calling_state != run && state == run)
          state = stationary;
        break;
    }
  }

  if(call_hit){
    if(state != hit){
      state = hit;
      anim_last_frame_t[hit] = glfwGetTime();
      anim_start_t[state] = glfwGetTime();
      call_hit = false;
    }
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

  // application du vecteur de mouvement final
  //std::cerr << "y: " << mouvement.y << std::endl;
  //std::cerr << "x: " << mouvement.x << std::endl;
  move(mouvement.x*delta_time_,mouvement.y*delta_time_);
  
  mouvement.y += weight * delta_time_; // utilser les attribut de la map

  //collision avec les plateformes
  bool isInCollision = false;
  for(CollisionBox box : map_->getCollision()){
    if(shapeCollisionBox.check(box)){

      //std::cerr << "box y: " << box.getPosition().y << std::endl;
      //std::cerr << "pos y: " << pos.y << std::endl;

      isInCollision = true;

      float overlap_left = box.getRight() - shapeCollisionBox.getLeft();
      float overlap_right = shapeCollisionBox.getRight() - box.getLeft();
      float overlap_top = box.getBottom() - shapeCollisionBox.getTop() - 8.f;
      float overlap_bottom = shapeCollisionBox.getBottom() - box.getTop()-8.f;

      float min_overlap = std::min({overlap_right, overlap_left, overlap_top, overlap_bottom});

      if(min_overlap == overlap_top){
        move(0.f,box.getBottom() - shapeCollisionBox.getTop()+0.1f);
        anim_start_t[jump] = glfwGetTime()-anim_t[jump];
        mouvement.y = 0.f;
      }else if(min_overlap == overlap_bottom) {
        move(0.f,-(shapeCollisionBox.getBottom()-box.getTop()));
        if(state == fall){
          state=run;
        }
        mouvement.y = 0.f;
      }else if(min_overlap == overlap_right) {
        move(-(shapeCollisionBox.getRight() - box.getLeft()), 0.f);

      }else if (min_overlap == overlap_left) {
        move(box.getRight() - shapeCollisionBox.getLeft(), 0.f);
      }
  
    }

  }
 
  // check pour éviter des situations anormale
  if(!isInCollision && state == run) state = fall;
  if(state == run && mouvement.x == 0.f) state = stationary;

  // gestion des frames et de l'animation
  if(anim_frame_t[state] < glfwGetTime()-anim_last_frame_t[state]){
    anim_frame_cpt[state]++;
    anim_last_frame_t[state] = glfwGetTime();
  }

  if(anim_frame_cpt[state] >= anim_frame_n[state])
    anim_frame_cpt[state] = 0;

  setTexture(animation[state][anim_frame_cpt[state]]);
  
  // reset du mouvement
  mouvement.x = mouvement.x * direction;
  mouvement.x = (mouvement.x > delta_time_*inertia) ? mouvement.x - (delta_time_ * inertia): 0.f;
  mouvement.x = mouvement.x * direction;

  calling_state = stationary;
}

void Personnage::goRight(double delta_time_, float value)
{
  // le dash est le seule mouvement en x qui passe au dessus des deplacement en priorité donc on utilise les joystick uniquement si le personnage ne dash pas
  if(state != dash && !isAttacking()){
    mouvement.x=speed*value;
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
  if(state != dash && !isAttacking()){
    mouvement.x=speed*value;
    if(direction==right){
      flipVertically();
      direction=left;
      rebuildCollisionBox();
    }
  }
  calling_state = run;
}

void Personnage::doHit(int dir, int percentage_)
{
  call_hit = true;
  percentage += percentage_;
  //std::cerr << "hit: " << percentage << std::endl;
  if(direction == static_cast<perso_direction>(dir))
    flipVertically();
  direction = static_cast<perso_direction>(-1*dir);
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
    anim_t[i] = info_.anim_t[i];
    anim_last_frame_t[i] = glfwGetTime();
    anim_start_t[i] = glfwGetTime();
    anim_frame_cpt[i] = 0;
    animation[i] = bbopLoadSpriteSheet(anim_file.c_str(), 1, anim_frame_n[i]);
  }
}

bool Personnage::isAttacking()
{
        return (state == attack || state == light_attack);
}

int Personnage::getPercentageToApply()
{
        return 10;
}

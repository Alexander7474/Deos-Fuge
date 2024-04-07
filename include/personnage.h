#pragma once
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <vector>
#include <iostream>
#include "map.h"

//enumeration des états possible du personnage
enum perso_state:int{
  stationary=0,
  run=1,
  dash=2,
  jump=3,
  fall=4
};

//enumeration des direction possible du personnage
enum perso_direction:int{
  left=-1,
  right=1
};

class Personnage : public Sprite
{
private:
  
  // caractèristique du personnage
  float percentage; //<! pourcentage de dégats 
  float speed; //<! vitesse
  float weight; //<! poid

  //stockage des animations
  std::vector<Texture> animation[6]; //<! stockage des anims

  // gestion des états du personnage
  perso_state state; //<! etat général
  perso_direction direction; //<! direction 
  Vector2f mouvement; //<! mouvement du personnage
  int dash_frame_cpt; //<! compteur de frame du dash
  int jump_frame_cpt; //<! compteur de frame du saut
  int jump_cpt; //<! compteur de jump

public:
  Personnage(std::string sprite_folder,float percentage_, float vitesse_ , float weight_);
  void update(Map &map_);
  //virtual void attack(Personnage Ennemy[], GLFWwindow *);
  //virtual bool gettinghit();
  //virtual void Dash();
};

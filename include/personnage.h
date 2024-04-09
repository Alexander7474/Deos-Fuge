#pragma once
#include <BBOP/Graphics.h>
#include <vector>
#include <iostream>
#include "map.h"
#include "perso_info.h"

static const int frame_divisor = 7;

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

/**
 * @class Personnage
 *
 * @brief gère un personnage, a ne pas utiliser directement mais avec un Bot ou un Personnage
 */
class Personnage : public Sprite
{
private:
  
  // caractèristique du personnage
  float percentage; //<! pourcentage de dégats 
  float speed; //<! vitesse
  float weight; //<! poid

  //stockage des animations
  std::vector<Texture> animation[6]; //<! stockage des anims
  int anim_frame_n[6]; //<! nombre de frame en fonction de chaque anim

  // gestion des états du personnage
  perso_state state; //<! etat général
  perso_direction direction; //<! direction 
  Vector2f mouvement; //<! mouvement du personnage
  int frame_cpt; //<! compteur de frame par default
  int dash_frame_cpt; //<! compteur de frame du dash
  int jump_frame_cpt; //<! compteur de frame du saut
  int jump_cpt; //<! compteur de jump
  bool jump_call; //<! stock si le jump est appelé lors de la frame
  bool dash_call; //<! stock si le dash est appelé lors de la frame
  
public:
 
  /**
  * @brief Constructeur de personnage
  *
  * @param[in] personnage_info_ information sur le personnage 
  *
  * @note le personnage se créer a partir d'une struct perso_info qui est déclarer dans perso_info.h
  */ 
  Personnage(perso_info personnage_info_);

  /**
  * @brief met à jour le personnage à partir de ces attribut et de ce de la map
  */
  void update(Map &map_);

  /**
  * @brief Va a gauche
  *
  * @param[in] value -1.0 vitesse max - 0.0 immobile
  */
  void goLeft(float value);

  /**
  * @brief Va a droite
  *
  * @param[in] value 1.0 vitesse max - 0.0 immobile
  */
  void goRight(float value);

  /**
  * @brief Saute si Cela est possible
  */
  void doJump();

  /**
  * @brief Dash si cela est possible
  */
  void doDash();
  //virtual void attack(Personnage Ennemy[], GLFWwindow *);
  //virtual bool gettinghit();
  //virtual void Dash();
};

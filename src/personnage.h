#pragma once
#include <BBOP/Graphics.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <vector>
#include <iostream>
#include "map.h"
#include "perso_info.h"

static const int frame_divisor = 1;

//enumeration des états possible du personnage
enum perso_state:int{
  stationary=0,
  run=1,
  dash=2,
  jump=3,
  fall=4,
  light_attack=5,
  attack=6,
  hit=7
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
 * @todo utiliser une list de int pour stocker le compteur de frame des états
 */
class Personnage : public Sprite
{
private:
  
  // caractèristique du personnage
  float percentage; //<! pourcentage de dégats 
  float speed; //<! vitesse
  float weight; //<! poid
  float jump_force; //<! force de saut

  //stockage des animations
  std::vector<Texture> animation[8]; //<! stockage des anims
  int anim_frame_n[8]; //<! nombre de frame en fonction de chaque anim
  double anim_frame_t[8]; //<! temps entre chaque frame en fonction de chaque anim
  double last_frame_t[8]; //<! timing de la dernière frame de l'anim

  // gestion des états du personnage
  perso_state state; //<! etat général
  perso_state calling_state; //<! état demandé par les inputs donné à personnage
  perso_direction direction; //<! direction 
  Vector2f mouvement; //<! mouvement du personnages
  double fall_start_t; //<! timing du debut de la chute 
  int frame_cpt; //<! compteur de frame par default
  int dash_frame_cpt; //<! compteur de frame du dash
  int jump_frame_cpt; //<! compteur de frame du saut
  int jump_cpt; //<! compteur de jump
  int light_attack_frame_cpt; //<! compteur de frame de l'attaque légère  
  int attack_frame_cpt; //<! compteur de frame de l'attaque légère  
  int hit_frame_cpt; //<! compteur de frame de hit

  CollisionBox attack_box; //<! box de collision pour les attaques du personnage
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
   * @brief Destructeur de personnages
   */

  /**
  * @brief met à jour le personnage à partir de ces attribut et de ce de la map
  */
  void updatePersonnage(double delta_time_, Map *map_);

  /**
  * @brief Va a gauche
  *
  * @param[in] value -1.0 vitesse max - 0.0 immobile
  */
  void goLeft(double delta_time_, float value);

  /**
  * @brief Va a droite
  *
  * @param[in] value 1.0 vitesse max - 0.0 immobile
  */
  void goRight(double delta_time_, float value);

  /**
  * @brief Saute si Cela est possible
  */
  void doJump();

  /**
  * @brief Dash si cela est possible
  */
  void doDash();

  /**
  * @brief attack légère
  */
  void doLightAttack();

  /**
  * @brief attack
  */
  void doAttack();

  /**
   * @brief le personnage est touché
   */ 
  void doHit(int dir);

  /**
   * @brief renvoie l'état du personnage
   *
   * @return state état du personnage
   */
  int getState();

  /**
   * @brief renvoie la direction 
   *
   * @return direction 
   */
  int getDirection();

  /**
   * @brief permet de recalculer la boite de collision
   */
  void rebuildCollisionBox();

  /**
   * @brief renvoie la box d'attaque du personnage 
   *
   * @return attackBox box d'attaque
   */
  const CollisionBox &getAttackBox() const;

  //virtual void attack(Personnage Ennemy[], GLFWwindow *);
  //virtual bool gettinghit();
  //virtual void Dash();
};
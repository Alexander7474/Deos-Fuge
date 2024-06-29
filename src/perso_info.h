#pragma once 
#include <BBOP/Graphics/bbopMathClass.h>
#include <iostream>

/**
  * @brief Structure d'information pour build des Personnage
  * 
  * @details Cette structure est utiliser dans le constructeur de Personnage pour déterminer ces attributs
  * @details Elle permet de rapidement créer de nouveau personnage avec des caractèristique propre
  *
  * @see Personnage
  */
struct perso_info{
  std::string folder_path; //<! chemin d'accès vers le dossier de frame du personnage
  int anim_frame_n[8]; //<! nombre de frame par anim du personnage
  double anim_frame_t[8]; //<! temps entre chaque frame par anim du personnage
  float speed; //<! vitesse du personnage
  float weight; //<! force de gravité subit par le perso (détermine si le personnage va tomber plus ou moin vite)
  float jump_force; //<! force de saut du personnage
  Vector2f size; //<! taille du personnage
  Vector2f collision_box_offset[2]; //<! offset de la boite de collision  // 0 offset X et 1 offset Y
  Vector2f attack_box_offset[2]; //<! offset de la boite d'attack         // ^
};

extern const perso_info DEFAULT_PERSO; //<! Personnage de par default

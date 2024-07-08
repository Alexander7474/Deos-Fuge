#pragma once 
#include <BBOP/Graphics/bbopMathClass.h>
#include <iostream>

/**
  * @brief Structure d'information pour build des Personnage
  * 
  * @details Cette structure est utiliser dans la methode buildAnimCache de personnage
  * @details elle sert a stocker les infos sur les animations
  *
  * @see Personnage
  */
struct perso_info{
  std::string folder_path; //<! chemin d'accès vers le dossier de frame du personnage
  int anim_frame_n[8]; //<! nombre de frame par anim du personnage
  double anim_frame_t[8]; //<! temps entre chaque frame par anim du personnage
  double anim_t[8]; //<! temps total de l'anim du personnage
};

extern const perso_info WHITE_KNIGHT; //<! Personnage de par default
extern const perso_info KNIGHT; //<! Personnage de par default

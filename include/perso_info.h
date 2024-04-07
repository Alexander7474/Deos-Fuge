#pragma once 
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
  int anim_frame_n[6]; //<! nombre de frame par anim du personnage
  float speed; //<! vitesse du personnage
  float weight; //<! poid du personnage
};

extern const perso_info DEFAULT_PERSO; //<! Personnage de par default

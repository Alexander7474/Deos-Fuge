#pragma once

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
* @class Map "map.h" "map.cpp"
* @brief La class Map initialise une map avec un tableau de case en 16x16 depuis un fichier map contenant toute les cellules  
* @author Alexandre
* @version 0.1
*/
class Map : public BbopDrawable
{
private:
  std::vector<Sprite>* tiles; //!< Liste de Sprite alloué dynamiquement pour stocké les tuiles 16x16 de la map
  Sprite background; 
  
public:
  /**
  * @brief Constructeur par défault de Map.
  *
  * @see Map::Map();
  */
  Map();

  /**
  * @brief Constructeur de Map.
  * @overload
  * @param tiles_folder Chemin d'accès vers le dossier contenant les tuiles de la map
  * @note Le dossier doit contenir un fichier background.png, definition.bmc et un set de tuiles en fonctions de defintion.bmc
  *
  * @see Map::Map(const char* tiles_folder);
  */
  Map(const char* tiles_folder, const char* background_folder);

  /**
  * @brief Conctructeur par copie de Map
  * @param other Autre Map
  *
  * @see Map::Map(const Map& other);
  */
  Map(const Map& other);

  ~Map();

  /**
  * @brief Dessine la map sur l'écran, à utiliser avec une scene ou une autre méthode draw
  *
  * @see Map::Draw(GLint renderModLoc) const;
  */
  virtual void Draw(GLint renderModLoc) const override;

  void Remplissage(const char* tiles_folder);
};

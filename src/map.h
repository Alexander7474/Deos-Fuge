#pragma once

#include <BBOP/Graphics.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
* @class Map "map.h" "map.cpp"
* @brief La class Map initialise une map avec un tableau de case en 16x16 depuis un fichier map contenant toute les cellules  
* @author Alexandre
* @version 0.1
*/
class Map
{
private:
  std::vector<Sprite> tiles; //!< Vecteur de Sprite pour stocké les tuiles 16x16 de la map
  std::vector<CollisionBox> collision_layer; //!< stock les collision de la box
  std::vector<Vector2f> spawn_points; //<! different point de spawn de la map
  Sprite background; //!< Background du jeu
  
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
  * @param tiles_folder Chemin d'accès vers le dossier contenant les tuiles de la map de la forme img/map/glace/
  * @note Le dossier doit contenir un fichier background.png, definition.bmc et un set de tuiles en fonctions de defintion.bmc
  *
  * @see Map::Map(const char* tiles_folder);
  */
  Map(const char* map_folder);

  /**
  * @brief Conctructeur par copie de Map
  * @param other Autre Map
  *
  * @see Map::Map(const Map& other);
  */
  Map(const Map& other);

  /**
  * @brief Dessine la map sur l'écran, à utiliser avec une scene ou une autre méthode draw
  *
  * @see Map::Draw(GLint renderModLoc) const;
  */
  void Draw(Scene &scene, Camera &ground_camera);

  /**
  * @brief Ouvre le fichier defnition.bmm , puis définis les Sprites selon les valeurs du fichier et les ajoute au vecteur tiles
  * @param tiles_folder chemin d'accès des tuiles
  * @note colonne du fichier definition.bmm : 
  * @note colonne 1 : correspond à l'index du sprite 
  * @note colonne 2 : position x du premier sprite
  * @note colonne 3 : position x du dernier sprite
  * @note colonne 4 : position y du premier sprite
  * @note colonne 5 : position y du dernier sprite
  * @note les positions sont des multiples de 16 (tailles des blocks de sprite)
  *
  * @see Map::Remplissage(const char* tiles_folder);
  */
  void remplissage(const char* tiles_folder);

  /**
  * @brief Remplie un tableau d'entier correspondant aux index des sprites/blocks de la map qui se trouvent dans la zone d'explosion
  * @param position coordonnées x et y de l'explosion
  * @param zone périmètre de l'explosion
  * @note A faire pour plus tard : augmenter ou réduire la zone avec un nombre aléatoire
  * 
  * @see Map::IndexZone(Vector2f, float);
  */
  void indexZone(Vector2f position, float zone, int *tab, int &cpt);

  /**
  * @brief supprime les bloques qui sont dans la zone d'explosion
  * @param position coordonnées x et y de l'explosion
  * @param zone périmètre de l'explosion
  * @note les arguments sont récupérés par la fonction IndexZone(Vector2f position, float zone, int *tab, int &cpt)
  * 
  * @see Map::DestroyBlock(Vector2f position, float zone);
  */
  void destroyBlock(Vector2f position, float zone);

  std::vector<Sprite>& getTiles();
  std::vector<CollisionBox>& getCollision();
  std::vector<Vector2f>& getSpawnPoints();
};

#include "map.h"

#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <cstring>
#include <iostream>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>
#include <ostream>

using namespace std;

Map::Map() : Map("assets/map/test/"){}

Map::Map(const char* map_folder) :
    background(Texture((string(map_folder) + "background.png").c_str()))
{
    remplissage(map_folder);
    background.setRGBFilterState(true);
    background.setColor(100,100,100);
}

void Map::remplissage(const char* map_folder)
{  
  // on charge la map
  ldtk::Project ldtk_project;

  string ldtk_map_file = map_folder;
  ldtk_map_file += "map.ldtk";
  ldtk_project.loadFromFile(ldtk_map_file);

  const auto& world = ldtk_project.getWorld();
  const auto& level = world.getLevel("Level");
  const auto& layer = level.getLayer("Tile_layer");
  const auto& c_layer = level.getLayer("Collision_layer");

  // on charge le tile set de la map
  string tileset_file = map_folder;
  tileset_file += "tileset.png";
  vector<Texture> tileset = bbopLoadSpriteSheet(tileset_file.c_str(), 32, 12);

  // iteration pour récupérer les tiles
  for (const auto& tile : layer.allTiles()) {
    Sprite tile_spr(tileset[tile.tileId]);
    tile_spr.setPosition(tile.getWorldPosition().x,tile.getWorldPosition().y);
    tile_spr.setSize(8,8);
    tiles.push_back(tile_spr);
  }

  //stockge de info sur le layer de collision 
  int collision_box_size = c_layer.getCellSize();
  int temp_collision_layer[c_layer.getGridSize().y][c_layer.getGridSize().x];
  std::memset(temp_collision_layer, 0, sizeof(temp_collision_layer));

  // iteration pour recupérer les box et les mettre dans le tableau temporaire
  for (const auto& box : c_layer.getIntGridValPositions(1)) {
     temp_collision_layer[box.y][box.x] = 1;  
  }

  //on parcours le tableau temporaire pour ajouter les box a collision layer en créant de grande box sur l'axe y
  for(int i = 0; i < c_layer.getGridSize().y; i++){
    for(int y = 0; y < c_layer.getGridSize().x; y++){
      CollisionBox box;
      box.setPosition(y*collision_box_size,i*collision_box_size);
      box.setSize(collision_box_size,0.f);

      int col = i;
      while(temp_collision_layer[col][y] == 1){
        temp_collision_layer[col][y] = 0;
        box.setSize(collision_box_size,box.getSize().y+collision_box_size);
        col++;
      }

      if(box.getSize().y > 0.f)
        collision_layer.push_back(box);
    }

    //on collapse les box sur l'axe x 
    for(long unsigned int i = 1; i < collision_layer.size(); i++){
      CollisionBox* pre_box = &collision_layer[i-1];
      if(pre_box->getSize().y == collision_layer[i].getSize().y && collision_layer[i].getPosition().x == pre_box->getPosition().x+pre_box->getSize().x){
        pre_box->setSize(pre_box->getSize().x+collision_layer[i].getSize().x,pre_box->getSize().y);
        collision_layer.erase(collision_layer.begin() + i);
      }
    }
  }

}

void Map::Draw(Scene &scene, Camera &ground_camera)
{
  scene.useCamera(nullptr);
  scene.Draw(background);

  scene.useCamera(&ground_camera);
  for (Sprite& tile : tiles)
  {
    if(ground_camera.isInCamView(tile))
      scene.Draw(tile);
  }

  //for(const auto& box : collision_layer){
    //bbopDebugCollisionBox(box, scene);
  //}
}

void Map::indexZone(Vector2f position, float zone, int * tab, int &cpt)
{
    for (unsigned i=0; i<tiles.size(); i++)
    {
        if(tiles[i].getPosition().x >= position.x-zone && tiles[i].getPosition().x <= position.x+zone 
        && tiles[i].getPosition().y >= position.y-zone && tiles[i].getPosition().y <= position.y+zone) 
        {
            cout << i << endl;
            tab[cpt] = i;
            cpt++;
        }
    }
}


void Map::destroyBlock(Vector2f position, float zone)
{
    int * tab = new int;
    int N=0;
    indexZone(position, zone, tab, N);

    for(int i=0; i<N; i++)
    {
        tab[i] = tab[i] - i;
        tiles.erase(tiles.begin()+tab[i]);
    }

    cout << "bloque(s) supprimé(s)" << endl;
}

vector<Sprite>& Map::getTiles()
{
    return tiles;
}

vector<CollisionBox>& Map::getCollision()
{
    return collision_layer;
}

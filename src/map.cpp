#include "map.h"

#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/bbopMathClass.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <LDtkLoader/DataTypes.hpp>
#include <LDtkLoader/Tileset.hpp>
#include <cstring>
#include <iostream>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>
#include <ostream>

using namespace std;

Map::Map() : Map("assets/map/green_hills/"){}

Map::Map(const char* map_folder) :
    background(Texture("assets/default.png"))
{
    remplissage(map_folder);
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
  const auto& c_layer = level.getLayer("Collision_layer");

  //recuperation des infos du level 
  x_size = level.size.x;
  y_size = level.size.y;

  // on charge le tile set de la map
  ldtk::Tileset world_tileset = world.getTileset("Tileset");
  ldtk::FilePath tileset_path = world_tileset.path;
  string tileset_file = map_folder;
  tileset_file += tileset_path.directory() + tileset_path.filename();
  int rows =  world_tileset.texture_size.y/world_tileset.tile_size;
  int columns = world_tileset.texture_size.x/world_tileset.tile_size;
  vector<Texture> tileset = bbopLoadSpriteSheet(tileset_file.c_str(), rows, columns);

  //recuperation du background 
  if(level.hasBgImage()){
    ldtk::FilePath bg_path = level.getBgImage().path;
    string bg_file = map_folder + bg_path.directory() + bg_path.filename();
    background.setTexture(Texture(bg_file.c_str()));
  }else {
    std::string bg_file = map_folder;
    bg_file += "map/png/Level_bg.png";
    background.setTexture(Texture(bg_file.c_str()));
  }
  background.setSize(1280,720);
 
  // iteration pour récupérer les tiles
  for (const auto& layer : level.allLayers()){
    if(layer.getName() != "Collision_layer"){
      for (const auto& tile : layer.allTiles()) {
        Sprite tile_spr(tileset[tile.tileId]);

        tile_spr.setPosition(tile.getWorldPosition().x,tile.getWorldPosition().y);
        tile_spr.setSize(layer.getCellSize(),layer.getCellSize());

        if(tile.flipX)
          tile_spr.flipVertically();
        if(tile.flipY)
          tile_spr.flipHorizontally();

        tiles.push_back(tile_spr);
      }
    }
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

  //recuperation des point de spawn
  const auto& spawns = level.getField<ldtk::FieldType::ArrayFloat>("spawns");

  // iterate on the array field
  Vector2f temp_point;
  for (long unsigned int i = 0; i < spawns.size(); i++) {
    if (!spawns[i].is_null()) {
      if(i % 2 == 0){
        temp_point.x = spawns[i].value();
      }else{
        temp_point.y = spawns[i].value();
        spawn_points.push_back(temp_point);
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

  //for(CollisionBox& box : collision_layer){
    //if(ground_camera.isInCamView(box))
    //  bbopDebugCollisionBox(box, scene);
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

vector<Vector2f>& Map::getSpawnPoints()
{
    return spawn_points;
}

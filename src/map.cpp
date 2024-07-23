#include "map.h"

#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <iostream>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>

using namespace std;

Map::Map() : 
    background(Texture("assets/map/glace/background.png"))
{
    // remplissage du tableau
    remplissage("assets/map/test/");
    background.setRGBFilterState(true);
    background.setColor(100,100,100);
}

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
  const auto& level = world.getLevel("AutoLayer");
  const auto& layer = level.getLayer("IntGrid_layer");
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
  
  // iteration pour recupérer les box 
  for (const auto& box : c_layer.getIntGridValPositions(1)) {
    CollisionBox b;
    b.setPosition(box.x*32, box.y*32);
    b.setSize(32,32);
    Collision_layer.push_back(b);
  }
}

void Map::Draw(Scene &scene, Camera &ground_camera)
{
  scene.useCamera(nullptr);
  scene.Draw(background);

  scene.useCamera(&ground_camera);
  for (unsigned i=0; i<tiles.size(); i++)
  {
    scene.Draw(tiles[i]);
  }

  for(const auto& box : Collision_layer){
  }
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
    return Collision_layer;
}

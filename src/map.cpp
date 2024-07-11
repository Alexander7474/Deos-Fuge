#include "map.h"

#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/collisionBoxClass.h>
#include <BBOP/Graphics/textureClass.h>
#include <iostream>
#include <fstream>
#include <LDtkLoader/Project.hpp>
#include <LDtkLoader/Layer.hpp>

using namespace std;

Map::Map() : 
    background(Texture("assets/map/glace/background.png"))
{
    // remplissage du tableau
    remplissage("assets/map/glace/");
    background.setRGBFilterState(true);
    background.setColor(100,100,100);
}

Map::Map(const char* tiles_folder) :
    background(Texture((string(tiles_folder) + "background.png").c_str()))
{
    remplissage(tiles_folder);
}

void Map::remplissage(const char* tiles_folder)
{  
  ldtk::Project ldtk_project;
  ldtk_project.loadFromFile("assets/map/test/map.ldtk");

  vector<Texture> tileset = bbopLoadSpriteSheet("assets/map/test/tileset.png", 32, 12);

  const auto& world = ldtk_project.getWorld();
  const auto& level = world.getLevel("AutoLayer");
  const auto& layer = level.getLayer("IntGrid_layer");
  const auto& c_layer = level.getLayer("Collision_layer");

  // iterate on the tiles of the layer
  for (const auto& tile : layer.allTiles()) {
    Sprite tile_spr(tileset[tile.tileId]);
    tile_spr.setPosition(tile.getWorldPosition().x,tile.getWorldPosition().y);
    tile_spr.setSize(8,8);
    tiles.push_back(tile_spr);
  }
  
  std::cerr << "herer" << std::endl;
  if(c_layer.hasTileset())
    std::cerr << "uiqsfheui" << std::endl;
  // iterate on the tiles of the layer
  for (const auto& box : c_layer.getIntGridValPositions(1)) {
    CollisionBox b;
    b.setPosition(box.x*32, box.y*32);
    b.setSize(32,32);
    std::cerr << "pos: " << b.getPosition().x << " " << b.getPosition().y << " size: " << b.getSize().x << " " << b.getSize().y << std::endl;
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
    bbopDebugCollisionBox(box, scene);
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

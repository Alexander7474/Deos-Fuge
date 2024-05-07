#include "../include/map.h"
#include <BBOP/Graphics/cameraClass.h>
#include <iostream>
#include <fstream>
using namespace std;

Map::Map() : 
    background(Texture("img/map/glace/background.png"))
{
    // remplissage du tableau
    remplissage("img/map/glace/");
}

Map::Map(const char* tiles_folder) :
    background(Texture((string(tiles_folder) + "background.png").c_str()))
{
    remplissage(tiles_folder);
}

void Map::remplissage(const char* tiles_folder)
{
    ifstream fichier(string(tiles_folder) + "definition.bmm");

    string path;
    int x, x_final, y, y_final;

    if (fichier)
    {
        string mot;

        for(int b=0; b<9; b++)
        {
            fichier >> mot;
            path = string(tiles_folder) + "tiles/" + mot + ".png";

            fichier >> mot;
            x = stoi(mot);

            fichier >> mot;
            x_final = stoi(mot);

            fichier >> mot;
            y = stoi(mot);

            fichier >> mot;
            y_final = stoi(mot);

            cout << path << " " << x << " " << x_final << " " << y << " " << y_final << endl;

            for(int j=y; j<=y_final; j+=16)
            {
                for(int i=x; i<=x_final; i+=16)
                {
                    Sprite sprite(Texture(path.c_str()));
                    sprite.setPosition(Vector2f(i, j));
                    tiles.push_back(sprite);
                    std::cerr << "tuile added" << std::endl;
                }
            }
        }

        cout << "Vecteur de Sprite remplie avec succès" << endl;
    }

    else
    {
        cout << "Erreur Impossible d'ouvrir le fichier" << endl;
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

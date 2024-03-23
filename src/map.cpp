#include "../include/map.h"

Map::Map() : background(Texture("img/background/ciel.png")), tiles(new Sprite*[45])
{
    // initialisation du tableau dynamique 2D
    for(int i=0; i<45; i++)
    {
        tiles[i] = new Sprite[80];
    }


    // remplissage du tableau
    Remplissage();
}

Map::~Map()
{
    for(int i=0; i<45; i++)
    {
        delete[] tiles[i];
    }
    delete[] tiles;
    std::cout << "tableau de tiles supprimer avec succès" << std::endl;
}

void Map::Remplissage()
{
    for(int i=0; i<45; i++)
    {
        for(int j=0; j<80; j++)
        {
            std::string tiles_folder = "img/map/" + std::to_string(16*i) + "-" + std::to_string(16*j) + ".png" ;
            std::cout << tiles_folder << std::endl;

            Sprite sprite(Texture(tiles_folder.c_str()));
            sprite.setSize(Vector2f(16, 16));
            sprite.setOrigin(Vector2f(0, 0));
            sprite.setPosition(Vector2f(i*16, j*16));

            tiles[i][j] = sprite;
        }
    }
    std::cout << "tableau de tiles remplies avec succès" << std::endl;
}

void Map::Draw(GLint renderModLoc) const
{
    std::cout << "test Draw" << std::endl;
    background.Draw(renderModLoc);
    std::cout << "background dessiné" << std::endl;

    for(int i=0; i<45; i++)
    {
        for(int j=0; j<80; j++)
        {
            tiles[i][j].Draw(renderModLoc);
        }
    } 
    std::cout << "map dessiné" << std::endl;
}



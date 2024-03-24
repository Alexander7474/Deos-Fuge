#include "../include/map.h"

Map::Map() : 
    background(Texture("img/background/ciel.png")), 
    tiles(new std::vector <Sprite>[45])
{
    // remplissage du tableau
    Remplissage("img/map/");
}

Map::Map(const char* tiles_folder, const char* background_folder) :
    background(Texture(background_folder)), 
    tiles(new std::vector <Sprite>[45])
{
    Remplissage(tiles_folder);
}

Map::~Map()
{
    delete[] this->tiles;
    std::cout << "vecteur de sprite dynamique désaloué avec succès" << std::endl;
}

void Map::Remplissage(const char* tiles_folder)
{
    for(int i=0; i<45; i++)
    {
        for(int j=0; j<80; j++)
        {
            std::string path = tiles_folder + std::to_string(16*i) + "-" + std::to_string(16*j) + ".png" ;
            std::cout << path << std::endl;

            Sprite sprite(Texture(path.c_str()));
            sprite.setSize(Vector2f(16, 16));
            sprite.setOrigin(Vector2f(0, 0));
            sprite.setPosition(Vector2f(j*16, i*16));
            tiles[i].push_back(sprite);
        }
    }
    std::cout << "tableau de tiles remplies avec succès" << std::endl;
}

void Map::Draw(GLint renderModLoc) const
{
    background.Draw(renderModLoc);

    for(int i=0; i<45; i++)
    {
        for(int j=0; j<80; j++)
        {
            tiles[i][j].Draw(renderModLoc);
        }
    } 

    std::cout << "map dessiné avec succès" << std::endl;
}



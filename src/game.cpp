#include "../include/game.h"


Game::Game(Map &map_, std::vector<Personnage> &personnages_, std::vector<Personnage> &bots_):
   game_state(start),
   personnages(personnages_),
   bots(bots_)
{
  map = map_;
}

void Game::update()
{
  for(long unsigned int i = 0; i < personnages.size(); i++){
    personnages[i].update();
  }
  for(long unsigned int i = 0; i < bots.size(); i++){
    bots[i].update();
  }
}

void Game::Draw()
{
  scene.Use();
  scene.Draw(map);
  for(long unsigned int i = 0; i < personnages.size(); i++){
    scene.Draw(personnages[i]);
  }
  for(long unsigned int i = 0; i < bots.size(); i++){
    scene.Draw(bots[i]);
  }
}

#include "../include/game.h"
#include <BBOP/Graphics/bbopFunc.h>


Game::Game(Map &map_, std::vector<Player> &players_, std::vector<Personnage> &bots_):
   game_state(start),
   players(players_),
   bots(bots_)
{
  map = map_;
}

void Game::update()
{
  for(long unsigned int i = 0; i < players.size(); i++){
    players[i].update(map);
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //bots[i].update();
  //}
}

void Game::Draw()
{
  scene.Use();
  scene.Draw(map);
  for(long unsigned int i = 0; i < players.size(); i++){
    scene.Draw(players[i]);
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //scene.Draw(bots[i]);
  //}
}

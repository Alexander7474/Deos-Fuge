#include "../include/game.h"
#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/fontsClass.h>
#include <GLFW/glfw3.h>
#include <string>

Game::Game(Map &map_, std::vector<Player> &players_, std::vector<Personnage> &bots_):
   game_state(start),
   font(48, "font/GohuFont11NerdFont-Regular.ttf"),
   players(players_),
   bots(bots_),
   fps_hud("0", nullptr),
   last_time(glfwGetTime()),
   fps_counter(0)
{
  map = map_;
  fps_hud.setFont(&font);
  fps_hud.setPosition(Vector2f(0.0f,50.f));
}

void Game::update()
{
  for(long unsigned int i = 0; i < players.size(); i++){
    players[i].update(map);
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //bots[i].update();
  //}
  updateHUD();
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
  scene.Draw(fps_hud);
}

void Game::updateHUD()
{
  double actual_time = glfwGetTime();
  double delta_time = actual_time-last_time;
  fps_counter++;
  std::string final_fps = std::to_string(fps_counter/delta_time);
  if (delta_time >= 1.0){
    fps_counter = 0;
    last_time = actual_time;
  }
  fps_hud.setTexte(final_fps.c_str());
}

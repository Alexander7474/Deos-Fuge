#include "../include/game.h"
#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/fontsClass.h>
#include <GLFW/glfw3.h>
#include <string>

Game::Game(GLFWwindow*& window_, Map &map_, std::vector<Player> &players_, std::vector<Personnage> &bots_):
   cam_scale_goal(0.33f),
   game_state(start),
   players(players_),
   bots(bots_),
   window(window_),
   font(48, "font/GohuFont11NerdFont-Regular.ttf"),
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
  Vector2f max_x(9999.f,0.f);
  Vector2f max_y(9999.f,0.f);
  for(long unsigned int i = 0; i < players.size(); i++){
    players[i].update(map);
    Vector2f player_pos = players[i].getPosition();
    if(player_pos.x > max_x.y)
       max_x.y = player_pos.x;
    if(player_pos.x < max_x.x)
       max_x.x = player_pos.x;
    if(player_pos.y > max_y.y)
       max_y.y = player_pos.y;
    if(player_pos.y < max_y.x)
       max_y.x = player_pos.y;
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //bots[i].update();
  //}
  Vector2f scale((max_x.y-max_x.x)/BBOP_WINDOW_RESOLUTION.x, (max_y.y-max_y.x)/BBOP_WINDOW_RESOLUTION.y);
  float cam_scale = (scale.x > scale.y) ? scale.x : scale.y;
  Vector2f cam_pos((max_x.y-max_x.x)/2.f+max_x.x,(max_y.y-max_y.x)/2.f+max_y.x);
  cam_scale+=0.33f;
  if (cam_scale>1.f) cam_scale = 1.f;
  players_camera.setScale(cam_scale);
  players_camera.setPosition(cam_pos);
  updateHUD();
}

void Game::Draw()
{
  scene.Use();

  map.Draw(scene, players_camera);
  for(long unsigned int i = 0; i < players.size(); i++){
    scene.Draw(players[i]);
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //scene.Draw(bots[i]);
  //}
  scene.useCamera(nullptr);
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

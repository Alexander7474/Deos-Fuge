#include "../include/game.h"
#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/fontsClass.h>
#include <GLFW/glfw3.h>
#include <string>

Game::Game(GLFWwindow*& window_, std::vector<Player> &players_, std::vector<Personnage> &bots_):
   cam_scale_goal(0.33f),
   cam_scale_last(0.33f),
   game_state(start),
   players(players_),
   bots(bots_),
   window(window_),
   font(48, "font/GohuFont11NerdFont-Regular.ttf"),
   fps_hud("0", nullptr),
   last_time(glfwGetTime()),
   fps_counter(0)
{
  fps_hud.setFont(&font);
  fps_hud.setPosition(Vector2f(0.0f,50.f));
}

void Game::update()
{
  Vector2f max_x(9999.f,0.f);
  Vector2f max_y(9999.f,0.f);
  for(long unsigned int i = 0; i < players.size(); i++){
    //update player
    players[i].update(&map);
    Vector2f player_pos = players[i].getPosition();

    //gestion des collisions entre les joueur
    if(players[i].getState() == light_attack){
      for(long unsigned int c = 0; c < players.size(); c++){
        if(players[i].getAttackBox().check(players[c].getCollisionBox()) && i != c){
          players[c].doHit(players[i].getDirection());
        }
      }
    }

    //gestion de la camera
    if(player_pos.x > max_x.y)
       max_x.y = player_pos.x;
    if(player_pos.x < max_x.x)
       max_x.x = player_pos.x;
    if(player_pos.y > max_y.y)
       max_y.y = player_pos.y;
    if(player_pos.y < max_y.x)
       max_y.x = player_pos.y;

    if(players[i].getPosition().y > 1800.f){
      players[i].setPosition(300.f,100.f);
    }
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //bots[i].update();
  //}

  // gestion de la camera
  Vector2f scale((max_x.y-max_x.x)/BBOP_WINDOW_RESOLUTION.x, (max_y.y-max_y.x)/BBOP_WINDOW_RESOLUTION.y);
  float cam_scale = (scale.x > scale.y) ? scale.x : scale.y;
  Vector2f cam_pos((max_x.y-max_x.x)/2.f+max_x.x,(max_y.y-max_y.x)/2.f+max_y.x);
  cam_scale+=0.33f;
  if (cam_scale>3.f) cam_scale = 3.f;
  if(cam_scale-0.2f > cam_scale_goal || cam_scale < cam_scale_goal-0.4f){
     cam_scale_goal = cam_scale;
  }
  cam_scale = cam_scale_last+((cam_scale_goal-cam_scale_last)/10.f);
  players_camera.setScale(cam_scale);
  cam_scale_last = cam_scale;
  players_camera.setPosition(cam_pos);
  updateHUD();
}

void Game::Draw()
{
  scene.Use();

  map.Draw(scene, players_camera);
  for(long unsigned int i = 0; i < players.size(); i++){
    scene.Draw(players[i]);
    bbopDebugCollisionBox(players[i].getCollisionBox(), scene);
    bbopDebugCollisionBox(players[i].getAttackBox(), scene);
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

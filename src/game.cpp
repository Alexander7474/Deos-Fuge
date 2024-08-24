#include "game.h"
#include <BBOP/Graphics/bbopFunc.h>
#include <BBOP/Graphics/bbopGlobal.h>
#include <BBOP/Graphics/cameraClass.h>
#include <BBOP/Graphics/fontsClass.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <string>

Game::Game(GLFWwindow*& window_, std::vector<Player> &players_):
   cam_scale_goal(0.166f),
   cam_scale_last(0.166f),
   game_state(start),
   players(players_),
   last_time(glfwGetTime()),
   window(window_),
   font(25, "font/BigBlueTerm437NerdFont-Regular.ttf"),
   fps_hud("0", nullptr),
   last_time_fps(glfwGetTime()),
   fps_counter(0)
{
  fps_hud.setFont(&font);
  fps_hud.setPosition(Vector2f(300.f,20.f));

  // attribution d'un point de spawn a chaque joueur
  unsigned int cpt = 0;
  for(Vector2f p : map.getSpawnPoints()){
    //std::cerr << cpt << std::endl;
    if(cpt < players.size())
      players[cpt].perso->setPosition(p);
    else 
      break;
    cpt++;
  }
 
  //list de position possible pourl es différent hud des joueurs 
  Vector2f hud_pos_list[4] = {
    Vector2f(0.f,720.f),
    Vector2f(1280.f,720.f),
    Vector2f(1280.f, 0.f),
    Vector2f(0.f, 0.f)
  };
  int cpt_pos = 0;

  //creation du hud
  
  players_hud = new player_hud*[players.size()];
  hud_padding = 2.f;

  for(Player p : players){
    std::string pp_path = p.perso->getInfo().folder_path + "pp.png";
    player_hud *h = new player_hud{
      TexteBox(p.name.c_str(), &font),
      TexteBox(std::to_string(p.perso->percentage).c_str(), &font),
      Sprite(Texture(pp_path.c_str()))
    };
    
    h->player_pp.setSize(75.f,75.f);

    //getsion des positions des éléments du hud des différents joueurs
    hud_pos_list[cpt_pos].x = (hud_pos_list[cpt_pos].x > 0.f) ? hud_pos_list[cpt_pos].x - (h->name.getSize().x+hud_padding) : 0.f + hud_padding;
    hud_pos_list[cpt_pos].y = (hud_pos_list[cpt_pos].y > 0.f) ? hud_pos_list[cpt_pos].y - (h->name.getSize().y+h->percent.getSize().y+h->player_pp.getSize().y+hud_padding*3) : 0.f + hud_padding;

    h->player_pp.setPosition(hud_pos_list[cpt_pos]);
    hud_pos_list[cpt_pos].y += hud_padding+h->player_pp.getSize().y+h->name.getSize().y;
    h->name.setPosition(hud_pos_list[cpt_pos]);
    hud_pos_list[cpt_pos].y += hud_padding+h->percent.getSize().y;
    h->percent.setPosition(hud_pos_list[cpt_pos]);

    players_hud[cpt_pos] = h;

    cpt_pos = (cpt_pos < 3) ? cpt_pos+1 : 3;
  }

}

void Game::update()
{
  //mise a jour de la map 
  map.update();

  //recupertation du temps depuis la dernière frame
  double current_time = glfwGetTime();
  double delta_time = current_time - last_time;
  last_time = current_time;

  Vector2f max_x(9999.f,0.f);
  Vector2f max_y(9999.f,0.f);
  for(long unsigned int i = 0; i < players.size(); i++){
    //update player
    players[i].update(delta_time, &map);
    Vector2f player_pos = players[i].perso->getPosition();

    //gestion des collisions entre les joueur
    if(players[i].perso->isAttacking()){
      for(long unsigned int c = 0; c < players.size(); c++){
        if(players[i].perso->getAttackBox().check(players[c].perso->getCollisionBox()) && i != c){
                if(players[c].perso->getState() != hit && players[c].perso->getState() != dash)
                        players[c].perso->doHit(players[i].perso->getDirection(), players[i].perso->getPercentageToApply());
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

    if(players[i].perso->getPosition().y > map.y_size){
      int r = rand() % map.getSpawnPoints().size();
      players[i].perso->setPosition(map.getSpawnPoints()[r]);
      players[i].perso->percentage = 0.f;
    }
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //bots[i].update();
  //}

  // gestion de la camera
  Vector2f scale((max_x.y-max_x.x)/BBOP_WINDOW_RESOLUTION.x, (max_y.y-max_y.x)/BBOP_WINDOW_RESOLUTION.y);
  float cam_scale = (scale.x > scale.y) ? scale.x : scale.y;
  Vector2f cam_pos((max_x.y-max_x.x)/2.f+max_x.x,(max_y.y-max_y.x)/2.f+max_y.x);
  cam_scale+=0.166f;
  if (cam_scale>0.166f*4) cam_scale = 0.166f*4;
  if(cam_scale-0.1f > cam_scale_goal || cam_scale < cam_scale_goal-0.2f){
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
    scene.Draw(*players[i].perso);
    //bbopDebugCollisionBox(players[i].perso->getCollisionBox(), scene);
    //bbopDebugCollisionBox(players[i].perso->getAttackBox(), scene);
  }
  //for(long unsigned int i = 0; i < bots.size(); i++){
    //scene.Draw(bots[i]);
  //}
  scene.useCamera(nullptr);

  //affichage du hud 
  scene.Draw(fps_hud);

  for(long unsigned int i = 0; i < players.size(); i++){
    scene.Draw(players_hud[i]->player_pp);
    scene.Draw(players_hud[i]->name);
    scene.Draw(players_hud[i]->percent);
  }
}

void Game::updateHUD()
{
  // hud fps
  double actual_time = glfwGetTime();
  double delta_time = actual_time-last_time_fps;
  fps_counter++;
  std::string final_fps = std::to_string(fps_counter/delta_time);
  if (delta_time >= 1.0){
    fps_counter = 0;
    last_time_fps = actual_time;
  }
  fps_hud.setTexte(final_fps.c_str());

  //player hud  
  for(long unsigned int i = 0; i < players.size(); i++){
    players_hud[i]->percent.setTexte(std::to_string(players[i].perso->percentage).c_str());
  }
  
}

Game::~Game()
{
  for(long unsigned int i = 0; i < players.size(); i++){
    delete players_hud[i];
  }
  delete [] players_hud;
}

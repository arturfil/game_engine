#include <iostream>
#include "./Game.h"
#include "./AssetManager.h"
#include "./Map.h"
#include "./Components/SpriteComponent.h"
#include "./Components/TransformComponent.h"
#include "../lib/glm/glm.hpp"
#include "./EntityManager.h"
#include "./Components/TransformComponent.h"
#include "./Components/KeyboardControlComponent.h"

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
Map* map;

Game::Game() {
  this -> isRunning = false;
}

Game::~Game() {
}

bool Game::IsRunning() const {
  return this -> isRunning;
}

glm::vec2 projectilePos = glm::vec2(0.0f, 0.0f);
glm::vec2 projectileVel = glm::vec2(20.0f, 20.0f);

float projectilePosX = 0.0f;
float projectilePosY = 0.0f;
float projectileVelX = 50.0f;
float projectileVelY = 50.0f;

void Game::Initialize(int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr <<"Error initializing SDL." << std::endl;
    return ;
  }
  window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_BORDERLESS);
  if (!window) {
    std::cerr << "Error creating SDL window." << std::endl;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if(!renderer) {
    std::cerr << "Error rendering window." << std::endl;
    return;
  }

  LoadLevel(0);

  isRunning = true;
  return;
}

void Game::LoadLevel(int levelNumber) {
  // Start including new assets to assetmanager list
  assetManager -> AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
  assetManager -> AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
  assetManager -> AddTexture("radar-image", std::string("./assets/images/radar.png").c_str()); 
  assetManager -> AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
  
  map = new Map("jungle-tiletexture", 1, 32);
  map -> LoadMap("./assets/tilemaps/jungle.map", 25, 20);
  // Choper
  Entity& chopperEntity(manager.AddEntity("chopper", PLAYER_LAYER));
  chopperEntity.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
  chopperEntity.AddComponent<SpriteComponent>("chopper-image", 2, 90, true, false);
  chopperEntity.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");

  Entity& tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
  tankEntity.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
  tankEntity.AddComponent<SpriteComponent>("tank-image");

  Entity& radarEntity(manager.AddEntity("Radar", UI_LAYER));
  radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
  radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);

}

void Game::ProcessInput() {
  SDL_PollEvent(&event);
  
  switch (event.type) {
    case SDL_QUIT: 
      isRunning = false;
      break;
    case SDL_KEYDOWN: 
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        isRunning = false;
      }
    default: 
      break;
  }
}

void Game::Update() {
  // Wait until 16ms has ellapsed since the last frame
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));
  // Delta time is the difference in ticks from last frame converted to seconds
  float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
  // Clamp deltaTime to a maximum value
  deltaTime = (deltaTime > 0.05f) ? 0.0f : deltaTime;
  // Sets the new ticks for the current frame to be used in the next pass
  ticksLastFrame = SDL_GetTicks();
  // use delta time to update the x & y objects
  manager.Update(deltaTime);
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  if (manager.HasNoEntities()) {
    return;
  }
  
  manager.Render();
  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
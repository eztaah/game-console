#include "globals.hpp"
#include "Game.hpp"
#include "MenuState.hpp"
#include "PlayingState.hpp"
#include "GameOverState.hpp"


///////////////// PUBLIC ///////////////// 
Game::Game(WindowManager& window)
    : _currentState(std::make_unique<MenuState>(this)),
      _score(0),
      _audiomanager(),
      _window(window)
{
    // Setup current game state
    _currentState->OnEnter();

    // Play music
    _audiomanager.PlayMusic();
}

Game::~Game() {}

void Game::Update()
{
    _audiomanager.Update();
    
    _currentState->Update();
}

void Game::Render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    // Set up the drawing area
    BeginScissorMode(MARGIN_X, MARGIN_Y, GAME_WIDTH, GAME_HEIGHT);
    ClearBackground(RAYWHITE);  // Fill the game area with green

    _currentState->Render();

    EndScissorMode();
    EndDrawing();
}

// === Window ===
void Game::ToggleFullScreenWindow() {
    _window.ManageFullScreen();
}


// === Games states ===
void Game::SetMenuState() {
    _ChangeState(std::make_unique<MenuState>(this));
}

void Game::SetPlayingState() {
    _ChangeState(std::make_unique<PlayingState>(this));     // Le constructeur de la classe playing state à besoin d'un pointeur vers l'instance de la classe game
}

void Game::SetGameOverState() {
    _ChangeState(std::make_unique<GameOverState>(this));
}


// === Score ===
unsigned int Game::GetScore() const {
    return _score;
}

void Game::IncreaseScore() {
    _score++;
}

void Game::ResetScore() {
    _score = 0;
}


// === Music ===
void Game::ToggleMusic()
{
    if(_audiomanager.IsMusicplaying())
        _audiomanager.StopMusic();
    else
        _audiomanager.PlayMusic();
}



///////////////// PRIVATE /////////////////
void Game::_ChangeState(std::unique_ptr<GameState> newState)
{
    if(_currentState)   // If the pointer exists
        _currentState->OnExit();    // Execute the exit logic related to the current state

    _currentState = std::move(newState);    // Change the state of the game
    _currentState->OnEnter();   // Execute the enter logic related to the new state
}
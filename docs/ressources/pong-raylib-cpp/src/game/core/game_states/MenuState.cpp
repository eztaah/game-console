#include "MenuState.hpp"
#include "globals.hpp"


MenuState::MenuState(Game* game)
    : _game(game),
      _cursorPosition(0),
      _state(0),
      _mainMenuTextures(),
      _settingsTextures()
{}


void MenuState::OnEnter()
{
    _cursorPosition = 0;

    // === Load textures ===
    // Main menu textures
    _mainMenuTextures.push_back(LoadTexture("./assets/textures/main-menu-0.png"));
    _mainMenuTextures.push_back(LoadTexture("./assets/textures/main-menu-1.png"));
    _mainMenuTextures.push_back(LoadTexture("./assets/textures/main-menu-2.png"));
    // Settings textures
    _settingsTextures.push_back(LoadTexture("./assets/textures/settings-0.png"));
    _settingsTextures.push_back(LoadTexture("./assets/textures/settings-1.png"));
    _settingsTextures.push_back(LoadTexture("./assets/textures/settings-2.png"));
}

void MenuState::OnExit()
{
    // Unload textures
    _mainMenuTextures.clear();
    _settingsTextures.clear();
}

void MenuState::Update()
{
    // Move cursor
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        if(_cursorPosition == 0)
            _cursorPosition = 2;
        else
            _cursorPosition -= 1;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        if(_cursorPosition == 2)
            _cursorPosition = 0;
        else
            _cursorPosition += 1;
    }

    // If key pressed
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
    {
        if(_state == 0)
            _ManageKeyPressedMainMenu();
        else if(_state == 1)
            _ManageKeyPressedSettings();
    }
}

void MenuState::Render()
{
    if(_state == 0)
    {
        float scale = static_cast<float>(GAME_WIDTH) / static_cast<float>(_mainMenuTextures[_cursorPosition].width);
        DrawTextureEx(_mainMenuTextures[_cursorPosition], {0.0f, 0.0f}, 0.0f, scale, WHITE); 
    } else if(_state == 1) 
    {
        float scale = static_cast<float>(GAME_WIDTH) / static_cast<float>(_settingsTextures[_cursorPosition].width);
        DrawTextureEx(_settingsTextures[_cursorPosition], {0.0f, 0.0f}, 0.0f, scale, WHITE); 
    }
}


////////////////////// PRIVATE /////////////////////////
void MenuState::_ManageKeyPressedMainMenu()
{
    switch(_cursorPosition)
    {
        case 0:
            _game->SetPlayingState();
            break;
        case 1:
            _state = 1;
            break;
        case 2:
            CloseWindow();
            exit(EXIT_SUCCESS);
    }
}

void MenuState::_ManageKeyPressedSettings()
{
    switch(_cursorPosition)
    {
        case 0:
            _game->ToggleFullScreenWindow();
            break;
        case 1:
            _game->ToggleMusic();
            break;
        case 2:
            _state = 0;
            break;
    }
}
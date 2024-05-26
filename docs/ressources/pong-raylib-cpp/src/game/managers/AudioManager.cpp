#include "Audiomanager.hpp"


AudioManager::AudioManager()
    : _backgroundMusic()
{
    InitAudioDevice();  
    _backgroundMusic = LoadMusicStream("./assets/sounds/music.mp3");
}

AudioManager::~AudioManager()
{
    StopMusicStream(_backgroundMusic);
    UnloadMusicStream(_backgroundMusic);
    CloseAudioDevice(); 
}


void AudioManager::Update() {
    UpdateMusicStream(_backgroundMusic);
}


bool AudioManager::IsMusicplaying() const {
    return IsMusicStreamPlaying(_backgroundMusic);
}


void AudioManager::PlayMusic()
{
    PlayMusicStream(_backgroundMusic);
    SetMusicVolume(_backgroundMusic, 0.3f); // Réduit le volume à 50%
}

void AudioManager::StopMusic()
{
    StopMusicStream(_backgroundMusic);
}


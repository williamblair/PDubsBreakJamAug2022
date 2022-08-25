#include "AudioManager.h"
#include <stdexcept>

AudioManager::AudioManager() :
    mInitted(false)
{}

AudioManager::~AudioManager()
{
    for (Mix_Music* mus : mMusics)
    {
        Mix_FreeMusic(mus);
    }
    for (Mix_Chunk* chunk : mSounds)
    {
        Mix_FreeChunk(chunk);
    }
    if (mInitted)
    {
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
    }
}

void AudioManager::Init()
{
    if (mInitted) { return; }
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        throw std::runtime_error("Failed to init SDL: " + std::string(SDL_GetError()));
    }
    mInitted = true;
    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    int initted = Mix_Init(flags);
    if ((initted&flags) != flags) {
        throw std::runtime_error("Failed to init SDL mixer: " + std::string(Mix_GetError()));
    }
    mInitted = true;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        throw std::runtime_error("Failed to open mix audio: " + std::string(Mix_GetError()));
    }
}

size_t AudioManager::LoadMusic(std::string fileName)
{
    Mix_Music* mus = Mix_LoadMUS(fileName.c_str());
    if (!mus) {
        throw std::runtime_error("Failed to load music: " + std::string(Mix_GetError()));
    }
    mMusics.push_back(mus);
    return mMusics.size()-1;
}

size_t AudioManager::LoadSound(std::string fileName)
{
    Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
    if (!sound) {
        throw std::runtime_error("Failed to load sound: " + std::string(Mix_GetError()));
    }
    mSounds.push_back(sound);
    return mSounds.size()-1;
}

void AudioManager::PlayMusic(size_t mus, bool loop)
{
    int loopVal = loop ? -1 : 1;
    if (Mix_PlayMusic(mMusics[mus], loopVal) < 0) {
        throw std::runtime_error("Failed to play music: " + std::string(Mix_GetError()));
    }
}

void AudioManager::StopMusic()
{
    Mix_HaltMusic();
}

void AudioManager::PlaySound(size_t sound, int channel)
{
    int loopVal = 0; // no looping
    if (Mix_PlayChannel(channel, mSounds[sound], loopVal) < 0) {
        throw std::runtime_error("Failed to play sound: " + std::string(Mix_GetError()));
    }
}

// global instance
AudioManager gAudioMgr;

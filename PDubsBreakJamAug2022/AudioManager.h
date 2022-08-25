#ifndef AUDIO_MANAGER_H_INCLUDED
#define AUDIO_MANAGER_H_INCLUDED

#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    void Init();

    size_t LoadMusic(std::string fileName);
    size_t LoadSound(std::string fileName);

    void PlayMusic(size_t mus, bool loop = true);
    void StopMusic();

    void PlaySound(size_t sound, int channel = -1);

private:
    bool mInitted;
    std::vector<Mix_Music*> mMusics;
    std::vector<Mix_Chunk*> mSounds;
};

// global instance
extern AudioManager gAudioMgr;

#endif // AUDIO_MANAGER_H_INCLUDED

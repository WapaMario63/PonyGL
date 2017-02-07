#ifndef SOUND_H
#define SOUND_H

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class Sound
{
public:
    enum SoundType
    {
        snd_BGM = 1,
        snd_SFX = 2,
        snd_NONE = 0,
    };

    Sound(const char *file, SoundType type);

    void Play();
    void Pause();
    void Stop();
    void setLoop(bool loop);
    void setPitch(float pitch);
    void setTime(float time);
    void setVolume(float vol);
private:
    SoundType _soundType;
    sf::Sound* _sound;
    sf::Music _music;
    std::string _filename;

    void SetupBGM();
    void SetupSFX();

};

#endif // SOUND_H

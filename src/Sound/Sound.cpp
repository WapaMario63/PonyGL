#include "Sound.h"

Sound::Sound(const char* file, SoundType type)
{
    _filename = file;
    _soundType = type;

    if (_soundType == snd_BGM)
    {
        _sound = nullptr;

        SetupBGM();
    }
    else if (_soundType == snd_SFX)
    {
    }

}

void Sound::SetupBGM()
{
    _music.openFromFile(_filename);
    std::cout << "Opened sound" << std::endl;
}

void Sound::SetupSFX()
{

}

void Sound::Play()
{
    switch (_soundType)
    {
    case snd_BGM:
        _music.play(); break;
    case snd_SFX:
        _sound->play(); break;
    case snd_NONE:
        std::cerr << "There is no sound to play u fuck" << std::endl;
    }
}

void Sound::Pause()
{
    switch (_soundType)
    {
    case snd_BGM:
        _music.pause(); break;
    case snd_SFX:
        _sound->pause(); break;
    case snd_NONE:
        std::cerr << "There is no sound playing u fuck" << std::endl;
    }
}

void Sound::Stop()
{
    switch (_soundType)
    {
    case snd_BGM:
        _music.stop(); break;
    case snd_SFX:
        _sound->stop(); break;
    case snd_NONE:
        std::cerr << "There is no sound to play u fuck" << std::endl;
    }
}

void Sound::setVolume(float vol)
{
    _music.setVolume(vol);
    _sound->setVolume(vol);
}

void Sound::setPitch(float pitch)
{
    _music.setPitch(pitch);
    _sound->setPitch(pitch);
}

void Sound::setTime(float time)
{
    sf::Time t = sf::seconds(time);

    _music.setPlayingOffset(t);
    _sound->setPlayingOffset(t);
}

void Sound::setLoop(bool loop)
{
    _music.setLoop(loop);
    _sound->setLoop(loop);
}


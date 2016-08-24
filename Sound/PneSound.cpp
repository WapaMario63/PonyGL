#include "PneSound.h"
#include <Utils/PneUtilities.h>
#include <pne_config.h>

PneUtilities utils;

/*PneSound::PneSound(std::string& filename, SndType type)
{
    if (type == SndFX2D)
    {
        sndBuffer.loadFromFile(filename);
    }
    else if (type == SndFX3D)
    {
        sndBuffer.loadFromFile(filename);
    }
    else
    {
        _music.openFromFile(filename);
    }
}

void PneSound::Play()
{
    if (sndType == SndFX2D || SndFX3D)
    {
        _sound.play();
    }
    else
    {
        _music.play();
    }
}
void PneSound::Pause()
{
    if (sndType == SndFX2D || SndFX3D)
    {
        _sound.pause();
    }
    else
    {
        _music.pause();
    }
}
void PneSound::Stop()
{
    if (sndType == SndFX2D || SndFX3D)
    {
        _sound.stop();
    }
    else
    {
        _music.stop();
    }
}

sf::SoundBuffer PneSound::getSoundBuffer() { return sndBuffer; }

void PneSound::setSoundBuffer(sf::SoundBuffer buffer) { this->sndBuffer = buffer; }*/

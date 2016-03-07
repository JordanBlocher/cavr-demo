#include "SoundManager.hpp"


SoundManager::SoundManager(const char* name)
{
	this->name = name;
    this->engine = irrklang::createIrrKlangDevice();

    this->bgm.push_back("media/bgm_0.wav");
    this->fx.push_back("media/fx_0.wav");

}

void SoundManager::PlayBgm(int bgmNum, bool looped, bool paused)
{
    this->bgmusic = this->engine->play2D(this->bgm[bgmNum], looped, paused, true);
}

void SoundManager::PlayFX(int fxNum, Vec3 pos)
{
    this->fxmusic = this->engine->play3D(this->fx[fxNum], irrklang::vec3df(pos.x,pos.y,pos.z), true, false, true);
}

void SoundManager::SetListener(Vec3 pos)
{
    pos.x *= 10;
    pos.z *= 10;
    this->engine->setListenerPosition(
            irrklang::vec3df(pos.x,pos.y,pos.z), // Listener's position
            irrklang::vec3df(0,0,1)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..
}

void SoundManager::Finished()
{
	this->engine->drop();
}

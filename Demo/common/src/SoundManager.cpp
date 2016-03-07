#include "SoundManager.hpp"


SoundManager::SoundManager(const char* name)
{
	this->name = name;
    this->engine = irrklang::createIrrKlangDevice();

    this->bgm.push_back("media/bgm_0.wav");
    this->fx.push_back("media/fx_0.wav");

}


void SoundManager::playBgm(int bgmNum)
{
    this->bgmusic = this->engine->play3D(this->bgm[bgmNum], irrklang::vec3df(0,1,0), true, false, true);
}

void SoundManager::playFX(int fxNum)
{
    this->fxmusic = this->engine->play2D(this->fx[fxNum], true, false, true);
}

void SoundManager::finished()
{
	this->engine->drop();
}

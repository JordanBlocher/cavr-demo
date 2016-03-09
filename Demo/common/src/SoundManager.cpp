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
    this->bgmusic = this->engine->play2D(this->bgm[bgmNum], looped, paused, false);
    cout << "BGMUSIC: " << this->bgmusic << endl;
}

void SoundManager::PlayFX(int fxNum, Vec3 pos)
{
    this->fxmusic = this->engine->play3D(this->fx[fxNum], irrklang::vec3df(pos.x,pos.y,pos.z), false, false, false);
}

void SoundManager::SetListener(Vec3 pos,Vec3 forward)
{
    this->engine->setListenerPosition(
            irrklang::vec3df(pos.x,pos.y,pos.z), // Listener's position
            irrklang::vec3df(forward.x,forward.y,forward.z)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..
}

void SoundManager::Finished()
{
	this->engine->drop();
}

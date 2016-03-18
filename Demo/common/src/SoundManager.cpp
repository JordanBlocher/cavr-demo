#include "SoundManager.hpp"


SoundManager::SoundManager(const char* name)
{
	this->name = name;
    this->engine = irrklang::createIrrKlangDevice();

    this->bgm["bgm0"] = "media/bgm_0.wav";
    this->fx["fx0"] = "media/fx_0.wav";

}

void SoundManager::PlayBgm(string bgmNum, bool looped, bool paused)
{
    this->bgmusic = this->engine->play2D(this->bgm[bgmNum].c_str(), looped, paused, false);
    cout << "BGMUSIC: " << this->bgmusic << endl;
}

irrklang::ISound* SoundManager::PlayFX(string fxNum, Vec3 pos,bool looped,bool paused,bool tracked,bool enableSoundEffects)
{
    return this->engine->play3D(this->fx[fxNum].c_str(), irrklang::vec3df(pos.x,pos.y,pos.z), looped, paused, tracked, irrklang::E_STREAM_MODE::ESM_AUTO_DETECT,enableSoundEffects);
    //return this->fxmusic;
}

void SoundManager::SetListener(Vec3 pos,Vec3 forward)
{
    this->engine->setListenerPosition(
            irrklang::vec3df(pos.x,pos.y,pos.z), // Listener's position
            irrklang::vec3df(forward.x,forward.y,forward.z)); // What direction is the listener's facing directiion -- in this case we are always stareing forward..
}

void SoundManager::AddFx(string filename,string alias)
{
    this->fx[alias] = filename;
}

void SoundManager::AddBgm(string filename,string alias)
{
    this->bgm[alias] = filename;
}


void SoundManager::Finished()
{
	this->engine->drop();
}

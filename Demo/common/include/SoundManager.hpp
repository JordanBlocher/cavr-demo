#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "GLNode.hpp"
#include <irrKlang.h>
#include <vector>

class SoundManager : public GLNode
{
	public:
		SoundManager(const char* name);
		void PlayBgm(int bgmNum, bool, bool);
		void PlayFX(int fxNum, Vec3);
        void SetListener(Vec3);

	protected:
		void Finished();

	private:
        irrklang::ISoundEngine* engine;
        irrklang::ISound* bgmusic; 
        irrklang::ISound* fxmusic; 
        std::vector<const char*> bgm;
        std::vector<const char*> fx;
};

#endif 

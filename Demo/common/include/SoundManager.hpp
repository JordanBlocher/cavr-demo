#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "GLNode.hpp"
#include <irrKlang.h>
#include <vector>

class SoundManager : public GLNode
{
	public:
		SoundManager(const char* name);
		void playBgm(int bgmNum);
		void playFX(int fxNum);

	protected:
		void finished();

	private:
        irrklang::ISoundEngine* engine;
        irrklang::ISound* bgmusic; 
        irrklang::ISound* fxmusic; 
        std::vector<const char*> bgm;
        std::vector<const char*> fx;
};

#endif 

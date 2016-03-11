#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "GLNode.hpp"
#include <irrKlang.h>
#include <vector>
#include <map>
#include <string>

// Strings are nice
using std::string;
class SoundManager : public GLNode
{
	public:
		SoundManager(const char* name="");
		void PlayBgm(string bgmNum, bool, bool);
		irrklang::ISound* PlayFX(string fxNum, Vec3,bool looped = false,bool paused = false,bool tracked = false,bool enableSoundEffects = false);
		void AddFx(string filename,string alias);
		void AddBgm(string filename,string alias);
        void SetListener(Vec3,Vec3 forward);

	protected:
		void Finished();

	private:
        irrklang::ISoundEngine* engine;
        irrklang::ISound* bgmusic; 
        irrklang::ISound* fxmusic;

        // Maps will make this a bit easier -- we can use named sounds -- i.e. call E4.mp3 -- E4  
        std::map<string,string> bgm;
        std::map<string,string> fx;
};

#endif 

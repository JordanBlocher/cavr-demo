#include <SoundManager.hpp>
#include <vector>
#include <map>
#include <string>
#include <irrKlang.h>

class SoundPlayer
{
	struct SoundStructure
	{
    	float time;
    	float offtime;
    	int sound;
    	Vec3 Position;
    	irrklang::ISound* Sound;
	};
public:
	SoundPlayer(){};
	SoundPlayer(shared_ptr<SoundManager>);
	void ClearTracked();
	void AddPoint(Vec3 Position,int sound);
	void Update(float dt);
	void SetSound(int mapping, string name);
	void PlaySound();
	// Set the last points off
	void SetOffPoint();
	void ToggleRecord()
	{
	 	record = !record;
	}
private:
	map<int,string> Sounds;	
	float currenttime;
	float playtime;
	float intervaltime;
	bool play;
	bool record;
	bool noteOn;
	vector<SoundStructure> sounds;
	shared_ptr<SoundManager> soundMan;
};
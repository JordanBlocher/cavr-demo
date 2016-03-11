#include <SoundPlayer.hpp>
//	map<int,string> Sounds;	
//	float currenttime;
SoundPlayer::SoundPlayer(shared_ptr<SoundManager> SoundMan)
{
	currenttime = playtime = 0;
	play = false;
	record = false;
	soundMan = SoundMan;
}

void SoundPlayer::ClearTracked()
{
	play = false;
	for(int i = 0; i < sounds.size(); i++ )
	{
		if (sounds[i].Sound != NULL )
		{
			sounds[i].Sound->stop();
			sounds[i].Sound = NULL;
		}
	}
	sounds.clear();
	currenttime = 0;
	intervaltime = 1.0/2.0;
	playtime = 0;

}

void SoundPlayer::AddPoint(Vec3 Position,int sound)
{
	if(intervaltime <= 0)
	{
		if(record)
		{
			SoundStructure temp;
			temp.Position = Position;
			temp.sound = sound;
			temp.time = currenttime;
			sounds.push_back(temp);
		}
		intervaltime = 1.0/2.0;

	}
}

void SoundPlayer::Update(float dt)
{
	if(record)
	{
		currenttime += dt;
	}
	intervaltime -= dt;
	if(play && sounds.size() > 0)
	{
		for(int i = 0; i < sounds.size(); i++ )
		{

			if(playtime >= sounds[i].time && sounds[i].Sound == NULL)// && (sounds[i].offtime > playtime))
			{
				sounds[i].Sound = soundMan->PlayFX(Sounds[sounds[i].sound], sounds[i].Position,false,false,false,false);

				//sounds[i].Sound->getSoundEffectControl()->enableEchoSoundEffect();
			}
			else if (sounds[i].Sound != NULL && (sounds[i].offtime <= playtime)  )
			{
				sounds[i].Sound->stop();
				sounds[i].Sound = NULL;
			}
		}
		if(playtime > sounds[sounds.size()-1].offtime)
		{
			play = false;
			playtime = 0;
			return;
		}
		playtime += dt;

	}
}

void SoundPlayer::SetSound(int mapping, string name)
{
	Sounds[mapping] = name;
}

void SoundPlayer::PlaySound()
{
	playtime = 0;
	play = true;
}

void SoundPlayer::SetOffPoint()
{
	if(record)
	{
		sounds[sounds.size()-1].offtime = currenttime;
		sounds[sounds.size()-1].Sound =NULL;
	}
}

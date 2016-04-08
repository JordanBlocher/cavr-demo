using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

[Serializable]
public class Outfitter : MonoBehaviour 
{

    UnityStandardAssets.Characters.ThirdPerson.WarriorCharacter ac;
	int oldWeaponIndex;
	[SerializeField]
	public List<WeaponSlot> weapons;
	
	// Use this for initialization
	void Start () 
	{
		ac = GetComponentInChildren<UnityStandardAssets.Characters.ThirdPerson.WarriorCharacter>();
		for(int i = 0;i<weapons.Count;i++)
		{
			for(int model=0;model<weapons[i].models.Count;model++)
			{
				weapons[i].models[model].enabled = false;
			}
		}
		for(int model=0;model<weapons[ac.WeaponIdx].models.Count;model++)
		{
			weapons[ac.WeaponIdx].models[model].enabled = true;
		}
		oldWeaponIndex=ac.WeaponIdx;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if(ac.WeaponIdx!=oldWeaponIndex)
		{
			for(int model=0;model<weapons[oldWeaponIndex].models.Count;model++)
			{
				weapons[oldWeaponIndex].models[model].enabled = false;
			}
			for(int model=0;model<weapons[ac.WeaponIdx].models.Count;model++)
			{
				weapons[ac.WeaponIdx].models[model].enabled = true;
			}
			oldWeaponIndex=ac.WeaponIdx;
		}
	}
}
[Serializable]
public class WeaponSlot
{
	[SerializeField]
	public List<Renderer> models = new List<Renderer>();
}

﻿using UnityEngine;
using System.Collections;

public class DragonHealth : EnemyPlayer {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if(IsDead())
		{
			Destroy(gameObject);
		}
	}
}
using UnityEngine;
using System.Collections;

public class OverlordHealth : EnemyPlayer {

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

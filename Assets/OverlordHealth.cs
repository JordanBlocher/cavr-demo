using UnityEngine;
using System.Collections;

public class OverlordHealth : EnemyPlayer {
    public AudioSource As; 
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if(IsDead())
		{
            As.Stop();
			Destroy(gameObject);
		}
	}
}

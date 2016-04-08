using UnityEngine;
using System.Collections;

public class PlayerHealth : EnemyPlayer {

	// Use this for initialization
	void Start () 
    {
        MaxHealth = 10;
        Health = MaxHealth; 
	}
	
	// Update is called once per frame
	void Update () {
	}
}

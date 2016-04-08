using UnityEngine;
using System.Collections;

public class PlayerHealth : EnemyPlayer {

    protected float Lightning = 100;
    protected float MaxLightning = 100;

    public float PHealth
    {
        get { return Health; }
        set { Health = value; }
    }

    // Use this for initialization
    void Start () 
    {
        MaxHealth = 100;
        Health = MaxHealth; 
	}
	
	// Update is called once per frame
	void Update () {
	}
}

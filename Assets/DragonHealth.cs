using UnityEngine;
using System.Collections;

public class DragonHealth : EnemyPlayer {
    public AudioSource As;
    public AudioSource BossTheme;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if(IsDead())
		{
			Destroy(gameObject);
            GameObject warrior = GameObject.Find("warrior");
            GameObject cam = GameObject.Find("ThirdPersonCamera");
            As.Play();
            BossTheme.Stop();
        }
    }
}

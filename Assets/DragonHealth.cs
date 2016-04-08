using UnityEngine;
using System.Collections;

public class DragonHealth : EnemyPlayer {

    GameObject cam1, cam2;

    // Use this for initialization
    void Start () {
        cam1 = GameObject.Find("FirstPersonCamera");
        cam2 = GameObject.Find("ThirdPersonCamera");
    }
	
	// Update is called once per frame
	void Update () {
		if(IsDead())
		{
			Destroy(gameObject);
            GameObject warrior = GameObject.Find("warrior");

            cam1.SetActive(true);
            cam2.SetActive(false);
            cam1.transform.RotateAround(warrior.transform.position, Vector3.forward * 2.0f, 360.0f);
            cam1.transform.RotateAround(warrior.transform.position, Vector3.forward * 1.0f, 360.0f);

        }
    }
}

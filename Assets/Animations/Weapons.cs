﻿using UnityEngine;
using System.Collections;

public class Weapons : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	}

    void OnTriggerEnter(Collider col)
    {
        Debug.Log(GetComponent<MeshRenderer>());
        Debug.Log(GetComponent<MeshRenderer>().isVisible);

        if (GetComponent<MeshRenderer>().isVisible)
        {
            Debug.Log("ATTACK");
            if (col.gameObject.CompareTag("Enemy"))
            {
                if (!col.gameObject.GetComponent<EnemyPlayer>().IsDead())
                {
                    if (gameObject.tag == "Melee")
                    {
                        col.gameObject.GetComponent<EnemyPlayer>().DamageEntity(50.0f);
                    }
                    else if (gameObject.tag == "Projectile")
                    {
                        col.gameObject.GetComponent<EnemyPlayer>().DamageEntity(25.0f);
                    }
                }
            }
        }
    }
}

using UnityEngine;
using System.Collections;

public class TestCollision : MonoBehaviour {

	public GameObject spider;
	public GameObject overlord;
	public GameObject dragon;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		if(Input.GetKeyDown(KeyCode.I))
		{
		spider.GetComponent<EnemyPlayer>().DamageEntity(10000);
		}
		if(Input.GetKeyDown(KeyCode.J))
		{
		overlord.GetComponent<EnemyPlayer>().DamageEntity(10000);
		}
		if(Input.GetKeyDown(KeyCode.K))
		{
		dragon.GetComponent<EnemyPlayer>().DamageEntity(10000);
		}
	}

	void OnCollisionEnter(Collision collision) {
        foreach (ContactPoint contact in collision.contacts) {
            Debug.DrawRay(contact.point, contact.normal, Color.white);
        }
     	//Debug.LogError("COLLISION HERE WITH" + collision.gameObject.name);
        
    }

}

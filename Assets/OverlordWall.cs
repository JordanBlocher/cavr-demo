using UnityEngine;
using System.Collections;

public class OverlordWall : MonoBehaviour {

	float time = 0;
	float damage = 1;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		time += Time.deltaTime;
		if(time > 3)
		{
			Destroy(gameObject);
		}
	}

	void OnParticleCollision(GameObject go)
	{
		//Debug.Break();
		if(go.tag == "Player")
		{
			go.GetComponent<EnemyPlayer>().DamageEntity(damage);
			Debug.LogError("HERE" + gameObject.name);
		}
	}
}

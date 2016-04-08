using UnityEngine;
using System.Collections;

public class FireballDamage : MonoBehaviour {
	float time;
	public ParticleSystem part;
	ParticleCollisionEvent[] events = new ParticleCollisionEvent[16];
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		time += Time.deltaTime;
		if(time > 6)
		{
			Destroy(gameObject);
		}
	}

	void OnParticleCollision(GameObject go)
	{
		if(go.tag == "Player")
		{

			Debug.LogError("Player");
			//Debug.Break();
		}

		GameObject wall = Resources.Load<GameObject>("FirewallOverlord");

		part.GetCollisionEvents(go,events);
		wall.transform.position = events[0].intersection;
		Instantiate(wall);
	}
}

using UnityEngine;
using System.Collections;

public class FireballDamage : MonoBehaviour {
	float time;
	public AudioSource ac;
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
		ac.Play();
	}
}

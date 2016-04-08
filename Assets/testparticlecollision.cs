using UnityEngine;
using System.Collections;

public class testparticlecollision : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}
	void OnParticleCollision(GameObject go)
	{
		Debug.LogError("PARTICLE COLLIDED");
	}
}

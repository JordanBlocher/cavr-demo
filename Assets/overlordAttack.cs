using UnityEngine;
using System.Collections;

public class overlordAttack : MonoBehaviour {

	public Animator anime;
	bool trigger = false;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () 
	{
		if(anime.GetCurrentAnimatorStateInfo(0).IsName("attacks") && anime.GetCurrentAnimatorStateInfo(0).normalizedTime > .8 && !trigger)
		{
			trigger = true;
			GameObject wall = Resources.Load<GameObject>("FirewallOverlord");

			wall.transform.position = gameObject.transform.position + gameObject.transform.forward * 4.0f;
			wall.transform.LookAt(GameObject.FindWithTag("Player").transform.position);

			Instantiate(wall);
		}
		else if(anime.GetCurrentAnimatorStateInfo(0).IsName("attacks")  && anime.GetCurrentAnimatorStateInfo(0).normalizedTime < .5 && trigger)
		{
			trigger = false;
		}
	}
}

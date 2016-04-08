using UnityEngine;
using System.Collections;

public class FireLauncher : MonoBehaviour {
	public Animator anime;
	public GameObject FireBall;
	bool toggle = false;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		if(anime.GetCurrentAnimatorStateInfo(0).IsName("Dragon_Attack") && anime.GetCurrentAnimatorStateInfo(0).normalizedTime > .5 && !toggle)
		{
			//Debug.LogError("UPDATE");
			GameObject FireBall = Resources.Load<GameObject>("FireballDamage") ;


			Debug.LogError(FireBall);

			FireBall.transform.position =  gameObject.transform.position;
			FireBall.transform.LookAt(GameObject.FindWithTag("Player").transform.position);

			Instantiate(FireBall);
			toggle = true;

		}
		else if (anime.GetCurrentAnimatorStateInfo(0).IsName("Dragon_Attack") && anime.GetCurrentAnimatorStateInfo(0).normalizedTime < .5 && toggle)
		{
			toggle = false;
		}
	}
}

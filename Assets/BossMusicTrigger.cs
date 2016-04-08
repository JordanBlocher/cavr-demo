using UnityEngine;
using System.Collections;

public class BossMusicTrigger : MonoBehaviour {

    public AudioSource As;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    void OnTriggerEnter(Collider col)
    {
        if(!As.isPlaying)
        As.Play();
    }
}

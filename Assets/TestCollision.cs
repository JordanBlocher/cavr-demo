using UnityEngine;
using System.Collections;

public class TestCollision : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	void OnCollisionEnter(Collision collision) {
        foreach (ContactPoint contact in collision.contacts) {
            Debug.DrawRay(contact.point, contact.normal, Color.white);
        }
     	//Debug.LogError("COLLISION HERE WITH" + collision.gameObject.name);
        
    }

}

using UnityEngine;
using System.Collections;

public class testcavr : MonoBehaviour {
	public CaVR ca;

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		Debug.LogError(ca.InputManger.GetButtonValue("attack"));
	}
}

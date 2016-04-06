using UnityEngine;
using System.Collections;

public class StateMachineTest : StateMachineBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	// This will be called when the animator first transitions to this state.
    override public void OnStateEnter(Animator animator, AnimatorStateInfo stateInfo, int layerIndex)
    {
		Debug.LogError(stateInfo.IsName("attack"));
    }
}

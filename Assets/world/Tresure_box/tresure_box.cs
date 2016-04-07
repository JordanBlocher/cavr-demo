using UnityEngine;
using System.Collections;

public class tresure_box : MonoBehaviour {

    Animator m_Animator;

    // Use this for initialization
    void Start () {
        m_Animator = GetComponent<Animator>();
        m_Animator.SetBool("isOpen", false);    
    }

    // Update is called once per frame
    void Update () {
    }

    void OnCollisionEnter(Collision col)
    {
        if (col.gameObject.CompareTag("Player"))
        {
            m_Animator.Play("box_open");
            m_Animator.SetBool("isOpen", true);
        }

    }
}

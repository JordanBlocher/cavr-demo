using UnityEngine;
using System.Collections.Generic;

public class Detect : MonoBehaviour
{

    public List<GameObject> Children;

    // Use this for initialization
    void Start()
    {
        foreach (Transform child in transform)
        {
            if (child.tag == "Enemy")
            {
                Children.Add(child.gameObject);
            }
        }

    }

    // Update is called once per frame
    void Update()
    {

    }

    void OnTriggerEnter(Collider col)
    {
        if (col.gameObject.CompareTag("Player"))
        {
            foreach (GameObject child in Children)
            {
                child.GetComponent<Goblin>().InRange = true;
            }
        }
    }
}

using UnityEngine;
using System.Collections;

public class SpiderDamage : EnemyWeapon {

    public Animator anime;
    bool triggered = false;
    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        if (!anime.GetCurrentAnimatorStateInfo(0).IsName("attack"))
        {
            triggered = false;
        }
    }

    void OnTriggerEnter(Collider col)
    {
        Debug.LogError("COLLIDER ENTER HERE NOW");
    }

    void OnTriggerStay(Collider collisionInfo)
    {
        if(collisionInfo == null)
        {
            return;
        }
        if (anime.GetCurrentAnimatorStateInfo(0).IsName("attack") && anime.GetCurrentAnimatorStateInfo(0).normalizedTime > .5 && !triggered && collisionInfo.tag == "Player")
        {
            Debug.LogError(collisionInfo.gameObject);
            collisionInfo.gameObject.GetComponent<EnemyPlayer>().DamageEntity(10);
            triggered = true;
        }
    }
}

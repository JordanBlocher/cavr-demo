using UnityEngine;
using System.Collections;

public class Lightning : MonoBehaviour {

    float time;
    float damage = 10;
    public ParticleSystem part;
    ParticleCollisionEvent[] events = new ParticleCollisionEvent[16];
    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;
        if (time > 6)
        {
            Destroy(gameObject);
        }
    }

    void OnParticleCollision(GameObject go)
    {
        if (go.tag == "Enemy")
        {
            go.GetComponent<EnemyPlayer>().DamageEntity(damage);
            //Debug.LogError("Player");
            //Debug.Break();
        }

    }
}

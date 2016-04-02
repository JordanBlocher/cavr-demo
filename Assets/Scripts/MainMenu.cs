using UnityEngine;
using System.Collections;
using DigitalRuby.PyroParticles;
using UnityEngine;
public class MainMenu : MonoBehaviour {

    private GameObject currentPrefabObject;
    private FireBaseScript currentPrefabScript;

    public GameObject Witch;

    // Use this for initialization
    void Start () {
        BeginEffect();
	}

    // Begin Effect
    private void BeginEffect()
    {
        Vector3 pos;
        float yRot = transform.rotation.eulerAngles.y;
        Vector3 forwardY = Quaternion.Euler(0.0f, yRot, 0.0f) * Vector3.forward;
        Vector3 forward = transform.forward;
        Vector3 right = transform.right;
        Vector3 up = transform.up;
        Quaternion rotation = Quaternion.identity;
        currentPrefabObject = GameObject.Instantiate(Resources.Load<GameObject>("OurPrefabs/FireField") );
        currentPrefabObject.transform.position= new Vector3();
        currentPrefabScript = currentPrefabObject.GetComponent<FireConstantBaseScript>();

       /* if (currentPrefabScript == null)
        {
            // temporary effect, like a fireball
            currentPrefabScript = currentPrefabObject.GetComponent<FireBaseScript>();
            if (currentPrefabScript.IsProjectile)
            {
                // set the start point near the player
                rotation = transform.rotation;
                pos = transform.position + forward + right + up;
            }
            else
            {
                // set the start point in front of the player a ways
                pos = transform.position + (forwardY * 10.0f);
            }
        }
        else
        {
            // set the start point in front of the player a ways, rotated the same way as the player
            pos = transform.position + (forwardY * 5.0f);
            rotation = transform.rotation;
            pos.y = 0.0f;
        }

        FireProjectileScript projectileScript = currentPrefabObject.GetComponentInChildren<FireProjectileScript>();
        if (projectileScript != null)
        {
            // make sure we don't collide with other friendly layers
            projectileScript.ProjectileCollisionLayers &= (~UnityEngine.LayerMask.NameToLayer("FriendlyLayer"));
        }

        currentPrefabObject.transform.position = pos;
        currentPrefabObject.transform.rotation = rotation;*/
    }

    // Update is called once per frame
    void Update () {
	
	}
}

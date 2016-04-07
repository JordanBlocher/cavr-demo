using UnityEngine;
using System.Collections;

public class Goblin : MonoBehaviour {

    [SerializeField]
    float m_Speed = 5.0f * Time.deltaTime;
    [SerializeField]
    bool m_Dead = false;
    [SerializeField]
    bool m_Roaming = false;
    [SerializeField]
    bool m_Lead = false;

    Animation m_Animation;
    bool pursuing;
    bool inRange;
    Vector3 move;   
    Vector3 m_Forward;

    // Use this for initialization
    void Start () {
        m_Animation = GetComponent<Animation>();
        pursuing = false;
        inRange = false;
        m_Forward = Vector3.zero;
        MoveRandom(); 
    }

    // Update is called once per frame
    void Update ()
    {
        if (!m_Dead)
        {
            if (gameObject.GetComponent<EnemyPlayer>().IsDead())
            {
                Die();
            }
            else if (inRange)
            {
                GameObject warrior = GameObject.Find("warrior");
                transform.LookAt(warrior.transform.position);
                if (pursuing)
                {
                    Move();
                    if (Vector3.Distance(warrior.transform.position, transform.position) < 1.5f)
                    {
                        pursuing = false;
                        Attack(warrior);
                    }
                }
                if (Vector3.Distance(warrior.transform.position, transform.position) > 4.5f)
                {
                    pursuing = true;
                }
            }
            else if (!inRange && !m_Roaming)
            {
                m_Animation.Play("combat_idle");
            }
            else if (m_Roaming)
            {
                RaycastHit hitInfo;

                if (Physics.Raycast(transform.position + move + (Vector3.up * 0.5f), Vector3.down, out hitInfo, 5.5f))
                {
                    if (m_Lead)
                    {
                        transform.LookAt(transform.position + move);
                        Move();
                    }
                    else
                    {
                        m_Animation.Play("run");
                    }
                }
                else
                {
                    MoveRandom();
                }
            }
        }
    }

    public void Move()
    {
        m_Forward.z = m_Speed;
        transform.Translate(m_Forward);
        m_Animation.Play("run");
    }

    void MoveRandom()
    {
        Vector2 temp = Random.insideUnitCircle;
        float tempDistance = Random.Range(0, 10.0f);
        temp = temp * tempDistance;
        move = new Vector3(temp.x, 0, temp.y);
    }

    void Attack(GameObject warrior)
    {
        transform.LookAt(warrior.transform.position);
        m_Animation.PlayQueued("attack1", QueueMode.PlayNow);
        warrior.GetComponent<EnemyPlayer>().DamageEntity(10.0f);
        warrior.GetComponent<Animator>().SetTrigger("Pain");
    }

    void OnTriggerEnter(Collider col)
    {
        if (col.gameObject.CompareTag("Bubble"))
        {
            inRange = true;
        }
    }

    public void Die()
    {
        m_Animation.Play("death");
        m_Dead = true;
    }

    public bool InRange()
    {
        return inRange;
    }

    public void SetRoaming()
    {
        m_Roaming = true;
    }
}

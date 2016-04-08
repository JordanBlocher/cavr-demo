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
    Vector3 m_Up;
    int counter;

    public bool Roaming
    {
        get { return m_Roaming; }
        set { m_Roaming = value; }
    }

    public bool InRange
    {
        get { return inRange; }
        set { inRange = value; }
    }

    // Use this for initialization
    void Start () {
        m_Animation = GetComponent<Animation>();
        pursuing = false;
        inRange = false;
        m_Forward = Vector3.zero;
        m_Up = Vector3.zero;
        MoveRandom();
        counter = 0;
    }

    // Update is called once per frame
    void Update ()
    {
        if (!m_Dead)
        {
            RaycastHit hitInfo;

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
                    if (Physics.Raycast(transform.position + move + (Vector3.up * 0.5f), Vector3.down, out hitInfo, 5.5f))
                    {
                        m_Up.y = hitInfo.point.y;
                        Move();
                    }
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
                counter++;
                if (Physics.Raycast(transform.position + move + (Vector3.up * 0.5f), Vector3.down, out hitInfo, 5.5f))
                {
                    m_Up.y = hitInfo.point.y;
                    if (m_Lead)
                    {
                        transform.LookAt(transform.position + move + (Vector3.up * 0.5f));
                        Move();
                    }
                    else
                    {
                        GameObject ogg = GameObject.Find("LeadGoblin");
                        transform.LookAt(transform.position + ogg.GetComponent<Goblin>().move);
                        Move();
                    }
                }
                if (counter >= 50)
                {
                    counter = 0;
                    MoveRandom();
                }
            }
        }
    }

    public void Move()
    {
        m_Forward.z = m_Speed;
        transform.Translate(m_Forward + m_Up);
        m_Animation.Play("run");
    }

    void MoveRandom()
    {
        Vector2 temp = Random.insideUnitCircle;
        float tempDistance = Random.Range(0, 10.0f);
        temp = temp * tempDistance;
        move = new Vector3(temp.x, m_Up.y, temp.y);
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

    void OnCollisionEnter(Collision col)
    {
        if (!inRange)
        {
            MoveRandom();
        }
    }

    public void Die()
    {
        m_Animation.Play("death");
        m_Dead = true;
    }

}

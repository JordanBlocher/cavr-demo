using UnityEngine;
using System.Collections;

public class EnemyPlayer : MonoBehaviour {

	protected float Health = 100;
	protected float MaxHealth = 100;
	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
	
	}

	public bool DamageEntity(float damage)
	{
		Health -= damage;
		return IsDead();
	}

	public float Normalizedhealth()
	{
		return Health / MaxHealth;
	}

	public float CurrentHealth()
	{
		return Health;
	}

	public bool IsDead()
	{
		return Health <= 0;
	}

}

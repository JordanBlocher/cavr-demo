using UnityEngine;

namespace UnityStandardAssets.Characters.ThirdPerson
{
	[RequireComponent(typeof(Rigidbody))]
	[RequireComponent(typeof(CapsuleCollider))]
	[RequireComponent(typeof(Animator))]
	public class WarriorCharacter : MonoBehaviour
	{
		[SerializeField] float m_MovingTurnSpeed = 360;
		[SerializeField] float m_StationaryTurnSpeed = 180;
		[SerializeField] float m_JumpPower = 12f;
		[Range(1f, 4f)][SerializeField] float m_GravityMultiplier = 2f;
		[SerializeField] float m_RunCycleLegOffset = 0.2f; //specific to the character in sample assets, will need to be modified to work with others
		[SerializeField] float m_MoveSpeedMultiplier = 4f;
		[SerializeField] float m_AnimSpeedMultiplier = 1f;
		[SerializeField] float m_GroundCheckDistance = 1.5f;
        [SerializeField]
        bool m_hasLightning = true;

        Rigidbody m_Rigidbody;
		Animator m_Animator;
        long m_runTime;
		bool m_IsGrounded;
		float m_OrigGroundCheckDistance;
		const float k_Half = 0.5f;
		float m_TurnAmount;
		float m_ForwardAmount;
		Vector3 m_GroundNormal;
		float m_CapsuleHeight;
		Vector3 m_CapsuleCenter;
		CapsuleCollider m_Capsule;
		bool m_Crouching;
        public int WeaponIdx = 0;//unarmed, 1H, 2H, bow, dual, pistol, rifle, spear and ss(sword and shield)
        public bool wasAttacking;// we need this so we can take lock the direction we are facing during attacks, mecanim sometimes moves past the target which would flip the character around wildly

        float rotateSpeed = 20.0f; //used to smooth out turning

        public Vector3 movementTargetPosition;
        public Vector3 attackPos;
        public Vector3 lookAtPos;
        public bool buttonDown = false;
        GameObject lightning;
        GameObject cavr;
        //GameObject cam1, cam2;

        int hold;
        int[] WeaponStates = { 0, 1, 2, 3, 4, 7, 8 };

        void Start()
		{
			m_Animator = GetComponent<Animator>();
			m_Rigidbody = GetComponent<Rigidbody>();
			m_Capsule = GetComponent<CapsuleCollider>();
			m_CapsuleHeight = m_Capsule.height;
			m_CapsuleCenter = m_Capsule.center;

			m_Rigidbody.constraints = RigidbodyConstraints.FreezeRotationX | RigidbodyConstraints.FreezeRotationY | RigidbodyConstraints.FreezeRotationZ;
			m_OrigGroundCheckDistance = m_GroundCheckDistance;
            movementTargetPosition = transform.position;//initializing our movement target as our current position
            m_runTime = 0;
            m_AnimSpeedMultiplier = 1.0f;
            WeaponIdx = 4;
            lightning = GameObject.Find("Lightning Strike");
            cavr = GameObject.Find("CAVR");
            m_hasLightning = true;
            hold = 0;
           // cam1 = GameObject.Find("ThirdPersonCamera");
          //  cam2 = GameObject.Find("FirstPersonCamera");
        }


        public void Move(Vector3 move, bool jump)
		{
            //lightning.SetActive(false);
            // convert the world relative moveInput vector into a local-relative
            // turn amount and forward amount required to head in the desired
            // direction.
            if (move.magnitude > 1f) move.Normalize();
			move = transform.InverseTransformDirection(move);
			CheckGroundStatus();
			move = Vector3.ProjectOnPlane(move, m_GroundNormal);
			m_TurnAmount = Mathf.Atan2(move.x, move.z);
			m_ForwardAmount = move.z;

			ApplyExtraTurnRotation();

			// control and velocity handling is different when grounded and airborne:
			if (m_IsGrounded)
			{
				HandleGroundedMovement(jump);
			}
			else
			{
				HandleAirborneMovement();
			}

			// send input and other state parameters to the animator
			UpdateAnimator(move);
		}


        void UpdateAnimator(Vector3 move)
        {

            // transform.
            // update the animator parameters
            m_Animator.SetFloat("Forward", m_ForwardAmount, 0.1f, Time.deltaTime);
            m_Animator.SetFloat("Turn", m_TurnAmount, 0.1f, Time.deltaTime);
            m_Animator.SetBool("Crouch", m_Crouching);
            m_Animator.SetBool("OnGround", m_IsGrounded);
            if (!m_IsGrounded)
            {
                m_Animator.SetFloat("Jump", m_Rigidbody.velocity.y);
            }
            
            // the anim speed multiplier allows the overall speed of walking/running to be tweaked in the inspector,
            // which affects the movement speed because of the root motion.
            if (m_IsGrounded && move.magnitude > 0)
            {
                m_Animator.speed = m_AnimSpeedMultiplier;
            }
            else
            {
                // don't use that while airborne
                m_Animator.speed = 1;
            }   
        }

        public static Vector3 raycastHit(Vector3 position)
        {
            Camera camera = Camera.main;
            float range = float.MaxValue;
            bool found = false;
            RaycastHit closestObject = new RaycastHit();
            /// Cast a ray into the screen at the specified position
            Ray ray = camera.ScreenPointToRay(position);

            // Get all the intersections
            RaycastHit[] raycasts = Physics.RaycastAll(ray);

            // Search the intersections
            foreach (RaycastHit i in raycasts)
            {

                bool ignore = false;
                // Check if distance is greater than current range 
                if (i.distance < range && !ignore)
                {
                    range = i.distance;
                    closestObject = i;
                    found = true;
                }
            }
            //case: no object intersection found
            if (!found)
            {
                //return the passed in position
                return position;
            }
            //return the position of the raycast intersection
            return closestObject.point;
        }

        void OnTriggerEnter(Collider col)
        {
            if (col.gameObject.CompareTag("Enchant"))
            {
                GameObject enchant = GameObject.Find("EnchantHero");
                if (enchant)
                {
                    enchant.SetActive(true);
                }
                m_hasLightning = true;

            }
            if (col.gameObject.CompareTag("Cave"))
            {
                GameObject enchant = GameObject.Find("EnchantHero");
                GameObject psystems = GameObject.Find("PSystem");
                if (psystems)
                {
                    if (psystems.activeInHierarchy)
                    {
                        psystems.SetActive(false);
                        if (enchant)
                        {
                            enchant.SetActive(false);
                        }
                    }
                    else
                    {
                        psystems.SetActive(true);
                    }

                }
            }
            if (col.gameObject.CompareTag("Dragon"))
            {
                if(Camera.main.name == "FirstPersonCamera")
                {
                    GameObject cam = GameObject.Find("FirstPersonCamera");
                    GameObject dragon = GameObject.Find("DragonBoss");
                    cam.GetComponent<UnityStandardAssets.Cameras.LookatTarget>().SetTarget(dragon.transform);
                }
            }
        }

        public void UpdateState()
        {
            hold++; 
            //if (Input.GetMouseButton(1))
            if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("attack"))
            {
                m_Animator.SetTrigger("Use");
            }
            //if (Input.GetMouseButton(0) && m_hasLightning)
            if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("lightning") && m_hasLightning)
            {
                Debug.Log("LIGHTNING");
                Vector3 enemyPos = raycastHit(cavr.GetComponent<CaVR>().InputManger.GetSixdofValue("pointer").Forward);
                if (enemyPos != Vector3.zero)
                {
                    lightning.transform.position = enemyPos;
                    lightning.SetActive(true);
                }

                GameObject dragon = GameObject.Find("DragonBoss");
                lightning.transform.position = dragon.transform.position - new Vector3(0,-5,0);
            }
            if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("level1"))
            {
                GameObject level1 = GameObject.Find("cavePortal");
                transform.position = level1.transform.position;
            }
            if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("level2"))
            {
                GameObject level2 = GameObject.Find("castlePortal");
                transform.position = level2.transform.position;
            }
            if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("camera"))
            {
                Debug.Log("CAMERA");           
                
            }
            if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("WeaponChangeUp") && hold > 50)
            {
                hold = 0;
                if (WeaponIdx < 6)
                    WeaponIdx++;
            }
            else if (cavr.GetComponent<CaVR>().InputManger.GetButtonValue("WeaponChangeDown") && hold > 50)
            {
                hold = 0;
                if (WeaponIdx > 0)
                    WeaponIdx--;
            }

            Debug.Log(WeaponStates[WeaponIdx]);
            m_Animator.SetInteger("WeaponState", WeaponStates[WeaponIdx]);// probably would be better to check for change rather than bashing the value in like this

            /*
            switch (Input.inputString)//get keyboard input, probably not a good idea to use strings here...Garbage collection problems with regards to local string usage are known to happen
            {                        //the garbage collection memory problem arises from local alloction of memory, and not freeing it up efficiently
                case "0":
                    WeaponState = 0;//unarmed
                    break;
                case "1":
                    WeaponState = 1;//1H one handed weapon
                    break;
                case "2":
                    WeaponState = 2;//2H two handed weapon(longsword or heavy axe)
                    break;
                case "3":
                    WeaponState = 3;//bow
                    break;
                case "4":
                    WeaponState = 4;//dual weild(short swords, light axes)
                    break;
                case "5":
                    WeaponState = 7;//spear
                    break;
                case "6":
                    WeaponState = 8;//Sword and Shield
                    break;
                case "n":
                    m_Animator.SetBool("NonCombat", true);//the animator controller will detect this non combat bool, and go into a non combat state "in" this weaponstate
                    break;                  
                default:                           
                    break;
            }
            m_Animator.SetInteger("WeaponState", WeaponState);// probably would be better to check for change rather than bashing the value in like this

            */
        }

        void HandleAirborneMovement()
		{
			// apply extra gravity from multiplier:
			Vector3 extraGravityForce = (Physics.gravity * m_GravityMultiplier) - Physics.gravity;
            m_Rigidbody.AddForce(new Vector3(0, 0, 2.5f*m_ForwardAmount));

            m_GroundCheckDistance = m_Rigidbody.velocity.y < 0 ? m_OrigGroundCheckDistance : 0.1f;
		}


		void HandleGroundedMovement( bool jump)
		{
            // check whether conditions are right to allow a jump:
            if (jump && m_Animator.GetCurrentAnimatorStateInfo(0).IsName("Grounded"))
			{
				// jump!
				m_Rigidbody.velocity = new Vector3(m_Rigidbody.velocity.x, m_JumpPower, m_Rigidbody.velocity.z);
				m_IsGrounded = false;
				m_Animator.applyRootMotion = false;
				m_GroundCheckDistance = 1.5f;
            }
        }

		void ApplyExtraTurnRotation()
		{
			// help the character turn faster (this is in addition to root rotation in the animation)
			float turnSpeed = Mathf.Lerp(m_StationaryTurnSpeed, m_MovingTurnSpeed, m_ForwardAmount);
			transform.Rotate(0, m_TurnAmount * turnSpeed * Time.deltaTime * 1.0f, 0);
		}


		public void OnAnimatorMove()
		{
			// we implement this function to override the default root motion.
			// this allows us to modify the positional speed before it's applied.
            
			if (m_IsGrounded && Time.deltaTime > 0)
			{
				Vector3 v = (m_Animator.deltaPosition * m_MoveSpeedMultiplier) / Time.deltaTime;

				// we preserve the existing y part of the current velocity.
				v.y = m_Rigidbody.velocity.y;
				m_Rigidbody.velocity = v;
			}
            
		}


		void CheckGroundStatus()
		{
			RaycastHit hitInfo;
#if UNITY_EDITOR
			// helper to visualise the ground check ray in the scene view
			Debug.DrawLine(transform.position + (Vector3.up * 0.1f), transform.position + (Vector3.up * 0.1f) + (Vector3.down * m_GroundCheckDistance));
#endif
			// 0.1f is a small offset to start the ray from inside the character
			// it is also good to note that the transform position in the sample assets is at the base of the character
			if (Physics.Raycast(transform.position + (Vector3.up * 0.5f), Vector3.down, out hitInfo, 5.5f))
			{
				m_GroundNormal = hitInfo.normal;
				m_IsGrounded = true;
				m_Animator.applyRootMotion = true;
			}
			else
			{
                m_IsGrounded = false;
				m_GroundNormal = Vector3.up;
				m_Animator.applyRootMotion = false;
			}
		}

    }


}

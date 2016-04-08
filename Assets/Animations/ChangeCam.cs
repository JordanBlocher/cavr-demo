using UnityEngine;
using System.Collections;

public class ChangeCam : MonoBehaviour {

    UnityStandardAssets.Characters.ThirdPerson.WarriorControl m_Control;
    GameObject cam1, cam2;
    bool toggle;
    int hold = 0;
    // Use this for initialization
    void Start () {
        m_Control = GetComponent<UnityStandardAssets.Characters.ThirdPerson.WarriorControl>();
        cam1 = GameObject.Find("FirstPersonCamera");
        cam2 = GameObject.Find("ThirdPersonCamera");
        cam1.SetActive(false);
        toggle = false;
    }

    // Update is called once per frame
    void Update () {
	    if(GameObject.Find("CAVR").GetComponent<CaVR>().InputManger.GetButtonValue("camera") && hold > 50)
        {
            Toggle();
            hold = 0;
        }
        hold++;
	}

    public void Toggle()
    {
        if (toggle)
        {
            Camera cam = cam1.GetComponent<Camera>();
            m_Control.m_Cam = cam.transform;
            cam1.SetActive(true);
            cam2.SetActive(false);
        }
        else 
        {
            Camera cam = cam2.GetComponent<Camera>();
            m_Control.m_Cam = cam.transform;
            cam2.SetActive(true);
            cam1.SetActive(false);
        }
        toggle = !toggle;
    }
}

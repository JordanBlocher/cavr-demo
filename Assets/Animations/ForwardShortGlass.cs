using UnityEngine;
using System.Collections.Generic;
using System;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

public class ForwardShortGlass : MonoBehaviour {

    GameObject cavr;

    // Use this for initialization
    void Start () {
        cavr = GameObject.Find("CAVR");
    }

    // Update is called once per frame
    void Update () {
        if (Camera.main.name == "FirstPersonCamera")
        {
            Sixdof glass = cavr.GetComponent<CaVR>().InputManger.GetSixdofValue("forward");
            Camera.main.transform.rotation = glass.Rotation;
        }
    }
}

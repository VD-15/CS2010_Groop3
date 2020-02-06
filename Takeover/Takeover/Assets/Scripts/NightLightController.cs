using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NightLightController : MonoBehaviour
{
	private Light lightComp;

    // Start is called before the first frame update
    void Start()
    {
		this.lightComp = this.GetComponent<Light>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

	public void TurnOn()
	{
		this.lightComp.intensity = 1f;
	}

	public void TurnOff()
	{
		this.lightComp.intensity = 0f;
	}
}

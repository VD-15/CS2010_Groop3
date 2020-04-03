using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DaylightController : MonoBehaviour
{
	//Length of a day in seconds
	[SerializeField] private float dayLength;

	//noon = 0.25, midnight = 0.75
	[SerializeField] private float timeOfDay;

	[SerializeField] private Color colorNoon;
	[SerializeField] private Color colorSunrise;
	[SerializeField] private Color colorSunset;
	[SerializeField] private Color colorMidnight;

	new private Light light;

    // Start is called before the first frame update
    void Start()
    {
		this.light = this.GetComponent<Light>();
    }

    // Update is called once per frame
    void Update()
    {
		float timestep = Time.deltaTime / this.dayLength;

		if (this.timeOfDay < 0.5f && this.timeOfDay + timestep >= 0.5f)
		{
			NightLightController[] lights = FindObjectsOfType<NightLightController>();

			foreach (NightLightController n in lights)
			{
				n.TurnOn();
			}
		}
		else if (this.timeOfDay < 1f && this.timeOfDay + timestep >= 1f)
		{
			NightLightController[] lights = FindObjectsOfType<NightLightController>();

			foreach (NightLightController n in lights)
			{
				n.TurnOff();
			}
		}

		this.timeOfDay += timestep;

		while (this.timeOfDay > 1.0f) this.timeOfDay -= 1.0f;

		this.transform.Rotate(Vector3.right, timestep * 360f);

		if (this.timeOfDay < 0.25f)
			this.light.color = MixColor(this.colorSunrise, this.colorNoon, this.timeOfDay * 4f);
		else if (this.timeOfDay < 0.5f)
			this.light.color = MixColor(this.colorNoon, this.colorSunset, (this.timeOfDay - 0.25f) * 4f);
		else if (this.timeOfDay < 0.75f)
			this.light.color = MixColor(this.colorSunset, this.colorMidnight, (this.timeOfDay - 0.5f) * 4f);
		else
			this.light.color = MixColor(this.colorMidnight, this.colorSunrise, (this.timeOfDay - 0.75f) * 4f);

		this.light.intensity = Mathf.Sin(this.timeOfDay * Mathf.PI * 2f);

    }

	private static Color MixColor(Color c1, Color c2, float f)
	{
		return (c1 * (1f - f)) + (c2 * f);
	}
}

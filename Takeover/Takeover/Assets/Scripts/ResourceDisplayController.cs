using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ResourceDisplayController : MonoBehaviour
{
	[SerializeField] private Team team;
	[SerializeField] private Text[] texts;

	private void Start()
	{
		//this.text = this.GetComponentInChildren<Text>();
	}

	private void OnGUI()
	{
		//this.text.text = "Resources:\n";
		this.PutResource(0, Resource.Steel);
		this.PutResource(1, Resource.Rubber);
		this.PutResource(2, Resource.Aluminium);
		this.PutResource(3, Resource.Oil);
		this.PutResource(4, Resource.Silicone);
		this.PutResource(5, Resource.Uranium);
		this.PutResource(6, Resource.Power);
		this.PutTime(7);
	}

	private void PutResource(int i, Resource res)
	{
		ResourceEntry r = ResourceManager.GetResourceInfo(res, this.team);

		this.texts[i].text = res.ToString() + " " + r.amount + "/" + r.capacity + "\n";
	}

	private void PutTime(int i)
	{/*
		int seconds = (int)(Time.time % 60.0f);
		int minutes = Mathf.FloorToInt(Time.time / 60.0f);

		this.texts[i].text = minutes.ToString() + ":" + seconds.ToString().PadLeft(2, '0');*/
		this.texts[i].text = "Turn: " + TurnManager.GetNumTurns().ToString();
	}
}

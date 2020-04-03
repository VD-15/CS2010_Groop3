using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//Controlls capturing of buildings
public class CaptureManager : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.Capture);
	}

	private void OnDisable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.Capture);
	}

	private void OnTurn()
	{
		Debug.Log("Capture onturn");

		CapturerController[] capturers = FindObjectsOfType<CapturerController>();
		CapturableController[] capturables = FindObjectsOfType<CapturableController>();

		foreach (CapturableController target in capturables)
		{
			bool friendlyPresent = false;
			bool hostilePresent = false;
			bool team1Present = false;
			bool team2Present = false;
			Team hostileTeam = Team.Neutral;

			foreach (CapturerController attacker in capturers)
			{
				if (target.IsTerritory(attacker.GetLocation()))
				{
					if (target.GetTeam() == attacker.GetTeam())
					{
						friendlyPresent = true;
					}
					else
					{
						hostilePresent = true;
						hostileTeam = attacker.GetTeam();
					}

					if (attacker.GetTeam() == Team.Team1) team1Present = true;
					if (attacker.GetTeam() == Team.Team2) team2Present = true;
				}
			}

			if (target.GetTeam() == Team.Neutral && team1Present && team2Present)
			{
				//Deadlock
				Debug.Log("Capture deadlock created. Do something about that.");
				target.UpdateCapture(true, true, Team.Neutral);
			}
			else
			{
				target.UpdateCapture(friendlyPresent, hostilePresent, hostileTeam);
			}
		}
	}
}

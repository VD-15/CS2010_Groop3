using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CommanderController : MonoBehaviour
{
	[SerializeField] private GameObject gameOverScreen;
	[SerializeField] private GameObject winScreen;

	private TeamController team;

    // Start is called before the first frame update
    void Start()
    {
		this.team = this.GetComponent<TeamController>();
    }

	private void OnDestroy()
	{
		if (this.team.team == Team.Team1)
		{
			//Loss
			this.gameOverScreen.SetActive(true);
		}
		else
		{
			//Victory
			this.winScreen.SetActive(true);
		}
	}
}

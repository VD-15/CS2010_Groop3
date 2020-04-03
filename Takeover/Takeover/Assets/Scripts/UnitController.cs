using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitController : MonoBehaviour
{
	[SerializeField] private int maxHealth;
	[SerializeField] private float luck;
	[SerializeField] private int health;
	[SerializeField] private int attack;

	private UnitController target;
	private TeamController team;

	[SerializeField] private Material materialRed;
	[SerializeField] private Material materialBlue;

	// Start is called before the first frame update
	void Start()
    {
		this.team = this.GetComponent<TeamController>();
		this.health = this.maxHealth;

		if (this.team.team == Team.Team2)
		{
			Renderer[] renderers = this.GetComponentsInChildren<Renderer>();

			foreach (Renderer r in renderers)
			{
				Material[] mats = r.materials;

				for (int i = 0; i < mats.Length; i++)
				{
					if (mats[i].name.Contains(this.materialBlue.name))
					{
						mats[i] = this.materialRed;
					}
				}

				r.materials = mats;
			}
		}
    }

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnCombat, TurnStage.Combat);
		TurnManager.AddTurnEvent(this.OnKillUnits, TurnStage.KillUnits);
	}

	private void OnDisable()
	{
		TurnManager.RemoveTurnEvent(this.OnCombat, TurnStage.Combat);
		TurnManager.RemoveTurnEvent(this.OnKillUnits, TurnStage.KillUnits);
	}

	private void OnDestroy()
	{
		ResourceManager.AddResources(Resource.Power, this.team.team, 1);
	}

	private void OnCombat()
	{
		if (this.target != null)
		{
			this.target.health -= this.attack;

			//Random bonus damage
			if (Random.Range(0f, 1f) < this.luck)
			{
				this.target.health--;
			}

			if (this.target.health <= 0)
			{
				this.target = null;
			}
		}
	}

	private void OnKillUnits()
	{
		if (this.health <= 0)
		{
			GameObject.Destroy(this.gameObject);
		}
	}

	public Team GetTeam()
	{
		return this.team.team;
	}

	public void IssueAttackCommand(UnitController target)
	{
		if (target.GetTeam() != this.team.team)
		{
			this.target = target;
		}
	}

	public UnitController GetTarget()
	{
		return this.target;
	}

	public int GetHealth()
	{
		return this.health;
	}

	public int GetMaxHealth()
	{
		return this.maxHealth;
	}
}

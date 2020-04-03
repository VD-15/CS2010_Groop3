using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum TurnStage
{
	EnemyAI =		0,

	Combat =		1,
	KillUnits =		2,

	Move =			3,
	Position =		4,

	Capture =		5,
	Resource =		6,
	Production =	7,

	UI =			8
}

public class TurnEventProxy
{
	public delegate void TurnFun();

	private event TurnFun OnTurn;

	public TurnEventProxy()
	{

	}

	public void AddEvent(TurnFun f)
	{
		this.OnTurn += f;
	}

	public void RemoveEvent(TurnFun f)
	{
		this.OnTurn -= f;
	}

	public void Execute()
	{
		this.OnTurn?.Invoke();
	}
}

//Halts progression of turn until object is disposed
//Use this for animations & coroutines
//Use this in conjunction with a `using` block
public class TurnBlocker : IDisposable
{
	public TurnBlocker()
	{
		TurnManager.AddTurnBlocker(this);
	}

	public void Dispose()
	{
		TurnManager.RemoveTurnBlocker(this);
	}
}

public class TurnManager : MonoBehaviour
{
	public delegate void TurnFun();

	//private static event TurnFun MoveUnits;

	private static Dictionary<TurnStage, TurnEventProxy> turnFuns;
	private static HashSet<TurnBlocker> turnBlocks;
	private static bool isTurnInProgress;
	private static int numTurns;

	public static void AddTurnEvent(TurnEventProxy.TurnFun fun, TurnStage stage)
	{
		if (TurnManager.turnFuns == null)
		{
			InitDictionary();
		}

		turnFuns[stage].AddEvent(fun);
	}

	public static void RemoveTurnEvent(TurnEventProxy.TurnFun fun, TurnStage stage)
	{
		turnFuns[stage].RemoveEvent(fun);
	}

	public void OnClick()
	{
		if (!isTurnInProgress)
		{
			Debug.Log("Turn issued");
			this.StartCoroutine(this.DoTurn());
		}
	}

	private IEnumerator DoTurn()
	{
		isTurnInProgress = true;
		numTurns++;

		foreach (TurnStage stage in (TurnStage[])System.Enum.GetValues(typeof(TurnStage)))
		{
			turnFuns[stage].Execute();

			//Wait until all blocks are cleared
			while (TurnManager.turnBlocks.Count > 0)
			{
				yield return null;
			}
		}

		isTurnInProgress = false;
	}

	private static void InitDictionary()
	{
		TurnManager.turnFuns = new Dictionary<TurnStage, TurnEventProxy>();
		TurnManager.turnBlocks = new HashSet<TurnBlocker>();
		TurnManager.isTurnInProgress = false;

		foreach (TurnStage stage in (TurnStage[])System.Enum.GetValues(typeof(TurnStage)))
		{
			turnFuns[stage] = new TurnEventProxy();
		}
	}

	public static int GetNumTurns()
	{
		return TurnManager.numTurns;
	}

	internal static void AddTurnBlocker(TurnBlocker b)
	{
		TurnManager.turnBlocks.Add(b);
	}

	internal static void RemoveTurnBlocker(TurnBlocker b)
	{
		TurnManager.turnBlocks.Remove(b);
	}
}

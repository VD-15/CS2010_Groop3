using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CursorController : MonoBehaviour
{
	[SerializeField] private Texture2D moveCursor;
	[SerializeField] private Texture2D attackCursor;

	private CursorMode mode;

    // Start is called before the first frame update
    void Start()
    {
		this.mode = CursorMode.Move;
    }

	private void OnEnable()
	{
		this.UpdateCursor();
	}

	private void OnDisable()
	{
		Cursor.SetCursor(null, Vector2.zero, UnityEngine.CursorMode.Auto);
	}

	private void UpdateCursor()
	{
		switch (this.mode)
		{
			case CursorMode.Move:
				Cursor.SetCursor(this.moveCursor, new Vector2(32f, 32f), UnityEngine.CursorMode.ForceSoftware);
				break;
			case CursorMode.Attack:
				Cursor.SetCursor(this.attackCursor, new Vector2(32f, 32f), UnityEngine.CursorMode.ForceSoftware);
				break;
			default:
				break;
		}
	}

	public void SetAttack()
	{
		this.mode = CursorMode.Attack;
		this.UpdateCursor();
	}

	public void SetMove()
	{
		this.mode = CursorMode.Move;
		this.UpdateCursor();
	}

}

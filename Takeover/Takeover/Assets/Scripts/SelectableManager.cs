using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public enum CursorMode
{
	Move,
	Attack
}

public class SelectableManager : MonoBehaviour
{
	[SerializeField] private GameObject previewCamera;
	[SerializeField] private Text previewName;

	private readonly int unitLayerMask = 1 << 8;
	private readonly int mapLayerMask = 1 << 9;

	private SelectableController hovered;
	private SelectableController selected;

	public delegate void SelectFun(SelectableController s);
	public delegate void DeselectFun();

	//Called when a new object is selected or the existing selection is changed
	public static event SelectFun OnObjectSelect;

	//Called when an object is deselected and no new object is selected
	public static event DeselectFun OnObjectDeselect;

	private CursorMode cursorMode;

    // Start is called before the first frame update
    private void Start()
    {
		this.hovered = null;
		this.selected = null;
		this.cursorMode = CursorMode.Move;
    }

	// Update is called once per frame
	void Update()
    {
		//Ignore UI-related stuff
		if (EventSystem.current.IsPointerOverGameObject())
		{
			return;
		}

		if (Input.GetMouseButtonDown(0))
		{
			this.ComputeSelect();
		}
		else if (Input.GetMouseButtonDown(1))
		{
			if (this.selected != null && this.selected.GetComponent<TeamController>().team != Team.Team1) return;

			switch (this.cursorMode)
			{
				case CursorMode.Move:
					this.ComputeMove();
					break;
				case CursorMode.Attack:
					this.ComputeAttack();
					break;
				default:
					break;
			}
		}
		else
		{
			this.ComputeHover();
		}
    }

	private void ComputeHover()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

		if (Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, this.unitLayerMask))
		{
			if (this.hovered == null)
			{
				SelectableController s = hit.transform.GetComponent<SelectableController>();

				if (s != null)
				{
					this.hovered = s;
					this.hovered.OnHoverEnter();
				}
			}
			else if(this.hovered.transform != hit.transform)
			{
				this.hovered.OnHoverLeave();

				SelectableController s = hit.transform.GetComponent<SelectableController>();

				if (s != null)
				{
					this.hovered = s;
					this.hovered.OnHoverEnter();
				}
			}
		}
		else
		{
			if (this.hovered != null)
			{
				this.hovered.OnHoverLeave();
				this.hovered = null;
			}
		}
	}

	private void ComputeSelect()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

		if (Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, this.unitLayerMask))
		{
			if (this.selected == null)
			{
				SelectableController s = hit.transform.GetComponent<SelectableController>();

				if (s != null)
				{
					this.selected = s;
					this.selected.OnSelect();

					SelectableManager.OnObjectSelect?.Invoke(s);
				}
			}
			else if (this.selected.transform != hit.transform)
			{
				this.selected.OnDeselect();

				SelectableController s = hit.transform.GetComponent<SelectableController>();

				if (s != null)
				{
					this.selected = s;
					this.selected.OnSelect();

					SelectableManager.OnObjectSelect?.Invoke(s);
				}
				else
				{
					this.selected = null;
					SelectableManager.OnObjectDeselect?.Invoke();
				}
			}
		}
		else
		{
			if (this.selected != null)
			{
				this.selected.OnDeselect();
				this.selected = null;

				SelectableManager.OnObjectDeselect?.Invoke();
			}
		}
	}

	private void ComputeMove()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

		if (Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, this.mapLayerMask))
		{
			Debug.Log(hit.point);

			if (this.selected != null)
			{
				MoveableComponent moveable = this.selected.GetComponent<MoveableComponent>();

				if (moveable != null)
				{
					moveable.IssueMoveCommand(hit.point);
					this.selected.UpdatePath();
				}
			}
		}
	}

	private void ComputeAttack()
	{
		Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

		if (Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, this.unitLayerMask))
		{
			Debug.Log("Attack command: " + hit.point.ToString());

			if (this.selected != null)
			{
				UnitController fUnit = this.selected.GetComponent<UnitController>();
				UnitController hUnit = hit.transform.GetComponent<UnitController>();

				if (fUnit != null && hUnit != null)
				{
					if (Vector3.Distance(fUnit.transform.position, hUnit.transform.position) < 1.1f)
					{
						fUnit.IssueAttackCommand(hUnit);

						MoveableComponent moveable = fUnit.GetComponent<MoveableComponent>();
						moveable.IssueMoveCommand(moveable.transform.position);
						this.selected.UpdatePath();
					}
				}
			}
		}
	}

	//Can't bind buttons to functions that take enum parameters, so that's why these are horrible:

	public void SetAttackCursor()
	{
		this.cursorMode = CursorMode.Attack;
	}

	public void SetMoveCursor()
	{
		this.cursorMode = CursorMode.Move;
	}
}

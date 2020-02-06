using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectableManager : MonoBehaviour
{
	private readonly int unitLayerMask = 1 << 8;
	private readonly int mapLayerMask = 1 << 9;

	private SelectableController hover;
	private SelectableController selected;

    // Start is called before the first frame update
    void Start()
    {
		this.hover = null;   
    }

    // Update is called once per frame
    void Update()
    {
		if (Input.GetMouseButtonDown(0))
		{
			this.ComputeSelect();
		}
		else if (Input.GetMouseButtonDown(1))
		{
			this.ComputeMove();
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
			if (this.hover == null)
			{
				this.hover = hit.transform.GetComponent<SelectableController>();

				if (this.hover != null)
				{
					this.hover.HoverEnter();
				}
			}
			else if(this.hover.transform != hit.transform)
			{
				this.hover.HoverLeave();

				this.hover = hit.transform.GetComponent<SelectableController>();

				if (this.hover != null)
				{
					this.hover.HoverEnter();
				}
			}
		}
		else
		{
			if (this.hover != null)
			{
				this.hover.HoverLeave();
				this.hover = null;
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
				this.selected = hit.transform.GetComponent<SelectableController>();

				if (this.selected != null)
				{
					this.selected.Select();
				}
			}
			else if (this.selected.transform != hit.transform)
			{
				this.selected.Deselect();

				this.selected = hit.transform.GetComponent<SelectableController>();

				if (this.selected != null)
				{
					this.selected.Select();
				}
			}
		}
		else
		{
			if (this.selected != null)
			{
				this.selected.Deselect();
				this.selected = null;
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
				MoveableComponent moveable = this.selected.transform.GetComponent<MoveableComponent>();

				if (moveable != null)
				{
					moveable.IssueMoveCommand(hit.point);
				}
			}
		}
	}
}

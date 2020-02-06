using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectableManager : MonoBehaviour
{
	private int unitLayerMask = 1 << 8;

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
		else
		{
			this.ComputeHover();
		}
    }

	void ComputeHover()
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

	void ComputeSelect()
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
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectableController : MonoBehaviour
{
	private SpriteRenderer highlight;
	private bool selected;

	// Start is called before the first frame update
	void Start()
    {
		this.highlight = this.GetComponentInChildren<SpriteRenderer>();
		this.enabled = false;
		this.selected = false;
    }

    // Update is called once per frame
    void Update()
    {
		//this.transform.Rotate(Vector3.up, Time.deltaTime, Space.World);
    }

	public void HoverEnter()
	{
		this.highlight.enabled = true;

		if (this.selected)
		{
			this.highlight.color = Color.cyan;
		}
		else
		{
			this.highlight.color = Color.green;
		}
	}

	public void HoverLeave()
	{
		if (this.selected)
		{
			this.highlight.color = Color.blue;
		}
		else
		{
			this.highlight.enabled = false;
		}
	}

	public void Select()
	{
		this.highlight.enabled = true;
		this.highlight.color = Color.cyan;
		this.selected = true;
	}

	public void Deselect()
	{
		this.highlight.enabled = false;
		this.selected = false;
	}
}

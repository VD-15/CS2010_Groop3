using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SelectableController : MonoBehaviour
{
	[SerializeField] private GameObject highlightPrefab;
	[SerializeField] private GameObject pathMarkerPrefab;
	[SerializeField] private Color selectedColor;
	[SerializeField] private Color hoveredColor;
	[SerializeField] private string unitName;

	//Can be null
	private MoveableComponent moveable;

	private SpriteRenderer highlightInstance;
	private List<MeshRenderer> markerInstances;
	private bool isSelected;

	private void Start()
	{
		this.isSelected = false;
		this.highlightInstance = null;
		this.markerInstances = new List<MeshRenderer>();
		this.moveable = this.GetComponent<MoveableComponent>();
	}

	private void OnEnable()
	{
		TurnManager.AddTurnEvent(this.OnTurn, TurnStage.Position);
	}

	private void OnDisable()
	{
		TurnManager.RemoveTurnEvent(this.OnTurn, TurnStage.Position);
	}

	private void OnDestroy()
	{
		if (this.highlightInstance != null)
		{
			GameObject.Destroy(this.highlightInstance);
		}

		foreach (MeshRenderer m in this.markerInstances)
		{
			GameObject.Destroy(m.gameObject);
		}
	}

	private void OnApplicationQuit()
	{
		this.highlightInstance = null;
		this.markerInstances.Clear();
	}

	private void Update()
	{
		if (this.isSelected)
		{
			this.highlightInstance.transform.Rotate(this.transform.TransformVector(Vector3.up), Time.deltaTime * 90f, Space.World);
		}
	}

	private void OnTurn()
	{
		if (this.moveable != null && this.isSelected)
		{
			this.UpdatePath();
		}
	}

	public void OnSelect()
	{
		if (this.highlightInstance == null)
		{
			this.highlightInstance = GameObject.Instantiate(this.highlightPrefab, this.transform).GetComponent<SpriteRenderer>();
		}

		this.highlightInstance.color = this.selectedColor + this.hoveredColor;

		foreach (MeshRenderer m in this.markerInstances)
		{
			m.material.color = this.selectedColor + this.hoveredColor;
		}

		this.isSelected = true;
	}

	public void OnDeselect()
	{
		this.isSelected = false;

		GameObject.Destroy(this.highlightInstance.gameObject);
		this.highlightInstance = null;

		foreach (MeshRenderer m in this.markerInstances)
		{
			GameObject.Destroy(m.gameObject);
		}

		this.markerInstances.Clear();
	}

	public void OnHoverEnter()
	{
		if (!this.isSelected)
		{
			this.highlightInstance = GameObject.Instantiate(this.highlightPrefab, this.transform).GetComponent<SpriteRenderer>();

			if (this.moveable != null)
			{
				this.UpdatePath();
			}
		}

		Color c;

		if (this.isSelected)	c = this.hoveredColor + this.selectedColor;
		else					c = this.hoveredColor;

		this.highlightInstance.color = c;

		foreach (MeshRenderer m in this.markerInstances)
		{
			m.material.color = c;
		}
	}

	public void OnHoverLeave()
	{
		if (this.isSelected)
		{
			this.highlightInstance.color = this.selectedColor;

			foreach (MeshRenderer m in this.markerInstances)
			{
				m.material.color = this.selectedColor;
			}
		}
		else
		{
			GameObject.Destroy(this.highlightInstance.gameObject);
			this.highlightInstance = null;

			foreach (MeshRenderer m in this.markerInstances)
			{
				GameObject.Destroy(m.gameObject);
			}

			this.markerInstances.Clear();
		}
	}

	//Assumes moveable is not null
	public void UpdatePath()
	{
		foreach (MeshRenderer m in this.markerInstances)
		{
			GameObject.Destroy(m.gameObject);
		}

		this.markerInstances.Clear();

		this.moveable.GetPath(out List<Vector3> path);

		if (this.markerInstances.Capacity < path.Count)
		{
			this.markerInstances.Capacity = path.Count;
		}

		foreach (Vector3 vec in path)
		{
			MeshRenderer m = GameObject.Instantiate(this.pathMarkerPrefab, vec, Quaternion.identity, null).GetComponent<MeshRenderer>();
			m.material.color = this.selectedColor;
			this.markerInstances.Add(m);
		}
	}

	public string GetName()
	{
		return this.unitName;
	}
}

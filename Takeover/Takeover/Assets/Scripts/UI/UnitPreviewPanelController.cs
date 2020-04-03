using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitPreviewPanelController : MonoBehaviour
{
	[SerializeField] private GameObject previewCamera;

    // Start is called before the first frame update
    private void Start()
    {
		SelectableManager.OnObjectSelect += this.OnObjectSelect;
		SelectableManager.OnObjectDeselect += this.OnObjectDeselect;
	}

	private void OnDestroy()
	{
		SelectableManager.OnObjectSelect -= this.OnObjectSelect;
		SelectableManager.OnObjectDeselect -= this.OnObjectDeselect;
	}

	private void OnObjectSelect(SelectableController s)
	{
		this.previewCamera.transform.position = s.transform.position;
	}

	private void OnObjectDeselect()
	{
		this.previewCamera.transform.position = new Vector3(0.0f, -5.0f, 0.0f);
	}
}

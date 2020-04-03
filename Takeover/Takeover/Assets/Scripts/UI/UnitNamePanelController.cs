using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UnitNamePanelController : MonoBehaviour
{
	private Text text;

    // Start is called before the first frame update
    void Start()
    {
		this.text = this.GetComponentInChildren<Text>();
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
		this.text.text = s.GetName();
	}

	private void OnObjectDeselect()
	{
		this.text.text = "";
	}
}

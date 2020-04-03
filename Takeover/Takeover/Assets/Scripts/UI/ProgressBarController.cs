using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ProgressBarController : MonoBehaviour
{
	[SerializeField] private RectTransform frontbarTransform;
	[SerializeField] private Image frontbarImage;
	[SerializeField] private Text frontText;

	[SerializeField] private Color fullColor;
	[SerializeField] private Color midColor;
	[SerializeField] private Color emptyColor;

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
		CapturableController cc = s.GetComponent<CapturableController>();

		if (cc != null)
		{
			this.ChangeProgress((float)cc.GetCaptureProgress());
			this.frontText.text = "Capture Progress";
		}

		//Check for combat controller here
	}

	private void OnObjectDeselect()
	{
		this.frontbarTransform.anchorMax = new Vector2(1.0f, 1.0f);
		this.frontbarImage.color = Color.white;
		this.frontText.text = "";
	}

	private void ChangeProgress(float progress)
	{
		this.frontbarTransform.anchorMax = new Vector2(progress, 1.0f);

		if (progress < 0.5f)
		{
			this.frontbarImage.color = Color.Lerp(this.emptyColor, this.midColor, progress * 2.0f);
		}
		else
		{
			this.frontbarImage.color = Color.Lerp(this.midColor, this.fullColor, (progress - 0.5f) * 2.0f);
		}
	}
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CaptureDisplayController : MonoBehaviour
{
	[SerializeField] private GameObject textPrefab;

	private CapturableController capture;
	private GameObject text;

    // Start is called before the first frame update
    void Start()
    {
		this.capture = this.GetComponent<CapturableController>();
    }

    // Update is called once per frame
    void Update()
    {
		CaptureState s = this.capture.GetState();

		if (s == CaptureState.Idle)
		{
			if (this.text != null)
			{
				GameObject.Destroy(this.text);
				this.text = null;
			}
		}
		else
		{
			if (this.text == null)
			{
				this.text = GameObject.Instantiate(this.textPrefab, this.transform);
			}

			this.text.transform.LookAt(new Vector3(this.transform.position.x, Camera.main.transform.position.y, Camera.main.transform.position.z));

			TextMesh mesh = this.text.GetComponent<TextMesh>();

			string text = "SAMPLE_TEXT";
			double captureProgress = this.capture.GetCaptureProgress();

			switch (s)
			{
				case CaptureState.Capturing:
					text = "Capturing";
					break;
				case CaptureState.Contested:
					text = "Contested";
					break;
				case CaptureState.Resolving:
					text = "Resolving";
					break;
			}

			mesh.text = text + "\n" + (captureProgress * 100.0).ToString() + "%";
		}
    }
}

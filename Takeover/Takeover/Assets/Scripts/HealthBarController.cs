using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HealthBarController : MonoBehaviour
{
	[SerializeField] private GameObject textPrefab;

	private UnitController unit;
	private GameObject text;

	private Transform barLeft;
	private Transform barRight;

	// Start is called before the first frame update
	void Start()
	{
		this.unit = this.GetComponent<UnitController>();

		this.barLeft = null;
		this.barRight = null;
	}

	// Update is called once per frame
	void Update()
	{
		if (this.unit.GetHealth() < this.unit.GetMaxHealth())
		{
			if (this.text == null)
			{
				this.text = GameObject.Instantiate(this.textPrefab, this.transform);

				this.barLeft = this.text.transform.GetChild(1);
				this.barRight = this.text.transform.GetChild(0);
			}

			this.text.transform.LookAt(new Vector3(this.transform.position.x, Camera.main.transform.position.y, Camera.main.transform.position.z));

			TextMesh mesh = this.text.GetComponent<TextMesh>();

			mesh.text = this.unit.GetHealth().ToString() + "/" + this.unit.GetMaxHealth();

			float ratio = (float)this.unit.GetHealth() / (float)this.unit.GetMaxHealth();

			this.barLeft.localScale = new Vector3(ratio, 1f, -1f);
			this.barRight.localScale = new Vector3(1f - ratio, 1f, -1f);
		}
	}
}

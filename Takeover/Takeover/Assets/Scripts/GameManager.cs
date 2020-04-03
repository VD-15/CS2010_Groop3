using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class GameManager : MonoBehaviour
{
	[SerializeField] private GameObject unitPrefab;
	[SerializeField] private GameObject factoryPrefab;

    // Start is called before the first frame update
    void Start()
    {
		GameObject.Instantiate(unitPrefab, new Vector3(-3.5f, 0.0f, -1.5f), Quaternion.identity);
		GameObject.Instantiate(unitPrefab, new Vector3(3.5f, 0.0f, 3.5f), Quaternion.identity).GetComponent<TeamController>().team = Team.Team2;
		GameObject.Instantiate(factoryPrefab, new Vector3(-5.5f, 0.5f, -1.5f), Quaternion.identity);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}

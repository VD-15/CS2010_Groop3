using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class QuitButtonController : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
		this.GetComponent<Button>().onClick.AddListener(() => SceneManager.LoadScene("MainMenu"));
    }
}

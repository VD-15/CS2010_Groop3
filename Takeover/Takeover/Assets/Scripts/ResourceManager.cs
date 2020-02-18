using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceManager : MonoBehaviour
{
  public static ResourceManager me;
  public int Steel = 0, Aluminium = 0, Rubber = 0, Oil = 0, Silicone = 0, Uranium = 0;

    void Awake()
    {
      me = this;
    }

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }

    publoc void IncreaseResources(int resource, int amount)
    {
      alterResourceVal (resource, amount * -1);
    }

    publoc void DecreaseResources(int resource, int amount)
    {
      alterResourceVal (resource, amount);
    }
}

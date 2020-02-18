using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResourceManager : MonoBehaviour
{
  public static ResourceManager me;
  public int steel = 0, aluminium = 0, rubber = 0, oil = 0, silicone = 0, uranium = 0;

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

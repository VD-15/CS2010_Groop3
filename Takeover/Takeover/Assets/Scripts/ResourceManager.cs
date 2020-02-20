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

    public void IncreaseResources(string resource, int amount) //
    {
      alterResourceVal (resource, amount * -1);
    }

    public void DecreaseResources(string resource, int amount)
    {
      alterResourceVal (resource, amount);
    }

    public bool resourceAmountCheck(string resource, int amount) //check we have enough of the required resource
    {
      switch (resource) {
        case "steel":
        if (steel > amount){
          return true;
        } else {
          return false;
        }
        break;
        case "aluminium":
        if (aluminium > amount){
          return true;
        } else {
          return false;
        }
        break;
        case "rubber":
        if (oil > amount){
          return true;
        } else {
          return false;
        }
        break;
        case "silicone":
        if (silicone > amount){
          return true;
        } else {
          return false;
        }
        break;
        case "uranium":
        if (uranium > amount){
          return true;
        } else {
          return false;
        }
        break;
      default:
        return false;
      }
    }

  void alterResourceVal(string resource, int amount)
  {
    switch(resource){
      case "steel":
        steel += amount;
        break;
      case "aluminium":
        aluminium += amount;
        break;
      case "rubber":
        rubber += amount;
        break;
      case "silicone":
        silicone += amount;
        break;
      case "uranium":
        uranium += amount;
        break;
      default:
      break;
    }
  }

}

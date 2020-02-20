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

    publoc void IncreaseResources(int resource, int amount) //
    {
      alterResourceVal (resource, amount * -1);
    }

    publoc void DecreaseResources(int resource, int amount)
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
        break;
      }
    }

}

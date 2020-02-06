using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveableComponent : MonoBehaviour
{
    [SerializeField] private float speed;
    private List<Vector3> path;

    // Start is called before the first frame update
    void Start()
    {
        this.path = new List<Vector3>();
    }

    // Update is called once per frame
    void Update()
    {
        if (this.path.Count > 0)
        {
            Vector3 next = this.path[0];

            Vector3 direction = (next - this.transform.position);

            this.transform.rotation = Quaternion.LookRotation(direction, Vector3.up);

            float moveAmount = this.speed * Time.deltaTime;

            if (direction.magnitude <= moveAmount)
            {
                this.transform.position = next;
                this.path.RemoveAt(0);
            }
            else
            {
                this.transform.Translate(direction.normalized * moveAmount, Space.World);
            }
        }
    }

    public void IssueMoveCommand(Vector3 newTarget)
    {
        Vector3 target = new Vector3(Mathf.Floor(newTarget.x) + 0.5f, newTarget.y, Mathf.Floor(newTarget.z) + 0.5f);

        if (target != this.path[this.path.Count - 1])
        {
            this.path.Add(target);
        }
    }
}

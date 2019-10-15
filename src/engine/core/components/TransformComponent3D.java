package engine.core.components;

import engine.core.Component;
import engine.core.Entity;
import engine.util.Vector3;

public class TransformComponent3D extends Component<TransformComponent3D>
{
	public Vector3 location;
	public Quaternion rotation;
	public Vector3 scale;
	
	public TransformComponent3D(Entity parent)
	{
		super(TransformComponent3D.class, parent);
	}
	
}

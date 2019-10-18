package engine.core.components;

import org.joml.Quaternionf;
import org.joml.Vector3f;

import engine.core.Component;
import engine.core.Entity;

public class TransformComponent3D extends Component<TransformComponent3D>
{
	public Vector3f location;
	public Quaternionf rotation;
	public Vector3f scale;
	
	public TransformComponent3D(Entity parent)
	{
		super(TransformComponent3D.class, parent);
		
		this.location = new Vector3f();
		this.rotation = new Quaternionf();
		this.scale = new Vector3f(1f);
	}
	
}

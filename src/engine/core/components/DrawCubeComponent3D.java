package engine.core.components;

import org.joml.Vector3f;
import org.joml.Vector4f;

import engine.core.Component;
import engine.core.Entity;

public class DrawCubeComponent3D extends Component<DrawCubeComponent3D>
{
	protected DrawCubeComponent3D(Entity parent, TransformComponent3D transform)
	{
		super(DrawCubeComponent3D.class, parent);
		this.transform = transform;
		this.color = new Vector4f(1f);
		this.radii = new Vector3f(1f);
	}

	TransformComponent3D transform;

	Vector4f color;
	
	/**
	 * Specifies the dimensions of the cube
	 */
	Vector3f radii;
}

package game.debug;

import org.joml.Vector3f;

import engine.core.Entity;
import engine.core.components.DrawCubeComponent3D;
import engine.core.components.TransformComponent3D;

public class TestEntity1 extends Entity
{
	private TransformComponent3D transform;
	private DrawCubeComponent3D draw;
	private TestComponent1 test;
	
	public TestEntity1(Vector3f location)
	{
		super();
		this.transform = new TransformComponent3D(this);
		this.draw = new DrawCubeComponent3D(this, transform);
		this.test = new TestComponent1(this, transform);
		
		this.transform.location.set(location);
		//this.transform.scale.set(2.0f);
		//this.draw.radii.mul(0.5f);
	}
	
	@Override
	public void Delete()
	{
		this.test.Delete();
		this.draw.Delete();
		this.transform.Delete();
	}
	
}

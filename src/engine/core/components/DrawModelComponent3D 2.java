package engine.core.components;

import engine.graphics.Model;

public class DrawModelComponent3D
{
	public final TransformComponent3D transform;
	public final Model model;
	
	public DrawModelComponent3D(TransformComponent3D transform, Model m)
	{
		this.transform = transform;
		this.model = m;
	}
}

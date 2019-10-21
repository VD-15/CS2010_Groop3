package engine.graphics;

import org.joml.Vector3f;

public class Material
{
	public final Vector3f colorAmbient;
	public final Vector3f colorDiffuse;
	public final Vector3f colorSpecular;
	public float transparency;
	public float specularExponent;
	
	public Material()
	{
		this.colorAmbient = new Vector3f();
		this.colorDiffuse = new Vector3f();
		this.colorSpecular = new Vector3f();
		this.transparency = 0f;
		this.specularExponent = 0f;
	}
}

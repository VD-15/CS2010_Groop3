package engine.util;

public class Vector3 implements Cloneable
{
	public float x;
	public float y;
	public float z;
	
	public Vector3()
	{
		this.x = 0f;
		this.y = 0f;
		this.z = 0f;
	}
	
	public Vector3(float x, float y, float z)
	{
		this.x = x;
		this.y = y;
		this.z = z;
	}
	
	public Vector3(Vector3 v)
	{
		this.x = v.x;
		this.y = v.y;
		this.z = v.z;
	}
	
	public void negate()
	{
		this.x = -this.x;
		this.y = -this.y;
		this.z = -this.z;
	}
	
	public void add(Vector3 v)
	{
		this.x += v.x;
		this.y += v.y;
		this.z += v.z;
	}
	
	public void subtract(Vector3 v)
	{
		this.x -= v.x;
		this.y -= v.y;
		this.z -= v.z;
	}
	
	public void scale(float s)
	{
		this.x *= s;
		this.y *= s;
		this.z *= s;
	}
	
	public void multiply(Vector3 v)
	{
		this.x *= v.x;
		this.y *= v.y;
		this.z *= v.z;
	}
	
	public float magnitude()
	{
		return (float)Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
	}
	
	public float magnitudeSquared()
	{
		return (this.x * this.x) + (this.y * this.y) + (this.z * this.z);
	}
	
	public void normalize()
	{
		float mag = this.magnitude();
		this.x /= mag;
		this.y /= mag;
		this.z /= mag;
	}
	
	public void setMagnitude(float newMag)
	{
		float mag = this.magnitude();
		float diff = newMag / mag;
		
		this.x *= diff;
		this.y *= diff;
		this.z *= diff;
	}
	
	public static Vector3 negate(Vector3 v)
	{
		return new Vector3(-v.x, -v.y, -v.z);
	}
	
	public static Vector3 add(Vector3 v1, Vector3 v2)
	{
		return new Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	
	/*
	public static Vector3 scale(Vector3 v, float s)
	{
		
	}
	
	public static float magnitude(Vector3 v)
	{
		
	}
	
	public static float magnitudeSquared(Vector3 v)
	{
		
	}
	
	public static Vector3 normalize(Vector3 v)
	{
		
	}
	
	public static Vector3 setMagnitude(Vector3 v, float f)
	{
		
	}
	*/
	
	@Override
	public boolean equals(Object o)
	{
		Vector3 c = (Vector3)o;
		if (c.x != this.x) return false;
		if (c.y != this.y) return false;
		if (c.z != this.z) return false;
		return true;
	}
	
	@Override
	public Vector3 clone()
	{
		return new Vector3(this);
	}
	
	@Override
	public String toString()
	{
		return this.getClass().getName() + ' ' + this.x + ' ' + this.y + ' ' + this.z;
	}
	
	public static Vector3 UNIT_Y() { return new Vector3(0f, 1f, 0f); }
	public static Vector3 UNIT_RY() { return new Vector3(0f, -1f, 0f); }
	public static Vector3 UNIT_RX() { return new Vector3(-1f, 0f, 0f); }
	public static Vector3 UNIT_X() { return new Vector3(1f, 0f, 0f); }
	public static Vector3 UNIT_Z() { return new Vector3(0f, 0f, 1f); }
	public static Vector3 UNIT_RZ() { return new Vector3(0f, 0f, -1f); }
	public static Vector3 ZERO() { return new Vector3(0f, 0f, 0f); }
}

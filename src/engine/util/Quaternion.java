package engine.util;

public class Quaternion implements Cloneable
{
	public float x;
	public float y;
	public float z;
	public float w;
	
	private Quaternion()
	{
		this.x = 0f;
		this.y = 0f;
		this.z = 0f;
		this.w = 1f;
	}
	
	public Quaternion(Quaternion q)
	{
		this.x = q.x;
		this.y = q.y;
		this.z = q.z;
		this.w = q.w;
	}
	
	public void multiply(Quaternion r)
	{
		float nW = r.w * this.w - r.x * this.x - r.y * this.y - r.z * this.z;
		float nX = r.w * this.x + r.x * this.w - r.y * this.z + r.z * this.y;
		float nY = r.w * this.y + r.x * this.z + r.y * this.w - r.z * this.x;
		float nZ = r.w * this.z - r.z * this.y + r.y * this.x + r.z * this.w;
		
		this.x = nX;
		this.y = nY;
		this.z = nZ;
		this.w = nW;
	}
	
	@Override
	public Quaternion clone()
	{
		return new Quaternion(this);			
	}
	
	@Override
	public boolean equals(Object o)
	{
		Quaternion q = (Quaternion)o;
		if (this.x != q.x) return false;
		if (this.y != q.y) return false;
		if (this.z != q.z) return false;
		if (this.w != q.w) return false;
		return true;
	}
	
	@Override
	public String toString()
	{
		return this.getClass().getName() + ' ' + this.x + ' ' + this.y + ' ' + this.z + ' ' + this.w;
	}
	
	public static Quaternion CreateIdentity()
	{
		return new Quaternion();
	}
	
	public static Quaternion CreateAxisAngle(Vector3 axis, float angle)
	{
		Quaternion q = new Quaternion();
		
		float sinA = (float)Math.sin(angle / 2f);
		
		q.x = axis.x * sinA;
		q.y = axis.y * sinA;
		q.z = axis.z * sinA;
		q.w = (float)Math.cos(angle / 2f);
		
		return q;
	}
	
	public static Quaternion multiply(Quaternion q1, Quaternion q2)
	{
		Quaternion q3 = q1.clone();
		q3.multiply(q2);
		return q3;
	}
}

package engine.util;

public class Matrix4 implements Cloneable
{
	private float[][] data;
	
	private Matrix4()
	{
		this.data = new float[4][4];
	}
	
	private Matrix4(Matrix4 m)
	{
		this.data = m.data.clone();
	}
	
	public void multiply(Matrix4 m)
	{
		Matrix4 n = this.clone();
		
		for (byte i = 0; i < 3; i++)
		{
			for (byte j = 0; j < 3; j++)
			{
				for (byte k = 0; k < 3; k++)
				{
					n.data[i][j] += this.data[k][j] * m.data[i][k];
				}
			}
		}
		
		this.data = n.data.clone();
	}
	
	@Override
	public Matrix4 clone()
	{
		return new Matrix4(this);
	}
	
	@Override
	public String toString()
	{
		return this.getClass().getName() + ' ' + this.hashCode();
	}
	
	public static Matrix4 createIdentity()
	{
		Matrix4 m = new Matrix4();
		
		for (byte x = 0; x < 4; x++)
		{
			for (byte y = 0; y < 4; y++)
			{
				if (x == y) m.data[x][y] = 1f;
				else m.data[x][y] = 0f;
			}
		}
		
		return m;
	}
	
	public static Matrix4 createTranslation(Vector3 v)
	{
		Matrix4 m = Matrix4.createIdentity();
		
		m.data[3][0] = v.x;
		m.data[3][1] = v.y;
		m.data[3][2] = v.z;
		
		return m;
	}
	
	public static Matrix4 createScale(Vector3 v)
	{
		Matrix4 m = Matrix4.createIdentity();

		m.data[0][0] = v.x;
		m.data[1][1] = v.y;
		m.data[2][2] = v.z;
		
		return m;
	}
	
	public static Matrix4 createRotationX(float angle)
	{
		Matrix4 m = Matrix4.createIdentity();

		float sinA = (float)Math.sin(angle);
		float cosA = (float)Math.cos(angle);
		
		m.data[1][1] = cosA;
		m.data[2][1] = sinA;
		m.data[1][2] = -sinA;
		m.data[2][2] = cosA;
		
		return m;
	}
	
	public static Matrix4 createRotationY(float angle)
	{
		Matrix4 m = Matrix4.createIdentity();

		float sinA = (float)Math.sin(angle);
		float cosA = (float)Math.cos(angle);

		m.data[0][0] = cosA;
		m.data[2][0] = -sinA;
		m.data[0][2] = sinA;
		m.data[2][2] = cosA;
		
		return m;
	}
	
	public static Matrix4 createRotationZ(float angle)
	{
		Matrix4 m = Matrix4.createIdentity();

		float sinA = (float)Math.sin(angle);
		float cosA = (float)Math.cos(angle);

		m.data[0][0] = cosA;
		m.data[1][0] = -sinA;
		m.data[0][1] = sinA;
		m.data[1][1] = cosA;
		
		return m;
	}
	
	public static Matrix4 createRotation(Quaternion q)
	{
		Matrix4 m = Matrix4.createIdentity();
		
		m.data[0][0] = 1f - 2f * q.y * q.y - 2f * q.z * q.z;
		m.data[1][0] =      2f * q.x * q.y - 2f * q.z * q.w;
		m.data[2][0] =      2f * q.x * q.z + 2f * q.y * q.w;
		
		m.data[0][1] =      2f * q.x * q.y + 2f * q.z * q.w;
		m.data[1][1] = 1f - 2f * q.x * q.x - 2f * q.z * q.z;
		m.data[2][1] =      2f * q.y * q.z - 2f * q.x * q.w;
		
		m.data[0][2] =      2f * q.x * q.z - 2f * q.y * q.w;
		m.data[1][2] =      2f * q.y * q.z + 2f * q.x * q.w;
		m.data[2][2] = 1f - 2f * q.x * q.x - 2f * q.y * q.y;
		
		return m;
	}
	
	public static Matrix4 multiply(Matrix4 m, Matrix4 n)
	{
		Matrix4 o = m.clone();
		o.multiply(n);
		return o;
	}
}

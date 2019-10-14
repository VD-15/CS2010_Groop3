package engine.util.vector;

public class Pair<T extends Number> implements Cloneable
{
	public T x;
	public T y;
	
	public Pair(T x, T y)
	{
		this.x = x;
		this.y = y;
	}
	
	@Override
	public boolean equals(Object o)
	{
		Pair<?> p = (Pair<?>)o;
		if (!p.x.equals(this.x)) return false;
		if (!p.y.equals(this.y)) return false;
		return true;
	}
	
	@Override
	public Pair<T> clone()
	{
		return new Pair<T>(this.x, this.y);
	}
	
	@Override 
	public String toString()
	{
		return this.getClass().getName() + ' ' + x.toString() + ' ' + y.toString();
	}
}

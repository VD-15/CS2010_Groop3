package engine.util;

public class Accumulator
{
	private long value;
	
	public Accumulator()
	{
		this.value = 0L;
	}
	
	public void add(long v)
	{
		this.value += v;
	}
	
	public long getValue()
	{
		return value;
	}
}

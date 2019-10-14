package game.debug;

import engine.core.Entity;

public class TestEntity1 extends Entity
{
	private TestComponent1 test;
	
	public TestEntity1()
	{
		super();
		this.test = new TestComponent1(this);
	}
	
	@Override
	public void Delete()
	{
		this.test.Delete();
	}
	
}

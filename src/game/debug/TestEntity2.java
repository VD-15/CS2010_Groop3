package game.debug;

import engine.core.Entity;

public class TestEntity2 extends Entity
{
	private TestComponent2 test;
	
	public TestEntity2()
	{
		test = new TestComponent2(this);
	}
	
	@Override
	public void Delete()
	{
		test.Delete();
	}
	
}

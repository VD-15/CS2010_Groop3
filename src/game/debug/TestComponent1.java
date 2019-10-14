package game.debug;

import engine.core.Component;
import engine.core.Entity;
import engine.core.events.EventBus;
import engine.core.events.UpdateEvent;
import engine.core.memory.Allocator;

public class TestComponent1 extends Component<TestComponent1>
{
	public static void init()
	{
		EventBus.get(UpdateEvent.class).subscribeEvent(TestComponent1::onUpdate);
	}
	
	public static void destroy()
	{
		EventBus.get(UpdateEvent.class).unsubscribeEvent(TestComponent1::onUpdate);
	}
	
	static void opEarlyUpdate(UpdateEvent ev)
	{
		System.out.println("EarlyUpdate called for TestComponent1!");

		Allocator.get(TestComponent1.class).forEach((c) ->
		{
			System.out.println(c.toString());
		});
	}
	
	static void onUpdate(UpdateEvent ev)
	{
		System.out.println("Update called for TestComponent1!");
		
		Allocator.get(TestComponent1.class).forEach((c) ->
		{
			System.out.println(c.toString());
		});
	}
	
	public TestComponent1(Entity parent)
	{
		super(TestComponent1.class, parent);
	}
	
	public void Delete()
	{
		this.Delete(TestComponent1.class);
	}
}

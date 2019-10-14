package game.debug;

import engine.core.Component;
import engine.core.EarlyUpdateEvent;
import engine.core.Entity;
import engine.core.events.EventBus;
import engine.core.memory.Allocator;

public class TestComponent2 extends Component<TestComponent2>
{
	public static void init()
	{
		EventBus.get(EarlyUpdateEvent.class).subscribeEvent(TestComponent2::onEarlyUpdate);
	}
	
	public static void destroy()
	{
		EventBus.get(EarlyUpdateEvent.class).unsubscribeEvent(TestComponent2::onEarlyUpdate);
	}
	
	public static void onEarlyUpdate(EarlyUpdateEvent ev)
	{
		System.out.println("EarlyUpdate called for TestComponent2!");

		Allocator.get(TestComponent2.class).forEach((c) ->
		{
			System.out.println(c.toString());
		});
	}
	
	public TestComponent2(Entity parent)
	{
		super(TestComponent2.class, parent);
	}
	
}

package game.debug;

import engine.core.Component;
import engine.core.EarlyUpdateEvent;
import engine.core.Entity;
import engine.core.events.EventBus;
import engine.core.events.LateUpdateEvent;
import engine.core.events.UpdateEvent;
import engine.core.memory.Allocator;

public class TestComponent1 extends Component<TestComponent1>
{
	public static void init()
	{
		EventBus.get(EarlyUpdateEvent.class).subscribeEvent(TestComponent1::onEarlyUpdate);
		EventBus.get(UpdateEvent.class).subscribeEvent(TestComponent1::onUpdate);
		EventBus.get(LateUpdateEvent.class).subscribeEvent(TestComponent1::onLateUpdate);
	}
	
	public static void destroy()
	{
		EventBus.get(EarlyUpdateEvent.class).unsubscribeEvent(TestComponent1::onEarlyUpdate);
		EventBus.get(UpdateEvent.class).unsubscribeEvent(TestComponent1::onUpdate);
		EventBus.get(LateUpdateEvent.class).unsubscribeEvent(TestComponent1::onLateUpdate);
	}
	
	static void onEarlyUpdate(EarlyUpdateEvent ev)
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
	
	static void onLateUpdate(LateUpdateEvent ev)
	{
		System.out.println("LateUpdate called for TestComponent1!");
		
		Allocator.get(TestComponent1.class).forEach((c) ->
		{
			System.out.println(c.toString());
		});
	}
	
	public TestComponent1(Entity parent)
	{
		super(TestComponent1.class, parent);
	}
}

package game.debug;

import engine.core.Component;
import engine.core.Entity;
import engine.core.EventBus;
import engine.core.components.TransformComponent3D;
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
	
	static void onUpdate(UpdateEvent ev)
	{
		//System.out.println("Update called for TestComponent1!");
		
		final float deltaTime = 0.015f;
		
		Allocator.get(TestComponent1.class).forEach((c) ->
		{
			c.transform.rotation.rotateX(1f * deltaTime);
			c.transform.rotation.rotateY(2f * deltaTime);
			//c.transform.rotation.rotateZ(2f * deltaTime);
		});
	}
	
	public final TransformComponent3D transform;
	
	public TestComponent1(Entity parent, TransformComponent3D transform)
	{
		super(TestComponent1.class, parent);
		this.transform = transform;
	}
}

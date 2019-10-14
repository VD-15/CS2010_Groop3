package game;

import engine.Engine;
import engine.core.EventBus;
import engine.core.events.EarlyUpdateEvent;
import engine.core.events.LateUpdateEvent;
import engine.core.events.UpdateEvent;
import game.debug.TestComponent1;
import game.debug.TestComponent2;
import game.debug.TestEntity1;
import game.debug.TestEntity2;

public class Program
{
	/**
	 * Entry point for the program
	 * @param args Arguments for the program
	 */
	public static void main(String[] args)
	{
		Engine.init();
		
		TestComponent1.init();
		TestComponent2.init();
		
		new TestEntity1();
		new TestEntity1();
		new TestEntity2();

		EventBus.get(EarlyUpdateEvent.class).postEvent(new EarlyUpdateEvent());
		
		TestEntity1 e = new TestEntity1();
		
		EventBus.get(UpdateEvent.class).postEvent(new UpdateEvent());
		
		e.Delete();
		
		EventBus.get(LateUpdateEvent.class).postEvent(new LateUpdateEvent());

		TestComponent1.destroy();
		TestComponent2.destroy();
		
		Engine.destroy();
	}
}

package game;

import engine.Engine;
import engine.core.events.EventBus;
import engine.core.events.UpdateEvent;
import game.debug.TestComponent1;
import game.debug.TestEntity1;

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
		
		new TestEntity1();
		
		EventBus.get(UpdateEvent.class).postEvent(new UpdateEvent());

		TestComponent1.destroy();
		
		Engine.destroy();
	}
}

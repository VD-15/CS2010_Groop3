package game;

import org.joml.Vector3f;

import engine.Engine;
import engine.core.Application;
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
		
		TestEntity1 t1 = new TestEntity1(new Vector3f(-5f, 0f, 0f));
		TestEntity1 t2 = new TestEntity1(new Vector3f(0f, 0f, 0f));
		TestEntity1 t3 = new TestEntity1(new Vector3f(5f, 0f, 0f));
		
		Application.start();

		t1.Delete();
		t2.Delete();
		t3.Delete();
		
		TestComponent1.destroy();
		
		Engine.destroy();
	}
}

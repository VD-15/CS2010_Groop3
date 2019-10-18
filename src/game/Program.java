package game;

import engine.Engine;
import engine.core.Application;

public class Program
{
	/**
	 * Entry point for the program
	 * @param args Arguments for the program
	 */
	public static void main(String[] args)
	{
		Engine.init();
		
		Application.start();
		
		Engine.destroy();
	}
}

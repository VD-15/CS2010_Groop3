package game;

import engine.Engine;

public class Program
{
	/**
	 * Entry point for the program
	 * @param args Arguments for the program
	 */
	public static void main(String[] args)
	{
		Engine.init();
		
		Engine.destroy();
	}
}

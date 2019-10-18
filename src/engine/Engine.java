package engine;

import engine.io.Window;

public abstract class Engine
{
	public static void init()
	{
		Window.init();
	}
	
	public static void destroy()
	{
		Window.destroy();
	}
}

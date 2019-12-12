package engine;

import engine.graphics.GLRenderer;
import engine.io.Window;

public abstract class Engine
{
	public static void init()
	{
		Window.init();
		GLRenderer.init();
	}
	
	public static void destroy()
	{
		GLRenderer.destroy();
		Window.destroy();
	}
}

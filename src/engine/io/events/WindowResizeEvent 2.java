package engine.io.events;

import engine.core.Event;

public class WindowResizeEvent extends Event<WindowResizeEvent>
{
	public final int width;
	public final int height;
	
	public WindowResizeEvent(int width, int height)
	{		
		super();
		
		this.width = width;
		this.height = height;
	}
	
}

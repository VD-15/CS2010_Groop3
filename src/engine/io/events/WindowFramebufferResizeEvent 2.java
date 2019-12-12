package engine.io.events;

import engine.core.Event;

public class WindowFramebufferResizeEvent extends Event<WindowFramebufferResizeEvent>
{
	public final int width;
	public final int height;
	
	public WindowFramebufferResizeEvent(int width, int height)
	{
		super();
		
		this.width = width;
		this.height = height;
	}
	
}

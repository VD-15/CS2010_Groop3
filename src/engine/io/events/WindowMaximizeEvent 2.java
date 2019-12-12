package engine.io.events;

import engine.core.Event;

public class WindowMaximizeEvent extends Event<WindowMaximizeEvent>
{
	public final boolean maximized;
	
	public WindowMaximizeEvent(boolean maximized)
	{
		super();
		
		this.maximized = maximized;
	}
	
}

package engine.io.events;

import engine.core.Event;

public class WindowIconifyEvent extends Event<WindowIconifyEvent>
{
	/**
	 * true if the window has been iconified, false if otherwise
	 */
	public final boolean iconified;
	
	public WindowIconifyEvent(boolean iconified)
	{
		super();
		
		this.iconified = iconified;
	}
	
}

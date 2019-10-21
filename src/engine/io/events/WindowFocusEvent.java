package engine.io.events;

import engine.core.Event;

public class WindowFocusEvent extends Event<WindowFocusEvent>
{
	/**
	 * 1 if the window has gained focus, 0 if it has lost it.
	 */
	public final int focus;
	
	public WindowFocusEvent(int focus)
	{
		super();
		
		this.focus = focus;
	}
	
}

package engine.io.events;

import engine.core.Event;

public class WindowCursorEnterEvent extends Event<WindowCursorEnterEvent>
{
	/**
	 * 1 if the cursor has entered the window, 0 otherwise
	 */
	public final boolean entered;
	
	public WindowCursorEnterEvent(boolean entered)
	{
		super();
		
		this.entered = entered;
	}
}

package engine.io.events;

import engine.core.Event;

public class WindowCharEvent extends Event<WindowCharEvent>
{
	/**
	 * The UTF-32 codepoint for the character that was typed
	 */
	public final int codePoint;
	
	public WindowCharEvent(int codePoint)
	{
		super();
		
		this.codePoint = codePoint;
	}
	
}

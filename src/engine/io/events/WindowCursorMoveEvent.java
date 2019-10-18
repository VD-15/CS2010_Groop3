package engine.io.events;

import engine.core.Event;

public class WindowCursorMoveEvent extends Event<WindowCursorMoveEvent>
{
	public final double xPos;
	public final double yPos;
	
	public WindowCursorMoveEvent(double xPos, double yPos)
	{
		super();
		
		this.xPos = xPos;
		this.yPos = yPos;
	}
	
}

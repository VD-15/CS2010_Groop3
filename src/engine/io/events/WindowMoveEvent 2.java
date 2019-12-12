package engine.io.events;

import engine.core.Event;

public class WindowMoveEvent extends Event<WindowMoveEvent>
{
	public final int xPos;
	public final int yPos;
	
	
	public WindowMoveEvent(int xPos, int yPos)
	{
		super();
		
		this.xPos = xPos;
		this.yPos = yPos;
	}
	
}

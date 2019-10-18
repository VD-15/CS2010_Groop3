package engine.io.events;

import engine.core.Event;

public class WindowScrollEvent extends Event<WindowScrollEvent>
{
	public final double xVal;
	public final double yVal;
	
	public WindowScrollEvent(double xVal, double yVal)
	{
		super();
		
		this.xVal = xVal;
		this.yVal = yVal;
	}
	
}

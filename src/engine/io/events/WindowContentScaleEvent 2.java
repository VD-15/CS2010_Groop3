package engine.io.events;

import engine.core.Event;

public class WindowContentScaleEvent extends Event<WindowContentScaleEvent>
{
	public final float xScale;
	public final float yScale;	
	
	public WindowContentScaleEvent(float xScale, float yScale)
	{
		super();
		
		this.xScale = xScale;
		this.yScale = yScale;
	}
	
}

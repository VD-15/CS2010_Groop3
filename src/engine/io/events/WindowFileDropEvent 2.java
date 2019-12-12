package engine.io.events;

import engine.core.Event;

public class WindowFileDropEvent extends Event<WindowFileDropEvent>
{
	public final int count;
	public final String[] paths;
	
	public WindowFileDropEvent(int count, String[] paths)
	{
		super();
		
		this.count = count;
		this.paths = paths.clone();
	}
	
}

package engine.io.events;

import engine.core.Event;

public class WindowMouseButtonEvent extends Event<WindowMouseButtonEvent>
{
	/**
	 * The mouse button that was pressed
	 */
	public final int button;
	
	/**
	 * 0 if released, 1 if pressed, 2 if repeated
	 */
	public final int action;
	
	/**
	 * bit field specifying what modifiers were held down
	 */
	public final int mods;
	
	public WindowMouseButtonEvent(int button, int action, int mods)
	{
		super();
		
		this.button = button;
		this.action = action;
		this.mods = mods;
	}
	
}

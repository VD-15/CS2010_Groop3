package engine.io.events;

import engine.core.Event;

public class WindowKeyEvent extends Event<WindowKeyEvent>
{
	/**
	 * The key that was pressed
	 */
	public final int key;
	
	/**
	 * The system-specific scan code of the key
	 */
	public final int scanCode;
	
	/**
	 * 0 if released, 1 if pressed, 2 if repeated
	 */
	public final int action;
	
	/**
	 * bit field describing what modifier keys are down
	 */
	public final int mods;
	
	public WindowKeyEvent(int key, int scanCode, int action, int mods)
	{
		super();
		
		this.key = key;
		this.scanCode = scanCode;
		this.action = action;
		this.mods = mods;
	}
}

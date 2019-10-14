package engine.core.events;

public abstract class Event<T>
{
	private boolean isCancelled;
	
	/**
	 * Constructs a new event
	 * @param data The data held by this event
	 */
	public Event()
	{
		this.isCancelled = false;
	}
	
	/**
	 * Cancels this event, stopping any further processing of it.
	 * @see {@link #isCancelled()}
	 */
	public void cancel()
	{
		this.isCancelled = true;
	}
	
	/**
	 * Checks if this event is cancelled.
	 * @return true if cancelled, false otherwise.
	 * @see {@link #cancel()}
	 */
	public boolean isCancelled()
	{
		return this.isCancelled;
	}
}

package engine.core.events;

import java.util.ArrayList;

public final class EventBus<T extends Event<?>>
{
	/**
	 * Event listeners subscribed to this bus
	 */
	private ArrayList<EventListener<T>> listeners;
	private final Class<T> eventType;
	
	private static ArrayList<EventBus<?>> BUSSES = new ArrayList<EventBus<?>>();
	
	@SuppressWarnings("unchecked")
	static <T extends Event<?>> EventBus<T> get(Class<T> c)
	{
		for (EventBus<?> eb : BUSSES)
		{
			if (eb.eventType.equals(c))
			{
				return (EventBus<T>)eb;
			}
		}
		
		EventBus<T> eb = new EventBus<T>(c);
		BUSSES.add(eb);
		return eb;
	}
	
	/**
	 * Constructs a new event bus.
	 */
	private EventBus(Class<T> c)
	{
		this.listeners = new ArrayList<EventListener<T>>();
		this.eventType = c;
		BUSSES.add(this);
	}
	
	/**
	 * Adds an event listener to this bus.
	 * @param el The listener to add.
	 */
	public void addEventListener(EventListener<T> el)
	{
		listeners.add(el);
	}
	
	/**
	 * Removes an event listener to this bus.
	 * @param el The listener to remove.
	 */
	public void removeEventListener(EventListener<T> el)
	{
		listeners.remove(el);
	}
	
	/**
	 * Posts an event to all listeners on this bus.
	 * @param ev The event to be posted.
	 */
	public void postEvent(T ev)
	{
		for (EventListener<T> el : listeners)
		{
			if (ev.isCancelled()) break;
			else el.OnEvent(ev);
		}
	}
}

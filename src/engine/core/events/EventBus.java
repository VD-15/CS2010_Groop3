package engine.core.events;

import java.util.ArrayList;
import java.util.function.Consumer;

import engine.core.Event;

public final class EventBus<T extends Event<?>>
{
	/**
	 * Event listeners subscribed to this bus
	 */
	private ArrayList<Consumer<T>> consumers;
	private final Class<T> eventType;
	
	private static ArrayList<EventBus<?>> BUSSES = new ArrayList<EventBus<?>>();
	
	@SuppressWarnings("unchecked")
	public static <U extends Event<?>> EventBus<U> get(Class<U> c)
	{
		for (EventBus<?> eb : BUSSES)
		{
			if (eb.eventType.equals(c))
			{
				return (EventBus<U>)eb;
			}
		}
		
		EventBus<U> eb = new EventBus<U>(c);
		BUSSES.add(eb);
		return eb;
	}
	
	/**
	 * Constructs a new event bus.
	 */
	private EventBus(Class<T> c)
	{
		this.consumers = new ArrayList<Consumer<T>>();
		this.eventType = c;
		BUSSES.add(this);
	}
	
	public void subscribeEvent(Consumer<T> consumer)
	{
		this.consumers.add(consumer);
	}

	public void unsubscribeEvent(Consumer<T> consumer)
	{
		this.consumers.remove(consumer);
	}
	
	public void postEvent(T ev)
	{
		for (Consumer<T> c : this.consumers)
		{
			if (ev.isCancelled()) break;
			else c.accept(ev);
		}
	}
}

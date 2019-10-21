package engine.core;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.function.Consumer;

public final class EventBus<T extends Event<?>>
{
	/**
	 * Event listeners subscribed to this bus
	 */
	private ArrayList<Consumer<T>> consumers;
	
	private static HashMap<Class<?>, EventBus<?>> BUSSES = new HashMap<Class<?>, EventBus<?>>();
	
	@SuppressWarnings("unchecked")
	public static <U extends Event<?>> EventBus<U> get(Class<U> c)
	{
		EventBus<?> eb = BUSSES.get(c);
		
		if (eb != null)
		{
			return (EventBus<U>)eb;
		}
		else
		{
			EventBus<U> ebu = new EventBus<U>(c);
			BUSSES.put(c, ebu);
			return ebu;
		}
	}
	
	/**
	 * Constructs a new event bus.
	 */
	private EventBus(Class<T> c)
	{
		this.consumers = new ArrayList<Consumer<T>>();
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

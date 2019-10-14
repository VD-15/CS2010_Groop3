package engine.core.events;

public interface EventListener<T extends Event<?>>
{
	void OnEvent(T ev);
}

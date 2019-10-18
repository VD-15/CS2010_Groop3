package engine.core;

import engine.core.events.*;

public class Application
{
	private static boolean isRunning;
	
	public static void start()
	{
		EventBus.get(ApplicationStartEvent.class).postEvent(new ApplicationStartEvent());
		
		isRunning = true;
		
		while (isRunning)
		{
			EventBus.get(PreUpdateEvent.class).postEvent(new PreUpdateEvent());
			EventBus.get(EarlyUpdateEvent.class).postEvent(new EarlyUpdateEvent());
			EventBus.get(UpdateEvent.class).postEvent(new UpdateEvent());
			EventBus.get(LateUpdateEvent.class).postEvent(new LateUpdateEvent());
			EventBus.get(PostUpdateEvent.class).postEvent(new PostUpdateEvent());
		}
		
		EventBus.get(ApplicationExitEvent.class).postEvent(new ApplicationExitEvent());
	}
	
	public static void stop()
	{
		isRunning = false;
	}
}

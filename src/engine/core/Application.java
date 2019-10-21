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
			//System.out.println("PreUpdateEvent");
			EventBus.get(PreUpdateEvent.class).postEvent(new PreUpdateEvent());
			
			//System.out.println("EarlyUpdateEvent");
			EventBus.get(EarlyUpdateEvent.class).postEvent(new EarlyUpdateEvent());
			
			//System.out.println("UpdateEvent");
			EventBus.get(UpdateEvent.class).postEvent(new UpdateEvent());
			
			//System.out.println("LateUpdateEvent");
			EventBus.get(LateUpdateEvent.class).postEvent(new LateUpdateEvent());
			
			//System.out.println("PostUpdateEvent");
			EventBus.get(PostUpdateEvent.class).postEvent(new PostUpdateEvent());
		}
		
		EventBus.get(ApplicationExitEvent.class).postEvent(new ApplicationExitEvent());
	}
	
	public static void stop()
	{
		isRunning = false;
	}
}

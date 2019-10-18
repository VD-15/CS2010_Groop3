package engine.graphics;

import engine.core.EventBus;
import engine.core.events.PostUpdateEvent;

public class GLRenderer
{
	private static int vertexBuffer;
	private static int elementBuffer;
	
	static void init()
	{
		EventBus.get(PostUpdateEvent.class).subscribeEvent(GLRenderer::onPostUpdate);
	}
	
	static void destroy()
	{
		EventBus.get(PostUpdateEvent.class).unsubscribeEvent(GLRenderer::onPostUpdate);
	}
	
	static void onPostUpdate(PostUpdateEvent ev)
	{
		
	}
}

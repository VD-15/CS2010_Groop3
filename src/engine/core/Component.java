package engine.core;

import engine.core.events.EntityRemovedEvent;
import engine.core.events.EventListener;

public abstract class Component implements EventListener<EntityRemovedEvent>
{
	private Entity parent;
	
	/**
	 * Creates a component
	 * @param e The parent of this component
	 */
	public Component(Entity parent)
	{
		this.parent = parent;
	}
	
	/**
	 * Gets the parent of this component
	 * @return The parent entity of this component
	 */
	public Entity getParent()
	{
		return parent;
	}
	
	public void Delete()
	{
		
	}
	
	@Override
	public void OnEvent(EntityRemovedEvent ev)
	{
		if (this.parent.equals(ev.entity))
		{
			this.Delete();
		}
	}
}

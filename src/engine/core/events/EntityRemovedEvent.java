package engine.core.events;

import engine.core.Entity;

public class EntityRemovedEvent extends Event<EntityRemovedEvent>
{
	public final Entity entity;
	
	public EntityRemovedEvent(Entity entity)
	{
		super();
		this.entity = entity;
	}
}

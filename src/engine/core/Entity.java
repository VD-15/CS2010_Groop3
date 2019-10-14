package engine.core;

public abstract class Entity
{
	/**
	 * ID of this entity
	 */
	private final long eID;
	
	private static long MAX_ENTITY_ID = Long.MIN_VALUE;
	
	/**
	 * Constructs a new entity
	 */
	public Entity()
	{
		this.eID = MAX_ENTITY_ID++;
	}
	
	@Override
	public boolean equals(Object o)
	{
		Entity e = (Entity)o;
		return (this.eID == e.eID);
	}

	public abstract void Delete();
}

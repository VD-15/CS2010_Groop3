package engine.core;

import engine.core.memory.Allocator;

public abstract class Component<T>
{
	private final Entity parent;
	private final Class<T> c;
	
	/**
	 * Creates a component
	 * @param e The parent of this component
	 */
	@SuppressWarnings("unchecked")
	protected Component(Class<T> c, Entity parent)
	{
		Allocator.get(c).addInstance((T)this);
		this.c = c;
		this.parent = parent;
	}
	
	/**
	 * Gets the parent of this component
	 * @return The parent entity of this component
	 */
	public final Entity getParent()
	{
		return parent;
	}
	
	/**
	 * Deletes this component from the ECS.
	 */
	@SuppressWarnings("unchecked")
	public final void Delete()
	{
		Allocator.get(this.c).removeInstance((T)this);
	}
}

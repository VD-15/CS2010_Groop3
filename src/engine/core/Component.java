package engine.core;

import engine.core.memory.Allocator;

public abstract class Component<T>
{
	private Entity parent;
	
	/**
	 * Creates a component
	 * @param e The parent of this component
	 */
	@SuppressWarnings("unchecked")
	protected Component(Class<T> c, Entity parent)
	{
		Allocator.get(c).addInstance((T)this);
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
	
	@SuppressWarnings("unchecked")
	protected final void Delete(Class<T> c)
	{
		Allocator.get(c).removeInstance((T)this);
	}
	
	public abstract void Delete();
}

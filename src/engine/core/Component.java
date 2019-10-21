package engine.core;

import engine.core.memory.Allocator;

public abstract class Component<T extends Component<T>>
{
	private final Entity parent;
	private final Class<T> c;
	/**
	 * Creates a component
	 * @param c The class of this component
	 * @param parent The parent of this component
	 */
	@SuppressWarnings("unchecked")
	protected Component(Class<T> c, Entity parent)
	{
		this.c = c;
		this.parent = parent;
		Allocator.get(c).addInstance((T)this);
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
	public void Delete()
	{
		Allocator.get(this.c).removeInstance((T)this);
	}
}

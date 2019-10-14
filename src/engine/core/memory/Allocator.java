package engine.core.memory;

import java.util.ArrayList;
import java.util.function.Consumer;

public class Allocator<T>
{
	private static ArrayList<Allocator<?>> allocators = new ArrayList<Allocator<?>>();
	
	private ArrayList<T> instances;
	private Class<T> type;
	
	@SuppressWarnings("unchecked")
	public static <U> Allocator<U> get(Class<U> c)
	{
		for (Allocator<?> a : allocators)
		{
			if (a.type.equals(c))
			{
				return (Allocator<U>)a;
			}
		}
		
		Allocator<U> a = new Allocator<U>(c);
		allocators.add(a);
		return a;
	}
	
	private Allocator(Class<T> c)
	{
		this.instances = new ArrayList<T>();
		this.type = c;
	}
	
	public void forEach(Consumer<T> c)
	{
		this.instances.forEach(c);
	}
	
	public void addInstance(T t)
	{
		this.instances.add(t);
	}
	
	public void removeInstance(T t)
	{
		this.instances.remove(t);
	}
}

package engine.core.memory;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.function.Consumer;

public class Allocator<T>
{
	private static HashMap<Class<?>, Allocator<?>> allocators = new HashMap<Class<?>, Allocator<?>>();
	
	private ArrayList<T> instances;
	private Class<T> type;
	
	@SuppressWarnings("unchecked")
	public static <U> Allocator<U> get(Class<U> c)
	{
		Allocator<U> a = (Allocator<U>) allocators.get(c);
		
		if (a == null)
		{
			a = new Allocator<U>(c);
			allocators.put(c, a);
		}
		
		return a;
	}
	
	private Allocator(Class<T> c)
	{
		this.instances = new ArrayList<T>();
		this.type = c;
	}
	
	public int getCount()
	{
		return instances.size();
	}
	
	public T[] getAll()
	{
		@SuppressWarnings("unchecked")
		T[] a = (T[]) Array.newInstance(type, instances.size());
		instances.toArray(a);
		return a;
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

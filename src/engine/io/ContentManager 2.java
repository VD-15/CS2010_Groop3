package engine.io;

import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;

import engine.graphics.Model;

public class ContentManager<T extends Content<T>>
{
	private HashMap<String, T> content;
	private Class<T> type;
	
	private static HashMap<Class<?>, ContentManager<?>> managers = new HashMap<Class<?>, ContentManager<?>>();
	
	private ContentManager(Class<T> c)
	{
		this.content = new HashMap<String, T>();
		this.type = c;
	}
	
	@SuppressWarnings("unchecked")
	public static <U extends Content<U>> ContentManager<U> get(Class<U> c)
	{
		ContentManager<U> m = (ContentManager<U>) managers.get(c);
		
		if (m == null)
		{
			m = new ContentManager<U>(c);
			managers.put(c, m);
		}
		
		return m;
	}
	
	public final T loadContent(String path, String name)
	{
		try
		{
			T t = type.getConstructor(String.class).newInstance(name);
			content.put(name, t);
			return t;
		}
		catch (NoSuchMethodException ex)
		{
			System.err.println("Content did not have constructor taking a string");
			System.err.println(ex.getMessage());
			ex.printStackTrace();
		} 
		catch (InstantiationException e)
		{
			System.err.println(e.getMessage());
			e.printStackTrace();
		} 
		catch (IllegalAccessException e)
		{
			System.err.println(e.getMessage());
			e.printStackTrace();
		} 
		catch (IllegalArgumentException e)
		{
			System.err.println(e.getMessage());
			e.printStackTrace();
		} 
		catch (InvocationTargetException e)
		{
			System.err.println(e.getMessage());
			e.printStackTrace();
		} 
		catch (SecurityException e)
		{
			System.err.println(e.getMessage());
			e.printStackTrace();
		}
		
		return null;
	}
	
	public final T getContent(String name)
	{
		return content.get(name);
	}
	
	public final void deleteContent(String name)
	{
		content.remove(name);
	}
}

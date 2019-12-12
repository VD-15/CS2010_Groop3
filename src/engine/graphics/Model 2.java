package engine.graphics;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

import de.javagl.obj.*;
import engine.io.Content;

public class Model extends Content<Model>
{
	public final Obj modelData;
	public final Map<String, Mtl> materials;
	
	public Model(String path)
	{
		super(path);
		
		Obj obj = null;
		
		try (InputStream in = new FileInputStream(path))
		{
			obj = ObjUtils.convertToRenderable(ObjReader.read(in));
			
		}
		catch (IOException ex)
		{
			System.err.println(ex.getMessage());
			ex.printStackTrace();
		}
		
		this.modelData = obj;
		
		this.materials = new HashMap<String, Mtl>();
		
		for (String mtlFileName : this.modelData.getMtlFileNames())
		{
			try (InputStream in = new FileInputStream(mtlFileName))
			{
				for (Mtl m : MtlReader.read(in))
				{
					materials.put(m.getName(), m);
					m.getMapKd();
					//TODO: make methods for specular maps
					//TODO: make materials seperate
				}
			}
			catch (IOException ex)
			{
				System.err.println(ex.getMessage());
				ex.printStackTrace();
			}			
		}
	}
}

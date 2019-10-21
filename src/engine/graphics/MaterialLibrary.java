package engine.graphics;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;

public class MaterialLibrary
{
	private HashMap<String, Material> materials;
	
	public MaterialLibrary(String path)
	{
		this.materials = new HashMap<String, Material>();

		try (InputStream in = new FileInputStream(path); BufferedReader reader = new BufferedReader(new InputStreamReader(in)))
		{
			String line;
			Material currentMat = null;
			
			while ((line = reader.readLine()) != null)
			{
				String[] s = line.split("\\s");
				
				switch (s[0])
				{
				case "newmtl":	//New material
					currentMat = new Material();
					materials.put(s[1], currentMat);
					break;
				case "Ns":		//Specular exponent
					currentMat.specularExponent = Float.parseFloat(s[1]);
					break;
				case "Ka":		//Ambient color
					currentMat.colorAmbient.set(Float.parseFloat(s[1]), Float.parseFloat(s[2]), Float.parseFloat(s[3]));
					break;
				case "Kd":		//Diffuse color
					currentMat.colorDiffuse.set(Float.parseFloat(s[1]), Float.parseFloat(s[2]), Float.parseFloat(s[3]));
					break;
				case "Ks":		//Specular color
					currentMat.colorSpecular.set(Float.parseFloat(s[1]), Float.parseFloat(s[2]), Float.parseFloat(s[3]));
					break;
				case "d":		//Transparency
				case "Tr":		//Also transparency
					currentMat.transparency = Float.parseFloat(s[1]);	
				default:
					break;
				}
			}
		}
		catch (IOException ex)
		{
			throw new RuntimeException("Failed to load shader file: " + path);
		}
	}
	
	public Material getMaterial(String name)
	{
		return materials.get(name);
	}
}

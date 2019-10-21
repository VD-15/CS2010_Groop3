package engine.graphics;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

import org.joml.Vector2f;
import org.joml.Vector3f;

public class Model
{
	private ArrayList<Vector3f> verticies;
	private ArrayList<Vector2f> textureCoords;
	private ArrayList<Vector3f> vertexNormals;
	private ArrayList<Integer> vertexIndicies;
	private ArrayList<Integer> uvIndicies;
	private ArrayList<Integer> normalIndicies;
	private ArrayList<Material> materialIndicies;
	
	@SuppressWarnings("unused")
	public Model(String path)
	{
		this.verticies = new ArrayList<Vector3f>();
		this.textureCoords = new ArrayList<Vector2f>();
		this.vertexNormals = new ArrayList<Vector3f>();
		
		try (InputStream in = new FileInputStream(path); BufferedReader reader = new BufferedReader(new InputStreamReader(in)))
		{
			String line;
			
			while ((line = reader.readLine()) != null)
			{
				String[] s = line.split("\\s");
				MaterialLibrary matLib = null;
				Material currentMat = null;
				
				switch (s[0])
				{
				case "mtllib": 	//Material library
					matLib = new MaterialLibrary(s[1]);
					break;
				case "v":		//Vertex
					verticies.add(new Vector3f(Float.parseFloat(s[1]), Float.parseFloat(s[2]), Float.parseFloat(s[3])));
					break;
				case "vt":		//Vertex texture coord
					textureCoords.add(new Vector2f(Float.parseFloat(s[1]), Float.parseFloat(s[2])));
					break;
				case "vn":		//Vertex normal
					vertexNormals.add(new Vector3f(Float.parseFloat(s[1]), Float.parseFloat(s[2]), Float.parseFloat(s[3])));
					break;
				case "usemtl":
					if (matLib == null)
					{
						throw new RuntimeException("Material library not specified.");
					}
					else 
					{
						currentMat = matLib.getMaterial(s[1]);
						if (currentMat == null) throw new RuntimeException("Material not found: " + s[1]);
					}
					break;
				case "f":		//Face
					//This expects faces to have defined texture coords and normals and to be triangular.
					if (s.length != 4)
					{
						throw new RuntimeException("Exactly three verticies must be defined per face.");
					}
					else
					{
						for (int i = 1; i < 4; i++)
						{
							String[] v = s[i].split("/");
							
							if (v.length != 3) throw new RuntimeException("Exactly three elements must be defined per vertex");

							vertexIndicies.add(Integer.parseInt(v[0]));
							uvIndicies.add(Integer.parseInt(v[1]));
							normalIndicies.add(Integer.parseInt(v[2]));
						}
						
						materialIndicies.add(currentMat);
					}
					break;
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
}

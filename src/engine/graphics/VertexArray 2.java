package engine.graphics;

import static org.lwjgl.opengl.GL33.*;
import static org.lwjgl.system.MemoryStack.stackPush;
import static org.lwjgl.system.MemoryUtil.memUTF8;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import org.lwjgl.system.MemoryStack;

public abstract class VertexArray
{
	protected int vertexArrayObject;
	protected int shaderProgram;
	
	public abstract void create();
	
	public abstract void bind();
	
	public void unbind()
	{
		glBindVertexArray(0);
		glUseProgram(0);
	}
	
	public abstract void delete();

	protected static void checkShaderCompile(int shader)
	{
		try (MemoryStack stack = stackPush())
		{
			IntBuffer compileStatus = stack.mallocInt(1);
			glGetShaderiv(shader, GL_COMPILE_STATUS, compileStatus);
			
			if (compileStatus.get(0) != GL_TRUE)
			{
				IntBuffer logLength = stack.mallocInt(1);
				
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, logLength);
				
				ByteBuffer infoLog = stack.malloc(logLength.get(0));
				
				glGetShaderInfoLog(shader, logLength, infoLog);
				
				System.err.println("Shader compilation failed.");
				System.err.println(memUTF8(infoLog));
				
				throw new RuntimeException("Shader compilation failed.");
			}
		}
	}
	
	protected static CharSequence loadShaderSource(String path)
	{
		StringBuilder builder = new StringBuilder();
		
		try (InputStream in = new FileInputStream(path); BufferedReader reader = new BufferedReader(new InputStreamReader(in)))
		{
			String line;
			
			while ((line = reader.readLine()) != null)
			{
				builder.append(line).append('\n');
			}
		}
		catch (IOException ex)
		{
			throw new RuntimeException("Failed to load shader file: " + path);
		}
		
		CharSequence source = builder.toString();
		
		return source;
	}
}

package engine.graphics;

import static org.lwjgl.opengl.GL11.GL_FLOAT;
import static org.lwjgl.opengl.GL20.glEnableVertexAttribArray;
import static org.lwjgl.opengl.GL20.glGetAttribLocation;
import static org.lwjgl.opengl.GL20.glGetUniformLocation;
import static org.lwjgl.opengl.GL20.glVertexAttribPointer;
import static org.lwjgl.opengl.GL30.glBindVertexArray;
import static org.lwjgl.opengl.GL30.glGenVertexArrays;
import static org.lwjgl.opengl.GL33.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.memUTF8;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import org.lwjgl.system.*;

public class PrimitiveVertexArray extends VertexArray
{
	public int viewportBinding;
	
	public PrimitiveVertexArray()
	{
		vertexArrayObject = 0;
		shaderProgram = 0;
		viewportBinding = 0;
	}

	@Override
	public void create()
	{
		//Create shader program
		try (MemoryStack stack = stackPush())
		{
			int vShader = glCreateShader(GL_VERTEX_SHADER);
			int fShader = glCreateShader(GL_FRAGMENT_SHADER);

			glShaderSource(vShader, loadShaderSource("res/shaders/primitiveShader.vert"));
			glShaderSource(fShader, loadShaderSource("res/shaders/primitiveShader.frag"));

			glCompileShader(vShader);
			glCompileShader(fShader);

			checkShaderCompile(vShader);
			checkShaderCompile(fShader);
			
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vShader);
			glAttachShader(shaderProgram, fShader);
			
			glLinkProgram(shaderProgram);
			
			IntBuffer linkStatus = stack.mallocInt(1);			
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, linkStatus);
			
			if (linkStatus.get(0) != GL_TRUE)
			{
				IntBuffer logLength = stack.mallocInt(1);
				glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, logLength);
				
				ByteBuffer infoLog = stack.malloc(logLength.get(0));
				
				glGetProgramInfoLog(shaderProgram, logLength, infoLog);

				System.err.println("Program linking failed.");
				System.err.println(memUTF8(infoLog));
				
				throw new RuntimeException("Program linking failed.");
			}
			
			glDeleteShader(vShader);
			glDeleteShader(fShader);
		}

		//Create VAO
		try (MemoryStack stack = stackPush())
		{
			viewportBinding = glGetUniformLocation(shaderProgram, "uViewport");
			
			IntBuffer buffers = stack.mallocInt(1);
			glGenVertexArrays(buffers);
			
			vertexArrayObject = buffers.get(0);
			
			glBindVertexArray(vertexArrayObject);
			
			int vertPosBinding = glGetAttribLocation(shaderProgram, "vertPos");
			int vertColBinding = glGetAttribLocation(shaderProgram, "vertCol");

			glVertexAttribPointer(vertPosBinding, 3, GL_FLOAT, false, 24, 0); //Location
			glVertexAttribPointer(vertColBinding, 3, GL_FLOAT, false, 24, 12);//Color
			
			glEnableVertexAttribArray(vertPosBinding);
			glEnableVertexAttribArray(vertColBinding);
			
			glBindVertexArray(0);
		}
	}

	@Override
	public void bind()
	{
		glUseProgram(shaderProgram);
		glBindVertexArray(vertexArrayObject);
	}

	@Override
	public void delete()
	{
		glDeleteProgram(shaderProgram);
		glDeleteVertexArrays(vertexArrayObject);
	}
	
}

package engine.graphics;

import org.joml.Vector4f;
import org.joml.Matrix4f;
import org.lwjgl.system.*;
import org.lwjgl.opengl.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;
import static org.lwjgl.opengl.GL33.*;

import java.nio.*;

import java.util.Random;

import engine.core.EventBus;
import engine.core.components.DrawCubeComponent3D;
import engine.core.components.DrawModelComponent3D;
import engine.core.events.PostUpdateEvent;
import engine.core.memory.Allocator;
import engine.io.events.WindowFramebufferResizeEvent;
import engine.util.Accumulator;

public class GLRenderer
{
	private static PrimitiveVertexArray primitiveVertexArray;
	
	private static int vertexBuffer;
	private static int elementBuffer;
	
	private static long vertexBufferSize;
	private static long elementBufferSize;	
	
	private static Matrix4f viewportMatrix;
	
	public static void init()
	{
		EventBus.get(PostUpdateEvent.class).subscribeEvent(GLRenderer::onPostUpdate);
		EventBus.get(WindowFramebufferResizeEvent.class).subscribeEvent(GLRenderer::onFramebufferResize);
		
		GL.createCapabilities();
		
		//Create buffers
		try (MemoryStack stack = stackPush())
		{
			IntBuffer buffers = stack.mallocInt(2);
			glGenBuffers(buffers);
			
			vertexBuffer = buffers.get(0);
			elementBuffer = buffers.get(1);
			
			vertexBufferSize = 1024;
			elementBufferSize = 1024;
			
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, GL_STREAM_DRAW);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferSize, GL_STREAM_DRAW);
		}
		
		primitiveVertexArray = new PrimitiveVertexArray();
		primitiveVertexArray.create();
		
		viewportMatrix = new Matrix4f();
	}
	
	public static void destroy()
	{
		glDeleteBuffers(vertexBuffer);
		glDeleteBuffers(elementBuffer);
		primitiveVertexArray.delete();
		
		EventBus.get(PostUpdateEvent.class).unsubscribeEvent(GLRenderer::onPostUpdate);
	}
	
	private static void onFramebufferResize(WindowFramebufferResizeEvent ev)
	{
		glViewport(0, 0, ev.width, ev.height);
		
		viewportMatrix.setPerspective((float) (Math.PI / 2f), (float) ev.width / (float) ev.height, 1f, 100f)
					  .lookAt(0f, 0f, -5f, 0f, 0f, 0f, 0f, 1f, 0f);
	}
	
	private static void onPostUpdate(PostUpdateEvent ev)
	{
		//Clear color and depth buffers to prevent overdraw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Bind primitive assembly
		primitiveVertexArray.bind();
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		
		//Enable depth test so nearer geometry isn't drawn over
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		//Enable back face culling to reduce unnecessary drawing
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		try (MemoryStack stack = stackPush())
		{
			FloatBuffer floats = stack.mallocFloat(16);
			viewportMatrix.get(floats);
			glUniformMatrix4fv(primitiveVertexArray.viewportBinding, false, floats);
		}
		
		drawCubes();
		drawModels();
		
	}
	
	private static void drawModels()
	{
		Allocator<DrawModelComponent3D> modelAllocator = Allocator.get(DrawModelComponent3D.class);

		Accumulator acc = new Accumulator();
		
		modelAllocator.forEach((DrawModelComponent3D) ->
		{
			
		});
	}
	
	private static void drawCubes()
	{
		Allocator<DrawCubeComponent3D> cubeAllocator = Allocator.get(DrawCubeComponent3D.class);

		ByteBuffer vBuffer = memAlloc(8 * 6 * 4 * cubeAllocator.getCount());
		ByteBuffer eBuffer = memAlloc(36 * 4 * cubeAllocator.getCount());
		
		Random rand = new Random(6546843266L);
		
		cubeAllocator.forEach((DrawCubeComponent3D c) ->
		{
			Matrix4f transform = new Matrix4f().scaling(c.transform.scale).rotate(c.transform.rotation).translate(c.transform.location);
			transform.translation(c.transform.location).scale(c.transform.scale).rotate(c.transform.rotation);
			
			for (Vector4f v : new Vector4f[]
				{
					new Vector4f(-c.radii.x, -c.radii.y, -c.radii.z, 1f), //left 	bottom	back
					new Vector4f( c.radii.x, -c.radii.y, -c.radii.z, 1f), //right	bottom 	back
					new Vector4f(-c.radii.x,  c.radii.y, -c.radii.z, 1f), //left	top 	back
					new Vector4f( c.radii.x,  c.radii.y, -c.radii.z, 1f), //right	top		back
					new Vector4f(-c.radii.x, -c.radii.y,  c.radii.z, 1f), //left	bottom	front
					new Vector4f( c.radii.x, -c.radii.y,  c.radii.z, 1f), //right	bottom	front
					new Vector4f(-c.radii.x,  c.radii.y,  c.radii.z, 1f), //left	top		front
					new Vector4f( c.radii.x,  c.radii.y,  c.radii.z, 1f)  //right	top		front
				})
			{/*
				float r = (v.x > 0f)? 1f : 0f;
				float g = (v.y > 0f)? 1f : 0f;
				float b = (v.z > 0f)? 1f : 0f;*/
				
				v.mul(transform);
				
				vBuffer.putFloat(v.x);
				vBuffer.putFloat(v.y);
				vBuffer.putFloat(v.z);
				
				vBuffer.putFloat(rand.nextFloat());
				vBuffer.putFloat(rand.nextFloat());
				vBuffer.putFloat(rand.nextFloat());
			}
		});

		for (int i = 0; i < cubeAllocator.getCount(); i++)
		{
			//These elements are ordered counter-clockwise to obey the front face culling.
			eBuffer.putInt(i * 8 + 0);
			eBuffer.putInt(i * 8 + 1);
			eBuffer.putInt(i * 8 + 4);
			eBuffer.putInt(i * 8 + 5);
			eBuffer.putInt(i * 8 + 4);
			eBuffer.putInt(i * 8 + 1);
			
			eBuffer.putInt(i * 8 + 3);
			eBuffer.putInt(i * 8 + 2);
			eBuffer.putInt(i * 8 + 6);
			eBuffer.putInt(i * 8 + 6);
			eBuffer.putInt(i * 8 + 7);
			eBuffer.putInt(i * 8 + 3);
			
			eBuffer.putInt(i * 8 + 1);
			eBuffer.putInt(i * 8 + 0);
			eBuffer.putInt(i * 8 + 2);
			eBuffer.putInt(i * 8 + 1);
			eBuffer.putInt(i * 8 + 2);
			eBuffer.putInt(i * 8 + 3);
			
			eBuffer.putInt(i * 8 + 4);
			eBuffer.putInt(i * 8 + 5);
			eBuffer.putInt(i * 8 + 6);
			eBuffer.putInt(i * 8 + 6);
			eBuffer.putInt(i * 8 + 5);
			eBuffer.putInt(i * 8 + 7);
			
			eBuffer.putInt(i * 8 + 2);
			eBuffer.putInt(i * 8 + 0);
			eBuffer.putInt(i * 8 + 4);
			eBuffer.putInt(i * 8 + 2);
			eBuffer.putInt(i * 8 + 4);
			eBuffer.putInt(i * 8 + 6);

			eBuffer.putInt(i * 8 + 1);
			eBuffer.putInt(i * 8 + 3);
			eBuffer.putInt(i * 8 + 5);
			eBuffer.putInt(i * 8 + 5);
			eBuffer.putInt(i * 8 + 3);
			eBuffer.putInt(i * 8 + 7);
		}
		
		vBuffer.position(0);
		eBuffer.position(0);
		
		if (vBuffer.capacity() < vertexBufferSize)
		{
			glBufferData(GL_ARRAY_BUFFER, vBuffer.asFloatBuffer(), GL_DYNAMIC_DRAW);
			vertexBufferSize = vBuffer.capacity();
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, vBuffer);
		}
		
		if (eBuffer.capacity() < elementBufferSize)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, eBuffer.asFloatBuffer(), GL_DYNAMIC_DRAW);
			elementBufferSize = eBuffer.capacity();
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, eBuffer);
		}

		glDrawElements(GL_TRIANGLES, 36 * cubeAllocator.getCount(), GL_UNSIGNED_INT, 0);
		
		primitiveVertexArray.unbind();
		
		memFree(vBuffer);
		memFree(eBuffer);
	}
}

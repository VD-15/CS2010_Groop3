package engine.io;

import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.*;

import engine.core.Application;
import engine.core.EventBus;
import engine.core.events.*;
import engine.io.events.*;

import java.nio.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

public class Window
{
	private static long window = 0;
	
	public static void init()
	{
		EventBus.get(PreUpdateEvent.class).subscribeEvent(Window::onPreUpdate);
		EventBus.get(ApplicationStartEvent.class).subscribeEvent(Window::onApplicationStart);
		EventBus.get(ApplicationExitEvent.class).subscribeEvent(Window::onApplicationExit);
		
		GLFWErrorCallback.createPrint(System.err).set();
		
		if (!glfwInit()) throw new IllegalStateException("GLFW failed to initialize.");
		
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		
		window = glfwCreateWindow(640, 480, "Window Title", 0, 0);
		
		if (window == 0) throw new RuntimeException("Failed to create a window.");

		glfwSetCharCallback(window, Window::onChar);
		glfwSetWindowCloseCallback(window, Window::onClose);
		glfwSetWindowContentScaleCallback(window, Window::onContentScale);
		glfwSetCursorEnterCallback(window, Window::onCursorEnter);
		glfwSetCursorPosCallback(window, Window::onCursorMove);
		glfwSetDropCallback(window, Window::onWindowDrop);
		glfwSetFramebufferSizeCallback(window, Window::onFramebufferResize);
		glfwSetWindowIconifyCallback(window, Window::onIconify);
		glfwSetKeyCallback(window, Window::onKey);
		glfwSetWindowMaximizeCallback(window, Window::onMaximize);
		glfwSetMouseButtonCallback(window, Window::onMouseButton);
		glfwSetWindowPosCallback(window, Window::onMove);
		glfwSetWindowRefreshCallback(window, Window::onRefresh);
		glfwSetWindowSizeCallback(window, Window::onResize);
		glfwSetScrollCallback(window, Window::onScroll);
		
		try (MemoryStack stack = stackPush())
		{
			IntBuffer pWidth = stack.mallocInt(1);
			IntBuffer pHeight = stack.mallocInt(1);
			
			glfwGetWindowSize(window, pWidth, pHeight);
			
			GLFWVidMode vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			
			//Center the window
			glfwSetWindowPos(window, (vidMode.width() - pWidth.get(0)) / 2, (vidMode.height() - pHeight.get(0)) / 2);
		} // stack is popped automatically
		
		glfwMakeContextCurrent(window);
		
		//V-Sync
		glfwSwapInterval(1);
	}
	
	public static void destroy()
	{
		glfwFreeCallbacks(window);
		glfwDestroyWindow(window);
		glfwTerminate();
		glfwSetErrorCallback(null).free();

		EventBus.get(PreUpdateEvent.class).unsubscribeEvent(Window::onPreUpdate);
		EventBus.get(ApplicationStartEvent.class).unsubscribeEvent(Window::onApplicationStart);
		EventBus.get(ApplicationExitEvent.class).unsubscribeEvent(Window::onApplicationExit);
	}
	
	public static void setTitle(CharSequence title)
	{
		glfwSetWindowTitle(window, title);
	}
	
	private static void onPreUpdate(PreUpdateEvent ev)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		if (glfwWindowShouldClose(window))
		{
			Application.stop();
		}
	}
	
	private static void onApplicationStart(ApplicationStartEvent ev)
	{
		glfwShowWindow(window);
	}
	
	private static void onApplicationExit(ApplicationExitEvent ev)
	{
		glfwHideWindow(window);
	}
	
	private static void onChar(long window, int codePoint)
	{
		if (window == Window.window) EventBus.get(WindowCharEvent.class).postEvent(new WindowCharEvent(codePoint));
	}
	
	private static void onClose(long window)
	{
		if (window == Window.window) EventBus.get(WindowCloseEvent.class).postEvent(new WindowCloseEvent());
	}
	
	private static void onContentScale(long window, float scaleX, float scaleY)
	{
		if (window == Window.window) EventBus.get(WindowContentScaleEvent.class).postEvent(new WindowContentScaleEvent(scaleX, scaleY));
	}
	
	
	private static void onCursorEnter(long window, boolean entered)

	{
		if (window == Window.window) EventBus.get(WindowCursorEnterEvent.class).postEvent(new WindowCursorEnterEvent(entered));
	}
	
	private static void onCursorMove(long window, double posX, double posY)
	{
		if (window == Window.window) EventBus.get(WindowCursorMoveEvent.class).postEvent(new WindowCursorMoveEvent(posX, posY));
	}

	private static void onWindowDrop(long window, int count, long pointer)
	{
		if (window == Window.window)
		{
			PointerBuffer nameBuffer = memPointerBuffer(pointer, count);
			
			String[] strings = new String[count];
			
			for (int i = 0; i < count; i++)
			{
				strings[i] = memUTF8(memByteBufferNT1(nameBuffer.get(count)));
			}
			
			EventBus.get(WindowFileDropEvent.class).postEvent(new WindowFileDropEvent(count, strings));
		}
	}

	private static void onFramebufferResize(long window, int width, int height)
	{
		if (window == Window.window) EventBus.get(WindowFramebufferResizeEvent.class).postEvent(new WindowFramebufferResizeEvent(width, height));
	}
	
	private static void onIconify(long window, boolean iconified)
	{
		if (window == Window.window) EventBus.get(WindowIconifyEvent.class).postEvent(new WindowIconifyEvent(iconified));
	}
	
	private static void onKey(long window, int key, int scancode, int action, int mods)
	{
		if (window == Window.window) EventBus.get(WindowKeyEvent.class).postEvent(new WindowKeyEvent(key, scancode, action, mods));
	}
	
	private static void onMaximize(long window, boolean maximized)
	{
		if (window == Window.window) EventBus.get(WindowMaximizeEvent.class).postEvent(new WindowMaximizeEvent(maximized));
	}
	
	private static void onMouseButton(long window, int button, int action, int mods)
	{
		if (window == Window.window) EventBus.get(WindowMouseButtonEvent.class).postEvent(new WindowMouseButtonEvent(button, action, mods));
	}
	
	private static void onMove(long window, int posX, int posY)
	{
		if (window == Window.window) EventBus.get(WindowMoveEvent.class).postEvent(new WindowMoveEvent(posX, posY));
	}
	
	private static void onRefresh(long window)
	{
		if (window == Window.window) EventBus.get(WindowRefreshEvent.class).postEvent(new WindowRefreshEvent());
	}
	
	private static void onResize(long window, int width, int height)
	{
		if (window == Window.window) EventBus.get(WindowResizeEvent.class).postEvent(new WindowResizeEvent(width, height));
	}
	
	private static void onScroll(long window, double xVal, double yVal)
	{
		if (window == Window.window) EventBus.get(WindowScrollEvent.class).postEvent(new WindowScrollEvent(xVal, yVal));
	}
}

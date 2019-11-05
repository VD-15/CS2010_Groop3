#pragma once
#include <string>

#include "ValkyrieEngine.h"
#include "../utils/Vector.h"
#include "EventBus.hpp"

namespace vlk
{
	//The waiting method a Window will use inbetween frames.
	enum class WindowWaitMode
	{
		//Process any events that are in the queue, then return.
		Poll,

		//Wait until an event is present in the queue, then process them.
		Wait,

		//Wait until an event is present in the queue, or until a specified amount of time elapses.
		WaitTimeout
	};

	#pragma region Callbacks
	//Called when a unicode character is input to the window.
	//Use this for text input.
	struct CharEvent : public Event 
	{ 
		CharEvent(UInt codepoint);

		const UInt codepoint; 
	};

	//Called whenever the cursor enters or leaves the window.
	struct CursorEnterEvent : public Event 
	{
		CursorEnterEvent(Int entered);

		const Int entered;
	};

	//Called whenever the cursor moves.
	struct CursorPosEvent : public Event 
	{
		CursorPosEvent(Double x, Double y);

		const Double x; 
		const Double y; 
	};

	//Called whenever a key is pressed or released.
	//Use this for non-text keyboard input.
	struct KeyEvent : public Event 
	{
		KeyEvent(Int key, Int scancode, Int action, Int mods);

		const Int key; 
		const Int scancode; 
		const Int action; 
		const Int mods; 
	};

	//Called whenever a mouse button is pressed or released.
	//Use this for mouse input.
	struct MouseButtonEvent : public Event 
	{
		MouseButtonEvent(Int button, Int action, Int mods);

		const Int button; 
		const Int action; 
		const Int mods; 
	};

	//Called whenever the mouse is scrolled.
	//Use this for mouse scrolling.
	struct ScrollEvent : public Event 
	{ 
		ScrollEvent(Double x, Double y);

		const Double x; 
		const Double y; 
	};

	//Called whenever a file is dropped into the window.
	//Use this for graphical file input.
	struct WindowFileDropEvent : public Event 
	{
		WindowFileDropEvent(Int fileCount, const char* const* paths);

		const Int fileCount; 
		const char* const* const paths; 
	};

	//Called whenever the window's frame buffer is resized.
	//Use this for UI / renderer resizes.
	struct WindowFramebufferEvent : public Event
	{
		WindowFramebufferEvent(Int width, Int height);

		const Int width; 
		const Int height; 
	};

	//Called when the window is closed.
	struct WindowCloseEvent : public Event {};

	//Called when the window becomes focused or unfocused.
	struct WindowFocusEvent : public Event 
	{
		WindowFocusEvent(Int focused);

		const Int focused; 
	};

	//Called when the window is iconified or restored.
	struct WindowIconifyEvent : public Event 
	{ 
		WindowIconifyEvent(Int iconified);

		const Int iconified; 
	};

	//Called when the window is moved.
	struct WindowMoveEvent : public Event 
	{
		WindowMoveEvent(Int x, Int y);

		const Int x; 
		const Int y; 
	};

	//Called when the window is refreshed.
	struct WindowRefreshEvent : public Event {};

	//Called when the window is resized.
	struct WindowResizeEvent : public Event 
	{
		WindowResizeEvent(Int width, Int height);

		const Int width; 
		const Int height; 
	};
	#pragma endregion

	namespace Window
	{
		#pragma region Lifetime
		//Initializes the window.
		//Any other Eventctions in Window may not be called before this unless otherwise stated.
		void Init();

		//Processes any events the window has recieved.
		//This should be called at the beginning of every update.
		void ProcessEvents(PreUpdateEvent& ev);

		//To be called when the application starts.
		void OnStart(ApplicationStartEvent& ev);

		//To be called when the application closes.
		void OnExit(ApplicationExitEvent& ev);

		//Destroyes the window.
		void Destroy();
		#pragma endregion

		#pragma region Modifiers
		//Sets the title for the window.
		void SetTitle(const std::string& title);

		//Sets the width of the window, in pixels.
		void SetWidth(Int width);

		//Sets the height of the window, in pixels.
		void SetHeight(Int height);

		//Sets the width and height of the window, in pixels.
		void SetSize(Int width, Int height);

		//Sets the width and height of the window, in pixels.
		void SetSize(Pair<Int> size);

		//Sets the swap interval of the window.
		//A value of 1 can be used to implement V-Sync,
		//Values greater than 1 can be used to implement buffered V-Sync,
		//Default value is 0.
		//Maybe don't set this to a negative number...
		void SetSwapInterval(Int interval);

		//Sets how the window should wait for events.
		void SetWaitMode(WindowWaitMode mode);

		//Sets how long the window should wait for events when on the WindowWaitMode::WaitTimeout setting
		void SetWaitTimeout(Double waitTimeout);

		//Shows the cursor.
		void ShowCursor();

		//Makes the cursor invisible.
		void HideCursor();

		//Disables (locks) the cursor.
		void DisableCursor();

		//Makes the window visible.
		void Show();

		//Makes the window invisible.
		void Hide();
		#pragma endregion

		#pragma region Accessors
		//Gets the width of the window, in pixels.
		Int GetWidth();

		//Gets the height of the window, in pixels.
		Int GetHeight();

		//Gets the size of the window, in pixels.
		Pair<Int>GetSize();
		#pragma endregion
	}
}
/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.	If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#include <SDL2/SDL.h>

#include "sys/platform.h"
#include "idlib/containers/List.h"
#include "idlib/Heap.h"
#include "framework/Common.h"
#include "framework/KeyInput.h"
#include "renderer/RenderSystem.h"
#include "renderer/tr_local.h"

#include "sys/sys_public.h"

const char *kbdNames[] = {
	"english", "french", "german", "italian", "spanish", "turkish", "norwegian", NULL
};

void SDL_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void *ptr );
void SDL_AddKeyboardPollEvent(int key, bool state);
void SDL_AddMousePollEvent(int action, int value);


idCVar in_kbd("in_kbd", "english", CVAR_SYSTEM | CVAR_ARCHIVE | CVAR_NOCHEAT, "keyboard layout", kbdNames, idCmdSystem::ArgCompletion_String<kbdNames> );

struct kbd_poll_t {
	int key;
	bool state;

	kbd_poll_t() {
	}

	kbd_poll_t(int k, bool s) {
		key = k;
		state = s;
	}
};

struct mouse_poll_t {
	int action;
	int value;

	mouse_poll_t() {
	}

	mouse_poll_t(int a, int v) {
		action = a;
		value = v;
	}
};

static idList<kbd_poll_t> kbd_polls;
static idList<mouse_poll_t> mouse_polls;

static byte mapkey(SDL_Keycode key) {
	switch (key) {
	case SDLK_BACKSPACE:
		return K_BACKSPACE;
	case SDLK_PAUSE:
		return K_PAUSE;
	}

	if (key <= SDLK_z)
		return key & 0xff;

	switch (key) {
	case SDLK_APPLICATION:
		return K_COMMAND;
	case SDLK_CAPSLOCK:
		return K_CAPSLOCK;
	case SDLK_SCROLLLOCK:
		return K_SCROLL;
	case SDLK_POWER:
		return K_POWER;

	case SDLK_UP:
		return K_UPARROW;
	case SDLK_DOWN:
		return K_DOWNARROW;
	case SDLK_LEFT:
		return K_LEFTARROW;
	case SDLK_RIGHT:
		return K_RIGHTARROW;

	case SDLK_LGUI:
		return K_LWIN;
	case SDLK_RGUI:
		return K_RWIN;
	case SDLK_MENU:
		return K_MENU;

	case SDLK_LALT:
	case SDLK_RALT:
		return K_ALT;
	case SDLK_RCTRL:
	case SDLK_LCTRL:
		return K_CTRL;
	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		return K_SHIFT;
	case SDLK_INSERT:
		return K_INS;
	case SDLK_DELETE:
		return K_DEL;
	case SDLK_PAGEDOWN:
		return K_PGDN;
	case SDLK_PAGEUP:
		return K_PGUP;
	case SDLK_HOME:
		return K_HOME;
	case SDLK_END:
		return K_END;

	case SDLK_F1:
		return K_F1;
	case SDLK_F2:
		return K_F2;
	case SDLK_F3:
		return K_F3;
	case SDLK_F4:
		return K_F4;
	case SDLK_F5:
		return K_F5;
	case SDLK_F6:
		return K_F6;
	case SDLK_F7:
		return K_F7;
	case SDLK_F8:
		return K_F8;
	case SDLK_F9:
		return K_F9;
	case SDLK_F10:
		return K_F10;
	case SDLK_F11:
		return K_F11;
	case SDLK_F12:
		return K_F12;
	// K_INVERTED_EXCLAMATION;
	case SDLK_F13:
		return K_F13;
	case SDLK_F14:
		return K_F14;
	case SDLK_F15:
		return K_F15;

	case SDLK_KP_7:
		return K_KP_HOME;
	case SDLK_KP_8:
		return K_KP_UPARROW;
	case SDLK_KP_9:
		return K_KP_PGUP;
	case SDLK_KP_4:
		return K_KP_LEFTARROW;
	case SDLK_KP_5:
		return K_KP_5;
	case SDLK_KP_6:
		return K_KP_RIGHTARROW;
	case SDLK_KP_1:
		return K_KP_END;
	case SDLK_KP_2:
		return K_KP_DOWNARROW;
	case SDLK_KP_3:
		return K_KP_PGDN;
	case SDLK_KP_ENTER:
		return K_KP_ENTER;
	case SDLK_KP_0:
		return K_KP_INS;
	case SDLK_KP_PERIOD:
		return K_KP_DEL;
	case SDLK_KP_DIVIDE:
		return K_KP_SLASH;
	// K_SUPERSCRIPT_TWO;
	case SDLK_KP_MINUS:
		return K_KP_MINUS;
	// K_ACUTE_ACCENT;
	case SDLK_KP_PLUS:
		return K_KP_PLUS;
	case SDLK_NUMLOCKCLEAR:
		return K_KP_NUMLOCK;
	case SDLK_KP_MULTIPLY:
		return K_KP_STAR;
	case SDLK_KP_EQUALS:
		return K_KP_EQUALS;

	// K_MASCULINE_ORDINATOR;
	// K_GRAVE_A;
	// K_AUX1;
	// K_CEDILLA_C;
	// K_GRAVE_E;
	// K_AUX2;
	// K_AUX3;
	// K_AUX4;
	// K_GRAVE_I;
	// K_AUX5;
	// K_AUX6;
	// K_AUX7;
	// K_AUX8;
	// K_TILDE_N;
	// K_GRAVE_O;
	// K_AUX9;
	// K_AUX10;
	// K_AUX11;
	// K_AUX12;
	// K_AUX13;
	// K_AUX14;
	// K_GRAVE_U;
	// K_AUX15;
	// K_AUX16;

	case SDLK_PRINTSCREEN:
		return K_PRINT_SCR;
	case SDLK_MODE:
		return K_RIGHT_ALT;
	}

	return 0;
}

static void PushConsoleEvent(const char *s) {
	char *b;
	size_t len;

	len = strlen(s) + 1;
	b = (char *)Mem_Alloc(len);
	strcpy(b, s);

	SDL_Event event;

	event.type = SDL_USEREVENT;
	event.user.code = SE_CONSOLE;
	event.user.data1 = (void *)len;
	event.user.data2 = b;

	SDL_PushEvent(&event);
}

/*
=================
Sys_InitInput
=================
*/
void Sys_InitInput() {
	kbd_polls.SetGranularity(64);
	mouse_polls.SetGranularity(64);

	in_kbd.SetModified();
}

/*
=================
Sys_ShutdownInput
=================
*/
void Sys_ShutdownInput() {
	kbd_polls.Clear();
	mouse_polls.Clear();
}

/*
===========
Sys_InitScanTable
===========
*/
void Sys_InitScanTable() {
}


/*
===============
Sys_GetConsoleKey
===============
*/
unsigned char Sys_GetConsoleKey(bool shifted) {
  return K_F12;
}

/*
===============
Sys_MapCharForKey
===============
*/
unsigned char Sys_MapCharForKey(int key) {
	return key & 0xff;
}

/*
===============
Sys_GrabMouseCursor
===============
*/
void Sys_GrabMouseCursor(bool grabIt) {
	int flags;

	if (grabIt)
		flags = GRAB_ENABLE | GRAB_HIDECURSOR | GRAB_SETSTATE;
	else
		flags = GRAB_SETSTATE;

	GLimp_GrabInput(flags);
}

/*
================
Sys_GetEvent
================
*/

/*
polls single event from SDL and appends to local processing que
*/

void SDL_PollEvents (void) { 

	SDL_Event ev;

	int doomKey;
	bool keyDownFlag;
		

	if (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_WINDOWEVENT:
			switch (ev.window.event) {
			        case SDL_WINDOWEVENT_SHOWN:
					SDL_Log("Window %d shown", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_HIDDEN:
					SDL_Log("Window %d hidden", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					SDL_Log("Window %d exposed", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_MOVED:
					SDL_Log("Window %d moved to %d,%d",
							ev.window.windowID, ev.window.data1,
							ev.window.data2);
					break;
				case SDL_WINDOWEVENT_RESIZED:
					SDL_Log("Window %d resized to %dx%d",
							ev.window.windowID, ev.window.data1,
							ev.window.data2);
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					SDL_Log("Window %d minimized", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					SDL_Log("Window %d maximized", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_RESTORED:
					SDL_Log("Window %d restored", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_ENTER:
					SDL_Log("Mouse entered window %d",
							ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_LEAVE:
					SDL_Log("Mouse left window %d", ev.window.windowID);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					SDL_Log("Window %d gained keyboard focus",
							ev.window.windowID);
					cvarSystem->SetCVarBool( "com_pause", false );
					GLimp_GrabInput(GRAB_ENABLE | GRAB_REENABLE | GRAB_HIDECURSOR);
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					SDL_Log("Window %d lost keyboard focus",
							ev.window.windowID);
					cvarSystem->SetCVarBool( "com_pause", true );
					GLimp_GrabInput(0);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					SDL_Log("Window %d closed", ev.window.windowID);
					break;
				default:
					SDL_Log("Window %d got unknown event %d",
							ev.window.windowID, ev.window.event);
					break;
			}
			break;

		case SDL_KEYDOWN:
			if (ev.key.keysym.sym == SDLK_RETURN && (ev.key.keysym.mod & KMOD_ALT) > 0) {
				cvarSystem->SetCVarBool("r_fullscreen", !renderSystem->IsFullScreen());
				PushConsoleEvent("vid_restart");
				/*
				 * FIXME vid_restart
				 */
				break;
			}

			// fall through
		case SDL_KEYUP:
		  {
		    doomKey = mapkey(ev.key.keysym.sym);
			if(!doomKey) {
			  if (ev.type == SDL_KEYDOWN) {
			    common->Warning("unmapped SDL key %d", ev.key.keysym.sym);
			    break;
			  }
			}
			
			keyDownFlag = ev.key.state == SDL_PRESSED ? 1 : 0;
			
			SDL_QueEvent( SE_KEY, doomKey, keyDownFlag, 0, NULL );
			if( keyDownFlag )
			  SDL_QueEvent( SE_CHAR, doomKey, 0, 0, NULL);
			SDL_AddKeyboardPollEvent( doomKey, keyDownFlag );
		  }
			break;
		case SDL_TEXTINPUT:
		  {
		      //unicode input, i.e. windows utf16 falls down here
		    byte c;
		    char *s;
		    if (ev.text.text && *ev.text.text) {
		      if (!ev.text.text[1])
			c = *ev.text.text;
		      else
			s = strdup(ev.text.text);
		    }
		    doomKey = mapkey(c);
			if(!doomKey) {
			  if (ev.type == SDL_KEYDOWN) {
			    common->Warning("unmapped SDL key %d", ev.key.keysym.sym);
			    break;
			  }
			}

			SDL_QueEvent( SE_KEY, doomKey, keyDownFlag, 0, NULL );
			if( keyDownFlag )
			  SDL_QueEvent( SE_CHAR, doomKey, 0, 0, NULL);
			SDL_AddKeyboardPollEvent( doomKey, keyDownFlag );

		  }
		  break;
		case SDL_TEXTEDITING:
		  SDL_StartTextInput();
		  SDL_Log("SDL_TextEditingEvent");
		  SDL_StopTextInput();
		  break;
		case SDL_SYSWMEVENT:
		  SDL_Log("SDL_SYSWMEVENT");
		  break;
		case SDL_CLIPBOARDUPDATE:
		  SDL_Log("SDL_CLIPBOARDUPDATE");
		  break;
		case SDL_MOUSEMOTION:
			SDL_QueEvent( SE_MOUSE, ev.motion.xrel, ev.motion.yrel, 0, NULL);

			SDL_AddMousePollEvent( M_DELTAX, ev.motion.xrel );
			SDL_AddMousePollEvent( M_DELTAY, ev.motion.yrel );
			break;
		case SDL_MOUSEWHEEL:
		  {
		    if( ev.wheel.y > 0 )
		      {
			SDL_QueEvent( SE_KEY, K_MWHEELUP, 1, 0, NULL);
			SDL_AddMousePollEvent( M_DELTAZ, 1 );
		      }
		    else
		      {
			SDL_QueEvent( SE_KEY, K_MWHEELDOWN, 1, 0, NULL);
			SDL_AddMousePollEvent( M_DELTAZ, -1 );
		      }
		  }
		  break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		  {
		  int updown = ( ev.type == SDL_MOUSEBUTTONDOWN );
		  int button;
		  switch( ev.button.button )
		    {
		    default:
		    case SDL_BUTTON_LEFT:
		      button = K_MOUSE1;
		      break;
		    case SDL_BUTTON_MIDDLE:
		      button = K_MOUSE3;
		      break;
		    case SDL_BUTTON_RIGHT:
		      button = K_MOUSE2;
		      break;
		    }
		  SDL_QueEvent( SE_KEY, button, updown, 0, NULL );
		  }
		  break;
		case SDL_QUIT:
			PushConsoleEvent("quit");
			SDL_Quit();
			break;
		case SDL_USEREVENT:
			switch (ev.user.code) {
			case SE_CONSOLE:

			  SDL_QueEvent( SE_CONSOLE, 0, 0, (intptr_t)ev.user.data1, ev.user.data2);
			  break;
			default:
				common->Warning("unknown user event %u", ev.user.code);
				break;
			}
		default:
			common->Warning("unknown event %u", ev.type);
			break;
		}
	}
}

/*
================
Sys_ClearEvents
================
*/
void Sys_ClearEvents() {
	SDL_Event ev;

	while (SDL_PollEvent(&ev))
		;

	kbd_polls.SetNum(0, false);
	mouse_polls.SetNum(0, false);
}

/*
================
Sys_GenerateEvents
================
*/
void Sys_GenerateEvents() {
	char *s = Sys_ConsoleInput();

	if (s)
		PushConsoleEvent(s);

	SDL_PumpEvents();
}

/*
================
Sys_PollKeyboardInputEvents
================
*/
int Sys_PollKeyboardInputEvents() {
	return kbd_polls.Num();
}

/*
================
Sys_ReturnKeyboardInputEvent
================
*/
int Sys_ReturnKeyboardInputEvent(const int n, int &key, bool &state) {
	if (n >= kbd_polls.Num())
		return 0;

	key = kbd_polls[n].key;
	state = kbd_polls[n].state;
	return 1;
}

/*
================
Sys_EndKeyboardInputEvents
================
*/
void Sys_EndKeyboardInputEvents() {
	kbd_polls.SetNum(0, false);
}

/*
================
Sys_PollMouseInputEvents
================
*/
int Sys_PollMouseInputEvents() {
	return mouse_polls.Num();
}

/*
================
Sys_ReturnMouseInputEvent
================
*/
int	Sys_ReturnMouseInputEvent(const int n, int &action, int &value) {
	if (n >= mouse_polls.Num())
		return 0;

	action = mouse_polls[n].action;
	value = mouse_polls[n].value;
	return 1;
}

/*
================
Sys_EndMouseInputEvents
================
*/
void Sys_EndMouseInputEvents() {
	mouse_polls.SetNum(0, false);
}

void SDL_AddKeyboardPollEvent(int key, bool state) {
	kbd_polls.Append(kbd_poll_t(key, state));
}
//
void SDL_AddMousePollEvent(int action, int value) {
	mouse_polls.Append(mouse_poll_t(action, value));
}

/*
================
SDL_QueEvent
 
ptr should either be null, or point to a block of data that can be freed later
================
*/
#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

static sysEvent_t eventQue[MAX_QUED_EVENTS];
static int eventHead, eventTail;

void SDL_QueEvent( sysEventType_t type, int value, int value2, int ptrLength, void *ptr ) {

	sysEvent_t *ev;

	ev = &eventQue[eventHead & MASK_QUED_EVENTS];
	if (eventHead - eventTail >= MAX_QUED_EVENTS) {
		common->Printf( "SDL_QueEvent: overflow\n" );
		// we are discarding an event, but don't leak memory
		// TTimo: verbose dropped event types?
		if (ev->evPtr) {
			Mem_Free(ev->evPtr);
			ev->evPtr = NULL;
		}
		eventTail++;
	}

	eventHead++;

	ev->evType = type;
	ev->evValue = value;
	ev->evValue2 = value2;
	ev->evPtrLength = ptrLength;
	ev->evPtr = ptr;
	
	//common->Printf( "Event %d: %d %d\n", ev->evType, ev->evValue, ev->evValue2 );
}

/*
================
Sys_GetEvent
================
*/
sysEvent_t Sys_GetEvent( void )
{
	static sysEvent_t ev;

	SDL_PollEvents();

	// return if we have data
	if (eventHead > eventTail) {
		eventTail++;
		return eventQue[(eventTail - 1) & MASK_QUED_EVENTS];
	}
	
	// return the empty event with the current time
	memset(&ev, 0, sizeof(ev));

	return ev;
}


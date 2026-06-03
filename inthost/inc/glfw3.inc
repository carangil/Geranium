section glfw

	type cursor end
	type VkAllocationCallbacks end
	type VkSurfaceKHR end
	type gamepadstate end
	type vidmode end
	type window end
	type image end
	type unsignedchar end
	type monitor end
	type gammaramp end
         3 constant VERSION_MAJOR

         3 constant VERSION_MINOR

      10 constant VERSION_REVISION

                  1 constant TRUE

                 0 constant FALSE

               0 constant RELEASE

                 1 constant PRESS

                2 constant REPEAT

          0 constant HAT_CENTERED

                1 constant HAT_UP

             2 constant HAT_RIGHT

              4 constant HAT_DOWN

              8 constant HAT_LEFT

             32 constant KEY_SPACE

                 48 constant KEY_0

                 49 constant KEY_1

                 50 constant KEY_2

                 51 constant KEY_3

                 52 constant KEY_4

                 53 constant KEY_5

                 54 constant KEY_6

                 55 constant KEY_7

                 56 constant KEY_8

                 57 constant KEY_9

                 65 constant KEY_A

                 66 constant KEY_B

                 67 constant KEY_C

                 68 constant KEY_D

                 69 constant KEY_E

                 70 constant KEY_F

                 71 constant KEY_G

                 72 constant KEY_H

                 73 constant KEY_I

                 74 constant KEY_J

                 75 constant KEY_K

                 76 constant KEY_L

                 77 constant KEY_M

                 78 constant KEY_N

                 79 constant KEY_O

                 80 constant KEY_P

                 81 constant KEY_Q

                 82 constant KEY_R

                 83 constant KEY_S

                 84 constant KEY_T

                 85 constant KEY_U

                 86 constant KEY_V

                 87 constant KEY_W

                 88 constant KEY_X

                 89 constant KEY_Y

                 90 constant KEY_Z

            256 constant KEY_ESCAPE

             257 constant KEY_ENTER

               258 constant KEY_TAB

         259 constant KEY_BACKSPACE

            260 constant KEY_INSERT

            261 constant KEY_DELETE

             262 constant KEY_RIGHT

              263 constant KEY_LEFT

              264 constant KEY_DOWN

                265 constant KEY_UP

           266 constant KEY_PAGE_UP

         267 constant KEY_PAGE_DOWN

              268 constant KEY_HOME

               269 constant KEY_END

         280 constant KEY_CAPS_LOCK

       281 constant KEY_SCROLL_LOCK

          282 constant KEY_NUM_LOCK

      283 constant KEY_PRINT_SCREEN

             284 constant KEY_PAUSE

                290 constant KEY_F1

                291 constant KEY_F2

                292 constant KEY_F3

                293 constant KEY_F4

                294 constant KEY_F5

                295 constant KEY_F6

                296 constant KEY_F7

                297 constant KEY_F8

                298 constant KEY_F9

               299 constant KEY_F10

               300 constant KEY_F11

               301 constant KEY_F12

               302 constant KEY_F13

               303 constant KEY_F14

               304 constant KEY_F15

               305 constant KEY_F16

               306 constant KEY_F17

               307 constant KEY_F18

               308 constant KEY_F19

               309 constant KEY_F20

               310 constant KEY_F21

               311 constant KEY_F22

               312 constant KEY_F23

               313 constant KEY_F24

               314 constant KEY_F25

              320 constant KEY_KP_0

              321 constant KEY_KP_1

              322 constant KEY_KP_2

              323 constant KEY_KP_3

              324 constant KEY_KP_4

              325 constant KEY_KP_5

              326 constant KEY_KP_6

              327 constant KEY_KP_7

              328 constant KEY_KP_8

              329 constant KEY_KP_9

        330 constant KEY_KP_DECIMAL

         331 constant KEY_KP_DIVIDE

       332 constant KEY_KP_MULTIPLY

       333 constant KEY_KP_SUBTRACT

            334 constant KEY_KP_ADD

          335 constant KEY_KP_ENTER

          336 constant KEY_KP_EQUAL

        340 constant KEY_LEFT_SHIFT

      341 constant KEY_LEFT_CONTROL

          342 constant KEY_LEFT_ALT

        343 constant KEY_LEFT_SUPER

       344 constant KEY_RIGHT_SHIFT

     345 constant KEY_RIGHT_CONTROL

         346 constant KEY_RIGHT_ALT

       347 constant KEY_RIGHT_SUPER

              348 constant KEY_MENU

          0x0001 constant MOD_SHIFT

        0x0002 constant MOD_CONTROL

            0x0004 constant MOD_ALT

          0x0008 constant MOD_SUPER

      0x0010 constant MOD_CAPS_LOCK

       0x0020 constant MOD_NUM_LOCK

        0 constant MOUSE_BUTTON_1

        1 constant MOUSE_BUTTON_2

        2 constant MOUSE_BUTTON_3

        3 constant MOUSE_BUTTON_4

        4 constant MOUSE_BUTTON_5

        5 constant MOUSE_BUTTON_6

        6 constant MOUSE_BUTTON_7

        7 constant MOUSE_BUTTON_8

            0 constant JOYSTICK_1

            1 constant JOYSTICK_2

            2 constant JOYSTICK_3

            3 constant JOYSTICK_4

            4 constant JOYSTICK_5

            5 constant JOYSTICK_6

            6 constant JOYSTICK_7

            7 constant JOYSTICK_8

            8 constant JOYSTICK_9

           9 constant JOYSTICK_10

           10 constant JOYSTICK_11

           11 constant JOYSTICK_12

           12 constant JOYSTICK_13

           13 constant JOYSTICK_14

           14 constant JOYSTICK_15

           15 constant JOYSTICK_16

              0 constant GAMEPAD_BUTTON_A

              1 constant GAMEPAD_BUTTON_B

              2 constant GAMEPAD_BUTTON_X

              3 constant GAMEPAD_BUTTON_Y

    4 constant GAMEPAD_BUTTON_LEFT_BUMPER

   5 constant GAMEPAD_BUTTON_RIGHT_BUMPER

           6 constant GAMEPAD_BUTTON_BACK

          7 constant GAMEPAD_BUTTON_START

          8 constant GAMEPAD_BUTTON_GUIDE

     9 constant GAMEPAD_BUTTON_LEFT_THUMB

    10 constant GAMEPAD_BUTTON_RIGHT_THUMB

        11 constant GAMEPAD_BUTTON_DPAD_UP

     12 constant GAMEPAD_BUTTON_DPAD_RIGHT

      13 constant GAMEPAD_BUTTON_DPAD_DOWN

      14 constant GAMEPAD_BUTTON_DPAD_LEFT

       0 constant GAMEPAD_AXIS_LEFT_X

       1 constant GAMEPAD_AXIS_LEFT_Y

      2 constant GAMEPAD_AXIS_RIGHT_X

      3 constant GAMEPAD_AXIS_RIGHT_Y

 4 constant GAMEPAD_AXIS_LEFT_TRIGGER

5 constant GAMEPAD_AXIS_RIGHT_TRIGGER

              0 constant NO_ERROR

       0x00010001 constant NOT_INITIALIZED

    0x00010002 constant NO_CURRENT_CONTEXT

          0x00010003 constant INVALID_ENUM

         0x00010004 constant INVALID_VALUE

         0x00010005 constant OUT_OF_MEMORY

       0x00010006 constant API_UNAVAILABLE

   0x00010007 constant VERSION_UNAVAILABLE

        0x00010008 constant PLATFORM_ERROR

    0x00010009 constant FORMAT_UNAVAILABLE

     0x0001000A constant NO_WINDOW_CONTEXT

               0x00020001 constant FOCUSED

             0x00020002 constant ICONIFIED

             0x00020003 constant RESIZABLE

               0x00020004 constant VISIBLE

             0x00020005 constant DECORATED

          0x00020006 constant AUTO_ICONIFY

              0x00020007 constant FLOATING

             0x00020008 constant MAXIMIZED

         0x00020009 constant CENTER_CURSOR

0x0002000A constant TRANSPARENT_FRAMEBUFFER

               0x0002000B constant HOVERED

         0x0002000C constant FOCUS_ON_SHOW

              0x00021001 constant RED_BITS

            0x00021002 constant GREEN_BITS

             0x00021003 constant BLUE_BITS

            0x00021004 constant ALPHA_BITS

            0x00021005 constant DEPTH_BITS

          0x00021006 constant STENCIL_BITS

        0x00021007 constant ACCUM_RED_BITS

      0x00021008 constant ACCUM_GREEN_BITS

       0x00021009 constant ACCUM_BLUE_BITS

      0x0002100A constant ACCUM_ALPHA_BITS

           0x0002100B constant AUX_BUFFERS

                0x0002100C constant STEREO

               0x0002100D constant SAMPLES

          0x0002100E constant SRGB_CAPABLE

          0x0002100F constant REFRESH_RATE

          0x00021010 constant DOUBLEBUFFER

            0x00022001 constant CLIENT_API

 0x00022002 constant CONTEXT_VERSION_MAJOR

 0x00022003 constant CONTEXT_VERSION_MINOR

      0x00022004 constant CONTEXT_REVISION

    0x00022005 constant CONTEXT_ROBUSTNESS

 0x00022006 constant OPENGL_FORWARD_COMPAT

  0x00022007 constant OPENGL_DEBUG_CONTEXT

        0x00022008 constant OPENGL_PROFILE

0x00022009 constant CONTEXT_RELEASE_BEHAVIOR

      0x0002200A constant CONTEXT_NO_ERROR

  0x0002200B constant CONTEXT_CREATION_API

      0x0002200C constant SCALE_TO_MONITOR

0x00023001 constant COCOA_RETINA_FRAMEBUFFER

        0x00023002 constant COCOA_FRAME_NAME

0x00023003 constant COCOA_GRAPHICS_SWITCHING

        0x00024001 constant X11_CLASS_NAME

     0x00024002 constant X11_INSTANCE_NAME

                         0 constant NO_API

            0x00030001 constant OPENGL_API

         0x00030002 constant OPENGL_ES_API

                  0 constant NO_ROBUSTNESS

 0x00031001 constant NO_RESET_NOTIFICATION

 0x00031002 constant LOSE_CONTEXT_ON_RESET

             0 constant OPENGL_ANY_PROFILE

   0x00032001 constant OPENGL_CORE_PROFILE

 0x00032002 constant OPENGL_COMPAT_PROFILE

                0x00033001 constant CURSOR

           0x00033002 constant STICKY_KEYS

  0x00033003 constant STICKY_MOUSE_BUTTONS

         0x00033004 constant LOCK_KEY_MODS

      0x00033005 constant RAW_MOUSE_MOTION

         0x00034001 constant CURSOR_NORMAL

         0x00034002 constant CURSOR_HIDDEN

       0x00034003 constant CURSOR_DISABLED

           0 constant ANY_RELEASE_BEHAVIOR

0x00035001 constant RELEASE_BEHAVIOR_FLUSH

 0x00035002 constant RELEASE_BEHAVIOR_NONE

    0x00036001 constant NATIVE_CONTEXT_API

       0x00036002 constant EGL_CONTEXT_API

    0x00036003 constant OSMESA_CONTEXT_API

   0x00038001 constant WAYLAND_PREFER_LIBDECOR

  0x00038002 constant WAYLAND_DISABLE_LIBDECOR

          0x00036001 constant ARROW_CURSOR

          0x00036002 constant IBEAM_CURSOR

      0x00036003 constant CROSSHAIR_CURSOR

           0x00036004 constant HAND_CURSOR

        0x00036005 constant HRESIZE_CURSOR

        0x00036006 constant VRESIZE_CURSOR

             0x00040001 constant CONNECTED

          0x00040002 constant DISCONNECTED

  0x00050001 constant JOYSTICK_HAT_BUTTONS

 0x00051001 constant COCOA_CHDIR_RESOURCES

         0x00051002 constant COCOA_MENUBAR

      0x00053001 constant WAYLAND_LIBDECOR

	// GLFWAPI int Init (void)
	 (  -> Z32 ) "libglfw.so.3" "glfwInit" sys optional Init 

	// GLFWAPI void Terminate (void)
	 ( ) "libglfw.so.3" "glfwTerminate" sys optional Terminate 

	// GLFWAPI void InitHint (int hint,int value)
	 ( Z32:_hint Z32:_value ) "libglfw.so.3" "glfwInitHint" sys optional InitHint 

	// GLFWAPI void GetVersion (int * major,int * minor,int * rev)
	 ( Z32&:_major Z32&:_minor Z32&:_rev ) "libglfw.so.3" "glfwGetVersion" sys optional GetVersion 

	// GLFWAPI const char * GetVersionString (void)
	 (  -> String ) "libglfw.so.3" "glfwGetVersionString" sys optional GetVersionString 

	// GLFWAPI int GetError (const char ** description)
	 ( String*:_description  -> Z32 ) "libglfw.so.3" "glfwGetError" sys optional GetError 

	// GLFWAPI GLFWerrorfun SetErrorCallback (GLFWerrorfun callback)
	 ( ( Z32:_error_code String:_description ):_callback  -> ( Z32:_error_code String:_description ) ) "libglfw.so.3" "glfwSetErrorCallback" sys optional SetErrorCallback 

	// GLFWAPI GLFWmonitor ** GetMonitors (int * count)
	 ( Z32&:_count  -> monitor** ) "libglfw.so.3" "glfwGetMonitors" sys optional GetMonitors 

	// GLFWAPI GLFWmonitor * GetPrimaryMonitor (void)
	 (  -> monitor* ) "libglfw.so.3" "glfwGetPrimaryMonitor" sys optional GetPrimaryMonitor 

	// GLFWAPI void GetMonitorPos (GLFWmonitor * monitor,int * xpos,int * ypos)
	 ( monitor*:_monitor Z32&:_xpos Z32&:_ypos ) "libglfw.so.3" "glfwGetMonitorPos" sys optional GetMonitorPos 

	// GLFWAPI void GetMonitorWorkarea (GLFWmonitor * monitor,int * xpos,int * ypos,int * width,int * height)
	 ( monitor*:_monitor Z32&:_xpos Z32&:_ypos Z32&:_width Z32&:_height ) "libglfw.so.3" "glfwGetMonitorWorkarea" sys optional GetMonitorWorkarea 

	// GLFWAPI void GetMonitorPhysicalSize (GLFWmonitor * monitor,int * widthMM,int * heightMM)
	 ( monitor*:_monitor Z32&:_widthMM Z32&:_heightMM ) "libglfw.so.3" "glfwGetMonitorPhysicalSize" sys optional GetMonitorPhysicalSize 

	// GLFWAPI void GetMonitorContentScale (GLFWmonitor * monitor,float * xscale,float * yscale)
	 ( monitor*:_monitor Real*:_xscale Real*:_yscale ) "libglfw.so.3" "glfwGetMonitorContentScale" sys optional GetMonitorContentScale 

	// GLFWAPI const char * GetMonitorName (GLFWmonitor * monitor)
	 ( monitor*:_monitor  -> String ) "libglfw.so.3" "glfwGetMonitorName" sys optional GetMonitorName 

	// GLFWAPI void SetMonitorUserPointer (GLFWmonitor * monitor,void * pointer)
	 ( monitor*:_monitor Any&:_pointer ) "libglfw.so.3" "glfwSetMonitorUserPointer" sys optional SetMonitorUserPointer 

	// GLFWAPI void * GetMonitorUserPointer (GLFWmonitor * monitor)
	 ( monitor*:_monitor  -> Any& ) "libglfw.so.3" "glfwGetMonitorUserPointer" sys optional GetMonitorUserPointer 

	// GLFWAPI GLFWmonitorfun SetMonitorCallback (GLFWmonitorfun callback)
	 ( ( monitor*:_monitor Z32:_event ):_callback  -> ( monitor*:_monitor Z32:_event ) ) "libglfw.so.3" "glfwSetMonitorCallback" sys optional SetMonitorCallback 

	// GLFWAPI const GLFWvidmode * GetVideoModes (GLFWmonitor * monitor,int * count)
	 ( monitor*:_monitor Z32&:_count  -> vidmode* ) "libglfw.so.3" "glfwGetVideoModes" sys optional GetVideoModes 

	// GLFWAPI const GLFWvidmode * GetVideoMode (GLFWmonitor * monitor)
	 ( monitor*:_monitor  -> vidmode* ) "libglfw.so.3" "glfwGetVideoMode" sys optional GetVideoMode 

	// GLFWAPI void SetGamma (GLFWmonitor * monitor,float gamma)
	 ( monitor*:_monitor Real:_gamma ) "libglfw.so.3" "glfwSetGamma" sys optional SetGamma 

	// GLFWAPI const GLFWgammaramp * GetGammaRamp (GLFWmonitor * monitor)
	 ( monitor*:_monitor  -> gammaramp* ) "libglfw.so.3" "glfwGetGammaRamp" sys optional GetGammaRamp 

	// GLFWAPI void SetGammaRamp (GLFWmonitor * monitor,const GLFWgammaramp * ramp)
	 ( monitor*:_monitor gammaramp*:_ramp ) "libglfw.so.3" "glfwSetGammaRamp" sys optional SetGammaRamp 

	// GLFWAPI void DefaultWindowHints (void)
	 ( ) "libglfw.so.3" "glfwDefaultWindowHints" sys optional DefaultWindowHints 

	// GLFWAPI void WindowHint (int hint,int value)
	 ( Z32:_hint Z32:_value ) "libglfw.so.3" "glfwWindowHint" sys optional WindowHint 

	// GLFWAPI void WindowHintString (int hint,const char * value)
	 ( Z32:_hint String:_value ) "libglfw.so.3" "glfwWindowHintString" sys optional WindowHintString 

	// GLFWAPI GLFWwindow * CreateWindow (int width,int height,const char * title,GLFWmonitor * monitor,GLFWwindow * share)
	 ( Z32:_width Z32:_height String:_title monitor*:_monitor window*:_share  -> window* ) "libglfw.so.3" "glfwCreateWindow" sys optional CreateWindow 

	// GLFWAPI void DestroyWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwDestroyWindow" sys optional DestroyWindow 

	// GLFWAPI int WindowShouldClose (GLFWwindow * window)
	 ( window*:_window  -> Z32 ) "libglfw.so.3" "glfwWindowShouldClose" sys optional WindowShouldClose 

	// GLFWAPI void SetWindowShouldClose (GLFWwindow * window,int value)
	 ( window*:_window Z32:_value ) "libglfw.so.3" "glfwSetWindowShouldClose" sys optional SetWindowShouldClose 

	// GLFWAPI void SetWindowTitle (GLFWwindow * window,const char * title)
	 ( window*:_window String:_title ) "libglfw.so.3" "glfwSetWindowTitle" sys optional SetWindowTitle 

	// GLFWAPI void SetWindowIcon (GLFWwindow * window,int count,const GLFWimage * images)
	 ( window*:_window Z32:_count image*:_images ) "libglfw.so.3" "glfwSetWindowIcon" sys optional SetWindowIcon 

	// GLFWAPI void GetWindowPos (GLFWwindow * window,int * xpos,int * ypos)
	 ( window*:_window Z32&:_xpos Z32&:_ypos ) "libglfw.so.3" "glfwGetWindowPos" sys optional GetWindowPos 

	// GLFWAPI void SetWindowPos (GLFWwindow * window,int xpos,int ypos)
	 ( window*:_window Z32:_xpos Z32:_ypos ) "libglfw.so.3" "glfwSetWindowPos" sys optional SetWindowPos 

	// GLFWAPI void GetWindowSize (GLFWwindow * window,int * width,int * height)
	 ( window*:_window Z32&:_width Z32&:_height ) "libglfw.so.3" "glfwGetWindowSize" sys optional GetWindowSize 

	// GLFWAPI void SetWindowSizeLimits (GLFWwindow * window,int minwidth,int minheight,int maxwidth,int maxheight)
	 ( window*:_window Z32:_minwidth Z32:_minheight Z32:_maxwidth Z32:_maxheight ) "libglfw.so.3" "glfwSetWindowSizeLimits" sys optional SetWindowSizeLimits 

	// GLFWAPI void SetWindowAspectRatio (GLFWwindow * window,int numer,int denom)
	 ( window*:_window Z32:_numer Z32:_denom ) "libglfw.so.3" "glfwSetWindowAspectRatio" sys optional SetWindowAspectRatio 

	// GLFWAPI void SetWindowSize (GLFWwindow * window,int width,int height)
	 ( window*:_window Z32:_width Z32:_height ) "libglfw.so.3" "glfwSetWindowSize" sys optional SetWindowSize 

	// GLFWAPI void GetFramebufferSize (GLFWwindow * window,int * width,int * height)
	 ( window*:_window Z32&:_width Z32&:_height ) "libglfw.so.3" "glfwGetFramebufferSize" sys optional GetFramebufferSize 

	// GLFWAPI void GetWindowFrameSize (GLFWwindow * window,int * left,int * top,int * right,int * bottom)
	 ( window*:_window Z32&:_left Z32&:_top Z32&:_right Z32&:_bottom ) "libglfw.so.3" "glfwGetWindowFrameSize" sys optional GetWindowFrameSize 

	// GLFWAPI void GetWindowContentScale (GLFWwindow * window,float * xscale,float * yscale)
	 ( window*:_window Real*:_xscale Real*:_yscale ) "libglfw.so.3" "glfwGetWindowContentScale" sys optional GetWindowContentScale 

	// GLFWAPI float GetWindowOpacity (GLFWwindow * window)
	 ( window*:_window  -> Real ) "libglfw.so.3" "glfwGetWindowOpacity" sys optional GetWindowOpacity 

	// GLFWAPI void SetWindowOpacity (GLFWwindow * window,float opacity)
	 ( window*:_window Real:_opacity ) "libglfw.so.3" "glfwSetWindowOpacity" sys optional SetWindowOpacity 

	// GLFWAPI void IconifyWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwIconifyWindow" sys optional IconifyWindow 

	// GLFWAPI void RestoreWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwRestoreWindow" sys optional RestoreWindow 

	// GLFWAPI void MaximizeWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwMaximizeWindow" sys optional MaximizeWindow 

	// GLFWAPI void ShowWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwShowWindow" sys optional ShowWindow 

	// GLFWAPI void HideWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwHideWindow" sys optional HideWindow 

	// GLFWAPI void FocusWindow (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwFocusWindow" sys optional FocusWindow 

	// GLFWAPI void RequestWindowAttention (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwRequestWindowAttention" sys optional RequestWindowAttention 

	// GLFWAPI GLFWmonitor * GetWindowMonitor (GLFWwindow * window)
	 ( window*:_window  -> monitor* ) "libglfw.so.3" "glfwGetWindowMonitor" sys optional GetWindowMonitor 

	// GLFWAPI void SetWindowMonitor (GLFWwindow * window,GLFWmonitor * monitor,int xpos,int ypos,int width,int height,int refreshRate)
	 ( window*:_window monitor*:_monitor Z32:_xpos Z32:_ypos Z32:_width Z32:_height Z32:_refreshRate ) "libglfw.so.3" "glfwSetWindowMonitor" sys optional SetWindowMonitor 

	// GLFWAPI int GetWindowAttrib (GLFWwindow * window,int attrib)
	 ( window*:_window Z32:_attrib  -> Z32 ) "libglfw.so.3" "glfwGetWindowAttrib" sys optional GetWindowAttrib 

	// GLFWAPI void SetWindowAttrib (GLFWwindow * window,int attrib,int value)
	 ( window*:_window Z32:_attrib Z32:_value ) "libglfw.so.3" "glfwSetWindowAttrib" sys optional SetWindowAttrib 

	// GLFWAPI void SetWindowUserPointer (GLFWwindow * window,void * pointer)
	 ( window*:_window Any&:_pointer ) "libglfw.so.3" "glfwSetWindowUserPointer" sys optional SetWindowUserPointer 

	// GLFWAPI void * GetWindowUserPointer (GLFWwindow * window)
	 ( window*:_window  -> Any& ) "libglfw.so.3" "glfwGetWindowUserPointer" sys optional GetWindowUserPointer 

	// GLFWAPI GLFWwindowposfun SetWindowPosCallback (GLFWwindow * window,GLFWwindowposfun callback)
	 ( window*:_window ( window*:_window Z32:_xpos Z32:_ypos ):_callback  -> ( window*:_window Z32:_xpos Z32:_ypos ) ) "libglfw.so.3" "glfwSetWindowPosCallback" sys optional SetWindowPosCallback 

	// GLFWAPI GLFWwindowsizefun SetWindowSizeCallback (GLFWwindow * window,GLFWwindowsizefun callback)
	 ( window*:_window ( window*:_window Z32:_width Z32:_height ):_callback  -> ( window*:_window Z32:_width Z32:_height ) ) "libglfw.so.3" "glfwSetWindowSizeCallback" sys optional SetWindowSizeCallback 

	// GLFWAPI GLFWwindowclosefun SetWindowCloseCallback (GLFWwindow * window,GLFWwindowclosefun callback)
	 ( window*:_window ( window*:_window ):_callback  -> ( window*:_window ) ) "libglfw.so.3" "glfwSetWindowCloseCallback" sys optional SetWindowCloseCallback 

	// GLFWAPI GLFWwindowrefreshfun SetWindowRefreshCallback (GLFWwindow * window,GLFWwindowrefreshfun callback)
	 ( window*:_window ( window*:_window ):_callback  -> ( window*:_window ) ) "libglfw.so.3" "glfwSetWindowRefreshCallback" sys optional SetWindowRefreshCallback 

	// GLFWAPI GLFWwindowfocusfun SetWindowFocusCallback (GLFWwindow * window,GLFWwindowfocusfun callback)
	 ( window*:_window ( window*:_window Z32:_focused ):_callback  -> ( window*:_window Z32:_focused ) ) "libglfw.so.3" "glfwSetWindowFocusCallback" sys optional SetWindowFocusCallback 

	// GLFWAPI GLFWwindowiconifyfun SetWindowIconifyCallback (GLFWwindow * window,GLFWwindowiconifyfun callback)
	 ( window*:_window ( window*:_window Z32:_iconified ):_callback  -> ( window*:_window Z32:_iconified ) ) "libglfw.so.3" "glfwSetWindowIconifyCallback" sys optional SetWindowIconifyCallback 

	// GLFWAPI GLFWwindowmaximizefun SetWindowMaximizeCallback (GLFWwindow * window,GLFWwindowmaximizefun callback)
	 ( window*:_window ( window*:_window Z32:_maximized ):_callback  -> ( window*:_window Z32:_maximized ) ) "libglfw.so.3" "glfwSetWindowMaximizeCallback" sys optional SetWindowMaximizeCallback 

	// GLFWAPI GLFWframebuffersizefun SetFramebufferSizeCallback (GLFWwindow * window,GLFWframebuffersizefun callback)
	 ( window*:_window ( window*:_window Z32:_width Z32:_height ):_callback  -> ( window*:_window Z32:_width Z32:_height ) ) "libglfw.so.3" "glfwSetFramebufferSizeCallback" sys optional SetFramebufferSizeCallback 

	// GLFWAPI GLFWwindowcontentscalefun SetWindowContentScaleCallback (GLFWwindow * window,GLFWwindowcontentscalefun callback)
	 ( window*:_window ( window*:_window Real:_xscale Real:_yscale ):_callback  -> ( window*:_window Real:_xscale Real:_yscale ) ) "libglfw.so.3" "glfwSetWindowContentScaleCallback" sys optional SetWindowContentScaleCallback 

	// GLFWAPI void PollEvents (void)
	 ( ) "libglfw.so.3" "glfwPollEvents" sys optional PollEvents 

	// GLFWAPI void WaitEvents (void)
	 ( ) "libglfw.so.3" "glfwWaitEvents" sys optional WaitEvents 

	// GLFWAPI void WaitEventsTimeout (double timeout)
	 ( Double32:_timeout ) "libglfw.so.3" "glfwWaitEventsTimeout" sys optional WaitEventsTimeout 

	// GLFWAPI void PostEmptyEvent (void)
	 ( ) "libglfw.so.3" "glfwPostEmptyEvent" sys optional PostEmptyEvent 

	// GLFWAPI int GetInputMode (GLFWwindow * window,int mode)
	 ( window*:_window Z32:_mode  -> Z32 ) "libglfw.so.3" "glfwGetInputMode" sys optional GetInputMode 

	// GLFWAPI void SetInputMode (GLFWwindow * window,int mode,int value)
	 ( window*:_window Z32:_mode Z32:_value ) "libglfw.so.3" "glfwSetInputMode" sys optional SetInputMode 

	// GLFWAPI int RawMouseMotionSupported (void)
	 (  -> Z32 ) "libglfw.so.3" "glfwRawMouseMotionSupported" sys optional RawMouseMotionSupported 

	// GLFWAPI const char * GetKeyName (int key,int scancode)
	 ( Z32:_key Z32:_scancode  -> String ) "libglfw.so.3" "glfwGetKeyName" sys optional GetKeyName 

	// GLFWAPI int GetKeyScancode (int key)
	 ( Z32:_key  -> Z32 ) "libglfw.so.3" "glfwGetKeyScancode" sys optional GetKeyScancode 

	// GLFWAPI int GetKey (GLFWwindow * window,int key)
	 ( window*:_window Z32:_key  -> Z32 ) "libglfw.so.3" "glfwGetKey" sys optional GetKey 

	// GLFWAPI int GetMouseButton (GLFWwindow * window,int button)
	 ( window*:_window Z32:_button  -> Z32 ) "libglfw.so.3" "glfwGetMouseButton" sys optional GetMouseButton 

	// GLFWAPI void GetCursorPos (GLFWwindow * window,double * xpos,double * ypos)
	 ( window*:_window Double32*:_xpos Double32*:_ypos ) "libglfw.so.3" "glfwGetCursorPos" sys optional GetCursorPos 

	// GLFWAPI void SetCursorPos (GLFWwindow * window,double xpos,double ypos)
	 ( window*:_window Double32:_xpos Double32:_ypos ) "libglfw.so.3" "glfwSetCursorPos" sys optional SetCursorPos 

	// GLFWAPI GLFWcursor * CreateCursor (const GLFWimage * image,int xhot,int yhot)
	 ( image*:_image Z32:_xhot Z32:_yhot  -> cursor* ) "libglfw.so.3" "glfwCreateCursor" sys optional CreateCursor 

	// GLFWAPI GLFWcursor * CreateStandardCursor (int shape)
	 ( Z32:_shape  -> cursor* ) "libglfw.so.3" "glfwCreateStandardCursor" sys optional CreateStandardCursor 

	// GLFWAPI void DestroyCursor (GLFWcursor * cursor)
	 ( cursor*:_cursor ) "libglfw.so.3" "glfwDestroyCursor" sys optional DestroyCursor 

	// GLFWAPI void SetCursor (GLFWwindow * window,GLFWcursor * cursor)
	 ( window*:_window cursor*:_cursor ) "libglfw.so.3" "glfwSetCursor" sys optional SetCursor 

	// GLFWAPI GLFWkeyfun SetKeyCallback (GLFWwindow * window,GLFWkeyfun callback)
	 ( window*:_window ( window*:_window Z32:_key Z32:_scancode Z32:_action Z32:_mods ):_callback  -> ( window*:_window Z32:_key Z32:_scancode Z32:_action Z32:_mods ) ) "libglfw.so.3" "glfwSetKeyCallback" sys optional SetKeyCallback 

	// GLFWAPI GLFWcharfun SetCharCallback (GLFWwindow * window,GLFWcharfun callback)
	 ( window*:_window ( window*:_window Z32:_codepoint ):_callback  -> ( window*:_window Z32:_codepoint ) ) "libglfw.so.3" "glfwSetCharCallback" sys optional SetCharCallback 

	// GLFWAPI GLFWcharmodsfun SetCharModsCallback (GLFWwindow * window,GLFWcharmodsfun callback)
	 ( window*:_window ( window*:_window Z32:_codepoint Z32:_mods ):_callback  -> ( window*:_window Z32:_codepoint Z32:_mods ) ) "libglfw.so.3" "glfwSetCharModsCallback" sys optional SetCharModsCallback 

	// GLFWAPI GLFWmousebuttonfun SetMouseButtonCallback (GLFWwindow * window,GLFWmousebuttonfun callback)
	 ( window*:_window ( window*:_window Z32:_button Z32:_action Z32:_mods ):_callback  -> ( window*:_window Z32:_button Z32:_action Z32:_mods ) ) "libglfw.so.3" "glfwSetMouseButtonCallback" sys optional SetMouseButtonCallback 

	// GLFWAPI GLFWcursorposfun SetCursorPosCallback (GLFWwindow * window,GLFWcursorposfun callback)
	 ( window*:_window ( window*:_window Double32:_xpos Double32:_ypos ):_callback  -> ( window*:_window Double32:_xpos Double32:_ypos ) ) "libglfw.so.3" "glfwSetCursorPosCallback" sys optional SetCursorPosCallback 

	// GLFWAPI GLFWcursorenterfun SetCursorEnterCallback (GLFWwindow * window,GLFWcursorenterfun callback)
	 ( window*:_window ( window*:_window Z32:_entered ):_callback  -> ( window*:_window Z32:_entered ) ) "libglfw.so.3" "glfwSetCursorEnterCallback" sys optional SetCursorEnterCallback 

	// GLFWAPI GLFWscrollfun SetScrollCallback (GLFWwindow * window,GLFWscrollfun callback)
	 ( window*:_window ( window*:_window Double32:_xoffset Double32:_yoffset ):_callback  -> ( window*:_window Double32:_xoffset Double32:_yoffset ) ) "libglfw.so.3" "glfwSetScrollCallback" sys optional SetScrollCallback 

	// GLFWAPI GLFWdropfun SetDropCallback (GLFWwindow * window,GLFWdropfun callback)
	 ( window*:_window ( window*:_window Z32:_path_count [String]:_paths ):_callback  -> ( window*:_window Z32:_path_count [String]:_paths ) ) "libglfw.so.3" "glfwSetDropCallback" sys optional SetDropCallback 

	// GLFWAPI int JoystickPresent (int jid)
	 ( Z32:_jid  -> Z32 ) "libglfw.so.3" "glfwJoystickPresent" sys optional JoystickPresent 

	// GLFWAPI const float * GetJoystickAxes (int jid,int * count)
	 ( Z32:_jid Z32&:_count  -> Real* ) "libglfw.so.3" "glfwGetJoystickAxes" sys optional GetJoystickAxes 

	// GLFWAPI const unsigned char * GetJoystickButtons (int jid,int * count)
	 ( Z32:_jid Z32&:_count  -> unsignedchar* ) "libglfw.so.3" "glfwGetJoystickButtons" sys optional GetJoystickButtons 

	// GLFWAPI const unsigned char * GetJoystickHats (int jid,int * count)
	 ( Z32:_jid Z32&:_count  -> unsignedchar* ) "libglfw.so.3" "glfwGetJoystickHats" sys optional GetJoystickHats 

	// GLFWAPI const char * GetJoystickName (int jid)
	 ( Z32:_jid  -> String ) "libglfw.so.3" "glfwGetJoystickName" sys optional GetJoystickName 

	// GLFWAPI const char * GetJoystickGUID (int jid)
	 ( Z32:_jid  -> String ) "libglfw.so.3" "glfwGetJoystickGUID" sys optional GetJoystickGUID 

	// GLFWAPI void SetJoystickUserPointer (int jid,void * pointer)
	 ( Z32:_jid Any&:_pointer ) "libglfw.so.3" "glfwSetJoystickUserPointer" sys optional SetJoystickUserPointer 

	// GLFWAPI void * GetJoystickUserPointer (int jid)
	 ( Z32:_jid  -> Any& ) "libglfw.so.3" "glfwGetJoystickUserPointer" sys optional GetJoystickUserPointer 

	// GLFWAPI int JoystickIsGamepad (int jid)
	 ( Z32:_jid  -> Z32 ) "libglfw.so.3" "glfwJoystickIsGamepad" sys optional JoystickIsGamepad 

	// GLFWAPI GLFWjoystickfun SetJoystickCallback (GLFWjoystickfun callback)
	 ( ( Z32:_jid Z32:_event ):_callback  -> ( Z32:_jid Z32:_event ) ) "libglfw.so.3" "glfwSetJoystickCallback" sys optional SetJoystickCallback 

	// GLFWAPI int UpdateGamepadMappings (const char * string)
	 ( String:_string  -> Z32 ) "libglfw.so.3" "glfwUpdateGamepadMappings" sys optional UpdateGamepadMappings 

	// GLFWAPI const char * GetGamepadName (int jid)
	 ( Z32:_jid  -> String ) "libglfw.so.3" "glfwGetGamepadName" sys optional GetGamepadName 

	// GLFWAPI int GetGamepadState (int jid,GLFWgamepadstate * state)
	 ( Z32:_jid gamepadstate*:_state  -> Z32 ) "libglfw.so.3" "glfwGetGamepadState" sys optional GetGamepadState 

	// GLFWAPI void SetClipboardString (GLFWwindow * window,const char * string)
	 ( window*:_window String:_string ) "libglfw.so.3" "glfwSetClipboardString" sys optional SetClipboardString 

	// GLFWAPI const char * GetClipboardString (GLFWwindow * window)
	 ( window*:_window  -> String ) "libglfw.so.3" "glfwGetClipboardString" sys optional GetClipboardString 

	// GLFWAPI double GetTime (void)
	 (  -> Double32 ) "libglfw.so.3" "glfwGetTime" sys optional GetTime 

	// GLFWAPI void SetTime (double time)
	 ( Double32:_time ) "libglfw.so.3" "glfwSetTime" sys optional SetTime 

	// GLFWAPI uint64_t GetTimerValue (void)
	 (  -> Size32 ) "libglfw.so.3" "glfwGetTimerValue" sys optional GetTimerValue 

	// GLFWAPI uint64_t GetTimerFrequency (void)
	 (  -> Size32 ) "libglfw.so.3" "glfwGetTimerFrequency" sys optional GetTimerFrequency 

	// GLFWAPI void MakeContextCurrent (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwMakeContextCurrent" sys optional MakeContextCurrent 

	// GLFWAPI GLFWwindow * GetCurrentContext (void)
	 (  -> window* ) "libglfw.so.3" "glfwGetCurrentContext" sys optional GetCurrentContext 

	// GLFWAPI void SwapBuffers (GLFWwindow * window)
	 ( window*:_window ) "libglfw.so.3" "glfwSwapBuffers" sys optional SwapBuffers 

	// GLFWAPI void SwapInterval (int interval)
	 ( Z32:_interval ) "libglfw.so.3" "glfwSwapInterval" sys optional SwapInterval 

	// GLFWAPI int ExtensionSupported (const char * extension)
	 ( String:_extension  -> Z32 ) "libglfw.so.3" "glfwExtensionSupported" sys optional ExtensionSupported 

	// GLFWAPI GLFWglproc GetProcAddress (const char * procname)
	 ( String:_procname  -> Any& ) "libglfw.so.3" "glfwGetProcAddress" sys optional GetProcAddress 

	// GLFWAPI int VulkanSupported (void)
	 (  -> Z32 ) "libglfw.so.3" "glfwVulkanSupported" sys optional VulkanSupported 

	// GLFWAPI const char ** GetRequiredInstanceExtensions (uint32_t * count)
	 ( Z32*:_count  -> String* ) "libglfw.so.3" "glfwGetRequiredInstanceExtensions" sys optional GetRequiredInstanceExtensions 

	// GLFWAPI GLFWvkproc GetInstanceProcAddress (VkInstance instance,const char * procname)
	 ( Any&:_instance String:_procname  -> Any& ) "libglfw.so.3" "glfwGetInstanceProcAddress" sys optional GetInstanceProcAddress 

	// GLFWAPI int GetPhysicalDevicePresentationSupport (VkInstance instance,VkPhysicalDevice device,uint32_t queuefamily)
	 ( Any&:_instance Any&:_device Z32:_queuefamily  -> Z32 ) "libglfw.so.3" "glfwGetPhysicalDevicePresentationSupport" sys optional GetPhysicalDevicePresentationSupport 

	// GLFWAPI VkResult CreateWindowSurface (VkInstance instance,GLFWwindow * window,const VkAllocationCallbacks * allocator,VkSurfaceKHR * surface)
	 ( Any&:_instance window*:_window VkAllocationCallbacks*:_allocator VkSurfaceKHR*:_surface  -> Any& ) "libglfw.so.3" "glfwCreateWindowSurface" sys optional CreateWindowSurface 

end


section x

	type window end
	type monitor end
	// void errorfun (int error_code,const char * description)
	 ( Z32:_error_code String:_description ) "x" "GLFWerrorfun" sys optional errorfun 

	// void windowposfun (GLFWwindow * window,int xpos,int ypos)
	 ( window*:_window Z32:_xpos Z32:_ypos ) "x" "GLFWwindowposfun" sys optional windowposfun 

	// void windowsizefun (GLFWwindow * window,int width,int height)
	 ( window*:_window Z32:_width Z32:_height ) "x" "GLFWwindowsizefun" sys optional windowsizefun 

	// void windowclosefun (GLFWwindow * window)
	 ( window*:_window ) "x" "GLFWwindowclosefun" sys optional windowclosefun 

	// void windowrefreshfun (GLFWwindow * window)
	 ( window*:_window ) "x" "GLFWwindowrefreshfun" sys optional windowrefreshfun 

	// void windowfocusfun (GLFWwindow * window,int focused)
	 ( window*:_window Z32:_focused ) "x" "GLFWwindowfocusfun" sys optional windowfocusfun 

	// void windowiconifyfun (GLFWwindow * window,int iconified)
	 ( window*:_window Z32:_iconified ) "x" "GLFWwindowiconifyfun" sys optional windowiconifyfun 

	// void windowmaximizefun (GLFWwindow * window,int maximized)
	 ( window*:_window Z32:_maximized ) "x" "GLFWwindowmaximizefun" sys optional windowmaximizefun 

	// void framebuffersizefun (GLFWwindow * window,int width,int height)
	 ( window*:_window Z32:_width Z32:_height ) "x" "GLFWframebuffersizefun" sys optional framebuffersizefun 

	// void windowcontentscalefun (GLFWwindow * window,float xscale,float yscale)
	 ( window*:_window Real:_xscale Real:_yscale ) "x" "GLFWwindowcontentscalefun" sys optional windowcontentscalefun 

	// void mousebuttonfun (GLFWwindow * window,int button,int action,int mods)
	 ( window*:_window Z32:_button Z32:_action Z32:_mods ) "x" "GLFWmousebuttonfun" sys optional mousebuttonfun 

	// void cursorposfun (GLFWwindow * window,double xpos,double ypos)
	 ( window*:_window Double32:_xpos Double32:_ypos ) "x" "GLFWcursorposfun" sys optional cursorposfun 

	// void cursorenterfun (GLFWwindow * window,int entered)
	 ( window*:_window Z32:_entered ) "x" "GLFWcursorenterfun" sys optional cursorenterfun 

	// void scrollfun (GLFWwindow * window,double xoffset,double yoffset)
	 ( window*:_window Double32:_xoffset Double32:_yoffset ) "x" "GLFWscrollfun" sys optional scrollfun 

	// void keyfun (GLFWwindow * window,int key,int scancode,int action,int mods)
	 ( window*:_window Z32:_key Z32:_scancode Z32:_action Z32:_mods ) "x" "GLFWkeyfun" sys optional keyfun 

	// void charfun (GLFWwindow * window,unsigned int codepoint)
	 ( window*:_window Z32:_codepoint ) "x" "GLFWcharfun" sys optional charfun 

	// void charmodsfun (GLFWwindow * window,unsigned int codepoint,int mods)
	 ( window*:_window Z32:_codepoint Z32:_mods ) "x" "GLFWcharmodsfun" sys optional charmodsfun 

	// void dropfun (GLFWwindow * window,int path_count,const char * paths[])
	 ( window*:_window Z32:_path_count [String]:_paths ) "x" "GLFWdropfun" sys optional dropfun 

	// void monitorfun (GLFWmonitor * monitor,int event)
	 ( monitor*:_monitor Z32:_event ) "x" "GLFWmonitorfun" sys optional monitorfun 

	// void joystickfun (int jid,int event)
	 ( Z32:_jid Z32:_event ) "x" "GLFWjoystickfun" sys optional joystickfun 

end


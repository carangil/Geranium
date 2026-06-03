//This file contains the glfw callback typedefs, extracted as functions by hand.

void  GLFWerrorfun(int error_code, const char* description);
void  GLFWwindowposfun(GLFWwindow* window, int xpos, int ypos);
void  GLFWwindowsizefun(GLFWwindow* window, int width, int height);
void  GLFWwindowclosefun(GLFWwindow* window);
void  GLFWwindowrefreshfun(GLFWwindow* window);
void  GLFWwindowfocusfun(GLFWwindow* window, int focused);
void  GLFWwindowiconifyfun(GLFWwindow* window, int iconified);
void  GLFWwindowmaximizefun(GLFWwindow* window, int maximized);
void  GLFWframebuffersizefun(GLFWwindow* window, int width, int height);
void  GLFWwindowcontentscalefun(GLFWwindow* window, float xscale, float yscale);
void  GLFWmousebuttonfun(GLFWwindow* window, int button, int action, int mods);
void  GLFWcursorposfun(GLFWwindow* window, double xpos, double ypos);
void  GLFWcursorenterfun(GLFWwindow* window, int entered);
void  GLFWscrollfun(GLFWwindow* window, double xoffset, double yoffset);
void  GLFWkeyfun(GLFWwindow* window, int key, int scancode, int action, int mods);
void  GLFWcharfun(GLFWwindow* window, unsigned int codepoint);
void  GLFWcharmodsfun(GLFWwindow* window, unsigned int codepoint, int mods);
void  GLFWdropfun(GLFWwindow* window, int path_count, const char* paths[]);
void  GLFWmonitorfun(GLFWmonitor* monitor, int event);
void  GLFWjoystickfun(int jid, int event);

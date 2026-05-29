()  proc newline
    10 printchar
end

//string

(String:s String:t -> Z32) "_INTERNAL"  "zstrcmp" sys strcmp

(String:s Size32:count -> String$) "_INTERNAL"  "zstrndup" sys left

(String$:dest String:src Size32:count Size32:count -> String$) "_INTERNAL" "zstrcatsub" sys midcat

(String$:dest String:src -> String$) proc +
    dest$$ src 0 -1 midcat
end

(String:a String:b -> Bit) proc ==

    a b strcmp 0 == return

end



//print for String already defined, but do it for String$ (possessive pointer):
(String$:s) proc print
    s print
end




/*
//(Z32:mode ) "libGL.so.1" "glCullFace" sys glCullFace




//glfw
type GLFWwindow end
(->Z32) "libglfw.so.3" "glfwInit" sys glfwInit
(Z32:w Z32:h String:title void*:v1 void*:v2 ->GLFWwindow*) "libglfw.so.3" "glfwCreateWindow" sys glfwCreateWindow
() "libglfw.so.3" "glfwPollEvents" sys glfwPollEvents
(GLFWwindow*) "libglfw.so.3" "glfwSwapBuffers" sys glfwSwapBuffers
(GLFWwindow*) "libglfw.so.3" "glfwMakeContextCurrent" sys glfwMakeContextCurrent
(GLFWwindow*:w (GLFWwindow*:w Z32:key Z32:scancode Z32:action Z32:mods):keycallback) "libglfw.so.3" "glfwSetKeyCallback" sys glfwSetKeyCallback



//gl
(Z32) "libGL.so.1" "glClear" sys glClear
(Real Real Real Real) "libGL.so.1" "glClearColor" sys glClearColor
(Real Real Real Real) "libGL.so.1" "glColor4f" sys glColor4f
(Real Real Real) "libGL.so.1" "glVertex3f" sys glVertex3f
(Z32) "libGL.so.1" "glBegin" sys glBegin
() "libGL.so.1" "glEnd" sys glEnd

*/


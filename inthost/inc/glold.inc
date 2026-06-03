section glold

	// void Begin (int prim)
	 ( Z32:_prim ) "libGL.so.1" "glBegin" sys optional Begin 

	// void End ()
	 ( ) "libGL.so.1" "glEnd" sys optional End 

	// void Vertex3f (float,float,float)
	 ( Real:___anon11fb5330010d Real:___anon11fb5330020d Real:___anon11fb5330030d ) "libGL.so.1" "glVertex3f" sys optional Vertex3f 

	// void Color4f (float,float,float,float)
	 ( Real:___anon11fb5330040d Real:___anon11fb5330050d Real:___anon11fb5330060d Real:___anon11fb5330070d ) "libGL.so.1" "glColor4f" sys optional Color4f 

end


// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


void gx_camera_pos(float x, float y, float z);
void gx_camera_pos_rot(vec3* position, vec3* xaxis, vec3* yaxis, vec3* zaxis);

//rotates 3 vectors around each other
void gx_spin(zfloat32 yaw, zfloat32 pitch, zfloat32 roll, vec3* right, vec3* up, vec3* forward);

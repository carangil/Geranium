#pragma once


//transfer our internal matrix to opengl fixed function pipeline
void ff_update_matrix(zfloat32* proj_matrix, zfloat32* matr);

void ff_texture_env(zuint32 i);


void ff_new_light_set();

zbool ff_light_parm(gfx_propertyT* p);


void ff_light_complete(vec4* ambientsum);
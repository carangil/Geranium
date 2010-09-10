// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../memory/ram.h"
#include <stdio.h>
#include "gx_image.h"
#include "gx_buffers.h"


#include "glstuff.h"



#define GX_UPDATE_FREQ  GL_STATIC_DRAW


static void _destruct_vbuffer(void* x)
{
	gx_vbuffer_t* v = x;
	zuint32 i;

	if (v->_sent_to_gl)
	{
		//todo: Free any buffers still in opengl
	}

	ram_free(v->color_data);
	ram_free(v->index_data);
	ram_free(v->vertex_data);

	for (i=0;i<v->num_textures;i++)
	{
		ram_free(v->texcoord_data);
	}

	ram_shallow_free(v);
}

#define VERTEX_COMPONENTS 3
#define COLOR_COMPONENTS 4
#define TEXTURE_COMPONENTS 2

gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, 
							zuint32 num_indices, 
							zbool use_color, 
							zuint32 texture_buffer_count)
{

	gx_vbuffer_t* v = NULL;

	zuint32 i = 0;

	if (num_vertices ==0)
		return NULL;

	v = ram_alloc(sizeof(gx_vbuffer_t), _destruct_vbuffer);

	if (!v) 
		return NULL;

	if (texture_buffer_count > GX_MAX_TEXTURES)
		return NULL;  //cannot offer that many textures (sorry!)

	v->vertex_capacity = num_vertices;

	v->vertex_data = ram_alloc(sizeof(zfloat32) * VERTEX_COMPONENTS * num_vertices, NULL);
	if (!v->vertex_data)
	{
		ram_free(v);
		return NULL;
	}

	if (num_indices)
	{
		v->index_data = ram_alloc(sizeof(zuint32) * num_indices, NULL);
		if (!v->index_data)
		{
			ram_free(v);
			return NULL;
		}
		v->index_capacity = num_indices;
	}

	if (use_color)  //if using color buffer, define it
	{
		v->color_data = ram_alloc(sizeof(zfloat32) * COLOR_COMPONENTS * num_vertices, NULL);
		if (!v->color_data)
		{
			ram_free(v);
			return NULL;
		}
	}

	//allocate texture coordinate buffers
	for (i=0;i<texture_buffer_count;i++)
	{
		v->texcoord_data[i] = ram_alloc(sizeof(zfloat32) * TEXTURE_COMPONENTS * num_vertices, NULL);
		if (!v->texcoord_data[i])
		{
			ram_free(v);
			return NULL;
		}
	}
	v->num_textures = texture_buffer_count;

	return v;
}


//sends a vbuffer to the graphics card
zbool gx_vbuffer_update(gx_vbuffer_t* v)
{
	zuint32 i = 0;

	//TODO: specify flags for what type of data to update!

	if (!v)
		return zfalse;

	if (!v->_sent_to_gl)
	{
		glGenBuffers(1, &(v->_vertex_vbo));

		if (v->color_data)
		{
			glGenBuffers(1, &(v->_color_vbo));
		}

		if (v->index_data)
		{
			glGenBuffers(1, &(v->_index_vbo));

		}
	
		if (v->num_textures)
		{
			glGenBuffers(v->num_textures, v->_texcoord_vbo);  //Generate VBO for each texture coordinate
		}

		v->_sent_to_gl = 1;
	}
	

	glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_vbo );
	glBufferData(GL_ARRAY_BUFFER, v->vertex_count * VERTEX_COMPONENTS *sizeof(zfloat32) , v->vertex_data, GX_UPDATE_FREQ);


	if (v->color_data)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_color_vbo );
		glBufferData(GL_ARRAY_BUFFER, v->vertex_count * COLOR_COMPONENTS *sizeof(zfloat32) , v->color_data, GX_UPDATE_FREQ);
	}

	//copy data for all the texture buffers
	for (i=0;i<v->num_textures;i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_texcoord_vbo[i] );
		glBufferData(GL_ARRAY_BUFFER, v->vertex_count * TEXTURE_COMPONENTS *sizeof(zfloat32) , v->texcoord_data[i], GX_UPDATE_FREQ);
	}

	 if (v->index_data)
	 {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v->_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, v->index_count * sizeof(v->index_data[0]  ) , v->index_data , GX_UPDATE_FREQ);
	 }

	return ztrue;
}






//adds color to a new vertex
void gx_vbuffer_add_color(gx_vbuffer_t* v, zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a)
{

	if (!v)
		return;

	if (!v->color_data)
		return;

	if (v->vertex_count == v->vertex_capacity)
		return; //we are full!

	v->color_data[COLOR_COMPONENTS * v->vertex_count] = r;
	v->color_data[COLOR_COMPONENTS * v->vertex_count + 1] = g;
	v->color_data[COLOR_COMPONENTS * v->vertex_count + 2] = b;
#if COLOR_COMPONENTS == 4
	v->color_data[COLOR_COMPONENTS * v->vertex_count + 3] = a;
#endif
}


//adds texture coordinate to a new vertex
void gx_vbuffer_add_tex(gx_vbuffer_t* v, zuint32 texture, zfloat32 s, zfloat32 t)
{

	if (!v)
		return;
	
	if (texture >= v->num_textures)
		return;

	if (!v->texcoord_data[texture])
		return;


	if (v->vertex_count == v->vertex_capacity)
		return; //we are full!

	v->texcoord_data[texture][TEXTURE_COMPONENTS * v->vertex_count] = s;
	v->texcoord_data[texture][TEXTURE_COMPONENTS * v->vertex_count + 1] = t;

}



//finalizes the current vertex, and returns a vertex index for it
zint32 gx_vbuffer_add_vertex(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z)
{
	if (!v)
		return -1;

	if (v->vertex_count == v->vertex_capacity)
		return -1; //we are full!

	v->vertex_data[VERTEX_COMPONENTS * v->vertex_count] = x;
	v->vertex_data[VERTEX_COMPONENTS * v->vertex_count + 1] = y;
	v->vertex_data[VERTEX_COMPONENTS * v->vertex_count + 2] = z;

	v->vertex_count ++;

	return v->vertex_count - 1;
}



void gx_vbuffer_add_index(gx_vbuffer_t* v, zuint32 i)
{
	if (!v)
		return;

	if (  v == GX_INDEX_INVALID  )
		return;

	if (v->index_count == v->index_capacity)
		return ; //we are full!

	if (!v->index_data)
		return;

	v->index_data[  (v->index_count) ++ ] = i;
	
	
}


//drawing a vbuffer
void gx_vbuffer_draw(gx_vbuffer_t* v, zuint32 start, zuint32 stop, gx_prim_e prim  , zbool indexed)
{
	zuint32 i;

	if (!v)
		return;
	

	if (stop <=start)
		return;


	if (v->_vertex_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_vbo );
		glVertexPointer(VERTEX_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);	
	}

	if (v->_color_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_color_vbo);
		glColorPointer(COLOR_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
		glColor4f(1,1,1,1);  //use white
	}

	if (v->_index_vbo)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  v->_index_vbo);
	}

	
	for (i=0;i<v->num_textures;i++)
	{
		glClientActiveTexture(GL_TEXTURE0+i);
		glBindBuffer(GL_ARRAY_BUFFER,  v->_texcoord_vbo[i]);
		glTexCoordPointer(TEXTURE_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}



	switch(prim)
	{
	case gx_points:
		if (indexed)
			glDrawElements(GL_POINTS, stop-start, GL_UNSIGNED_INT, sizeof(zuint32) * start  );
		else 
			glDrawArrays(GL_POINTS, start, stop-start);
		break;

	case gx_lines:
		if (indexed)
			glDrawElements(GL_LINES, stop-start, GL_UNSIGNED_INT,sizeof(zuint32) * start);
		else 
			glDrawArrays(GL_LINES, start, stop-start);
		break;

	case gx_triangles:
		if (indexed)
			glDrawElements(GL_TRIANGLES, stop-start, GL_UNSIGNED_INT,sizeof(zuint32) * start);
		else 
			glDrawArrays(GL_TRIANGLES, start, stop-start);
		break;
	}
	
}



//test function to dump a set of vertices onto the screen
void gx_test_draw_vertices(gx_vbuffer_t* v)
{

	if (!v)
		return;

	if (v->_vertex_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_vbo );
		glVertexPointer(VERTEX_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_VERTEX_ARRAY);	
	}

	if (v->_color_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_color_vbo);
		glColorPointer(COLOR_COMPONENTS, GL_FLOAT, 0, 0);
		glEnableClientState(GL_COLOR_ARRAY);
	}
	else
	{
		glColor4f(1,1,1,1);  //use white
	}

	glDrawArrays(GL_POINTS,0, v->vertex_count);
}


//lame way to create geometry: axis-aligned image
//this type of geometry-related crap should be shoved into a separate file
#if 1
gx_vbuffer_t* gx_mesh_from_image(gx_image_t* image, 
								zfloat32 xoff,
								zfloat32 yoff,
								zfloat32 zoff,
								zbyte xaxis,
								zbyte yaxis,
 								zbyte zaxis,
								zfloat32 xsize, 
								zfloat32 ysize, 
								zfloat32 zsize, 
								zbool use_color,
								zuint32 num_texture)
{
	zuint32 i,j,t;
	float coord[3];
	
	zuint32 numpoints = image->width * image->height;
	zuint32 numtriangles = (image->width-1) * (image->height -1)  *2 ;
	
	gx_vbuffer_t* vbuf = gx_vbuffer_mk(numpoints,numtriangles*3, 
		use_color, 
		num_texture );

	zuint32* lastcol = ram_alloc( sizeof(zuint32) * image->height, NULL);

	if (xaxis>2 || yaxis>2 || zaxis>2)
	{
		//use defaults if user was stupid
		xaxis=1;
		yaxis=2;
		zaxis=3;
	}
	
	
	//create vertices for each point
	for (i=0;i<image->width;i++)
	{
		for (j=0;j<image->height;j++)
		{
	

			zuint32 nv = -1;

			zfloat32 hf =  image->data[j*image->height +i]/255.0;
			if(use_color)
			{
				gx_vbuffer_add_color(vbuf, hf,hf,hf,1);
			}

			for (t=0; t< num_texture;t++)
			{
				//give all texture layers the same coordinates
				gx_vbuffer_add_tex(vbuf, t,  ((i) / (float)(image->width -1)), ((j) / (float)(image->width -1)));
			}

			coord[0]= xoff +  (i*xsize) / (image->width-1);
			coord[1]= yoff +   ysize * hf;
			coord[2]= zoff +  (j*zsize) / (image->height-1);


			nv = gx_vbuffer_add_vertex(vbuf, coord[xaxis], coord[yaxis], coord [zaxis]  );


			if (i>0)
			{
				if (j+1<image->height)
				{
					gx_vbuffer_add_index(vbuf, nv);
					gx_vbuffer_add_index(vbuf, lastcol[j]);
					gx_vbuffer_add_index(vbuf, lastcol[j+1]);
				}

				if(j>0)
				{

					gx_vbuffer_add_index(vbuf, nv);
					
					gx_vbuffer_add_index(vbuf, lastcol[j-1]);
					gx_vbuffer_add_index(vbuf, lastcol[j]);

				}
			}

			lastcol[j]=nv; //store this vertex in the 'last col' table.


		}
	}


	return vbuf;


}
#endif
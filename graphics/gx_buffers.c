// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#include "../ztypes.h"
#include "../vmath/zmath.h"
#include "../memory/zmem.h"
#include "../structures/zvector.h"
#include <stdio.h>
#include "gx_image.h"
#include "gx_buffers.h"
#include "gx_drawstyle.h"
#include "gx_trans.h"
#include "gx_sys.h"

#include "glheaders.h"


extern  int _gx_no_vbos;

#define GX_UPDATE_FREQ  GL_STATIC_DRAW


static zbool _destruct_vbuffer(void* x)
{
	gx_vbuffer_t* v = x;
	

	if ( !_gx_no_vbos && v->_sent_to_gl)
	{
		//todo: Free any buffers still in opengl	
		glDeleteBuffers(1,  &(v->_vertex_combined_vbo));
		_gx_gl_vbos_del++;

		if (v->index_data)
		{
			glDeleteBuffers(1, &(v->_index_vbo));
			_gx_gl_vbos_del++;
		}
	}


	//free combined data
	ram_free(v->combined_vertex_data);

	//free index data
	ram_free(v->index_data);

	return ZTRUE;
}


#define ATTR_TODISABLE	2
#define ATTR_ENABLED		1
#define ATTR_OFF 0




static int *gxi_attr_buffer = NULL;
static int gxi_num_attr = 0;

void init_attrib_buffer(){
	gxi_num_attr  = 0;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &gxi_num_attr);
	
	if (gxi_num_attr <=0 || gxi_num_attr > 65536) {
			printf("Something weird is happening %s:%d\n", __FILE__,__LINE__);
			exit(1);
	}
	
	gxi_attr_buffer = ram_alloc(sizeof(int) * gxi_num_attr, NULL);
	
	if (!gxi_attr_buffer ) {
			printf(" Can't allocate space for %d attributes!\n", gxi_num_attr);
			exit(0);
	}
	
	
}

gx_vbuffer_t* gx_vbuffer_mk(zuint32 num_vertices, 
                            zuint32 num_indices, 
                            zuint32 options)
{

	gx_vbuffer_t* v = NULL;
	size_t size_per_vertex = 0;
	zfloat32* vp = 0;

       
    
	zuint32 i = 0;
	
	if (gxi_attr_buffer== NULL) {
			init_attrib_buffer();
	}
	

	printf(" VERTEX_USE_COMPONENTS: %d, VERTEX_COMPONENTS: %d\n", VERTEX_USE_COMPONENTS, VERTEX_COMPONENTS);

	if (num_vertices ==0)
		return NULL;

	v = ram_alloc(sizeof(gx_vbuffer_t), _destruct_vbuffer);

	if (!v) 
		return NULL;

//	if (texture_buffer_count > GX_MAX_TEXTURES)
//		return NULL;  //cannot offer that many textures (sorry!)

	//allocated indices

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


	//calculate how much space needed per vertex in the VBO
	v->vertex_capacity = num_vertices;

	size_per_vertex = VERTEX_COMPONENTS;
    
	if (options & GX_VBUFFER_COLOR)
		size_per_vertex += COLOR_COMPONENTS;
	

	if (options & GX_VBUFFER_NORMAL)
		size_per_vertex += VERTEX_COMPONENTS; 
	

	if (options & GX_VBUFFER_TEXCOORD)
	        size_per_vertex += TEXTURE_COMPONENTS;

    
	//allocate one big buffer for all the vertex data

	v->size_per_vertex = size_per_vertex;

	vp = ram_alloc(sizeof(zfloat32) * size_per_vertex * num_vertices, NULL);

	if (!vp)
	{
		ram_free(v);
		return NULL;
	}

	v->combined_vertex_data = vp; //we have 1 combined buffer

	//lets break out into pieces
	
	v->pos_data = vp; //position data
	vp += (VERTEX_COMPONENTS * num_vertices);


	if (options & GX_VBUFFER_COLOR)  //if using color buffer, define it
	{
		v->color_data = vp;
		vp += (COLOR_COMPONENTS * num_vertices);
	}

	if (options & GX_VBUFFER_NORMAL)  //if using normal buffer, define it
	{
		v->normal_data = vp;
		vp += (VERTEX_COMPONENTS * num_vertices);
	}

	if (options & GX_VBUFFER_TEXCOORD) {
        v->texcoord_data[0] = vp;    
        v->num_texcoord = 1;
        vp += (TEXTURE_COMPONENTS * num_vertices);
    }

	return v;
}




zint32 gx_vbuffer_current_index(gx_vbuffer_t* v)
{

	if (v)
		return v->index_count;
	else
		return 0;
}



zint32 gx_vbuffer_current_vertex(gx_vbuffer_t* v)
{

	if (v)
		return v->vertex_count;
	else
		return 0;
}




zbool gx_vbuffer_update(gx_vbuffer_t* v)
{
	zuint32 i = 0;

	//TODO: specify flags for what type of data to update!

	if (!v)
		return ZFALSE;

	if (_gx_no_vbos)
	{
		return ZTRUE; //if using vertex arrays, we don't need to send anything
	}

	if (!v->_sent_to_gl)
	{
		glGenBuffers(1, &(v->_vertex_combined_vbo));
		_gx_gl_vbos_gen++;
 

		if (v->index_data)
		{
			glGenBuffers(1, &(v->_index_vbo));
			_gx_gl_vbos_gen++;

		}

		v->_sent_to_gl = 1;
	}


	glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_combined_vbo );
	glBufferData(GL_ARRAY_BUFFER, v->size_per_vertex * sizeof(zfloat32) * v->vertex_capacity , v->combined_vertex_data, GX_UPDATE_FREQ);
	
	
	 if (v->index_data)
	 {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v->_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, v->index_count * sizeof(v->index_data[0]  ) , v->index_data , GX_UPDATE_FREQ);	
	 }

	return ZTRUE;
}


//only resends the index buffer to opengl
zbool gx_vbuffer_update_indices(gx_vbuffer_t* v)
{
	zuint32 i = 0;

	//TODO: specify flags for what type of data to update!

	if (!v)
		return ZFALSE;

	if (!v->_sent_to_gl)
	{
		return ZFALSE;
	}
	
	 if (v->index_data)
	 {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v->_index_vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, v->index_count * sizeof(v->index_data[0]  ) , v->index_data , GX_UPDATE_FREQ);
	 }

	return ZTRUE;
}






//adds normal to a new vertex
void gx_vbuffer_normal3(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z)
{

	if (!v)
		return;

	if (!v->normal_data)
		return;

	if (v->vertex_count == v->vertex_capacity)
		return; //we are full!

	v->normal_data[VERTEX_COMPONENTS * v->vertex_count] = x;
	v->normal_data[VERTEX_COMPONENTS * v->vertex_count + 1] = y;
	v->normal_data[VERTEX_COMPONENTS * v->vertex_count + 2] = z;

	if (VERTEX_USE_COMPONENTS==4)
			v->normal_data[VERTEX_COMPONENTS * v->vertex_count + 3] = 0;


}

//adds color to a new vertex
void gx_vbuffer_color4(gx_vbuffer_t* v, zfloat32 r, zfloat32 g, zfloat32 b, zfloat32 a)
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
void gx_vbuffer_tex2(gx_vbuffer_t* v, zuint32 texture, zfloat32 s, zfloat32 t)
{


	if (!v)
		return;
	
	if (texture >= v->num_texcoord)
		return;

	if (!v->texcoord_data[texture])
		return;


	if (v->vertex_count == v->vertex_capacity)
		return; //we are full!


	if (texture >= v->num_texcoord)
		return; //too many textures
	
	v->texcoord_data[texture][TEXTURE_COMPONENTS * v->vertex_count] = s;
	v->texcoord_data[texture][TEXTURE_COMPONENTS * v->vertex_count + 1] = t;

}



//finalizes the current vertex, and returns a vertex index for it
zint32 gx_vbuffer_vertex3(gx_vbuffer_t* v, zfloat32 x, zfloat32 y, zfloat32 z)
{
	if (!v)
		return -1;

	if (v->vertex_count == v->vertex_capacity)
		return -1; //we are full!

	v->pos_data[VERTEX_COMPONENTS * v->vertex_count] = x;
	v->pos_data[VERTEX_COMPONENTS * v->vertex_count + 1] = y;
	v->pos_data[VERTEX_COMPONENTS * v->vertex_count + 2] = z;

	if (VERTEX_USE_COMPONENTS==4)
			v->pos_data[VERTEX_COMPONENTS * v->vertex_count + 3] = 1;


	v->vertex_count ++;

	return v->vertex_count - 1;
}



zint32 gx_vbuffer_import_vertex(gx_vbuffer_t* dest, gx_vbuffer_t* src, zint32 vertex)
{


	if (dest == src)
		return vertex;  //if want in same vbuffer, keep it

	//otherwise clone to other vbuffer


	// TODO: also need to copy normals, colors, and other texcoords
	
	gx_vbuffer_tex2(dest, 0, _gx_vbuffer_s(src, vertex, 0), _gx_vbuffer_t(src, vertex, 0));
	return gx_vbuffer_vertex( dest,*_gx_vbuffer_v(src, vertex));

}


void gx_vbuffer_index(gx_vbuffer_t* v, zuint32 i)
{
	if (!v)
		return;

	if (  i == GX_INDEX_INVALID  )
		return;

	if (v->index_count == v->index_capacity)
		return ; //we are full!

	if (!v->index_data)
		return;

	v->index_data[  (v->index_count) ++ ] = i;
}


zuint32 gx_remaining_indices(gx_vbuffer_t* v)
{
	if (!v)
		return 0;
	
	return v->index_capacity - v->index_count;
	
}

zuint32 gx_remaining_vertices(gx_vbuffer_t* v)
{
	if (!v)
		return 0;

	return v->vertex_capacity - v->vertex_count;
}

//clearing a vbuffer
void gx_vbuffer_clear(gx_vbuffer_t* v, zbool clear_index, zbool clear_vertex)
{
	if (!v)
		return;

	if (clear_index)
		v->index_count = 0;

	if (clear_vertex)
		v->vertex_count = 0;

}



static zbool legacy_arrays_enabled = ZFALSE;  	//Set to true whenever vertex arrays/VBOs are used without shaders (FF pipeline. ) (So we know to disable when enable shaders
												//We don't want to always disable when enabling shaders, because in the future, the FF calls will be unavailable




static zuint32 _gx_texture_pointer_enabled_count = 0;  //specified how many texture units have been turned on

//drawing a vbuffer
void gx_vbuffer_draw(gx_vbuffer_t* v, zuint32 start, zuint32 stop, gx_prim_e prim  , zbool indexed)
{
	zuint32 i=0;
	zuint32 newtcount=0;

	gx_shaderset_t* shader = gxi_active_shaderset(); //get the active shader, if there is one
	
	if (!v)
		return;
	
	if (stop <=start)
		return;

	printf(" refresh matrix... %p %d\n", shader, shader? shader->matrix_version : 666);
	gxi_refresh_matrix(shader);
	
	
	for (i=0;i<gxi_num_attr;i++){
			if (gxi_attr_buffer[i] == ATTR_ENABLED) {
				printf(" flagging attr %d for posible disable\n",i);
				gxi_attr_buffer[i] = ATTR_TODISABLE;
			}
	}
	
	
	if (v->_vertex_combined_vbo)
	{
		glBindBuffer(GL_ARRAY_BUFFER,  v->_vertex_combined_vbo );
	}

	
	if (shader && legacy_arrays_enabled ) {
		//if we are drawing with a shader, but legacy were previously used, disable all of them
		printf("Disable legacy attribute arrays\n");
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		for (i=0 ; i< _gx_texture_pointer_enabled_count; i++)
		{
			glClientActiveTexture(GL_TEXTURE0+i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		
		legacy_arrays_enabled = ZFALSE;
		_gx_texture_pointer_enabled_count=0;
	}
	
	//TODO: switch pos_data, normal_data and texcoord0 data to use generic attributes
	// Then we also need to track which attribute indices are enabled, so that later when we run thru here
	// we disable the attributes that are not used

	if (shader) {
		/* Set generic attributes for shader */
		
		 if (shader->vertex_loc != -1 ) {
			printf(" set vertex attrib\n");
			glVertexAttribPointer(shader->vertex_loc, VERTEX_COMPONENTS, GL_FLOAT, 0, 0, (void*) ( (v->pos_data - v->combined_vertex_data) * sizeof (zfloat32)) );
			glEnableVertexAttribArray(shader->vertex_loc);
			gxi_attr_buffer[shader->vertex_loc] = ATTR_ENABLED;
		 }
		
		 if (shader->color_loc != -1 ) {
			printf(" set color attrib\n");
			glVertexAttribPointer(shader->color_loc, COLOR_COMPONENTS, GL_FLOAT, 0, 0, (void*) ( (v->color_data - v->combined_vertex_data) * sizeof (zfloat32)) );
			glEnableVertexAttribArray(shader->color_loc);
			gxi_attr_buffer[shader->color_loc] = ATTR_ENABLED;
		 }
		 
		if (shader->normal_loc != -1 ) {
			printf(" set norm attrib\n");
			glVertexAttribPointer(shader->normal_loc, VERTEX_COMPONENTS, GL_FLOAT, 0, 0, (void*) ( (v->normal_data - v->combined_vertex_data) * sizeof (zfloat32)) );
			glEnableVertexAttribArray(shader->normal_loc);
			gxi_attr_buffer[shader->normal_loc] = ATTR_ENABLED;
		 }
		
		
		for (i=0;i<GX_MAX_TEXCOORD;i++) {
			if (shader->texcoord_loc[i] != -1 ) {
				printf(" set tex %d attrib\n", i);
				glVertexAttribPointer(shader->texcoord_loc[i], 2, GL_FLOAT, 0, 0, (void*) ( (v->texcoord_data[i] - v->combined_vertex_data) * sizeof (zfloat32)) );
				glEnableVertexAttribArray(shader->texcoord_loc[i]);
				gxi_attr_buffer[shader->texcoord_loc[i]] = ATTR_ENABLED;
			}
		}
		
	}
	
	
	//disable all non-enabled attributes
	for (i=0;i<gxi_num_attr;i++){
		if (gxi_attr_buffer[i] == ATTR_TODISABLE) {
			printf(" disabling previously used but now unused attr %d \n",i);
			gxi_attr_buffer[i] = ATTR_OFF;
		}
}
	
	
	//things sent in both cases:
	if (v->_index_vbo)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  v->_index_vbo);
	}
	

	

	if (!shader) {
		
		
		if (v->pos_data)
		{
			if (_gx_no_vbos)
				glVertexPointer(VERTEX_USE_COMPONENTS, GL_FLOAT, VERTEX_COMPONENTS*sizeof(float),   v->pos_data );
			else
				glVertexPointer(VERTEX_USE_COMPONENTS, GL_FLOAT, VERTEX_COMPONENTS*sizeof(float), (void*) ( (v->pos_data - v->combined_vertex_data) * sizeof (zfloat32)) );
			glEnableClientState(GL_VERTEX_ARRAY);	
			legacy_arrays_enabled = ZTRUE; //yes, we are using the legacy arrays
		}
		
		
		if (v->color_data) {
			
		
			
			if (_gx_no_vbos)
					glColorPointer(COLOR_COMPONENTS, GL_FLOAT, COLOR_COMPONENTS*sizeof(float),   v->color_data );
			else  {
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(COLOR_COMPONENTS, GL_FLOAT, COLOR_COMPONENTS*sizeof(float), (void*) ( (v->color_data - v->combined_vertex_data) * sizeof (zfloat32)) );
			}
		} else 
				glDisableClientState(GL_COLOR_ARRAY);
		
		if (v->normal_data)
		{
			
			if (_gx_no_vbos)
				glNormalPointer( GL_FLOAT, VERTEX_COMPONENTS*sizeof(float),  v->normal_data  );
			else
				glNormalPointer( GL_FLOAT, VERTEX_COMPONENTS*sizeof(float),  (void*) ( (v->normal_data - v->combined_vertex_data) * sizeof (zfloat32)) );
			glEnableClientState(GL_NORMAL_ARRAY);
		}
		else
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		
		/* single texcoord mode */
		if (v->num_texcoord == 1) {
			
			int num_active_texture_units = gxi_num_texture_units();
				//single texture coordinate mode
				//this texture coordinate is used for all enabled texture stages
			
			for (i=0;i<num_active_texture_units;i++) {
				//printf(" Alias texcoord %d for unit %d\n", 0, i);
				glClientActiveTexture(GL_TEXTURE0+i);
			
				if (_gx_no_vbos)
					glTexCoordPointer(TEXTURE_COMPONENTS, GL_FLOAT, 0, v->texcoord_data[0] );
				else
					glTexCoordPointer(TEXTURE_COMPONENTS, GL_FLOAT, 0, (void*)( (v->texcoord_data[0] - v->combined_vertex_data) * sizeof (zfloat32)));
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			newtcount = num_active_texture_units;
		} 
		else {
				printf("Multiple texcoord inputs not currently supported (sorry)\n");
		}
				
		//disable texture pointers for any leftover units
		for (newtcount ; i< _gx_texture_pointer_enabled_count; i++)
		{
			glClientActiveTexture(GL_TEXTURE0+i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		_gx_texture_pointer_enabled_count = newtcount;
				
	}  //end of legacy arrays
	
	

	
	
	
	
#if 0
	else {
		//TODO: THIS PATH NOT USED MUCH, MAKE SURE IT ACTS NORMALISH
		for (i=0;i<v->num_textures;i++)
		{
			glClientActiveTexture(GL_TEXTURE0+i);

			if (_gx_no_vbos)
				glTexCoordPointer(TEXTURE_COMPONENTS, GL_FLOAT, 0, v->texcoord_data[i] );
			else
				glTexCoordPointer(TEXTURE_COMPONENTS, GL_FLOAT, 0, (void*)( (v->texcoord_data[i] - v->combined_vertex_data) * sizeof (zfloat32)));
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			newtcount = v->num_textures; //keep track of how many we have enabled currently
		}
	}
#endif
	




	switch(prim)
	{
	case gx_points:
		if (indexed && _gx_no_vbos)
			printf(" TODO: indexed points vertex arrays\n");
		else if (indexed)
			glDrawElements(GL_POINTS, stop-start, GL_UNSIGNED_INT, (void*) (sizeof(zuint32) * start)  );
		else 
			glDrawArrays(GL_POINTS, start, stop-start);
		break;

	case gx_lines:
		if (indexed && _gx_no_vbos)
			printf(" TODO: indexed lines vertex arrays\n");
		else if (indexed)
			glDrawElements(GL_LINES, stop-start, GL_UNSIGNED_INT, (void*) (sizeof(zuint32) * start));
		else 
			glDrawArrays(GL_LINES, start, stop-start);
		break;

	case gx_triangles:
		if (indexed && _gx_no_vbos)
			glDrawElements(GL_TRIANGLES, stop-start, GL_UNSIGNED_INT,  v->index_data+  start  );
		else if (indexed)
			glDrawElements(GL_TRIANGLES, stop-start, GL_UNSIGNED_INT,  (void*) (sizeof(zuint32) * start) );
		else 
			glDrawArrays(GL_TRIANGLES, start, stop-start);
		break;


	case gx_quads:
		if (indexed && _gx_no_vbos)
			printf(" TODO: indexed quads vertex arrays\n");
		else if (indexed)
			glDrawElements(GL_QUADS, stop-start, GL_UNSIGNED_INT,(void*) (sizeof(zuint32) * start));
		else 
			glDrawArrays(GL_QUADS, start, stop-start);
		break;
	}
	
	
	
}









//lame way to create geometry: axis-aligned image
//this type of geometry-related crap should be shoved into a separate file
//if the user sets the preferred buffer argument, if the data will fit, it will be put in the existing buffer
#if 0
gx_vbuffer_t* gx_vbuffer_from_image(gx_vbuffer_t* preferred_buffer,
									gx_image_t* image, 
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
									zuint32 num_texture,
									zuint32* start,  //start and end return the draw start and end calls for the vbuffer
									zuint32* end, 
									zbool flipnorm
									)
{
	zuint32 i,j,t;
	float coord[3];
	
	gx_vbuffer_t* vbuf = NULL;
	int bpp = 0;

	zuint32* lastcol = NULL;
	zuint32 numpoints = image->width * image->height;
	zuint32 numtriangles = (image->width-1) * (image->height -1)  *2 ;

	if (!preferred_buffer || gx_remaining_indices(preferred_buffer)<(numtriangles*3) || gx_remaining_vertices(preferred_buffer) < numpoints)
	{
		vbuf = gx_vbuffer_mk(numpoints,numtriangles*3, 
			use_color, ZFALSE,
			num_texture );
	}
	else
	{
		vbuf = preferred_buffer;
	}
	
	bpp = image->bpp;

	lastcol = ram_alloc( sizeof(zuint32) * image->height, NULL);

	if (xaxis>2 || yaxis>2 || zaxis>2)
	{
		//use defaults if user was stupid
		xaxis=1;
		yaxis=2;
		zaxis=3;
	}
	
	if (start)
		*start = vbuf->index_count;

	
	
	//create vertices for each point
	for (i=0;i<image->width;i++)
	{
		for (j=0;j<image->height;j++)
		{

			
			zuint32 nv = -1;
			
			zfloat32 hf =  image->data[ (j*image->height +i) * bpp]/255.0f;

		

			zfloat32 hfl= hf;
			zfloat32 hfr= hf;
			zfloat32 hfu= hf;
			zfloat32 hfd= hf;
			vec3 norm;
		
			if (bpp==2)
			{
				if (image->data[ (j*image->height +i)*bpp+1] == 0)
				{
					lastcol[j] = -1;// invalid;
					continue;
				}  //zero is not 'in' the heightmap
			}


			if (i>0)
				hfl =  image->data[(j*image->height +i-1)*bpp]/255.0f;

			if (i<image->width-1)
				hfr =  image->data[(j*image->height +i+1)*bpp]/255.0f;

			if (j>0)
				hfu =  image->data[((j-1)*image->height +i)*bpp]/255.0f;

			if (j<image->height-1)
				hfd =  image->data[((j+1)*image->height +i)*bpp]/255.0f;

		
			vec3set(norm, 0,ysize,0); //normal pointing straight up

			norm.named.x = (hfl-hfr) /ysize;
			norm.named.z =  (hfu-hfd)/ysize;
			
			if (flipnorm)
			{

				vec3scale(norm, -1);
			}
			
			gx_vbuffer_add_normal(vbuf, norm.named.x, norm.named.y, norm.named.z);



			if(use_color)
			{
				gx_vbuffer_add_color(vbuf, hf,hf,hf,1);
			}

			for (t=0; t< num_texture;t++)
			{
				//give all texture layers the same coordinates
				gx_vbuffer_add_tex(vbuf, t,  ((i) / (float)(image->width -1)), ((j) / (float)(image->width -1)));
			}

			coord[0]= (i*xsize) / (image->width-1);
			coord[1]= ysize * hf;
			coord[2]= (j*zsize) / (image->height-1);


			nv = gx_vbuffer_add_vertex(vbuf, coord[xaxis]+xoff, coord[yaxis]+yoff, coord [zaxis] +zoff );


			if (i>0)
			{
				if (j+1<image->height)
				{
					if (lastcol[j] == -1)
					{
						lastcol[j]=nv;
						continue;
					}


					if (lastcol[j+1] == -1)
					{
						lastcol[j]=nv;
						continue;
					}


					gx_vbuffer_add_index(vbuf, nv);
					gx_vbuffer_add_index(vbuf, lastcol[j]);
					gx_vbuffer_add_index(vbuf, lastcol[j+1]);
				}

				if(j>0)
				{

					if (lastcol[j] == -1)
					{
						lastcol[j]=nv;
						continue;
					}



					if (lastcol[j-1] == -1)
					{
						lastcol[j]=nv;
						continue;
					}



					gx_vbuffer_add_index(vbuf, nv);
					
					gx_vbuffer_add_index(vbuf, lastcol[j-1]);
					gx_vbuffer_add_index(vbuf, lastcol[j]);

				}
			}

			lastcol[j]=nv; //store this vertex in the 'last col' table.


		}
	}

	if (end)
		*end = vbuf->index_count;

	return vbuf;


}
#endif

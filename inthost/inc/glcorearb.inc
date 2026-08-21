section gl

	type DEBUGPROCARB end
	type DEBUGPROC end
	type eglImageOES end
	type VULKANPROCNV end
1 constant __gl_glcorearb_h_

1 constant WIN32_LEAN_AND_MEAN

1 constant VERSION_1_0

              0x00000100 constant DEPTH_BUFFER_BIT

            0x00000400 constant STENCIL_BUFFER_BIT

              0x00004000 constant COLOR_BUFFER_BIT

                         0 constant FALSE

                          1 constant TRUE

                        0x0000 constant POINTS

                         0x0001 constant LINES

                     0x0002 constant LINE_LOOP

                    0x0003 constant LINE_STRIP

                     0x0004 constant TRIANGLES

                0x0005 constant TRIANGLE_STRIP

                  0x0006 constant TRIANGLE_FAN

                         0x0007 constant QUADS

                         0x0200 constant NEVER

                          0x0201 constant LESS

                         0x0202 constant EQUAL

                        0x0203 constant LEQUAL

                       0x0204 constant GREATER

                      0x0205 constant NOTEQUAL

                        0x0206 constant GEQUAL

                        0x0207 constant ALWAYS

                          0 constant ZERO

                           1 constant ONE

                     0x0300 constant SRC_COLOR

           0x0301 constant ONE_MINUS_SRC_COLOR

                     0x0302 constant SRC_ALPHA

           0x0303 constant ONE_MINUS_SRC_ALPHA

                     0x0304 constant DST_ALPHA

           0x0305 constant ONE_MINUS_DST_ALPHA

                     0x0306 constant DST_COLOR

           0x0307 constant ONE_MINUS_DST_COLOR

            0x0308 constant SRC_ALPHA_SATURATE

                          0 constant NONE

                    0x0400 constant FRONT_LEFT

                   0x0401 constant FRONT_RIGHT

                     0x0402 constant BACK_LEFT

                    0x0403 constant BACK_RIGHT

                         0x0404 constant FRONT

                          0x0405 constant BACK

                          0x0406 constant LEFT

                         0x0407 constant RIGHT

                0x0408 constant FRONT_AND_BACK

                      0 constant NO_ERROR

                  0x0500 constant INVALID_ENUM

                 0x0501 constant INVALID_VALUE

             0x0502 constant INVALID_OPERATION

                 0x0505 constant OUT_OF_MEMORY

                            0x0900 constant CW

                           0x0901 constant CCW

                    0x0B11 constant POINT_SIZE

              0x0B12 constant POINT_SIZE_RANGE

        0x0B13 constant POINT_SIZE_GRANULARITY

                   0x0B20 constant LINE_SMOOTH

                    0x0B21 constant LINE_WIDTH

              0x0B22 constant LINE_WIDTH_RANGE

        0x0B23 constant LINE_WIDTH_GRANULARITY

                  0x0B40 constant POLYGON_MODE

                0x0B41 constant POLYGON_SMOOTH

                     0x0B44 constant CULL_FACE

                0x0B45 constant CULL_FACE_MODE

                    0x0B46 constant FRONT_FACE

                   0x0B70 constant DEPTH_RANGE

                    0x0B71 constant DEPTH_TEST

               0x0B72 constant DEPTH_WRITEMASK

             0x0B73 constant DEPTH_CLEAR_VALUE

                    0x0B74 constant DEPTH_FUNC

                  0x0B90 constant STENCIL_TEST

           0x0B91 constant STENCIL_CLEAR_VALUE

                  0x0B92 constant STENCIL_FUNC

            0x0B93 constant STENCIL_VALUE_MASK

                  0x0B94 constant STENCIL_FAIL

       0x0B95 constant STENCIL_PASS_DEPTH_FAIL

       0x0B96 constant STENCIL_PASS_DEPTH_PASS

                   0x0B97 constant STENCIL_REF

             0x0B98 constant STENCIL_WRITEMASK

                      0x0BA2 constant VIEWPORT

                        0x0BD0 constant DITHER

                     0x0BE0 constant BLEND_DST

                     0x0BE1 constant BLEND_SRC

                         0x0BE2 constant BLEND

                 0x0BF0 constant LOGIC_OP_MODE

                   0x0C01 constant DRAW_BUFFER

                   0x0C02 constant READ_BUFFER

                   0x0C10 constant SCISSOR_BOX

                  0x0C11 constant SCISSOR_TEST

             0x0C22 constant COLOR_CLEAR_VALUE

               0x0C23 constant COLOR_WRITEMASK

                  0x0C32 constant DOUBLEBUFFER

                        0x0C33 constant STEREO

              0x0C52 constant LINE_SMOOTH_HINT

           0x0C53 constant POLYGON_SMOOTH_HINT

             0x0CF0 constant UNPACK_SWAP_BYTES

              0x0CF1 constant UNPACK_LSB_FIRST

             0x0CF2 constant UNPACK_ROW_LENGTH

              0x0CF3 constant UNPACK_SKIP_ROWS

            0x0CF4 constant UNPACK_SKIP_PIXELS

              0x0CF5 constant UNPACK_ALIGNMENT

               0x0D00 constant PACK_SWAP_BYTES

                0x0D01 constant PACK_LSB_FIRST

               0x0D02 constant PACK_ROW_LENGTH

                0x0D03 constant PACK_SKIP_ROWS

              0x0D04 constant PACK_SKIP_PIXELS

                0x0D05 constant PACK_ALIGNMENT

              0x0D33 constant MAX_TEXTURE_SIZE

             0x0D3A constant MAX_VIEWPORT_DIMS

                 0x0D50 constant SUBPIXEL_BITS

                    0x0DE0 constant TEXTURE_1D

                    0x0DE1 constant TEXTURE_2D

                 0x1000 constant TEXTURE_WIDTH

                0x1001 constant TEXTURE_HEIGHT

          0x1004 constant TEXTURE_BORDER_COLOR

                     0x1100 constant DONT_CARE

                       0x1101 constant FASTEST

                        0x1102 constant NICEST

                          0x1400 constant BYTE

                 0x1401 constant UNSIGNED_BYTE

                         0x1402 constant SHORT

                0x1403 constant UNSIGNED_SHORT

                           0x1404 constant INT

                  0x1405 constant UNSIGNED_INT

                         0x1406 constant FLOAT

                0x0503 constant STACK_OVERFLOW

               0x0504 constant STACK_UNDERFLOW

                         0x1500 constant CLEAR

                           0x1501 constant AND

                   0x1502 constant AND_REVERSE

                          0x1503 constant COPY

                  0x1504 constant AND_INVERTED

                          0x1505 constant NOOP

                           0x1506 constant XOR

                            0x1507 constant OR

                           0x1508 constant NOR

                         0x1509 constant EQUIV

                        0x150A constant INVERT

                    0x150B constant OR_REVERSE

                 0x150C constant COPY_INVERTED

                   0x150D constant OR_INVERTED

                          0x150E constant NAND

                           0x150F constant SET

                       0x1702 constant TEXTURE

                         0x1800 constant COLOR

                         0x1801 constant DEPTH

                       0x1802 constant STENCIL

                 0x1901 constant STENCIL_INDEX

               0x1902 constant DEPTH_COMPONENT

                           0x1903 constant RED

                         0x1904 constant GREEN

                          0x1905 constant BLUE

                         0x1906 constant ALPHA

                           0x1907 constant RGB

                          0x1908 constant RGBA

                         0x1B00 constant POINT

                          0x1B01 constant LINE

                          0x1B02 constant FILL

                          0x1E00 constant KEEP

                       0x1E01 constant REPLACE

                          0x1E02 constant INCR

                          0x1E03 constant DECR

                        0x1F00 constant VENDOR

                      0x1F01 constant RENDERER

                       0x1F02 constant VERSION

                    0x1F03 constant EXTENSIONS

                       0x2600 constant NEAREST

                        0x2601 constant LINEAR

        0x2700 constant NEAREST_MIPMAP_NEAREST

         0x2701 constant LINEAR_MIPMAP_NEAREST

         0x2702 constant NEAREST_MIPMAP_LINEAR

          0x2703 constant LINEAR_MIPMAP_LINEAR

            0x2800 constant TEXTURE_MAG_FILTER

            0x2801 constant TEXTURE_MIN_FILTER

                0x2802 constant TEXTURE_WRAP_S

                0x2803 constant TEXTURE_WRAP_T

                        0x2901 constant REPEAT

1 constant VERSION_1_1

                0x0BF2 constant COLOR_LOGIC_OP

          0x2A00 constant POLYGON_OFFSET_UNITS

          0x2A01 constant POLYGON_OFFSET_POINT

           0x2A02 constant POLYGON_OFFSET_LINE

           0x8037 constant POLYGON_OFFSET_FILL

         0x8038 constant POLYGON_OFFSET_FACTOR

            0x8068 constant TEXTURE_BINDING_1D

            0x8069 constant TEXTURE_BINDING_2D

       0x1003 constant TEXTURE_INTERNAL_FORMAT

              0x805C constant TEXTURE_RED_SIZE

            0x805D constant TEXTURE_GREEN_SIZE

             0x805E constant TEXTURE_BLUE_SIZE

            0x805F constant TEXTURE_ALPHA_SIZE

                        0x140A constant DOUBLE

              0x8063 constant PROXY_TEXTURE_1D

              0x8064 constant PROXY_TEXTURE_2D

                      0x2A10 constant R3_G3_B2

                          0x804F constant RGB4

                          0x8050 constant RGB5

                          0x8051 constant RGB8

                         0x8052 constant RGB10

                         0x8053 constant RGB12

                         0x8054 constant RGB16

                         0x8055 constant RGBA2

                         0x8056 constant RGBA4

                       0x8057 constant RGB5_A1

                         0x8058 constant RGBA8

                      0x8059 constant RGB10_A2

                        0x805A constant RGBA12

                        0x805B constant RGBA16

                  0x8074 constant VERTEX_ARRAY

1 constant VERSION_1_2

           0x8032 constant UNSIGNED_BYTE_3_3_2

        0x8033 constant UNSIGNED_SHORT_4_4_4_4

        0x8034 constant UNSIGNED_SHORT_5_5_5_1

          0x8035 constant UNSIGNED_INT_8_8_8_8

       0x8036 constant UNSIGNED_INT_10_10_10_2

            0x806A constant TEXTURE_BINDING_3D

              0x806B constant PACK_SKIP_IMAGES

             0x806C constant PACK_IMAGE_HEIGHT

            0x806D constant UNPACK_SKIP_IMAGES

           0x806E constant UNPACK_IMAGE_HEIGHT

                    0x806F constant TEXTURE_3D

              0x8070 constant PROXY_TEXTURE_3D

                 0x8071 constant TEXTURE_DEPTH

                0x8072 constant TEXTURE_WRAP_R

           0x8073 constant MAX_3D_TEXTURE_SIZE

       0x8362 constant UNSIGNED_BYTE_2_3_3_REV

          0x8363 constant UNSIGNED_SHORT_5_6_5

      0x8364 constant UNSIGNED_SHORT_5_6_5_REV

    0x8365 constant UNSIGNED_SHORT_4_4_4_4_REV

    0x8366 constant UNSIGNED_SHORT_1_5_5_5_REV

      0x8367 constant UNSIGNED_INT_8_8_8_8_REV

   0x8368 constant UNSIGNED_INT_2_10_10_10_REV

                           0x80E0 constant BGR

                          0x80E1 constant BGRA

         0x80E8 constant MAX_ELEMENTS_VERTICES

          0x80E9 constant MAX_ELEMENTS_INDICES

                 0x812F constant CLAMP_TO_EDGE

               0x813A constant TEXTURE_MIN_LOD

               0x813B constant TEXTURE_MAX_LOD

            0x813C constant TEXTURE_BASE_LEVEL

             0x813D constant TEXTURE_MAX_LEVEL

       0x0B12 constant SMOOTH_POINT_SIZE_RANGE

 0x0B13 constant SMOOTH_POINT_SIZE_GRANULARITY

       0x0B22 constant SMOOTH_LINE_WIDTH_RANGE

 0x0B23 constant SMOOTH_LINE_WIDTH_GRANULARITY

      0x846E constant ALIASED_LINE_WIDTH_RANGE

1 constant VERSION_1_3

                      0x84C0 constant TEXTURE0

                      0x84C1 constant TEXTURE1

                      0x84C2 constant TEXTURE2

                      0x84C3 constant TEXTURE3

                      0x84C4 constant TEXTURE4

                      0x84C5 constant TEXTURE5

                      0x84C6 constant TEXTURE6

                      0x84C7 constant TEXTURE7

                      0x84C8 constant TEXTURE8

                      0x84C9 constant TEXTURE9

                     0x84CA constant TEXTURE10

                     0x84CB constant TEXTURE11

                     0x84CC constant TEXTURE12

                     0x84CD constant TEXTURE13

                     0x84CE constant TEXTURE14

                     0x84CF constant TEXTURE15

                     0x84D0 constant TEXTURE16

                     0x84D1 constant TEXTURE17

                     0x84D2 constant TEXTURE18

                     0x84D3 constant TEXTURE19

                     0x84D4 constant TEXTURE20

                     0x84D5 constant TEXTURE21

                     0x84D6 constant TEXTURE22

                     0x84D7 constant TEXTURE23

                     0x84D8 constant TEXTURE24

                     0x84D9 constant TEXTURE25

                     0x84DA constant TEXTURE26

                     0x84DB constant TEXTURE27

                     0x84DC constant TEXTURE28

                     0x84DD constant TEXTURE29

                     0x84DE constant TEXTURE30

                     0x84DF constant TEXTURE31

                0x84E0 constant ACTIVE_TEXTURE

                   0x809D constant MULTISAMPLE

      0x809E constant SAMPLE_ALPHA_TO_COVERAGE

           0x809F constant SAMPLE_ALPHA_TO_ONE

               0x80A0 constant SAMPLE_COVERAGE

                0x80A8 constant SAMPLE_BUFFERS

                       0x80A9 constant SAMPLES

         0x80AA constant SAMPLE_COVERAGE_VALUE

        0x80AB constant SAMPLE_COVERAGE_INVERT

              0x8513 constant TEXTURE_CUBE_MAP

      0x8514 constant TEXTURE_BINDING_CUBE_MAP

   0x8515 constant TEXTURE_CUBE_MAP_POSITIVE_X

   0x8516 constant TEXTURE_CUBE_MAP_NEGATIVE_X

   0x8517 constant TEXTURE_CUBE_MAP_POSITIVE_Y

   0x8518 constant TEXTURE_CUBE_MAP_NEGATIVE_Y

   0x8519 constant TEXTURE_CUBE_MAP_POSITIVE_Z

   0x851A constant TEXTURE_CUBE_MAP_NEGATIVE_Z

        0x851B constant PROXY_TEXTURE_CUBE_MAP

     0x851C constant MAX_CUBE_MAP_TEXTURE_SIZE

                0x84ED constant COMPRESSED_RGB

               0x84EE constant COMPRESSED_RGBA

      0x84EF constant TEXTURE_COMPRESSION_HINT

 0x86A0 constant TEXTURE_COMPRESSED_IMAGE_SIZE

            0x86A1 constant TEXTURE_COMPRESSED

0x86A2 constant NUM_COMPRESSED_TEXTURE_FORMATS

    0x86A3 constant COMPRESSED_TEXTURE_FORMATS

               0x812D constant CLAMP_TO_BORDER

1 constant VERSION_1_4

                 0x80C8 constant BLEND_DST_RGB

                 0x80C9 constant BLEND_SRC_RGB

               0x80CA constant BLEND_DST_ALPHA

               0x80CB constant BLEND_SRC_ALPHA

     0x8128 constant POINT_FADE_THRESHOLD_SIZE

             0x81A5 constant DEPTH_COMPONENT16

             0x81A6 constant DEPTH_COMPONENT24

             0x81A7 constant DEPTH_COMPONENT32

               0x8370 constant MIRRORED_REPEAT

          0x84FD constant MAX_TEXTURE_LOD_BIAS

              0x8501 constant TEXTURE_LOD_BIAS

                     0x8507 constant INCR_WRAP

                     0x8508 constant DECR_WRAP

            0x884A constant TEXTURE_DEPTH_SIZE

          0x884C constant TEXTURE_COMPARE_MODE

          0x884D constant TEXTURE_COMPARE_FUNC

                   0x8005 constant BLEND_COLOR

                0x8009 constant BLEND_EQUATION

                0x8001 constant CONSTANT_COLOR

      0x8002 constant ONE_MINUS_CONSTANT_COLOR

                0x8003 constant CONSTANT_ALPHA

      0x8004 constant ONE_MINUS_CONSTANT_ALPHA

                      0x8006 constant FUNC_ADD

         0x800B constant FUNC_REVERSE_SUBTRACT

                 0x800A constant FUNC_SUBTRACT

                           0x8007 constant MIN

                           0x8008 constant MAX

1 constant VERSION_1_5

                   0x8764 constant BUFFER_SIZE

                  0x8765 constant BUFFER_USAGE

            0x8864 constant QUERY_COUNTER_BITS

                 0x8865 constant CURRENT_QUERY

                  0x8866 constant QUERY_RESULT

        0x8867 constant QUERY_RESULT_AVAILABLE

                  0x8892 constant ARRAY_BUFFER

          0x8893 constant ELEMENT_ARRAY_BUFFER

          0x8894 constant ARRAY_BUFFER_BINDING

  0x8895 constant ELEMENT_ARRAY_BUFFER_BINDING

0x889F constant VERTEX_ATTRIB_ARRAY_BUFFER_BINDING

                     0x88B8 constant READ_ONLY

                    0x88B9 constant WRITE_ONLY

                    0x88BA constant READ_WRITE

                 0x88BB constant BUFFER_ACCESS

                 0x88BC constant BUFFER_MAPPED

            0x88BD constant BUFFER_MAP_POINTER

                   0x88E0 constant STREAM_DRAW

                   0x88E1 constant STREAM_READ

                   0x88E2 constant STREAM_COPY

                   0x88E4 constant STATIC_DRAW

                   0x88E5 constant STATIC_READ

                   0x88E6 constant STATIC_COPY

                  0x88E8 constant DYNAMIC_DRAW

                  0x88E9 constant DYNAMIC_READ

                  0x88EA constant DYNAMIC_COPY

                0x8914 constant SAMPLES_PASSED

                    0x8589 constant SRC1_ALPHA

1 constant VERSION_2_0

            0x8009 constant BLEND_EQUATION_RGB

   0x8622 constant VERTEX_ATTRIB_ARRAY_ENABLED

      0x8623 constant VERTEX_ATTRIB_ARRAY_SIZE

    0x8624 constant VERTEX_ATTRIB_ARRAY_STRIDE

      0x8625 constant VERTEX_ATTRIB_ARRAY_TYPE

         0x8626 constant CURRENT_VERTEX_ATTRIB

     0x8642 constant VERTEX_PROGRAM_POINT_SIZE

   0x8645 constant VERTEX_ATTRIB_ARRAY_POINTER

             0x8800 constant STENCIL_BACK_FUNC

             0x8801 constant STENCIL_BACK_FAIL

  0x8802 constant STENCIL_BACK_PASS_DEPTH_FAIL

  0x8803 constant STENCIL_BACK_PASS_DEPTH_PASS

              0x8824 constant MAX_DRAW_BUFFERS

                  0x8825 constant DRAW_BUFFER0

                  0x8826 constant DRAW_BUFFER1

                  0x8827 constant DRAW_BUFFER2

                  0x8828 constant DRAW_BUFFER3

                  0x8829 constant DRAW_BUFFER4

                  0x882A constant DRAW_BUFFER5

                  0x882B constant DRAW_BUFFER6

                  0x882C constant DRAW_BUFFER7

                  0x882D constant DRAW_BUFFER8

                  0x882E constant DRAW_BUFFER9

                 0x882F constant DRAW_BUFFER10

                 0x8830 constant DRAW_BUFFER11

                 0x8831 constant DRAW_BUFFER12

                 0x8832 constant DRAW_BUFFER13

                 0x8833 constant DRAW_BUFFER14

                 0x8834 constant DRAW_BUFFER15

          0x883D constant BLEND_EQUATION_ALPHA

            0x8869 constant MAX_VERTEX_ATTRIBS

0x886A constant VERTEX_ATTRIB_ARRAY_NORMALIZED

       0x8872 constant MAX_TEXTURE_IMAGE_UNITS

               0x8B30 constant FRAGMENT_SHADER

                 0x8B31 constant VERTEX_SHADER

0x8B49 constant MAX_FRAGMENT_UNIFORM_COMPONENTS

 0x8B4A constant MAX_VERTEX_UNIFORM_COMPONENTS

            0x8B4B constant MAX_VARYING_FLOATS

0x8B4C constant MAX_VERTEX_TEXTURE_IMAGE_UNITS

0x8B4D constant MAX_COMBINED_TEXTURE_IMAGE_UNITS

                   0x8B4F constant SHADER_TYPE

                    0x8B50 constant FLOAT_VEC2

                    0x8B51 constant FLOAT_VEC3

                    0x8B52 constant FLOAT_VEC4

                      0x8B53 constant INT_VEC2

                      0x8B54 constant INT_VEC3

                      0x8B55 constant INT_VEC4

                          0x8B56 constant BOOL

                     0x8B57 constant BOOL_VEC2

                     0x8B58 constant BOOL_VEC3

                     0x8B59 constant BOOL_VEC4

                    0x8B5A constant FLOAT_MAT2

                    0x8B5B constant FLOAT_MAT3

                    0x8B5C constant FLOAT_MAT4

                    0x8B5D constant SAMPLER_1D

                    0x8B5E constant SAMPLER_2D

                    0x8B5F constant SAMPLER_3D

                  0x8B60 constant SAMPLER_CUBE

             0x8B61 constant SAMPLER_1D_SHADOW

             0x8B62 constant SAMPLER_2D_SHADOW

                 0x8B80 constant DELETE_STATUS

                0x8B81 constant COMPILE_STATUS

                   0x8B82 constant LINK_STATUS

               0x8B83 constant VALIDATE_STATUS

               0x8B84 constant INFO_LOG_LENGTH

              0x8B85 constant ATTACHED_SHADERS

               0x8B86 constant ACTIVE_UNIFORMS

     0x8B87 constant ACTIVE_UNIFORM_MAX_LENGTH

          0x8B88 constant SHADER_SOURCE_LENGTH

             0x8B89 constant ACTIVE_ATTRIBUTES

   0x8B8A constant ACTIVE_ATTRIBUTE_MAX_LENGTH

0x8B8B constant FRAGMENT_SHADER_DERIVATIVE_HINT

      0x8B8C constant SHADING_LANGUAGE_VERSION

               0x8B8D constant CURRENT_PROGRAM

     0x8CA0 constant POINT_SPRITE_COORD_ORIGIN

                    0x8CA1 constant LOWER_LEFT

                    0x8CA2 constant UPPER_LEFT

              0x8CA3 constant STENCIL_BACK_REF

       0x8CA4 constant STENCIL_BACK_VALUE_MASK

        0x8CA5 constant STENCIL_BACK_WRITEMASK

1 constant VERSION_2_1

             0x88EB constant PIXEL_PACK_BUFFER

           0x88EC constant PIXEL_UNPACK_BUFFER

     0x88ED constant PIXEL_PACK_BUFFER_BINDING

   0x88EF constant PIXEL_UNPACK_BUFFER_BINDING

                  0x8B65 constant FLOAT_MAT2x3

                  0x8B66 constant FLOAT_MAT2x4

                  0x8B67 constant FLOAT_MAT3x2

                  0x8B68 constant FLOAT_MAT3x4

                  0x8B69 constant FLOAT_MAT4x2

                  0x8B6A constant FLOAT_MAT4x3

                          0x8C40 constant SRGB

                         0x8C41 constant SRGB8

                    0x8C42 constant SRGB_ALPHA

                  0x8C43 constant SRGB8_ALPHA8

               0x8C48 constant COMPRESSED_SRGB

         0x8C49 constant COMPRESSED_SRGB_ALPHA

1 constant VERSION_3_0

        0x884E constant COMPARE_REF_TO_TEXTURE

                0x3000 constant CLIP_DISTANCE0

                0x3001 constant CLIP_DISTANCE1

                0x3002 constant CLIP_DISTANCE2

                0x3003 constant CLIP_DISTANCE3

                0x3004 constant CLIP_DISTANCE4

                0x3005 constant CLIP_DISTANCE5

                0x3006 constant CLIP_DISTANCE6

                0x3007 constant CLIP_DISTANCE7

            0x0D32 constant MAX_CLIP_DISTANCES

                 0x821B constant MAJOR_VERSION

                 0x821C constant MINOR_VERSION

                0x821D constant NUM_EXTENSIONS

                 0x821E constant CONTEXT_FLAGS

                0x8225 constant COMPRESSED_RED

                 0x8226 constant COMPRESSED_RG

0x00000001 constant CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT

                       0x8814 constant RGBA32F

                        0x8815 constant RGB32F

                       0x881A constant RGBA16F

                        0x881B constant RGB16F

   0x88FD constant VERTEX_ATTRIB_ARRAY_INTEGER

      0x88FF constant MAX_ARRAY_TEXTURE_LAYERS

      0x8904 constant MIN_PROGRAM_TEXEL_OFFSET

      0x8905 constant MAX_PROGRAM_TEXEL_OFFSET

              0x891C constant CLAMP_READ_COLOR

                    0x891D constant FIXED_ONLY

        0x8B4B constant MAX_VARYING_COMPONENTS

              0x8C18 constant TEXTURE_1D_ARRAY

        0x8C19 constant PROXY_TEXTURE_1D_ARRAY

              0x8C1A constant TEXTURE_2D_ARRAY

        0x8C1B constant PROXY_TEXTURE_2D_ARRAY

      0x8C1C constant TEXTURE_BINDING_1D_ARRAY

      0x8C1D constant TEXTURE_BINDING_2D_ARRAY

                0x8C3A constant R11F_G11F_B10F

  0x8C3B constant UNSIGNED_INT_10F_11F_11F_REV

                       0x8C3D constant RGB9_E5

      0x8C3E constant UNSIGNED_INT_5_9_9_9_REV

           0x8C3F constant TEXTURE_SHARED_SIZE

0x8C76 constant TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH

0x8C7F constant TRANSFORM_FEEDBACK_BUFFER_MODE

0x8C80 constant MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS

   0x8C83 constant TRANSFORM_FEEDBACK_VARYINGS

0x8C84 constant TRANSFORM_FEEDBACK_BUFFER_START

0x8C85 constant TRANSFORM_FEEDBACK_BUFFER_SIZE

          0x8C87 constant PRIMITIVES_GENERATED

0x8C88 constant TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN

            0x8C89 constant RASTERIZER_DISCARD

0x8C8A constant MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS

0x8C8B constant MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS

           0x8C8C constant INTERLEAVED_ATTRIBS

              0x8C8D constant SEPARATE_ATTRIBS

     0x8C8E constant TRANSFORM_FEEDBACK_BUFFER

0x8C8F constant TRANSFORM_FEEDBACK_BUFFER_BINDING

                      0x8D70 constant RGBA32UI

                       0x8D71 constant RGB32UI

                      0x8D76 constant RGBA16UI

                       0x8D77 constant RGB16UI

                       0x8D7C constant RGBA8UI

                        0x8D7D constant RGB8UI

                       0x8D82 constant RGBA32I

                        0x8D83 constant RGB32I

                       0x8D88 constant RGBA16I

                        0x8D89 constant RGB16I

                        0x8D8E constant RGBA8I

                         0x8D8F constant RGB8I

                   0x8D94 constant RED_INTEGER

                 0x8D95 constant GREEN_INTEGER

                  0x8D96 constant BLUE_INTEGER

                   0x8D98 constant RGB_INTEGER

                  0x8D99 constant RGBA_INTEGER

                   0x8D9A constant BGR_INTEGER

                  0x8D9B constant BGRA_INTEGER

              0x8DC0 constant SAMPLER_1D_ARRAY

              0x8DC1 constant SAMPLER_2D_ARRAY

       0x8DC3 constant SAMPLER_1D_ARRAY_SHADOW

       0x8DC4 constant SAMPLER_2D_ARRAY_SHADOW

           0x8DC5 constant SAMPLER_CUBE_SHADOW

             0x8DC6 constant UNSIGNED_INT_VEC2

             0x8DC7 constant UNSIGNED_INT_VEC3

             0x8DC8 constant UNSIGNED_INT_VEC4

                0x8DC9 constant INT_SAMPLER_1D

                0x8DCA constant INT_SAMPLER_2D

                0x8DCB constant INT_SAMPLER_3D

              0x8DCC constant INT_SAMPLER_CUBE

          0x8DCE constant INT_SAMPLER_1D_ARRAY

          0x8DCF constant INT_SAMPLER_2D_ARRAY

       0x8DD1 constant UNSIGNED_INT_SAMPLER_1D

       0x8DD2 constant UNSIGNED_INT_SAMPLER_2D

       0x8DD3 constant UNSIGNED_INT_SAMPLER_3D

     0x8DD4 constant UNSIGNED_INT_SAMPLER_CUBE

 0x8DD6 constant UNSIGNED_INT_SAMPLER_1D_ARRAY

 0x8DD7 constant UNSIGNED_INT_SAMPLER_2D_ARRAY

                    0x8E13 constant QUERY_WAIT

                 0x8E14 constant QUERY_NO_WAIT

          0x8E15 constant QUERY_BY_REGION_WAIT

       0x8E16 constant QUERY_BY_REGION_NO_WAIT

           0x911F constant BUFFER_ACCESS_FLAGS

             0x9120 constant BUFFER_MAP_LENGTH

             0x9121 constant BUFFER_MAP_OFFSET

            0x8CAC constant DEPTH_COMPONENT32F

             0x8CAD constant DEPTH32F_STENCIL8

0x8DAD constant FLOAT_32_UNSIGNED_INT_24_8_REV

 0x0506 constant INVALID_FRAMEBUFFER_OPERATION

0x8210 constant FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING

0x8211 constant FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE

0x8212 constant FRAMEBUFFER_ATTACHMENT_RED_SIZE

0x8213 constant FRAMEBUFFER_ATTACHMENT_GREEN_SIZE

0x8214 constant FRAMEBUFFER_ATTACHMENT_BLUE_SIZE

0x8215 constant FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE

0x8216 constant FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE

0x8217 constant FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE

           0x8218 constant FRAMEBUFFER_DEFAULT

         0x8219 constant FRAMEBUFFER_UNDEFINED

      0x821A constant DEPTH_STENCIL_ATTACHMENT

         0x84E8 constant MAX_RENDERBUFFER_SIZE

                 0x84F9 constant DEPTH_STENCIL

             0x84FA constant UNSIGNED_INT_24_8

              0x88F0 constant DEPTH24_STENCIL8

          0x88F1 constant TEXTURE_STENCIL_SIZE

              0x8C10 constant TEXTURE_RED_TYPE

            0x8C11 constant TEXTURE_GREEN_TYPE

             0x8C12 constant TEXTURE_BLUE_TYPE

            0x8C13 constant TEXTURE_ALPHA_TYPE

            0x8C16 constant TEXTURE_DEPTH_TYPE

           0x8C17 constant UNSIGNED_NORMALIZED

           0x8CA6 constant FRAMEBUFFER_BINDING

      0x8CA6 constant DRAW_FRAMEBUFFER_BINDING

          0x8CA7 constant RENDERBUFFER_BINDING

              0x8CA8 constant READ_FRAMEBUFFER

              0x8CA9 constant DRAW_FRAMEBUFFER

      0x8CAA constant READ_FRAMEBUFFER_BINDING

          0x8CAB constant RENDERBUFFER_SAMPLES

0x8CD0 constant FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE

0x8CD1 constant FRAMEBUFFER_ATTACHMENT_OBJECT_NAME

0x8CD2 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL

0x8CD3 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE

0x8CD4 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER

          0x8CD5 constant FRAMEBUFFER_COMPLETE

0x8CD6 constant FRAMEBUFFER_INCOMPLETE_ATTACHMENT

0x8CD7 constant FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT

0x8CDB constant FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER

0x8CDC constant FRAMEBUFFER_INCOMPLETE_READ_BUFFER

       0x8CDD constant FRAMEBUFFER_UNSUPPORTED

         0x8CDF constant MAX_COLOR_ATTACHMENTS

             0x8CE0 constant COLOR_ATTACHMENT0

             0x8CE1 constant COLOR_ATTACHMENT1

             0x8CE2 constant COLOR_ATTACHMENT2

             0x8CE3 constant COLOR_ATTACHMENT3

             0x8CE4 constant COLOR_ATTACHMENT4

             0x8CE5 constant COLOR_ATTACHMENT5

             0x8CE6 constant COLOR_ATTACHMENT6

             0x8CE7 constant COLOR_ATTACHMENT7

             0x8CE8 constant COLOR_ATTACHMENT8

             0x8CE9 constant COLOR_ATTACHMENT9

            0x8CEA constant COLOR_ATTACHMENT10

            0x8CEB constant COLOR_ATTACHMENT11

            0x8CEC constant COLOR_ATTACHMENT12

            0x8CED constant COLOR_ATTACHMENT13

            0x8CEE constant COLOR_ATTACHMENT14

            0x8CEF constant COLOR_ATTACHMENT15

            0x8CF0 constant COLOR_ATTACHMENT16

            0x8CF1 constant COLOR_ATTACHMENT17

            0x8CF2 constant COLOR_ATTACHMENT18

            0x8CF3 constant COLOR_ATTACHMENT19

            0x8CF4 constant COLOR_ATTACHMENT20

            0x8CF5 constant COLOR_ATTACHMENT21

            0x8CF6 constant COLOR_ATTACHMENT22

            0x8CF7 constant COLOR_ATTACHMENT23

            0x8CF8 constant COLOR_ATTACHMENT24

            0x8CF9 constant COLOR_ATTACHMENT25

            0x8CFA constant COLOR_ATTACHMENT26

            0x8CFB constant COLOR_ATTACHMENT27

            0x8CFC constant COLOR_ATTACHMENT28

            0x8CFD constant COLOR_ATTACHMENT29

            0x8CFE constant COLOR_ATTACHMENT30

            0x8CFF constant COLOR_ATTACHMENT31

              0x8D00 constant DEPTH_ATTACHMENT

            0x8D20 constant STENCIL_ATTACHMENT

                   0x8D40 constant FRAMEBUFFER

                  0x8D41 constant RENDERBUFFER

            0x8D42 constant RENDERBUFFER_WIDTH

           0x8D43 constant RENDERBUFFER_HEIGHT

  0x8D44 constant RENDERBUFFER_INTERNAL_FORMAT

                0x8D46 constant STENCIL_INDEX1

                0x8D47 constant STENCIL_INDEX4

                0x8D48 constant STENCIL_INDEX8

               0x8D49 constant STENCIL_INDEX16

         0x8D50 constant RENDERBUFFER_RED_SIZE

       0x8D51 constant RENDERBUFFER_GREEN_SIZE

        0x8D52 constant RENDERBUFFER_BLUE_SIZE

       0x8D53 constant RENDERBUFFER_ALPHA_SIZE

       0x8D54 constant RENDERBUFFER_DEPTH_SIZE

     0x8D55 constant RENDERBUFFER_STENCIL_SIZE

0x8D56 constant FRAMEBUFFER_INCOMPLETE_MULTISAMPLE

                   0x8D57 constant MAX_SAMPLES

              0x8DB9 constant FRAMEBUFFER_SRGB

                    0x140B constant HALF_FLOAT

                  0x0001 constant MAP_READ_BIT

                 0x0002 constant MAP_WRITE_BIT

      0x0004 constant MAP_INVALIDATE_RANGE_BIT

     0x0008 constant MAP_INVALIDATE_BUFFER_BIT

        0x0010 constant MAP_FLUSH_EXPLICIT_BIT

        0x0020 constant MAP_UNSYNCHRONIZED_BIT

          0x8DBB constant COMPRESSED_RED_RGTC1

   0x8DBC constant COMPRESSED_SIGNED_RED_RGTC1

           0x8DBD constant COMPRESSED_RG_RGTC2

    0x8DBE constant COMPRESSED_SIGNED_RG_RGTC2

                            0x8227 constant RG

                    0x8228 constant RG_INTEGER

                            0x8229 constant R8

                           0x822A constant R16

                           0x822B constant RG8

                          0x822C constant RG16

                          0x822D constant R16F

                          0x822E constant R32F

                         0x822F constant RG16F

                         0x8230 constant RG32F

                           0x8231 constant R8I

                          0x8232 constant R8UI

                          0x8233 constant R16I

                         0x8234 constant R16UI

                          0x8235 constant R32I

                         0x8236 constant R32UI

                          0x8237 constant RG8I

                         0x8238 constant RG8UI

                         0x8239 constant RG16I

                        0x823A constant RG16UI

                         0x823B constant RG32I

                        0x823C constant RG32UI

          0x85B5 constant VERTEX_ARRAY_BINDING

1 constant VERSION_3_1

               0x8B63 constant SAMPLER_2D_RECT

        0x8B64 constant SAMPLER_2D_RECT_SHADOW

                0x8DC2 constant SAMPLER_BUFFER

           0x8DCD constant INT_SAMPLER_2D_RECT

            0x8DD0 constant INT_SAMPLER_BUFFER

  0x8DD5 constant UNSIGNED_INT_SAMPLER_2D_RECT

   0x8DD8 constant UNSIGNED_INT_SAMPLER_BUFFER

                0x8C2A constant TEXTURE_BUFFER

       0x8C2B constant MAX_TEXTURE_BUFFER_SIZE

        0x8C2C constant TEXTURE_BINDING_BUFFER

0x8C2D constant TEXTURE_BUFFER_DATA_STORE_BINDING

             0x84F5 constant TEXTURE_RECTANGLE

     0x84F6 constant TEXTURE_BINDING_RECTANGLE

       0x84F7 constant PROXY_TEXTURE_RECTANGLE

    0x84F8 constant MAX_RECTANGLE_TEXTURE_SIZE

                      0x8F94 constant R8_SNORM

                     0x8F95 constant RG8_SNORM

                    0x8F96 constant RGB8_SNORM

                   0x8F97 constant RGBA8_SNORM

                     0x8F98 constant R16_SNORM

                    0x8F99 constant RG16_SNORM

                   0x8F9A constant RGB16_SNORM

                  0x8F9B constant RGBA16_SNORM

             0x8F9C constant SIGNED_NORMALIZED

             0x8F9D constant PRIMITIVE_RESTART

       0x8F9E constant PRIMITIVE_RESTART_INDEX

              0x8F36 constant COPY_READ_BUFFER

             0x8F37 constant COPY_WRITE_BUFFER

                0x8A11 constant UNIFORM_BUFFER

        0x8A28 constant UNIFORM_BUFFER_BINDING

          0x8A29 constant UNIFORM_BUFFER_START

           0x8A2A constant UNIFORM_BUFFER_SIZE

     0x8A2B constant MAX_VERTEX_UNIFORM_BLOCKS

   0x8A2C constant MAX_GEOMETRY_UNIFORM_BLOCKS

   0x8A2D constant MAX_FRAGMENT_UNIFORM_BLOCKS

   0x8A2E constant MAX_COMBINED_UNIFORM_BLOCKS

   0x8A2F constant MAX_UNIFORM_BUFFER_BINDINGS

        0x8A30 constant MAX_UNIFORM_BLOCK_SIZE

0x8A31 constant MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS

0x8A32 constant MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS

0x8A33 constant MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS

0x8A34 constant UNIFORM_BUFFER_OFFSET_ALIGNMENT

0x8A35 constant ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH

         0x8A36 constant ACTIVE_UNIFORM_BLOCKS

                  0x8A37 constant UNIFORM_TYPE

                  0x8A38 constant UNIFORM_SIZE

           0x8A39 constant UNIFORM_NAME_LENGTH

           0x8A3A constant UNIFORM_BLOCK_INDEX

                0x8A3B constant UNIFORM_OFFSET

          0x8A3C constant UNIFORM_ARRAY_STRIDE

         0x8A3D constant UNIFORM_MATRIX_STRIDE

          0x8A3E constant UNIFORM_IS_ROW_MAJOR

         0x8A3F constant UNIFORM_BLOCK_BINDING

       0x8A40 constant UNIFORM_BLOCK_DATA_SIZE

     0x8A41 constant UNIFORM_BLOCK_NAME_LENGTH

 0x8A42 constant UNIFORM_BLOCK_ACTIVE_UNIFORMS

0x8A43 constant UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES

0x8A44 constant UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER

0x8A45 constant UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER

0x8A46 constant UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER

                 0xFFFFFFFF constant INVALID_INDEX

1 constant VERSION_3_2

      0x00000001 constant CONTEXT_CORE_PROFILE_BIT

0x00000002 constant CONTEXT_COMPATIBILITY_PROFILE_BIT

               0x000A constant LINES_ADJACENCY

          0x000B constant LINE_STRIP_ADJACENCY

           0x000C constant TRIANGLES_ADJACENCY

      0x000D constant TRIANGLE_STRIP_ADJACENCY

            0x8642 constant PROGRAM_POINT_SIZE

0x8C29 constant MAX_GEOMETRY_TEXTURE_IMAGE_UNITS

0x8DA7 constant FRAMEBUFFER_ATTACHMENT_LAYERED

0x8DA8 constant FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS

               0x8DD9 constant GEOMETRY_SHADER

         0x8916 constant GEOMETRY_VERTICES_OUT

           0x8917 constant GEOMETRY_INPUT_TYPE

          0x8918 constant GEOMETRY_OUTPUT_TYPE

0x8DDF constant MAX_GEOMETRY_UNIFORM_COMPONENTS

  0x8DE0 constant MAX_GEOMETRY_OUTPUT_VERTICES

0x8DE1 constant MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS

  0x9122 constant MAX_VERTEX_OUTPUT_COMPONENTS

 0x9123 constant MAX_GEOMETRY_INPUT_COMPONENTS

0x9124 constant MAX_GEOMETRY_OUTPUT_COMPONENTS

 0x9125 constant MAX_FRAGMENT_INPUT_COMPONENTS

          0x9126 constant CONTEXT_PROFILE_MASK

                   0x864F constant DEPTH_CLAMP

0x8E4C constant QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION

       0x8E4D constant FIRST_VERTEX_CONVENTION

        0x8E4E constant LAST_VERTEX_CONVENTION

              0x8E4F constant PROVOKING_VERTEX

     0x884F constant TEXTURE_CUBE_MAP_SEAMLESS

       0x9111 constant MAX_SERVER_WAIT_TIMEOUT

                   0x9112 constant OBJECT_TYPE

                0x9113 constant SYNC_CONDITION

                   0x9114 constant SYNC_STATUS

                    0x9115 constant SYNC_FLAGS

                    0x9116 constant SYNC_FENCE

    0x9117 constant SYNC_GPU_COMMANDS_COMPLETE

                    0x9118 constant UNSIGNALED

                      0x9119 constant SIGNALED

              0x911A constant ALREADY_SIGNALED

               0x911B constant TIMEOUT_EXPIRED

           0x911C constant CONDITION_SATISFIED

                   0x911D constant WAIT_FAILED

               0xFFFFFFFFFFFFFFFF constant TIMEOUT_IGNORED

       0x00000001 constant SYNC_FLUSH_COMMANDS_BIT

               0x8E50 constant SAMPLE_POSITION

                   0x8E51 constant SAMPLE_MASK

             0x8E52 constant SAMPLE_MASK_VALUE

         0x8E59 constant MAX_SAMPLE_MASK_WORDS

        0x9100 constant TEXTURE_2D_MULTISAMPLE

  0x9101 constant PROXY_TEXTURE_2D_MULTISAMPLE

  0x9102 constant TEXTURE_2D_MULTISAMPLE_ARRAY

0x9103 constant PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY

0x9104 constant TEXTURE_BINDING_2D_MULTISAMPLE

0x9105 constant TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY

               0x9106 constant TEXTURE_SAMPLES

0x9107 constant TEXTURE_FIXED_SAMPLE_LOCATIONS

        0x9108 constant SAMPLER_2D_MULTISAMPLE

    0x9109 constant INT_SAMPLER_2D_MULTISAMPLE

0x910A constant UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE

  0x910B constant SAMPLER_2D_MULTISAMPLE_ARRAY

0x910C constant INT_SAMPLER_2D_MULTISAMPLE_ARRAY

0x910D constant UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY

     0x910E constant MAX_COLOR_TEXTURE_SAMPLES

     0x910F constant MAX_DEPTH_TEXTURE_SAMPLES

           0x9110 constant MAX_INTEGER_SAMPLES

1 constant VERSION_3_3

   0x88FE constant VERTEX_ATTRIB_ARRAY_DIVISOR

                    0x88F9 constant SRC1_COLOR

          0x88FA constant ONE_MINUS_SRC1_COLOR

          0x88FB constant ONE_MINUS_SRC1_ALPHA

  0x88FC constant MAX_DUAL_SOURCE_DRAW_BUFFERS

            0x8C2F constant ANY_SAMPLES_PASSED

               0x8919 constant SAMPLER_BINDING

                    0x906F constant RGB10_A2UI

             0x8E42 constant TEXTURE_SWIZZLE_R

             0x8E43 constant TEXTURE_SWIZZLE_G

             0x8E44 constant TEXTURE_SWIZZLE_B

             0x8E45 constant TEXTURE_SWIZZLE_A

          0x8E46 constant TEXTURE_SWIZZLE_RGBA

                  0x88BF constant TIME_ELAPSED

                     0x8E28 constant TIMESTAMP

            0x8D9F constant INT_2_10_10_10_REV

1 constant VERSION_4_0

                0x8C36 constant SAMPLE_SHADING

      0x8C37 constant MIN_SAMPLE_SHADING_VALUE

0x8E5E constant MIN_PROGRAM_TEXTURE_GATHER_OFFSET

0x8E5F constant MAX_PROGRAM_TEXTURE_GATHER_OFFSET

        0x9009 constant TEXTURE_CUBE_MAP_ARRAY

0x900A constant TEXTURE_BINDING_CUBE_MAP_ARRAY

  0x900B constant PROXY_TEXTURE_CUBE_MAP_ARRAY

        0x900C constant SAMPLER_CUBE_MAP_ARRAY

 0x900D constant SAMPLER_CUBE_MAP_ARRAY_SHADOW

    0x900E constant INT_SAMPLER_CUBE_MAP_ARRAY

0x900F constant UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY

          0x8F3F constant DRAW_INDIRECT_BUFFER

  0x8F43 constant DRAW_INDIRECT_BUFFER_BINDING

   0x887F constant GEOMETRY_SHADER_INVOCATIONS

0x8E5A constant MAX_GEOMETRY_SHADER_INVOCATIONS

0x8E5B constant MIN_FRAGMENT_INTERPOLATION_OFFSET

0x8E5C constant MAX_FRAGMENT_INTERPOLATION_OFFSET

0x8E5D constant FRAGMENT_INTERPOLATION_OFFSET_BITS

            0x8E71 constant MAX_VERTEX_STREAMS

                   0x8FFC constant DOUBLE_VEC2

                   0x8FFD constant DOUBLE_VEC3

                   0x8FFE constant DOUBLE_VEC4

                   0x8F46 constant DOUBLE_MAT2

                   0x8F47 constant DOUBLE_MAT3

                   0x8F48 constant DOUBLE_MAT4

                 0x8F49 constant DOUBLE_MAT2x3

                 0x8F4A constant DOUBLE_MAT2x4

                 0x8F4B constant DOUBLE_MAT3x2

                 0x8F4C constant DOUBLE_MAT3x4

                 0x8F4D constant DOUBLE_MAT4x2

                 0x8F4E constant DOUBLE_MAT4x3

            0x8DE5 constant ACTIVE_SUBROUTINES

    0x8DE6 constant ACTIVE_SUBROUTINE_UNIFORMS

0x8E47 constant ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS

  0x8E48 constant ACTIVE_SUBROUTINE_MAX_LENGTH

0x8E49 constant ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH

               0x8DE7 constant MAX_SUBROUTINES

0x8DE8 constant MAX_SUBROUTINE_UNIFORM_LOCATIONS

    0x8E4A constant NUM_COMPATIBLE_SUBROUTINES

        0x8E4B constant COMPATIBLE_SUBROUTINES

                       0x000E constant PATCHES

                0x8E72 constant PATCH_VERTICES

     0x8E73 constant PATCH_DEFAULT_INNER_LEVEL

     0x8E74 constant PATCH_DEFAULT_OUTER_LEVEL

  0x8E75 constant TESS_CONTROL_OUTPUT_VERTICES

                 0x8E76 constant TESS_GEN_MODE

              0x8E77 constant TESS_GEN_SPACING

         0x8E78 constant TESS_GEN_VERTEX_ORDER

           0x8E79 constant TESS_GEN_POINT_MODE

                      0x8E7A constant ISOLINES

                0x8E7B constant FRACTIONAL_ODD

               0x8E7C constant FRACTIONAL_EVEN

            0x8E7D constant MAX_PATCH_VERTICES

            0x8E7E constant MAX_TESS_GEN_LEVEL

0x8E7F constant MAX_TESS_CONTROL_UNIFORM_COMPONENTS

0x8E80 constant MAX_TESS_EVALUATION_UNIFORM_COMPONENTS

0x8E81 constant MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS

0x8E82 constant MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS

0x8E83 constant MAX_TESS_CONTROL_OUTPUT_COMPONENTS

     0x8E84 constant MAX_TESS_PATCH_COMPONENTS

0x8E85 constant MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS

0x8E86 constant MAX_TESS_EVALUATION_OUTPUT_COMPONENTS

0x8E89 constant MAX_TESS_CONTROL_UNIFORM_BLOCKS

0x8E8A constant MAX_TESS_EVALUATION_UNIFORM_BLOCKS

0x886C constant MAX_TESS_CONTROL_INPUT_COMPONENTS

0x886D constant MAX_TESS_EVALUATION_INPUT_COMPONENTS

0x8E1E constant MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS

0x8E1F constant MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS

0x84F0 constant UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER

0x84F1 constant UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER

        0x8E87 constant TESS_EVALUATION_SHADER

           0x8E88 constant TESS_CONTROL_SHADER

            0x8E22 constant TRANSFORM_FEEDBACK

0x8E23 constant TRANSFORM_FEEDBACK_BUFFER_PAUSED

0x8E24 constant TRANSFORM_FEEDBACK_BUFFER_ACTIVE

    0x8E25 constant TRANSFORM_FEEDBACK_BINDING

0x8E70 constant MAX_TRANSFORM_FEEDBACK_BUFFERS

1 constant VERSION_4_1

                         0x140C constant FIXED

0x8B9A constant IMPLEMENTATION_COLOR_READ_TYPE

0x8B9B constant IMPLEMENTATION_COLOR_READ_FORMAT

                     0x8DF0 constant LOW_FLOAT

                  0x8DF1 constant MEDIUM_FLOAT

                    0x8DF2 constant HIGH_FLOAT

                       0x8DF3 constant LOW_INT

                    0x8DF4 constant MEDIUM_INT

                      0x8DF5 constant HIGH_INT

               0x8DFA constant SHADER_COMPILER

         0x8DF8 constant SHADER_BINARY_FORMATS

     0x8DF9 constant NUM_SHADER_BINARY_FORMATS

    0x8DFB constant MAX_VERTEX_UNIFORM_VECTORS

           0x8DFC constant MAX_VARYING_VECTORS

  0x8DFD constant MAX_FRAGMENT_UNIFORM_VECTORS

                        0x8D62 constant RGB565

0x8257 constant PROGRAM_BINARY_RETRIEVABLE_HINT

         0x8741 constant PROGRAM_BINARY_LENGTH

    0x87FE constant NUM_PROGRAM_BINARY_FORMATS

        0x87FF constant PROGRAM_BINARY_FORMATS

             0x00000001 constant VERTEX_SHADER_BIT

           0x00000002 constant FRAGMENT_SHADER_BIT

           0x00000004 constant GEOMETRY_SHADER_BIT

       0x00000008 constant TESS_CONTROL_SHADER_BIT

    0x00000010 constant TESS_EVALUATION_SHADER_BIT

               0xFFFFFFFF constant ALL_SHADER_BITS

             0x8258 constant PROGRAM_SEPARABLE

                0x8259 constant ACTIVE_PROGRAM

      0x825A constant PROGRAM_PIPELINE_BINDING

                 0x825B constant MAX_VIEWPORTS

        0x825C constant VIEWPORT_SUBPIXEL_BITS

         0x825D constant VIEWPORT_BOUNDS_RANGE

        0x825E constant LAYER_PROVOKING_VERTEX

0x825F constant VIEWPORT_INDEX_PROVOKING_VERTEX

              0x8260 constant UNDEFINED_VERTEX

1 constant VERSION_4_2

      0x8F36 constant COPY_READ_BUFFER_BINDING

     0x8F37 constant COPY_WRITE_BUFFER_BINDING

     0x8E24 constant TRANSFORM_FEEDBACK_ACTIVE

     0x8E23 constant TRANSFORM_FEEDBACK_PAUSED

 0x9127 constant UNPACK_COMPRESSED_BLOCK_WIDTH

0x9128 constant UNPACK_COMPRESSED_BLOCK_HEIGHT

 0x9129 constant UNPACK_COMPRESSED_BLOCK_DEPTH

  0x912A constant UNPACK_COMPRESSED_BLOCK_SIZE

   0x912B constant PACK_COMPRESSED_BLOCK_WIDTH

  0x912C constant PACK_COMPRESSED_BLOCK_HEIGHT

   0x912D constant PACK_COMPRESSED_BLOCK_DEPTH

    0x912E constant PACK_COMPRESSED_BLOCK_SIZE

             0x9380 constant NUM_SAMPLE_COUNTS

      0x90BC constant MIN_MAP_BUFFER_ALIGNMENT

         0x92C0 constant ATOMIC_COUNTER_BUFFER

 0x92C1 constant ATOMIC_COUNTER_BUFFER_BINDING

   0x92C2 constant ATOMIC_COUNTER_BUFFER_START

    0x92C3 constant ATOMIC_COUNTER_BUFFER_SIZE

0x92C4 constant ATOMIC_COUNTER_BUFFER_DATA_SIZE

0x92C5 constant ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS

0x92C6 constant ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES

0x92C7 constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER

0x92C8 constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER

0x92C9 constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER

0x92CA constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER

0x92CB constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER

0x92CC constant MAX_VERTEX_ATOMIC_COUNTER_BUFFERS

0x92CD constant MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS

0x92CE constant MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS

0x92CF constant MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS

0x92D0 constant MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS

0x92D1 constant MAX_COMBINED_ATOMIC_COUNTER_BUFFERS

    0x92D2 constant MAX_VERTEX_ATOMIC_COUNTERS

0x92D3 constant MAX_TESS_CONTROL_ATOMIC_COUNTERS

0x92D4 constant MAX_TESS_EVALUATION_ATOMIC_COUNTERS

  0x92D5 constant MAX_GEOMETRY_ATOMIC_COUNTERS

  0x92D6 constant MAX_FRAGMENT_ATOMIC_COUNTERS

  0x92D7 constant MAX_COMBINED_ATOMIC_COUNTERS

0x92D8 constant MAX_ATOMIC_COUNTER_BUFFER_SIZE

0x92DC constant MAX_ATOMIC_COUNTER_BUFFER_BINDINGS

 0x92D9 constant ACTIVE_ATOMIC_COUNTER_BUFFERS

0x92DA constant UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX

   0x92DB constant UNSIGNED_INT_ATOMIC_COUNTER

0x00000001 constant VERTEX_ATTRIB_ARRAY_BARRIER_BIT

     0x00000002 constant ELEMENT_ARRAY_BARRIER_BIT

           0x00000004 constant UNIFORM_BARRIER_BIT

     0x00000008 constant TEXTURE_FETCH_BARRIER_BIT

0x00000020 constant SHADER_IMAGE_ACCESS_BARRIER_BIT

           0x00000040 constant COMMAND_BARRIER_BIT

      0x00000080 constant PIXEL_BUFFER_BARRIER_BIT

    0x00000100 constant TEXTURE_UPDATE_BARRIER_BIT

     0x00000200 constant BUFFER_UPDATE_BARRIER_BIT

       0x00000400 constant FRAMEBUFFER_BARRIER_BIT

0x00000800 constant TRANSFORM_FEEDBACK_BARRIER_BIT

    0x00001000 constant ATOMIC_COUNTER_BARRIER_BIT

              0xFFFFFFFF constant ALL_BARRIER_BITS

               0x8F38 constant MAX_IMAGE_UNITS

0x8F39 constant MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS

            0x8F3A constant IMAGE_BINDING_NAME

           0x8F3B constant IMAGE_BINDING_LEVEL

         0x8F3C constant IMAGE_BINDING_LAYERED

           0x8F3D constant IMAGE_BINDING_LAYER

          0x8F3E constant IMAGE_BINDING_ACCESS

                      0x904C constant IMAGE_1D

                      0x904D constant IMAGE_2D

                      0x904E constant IMAGE_3D

                 0x904F constant IMAGE_2D_RECT

                    0x9050 constant IMAGE_CUBE

                  0x9051 constant IMAGE_BUFFER

                0x9052 constant IMAGE_1D_ARRAY

                0x9053 constant IMAGE_2D_ARRAY

          0x9054 constant IMAGE_CUBE_MAP_ARRAY

          0x9055 constant IMAGE_2D_MULTISAMPLE

    0x9056 constant IMAGE_2D_MULTISAMPLE_ARRAY

                  0x9057 constant INT_IMAGE_1D

                  0x9058 constant INT_IMAGE_2D

                  0x9059 constant INT_IMAGE_3D

             0x905A constant INT_IMAGE_2D_RECT

                0x905B constant INT_IMAGE_CUBE

              0x905C constant INT_IMAGE_BUFFER

            0x905D constant INT_IMAGE_1D_ARRAY

            0x905E constant INT_IMAGE_2D_ARRAY

      0x905F constant INT_IMAGE_CUBE_MAP_ARRAY

      0x9060 constant INT_IMAGE_2D_MULTISAMPLE

0x9061 constant INT_IMAGE_2D_MULTISAMPLE_ARRAY

         0x9062 constant UNSIGNED_INT_IMAGE_1D

         0x9063 constant UNSIGNED_INT_IMAGE_2D

         0x9064 constant UNSIGNED_INT_IMAGE_3D

    0x9065 constant UNSIGNED_INT_IMAGE_2D_RECT

       0x9066 constant UNSIGNED_INT_IMAGE_CUBE

     0x9067 constant UNSIGNED_INT_IMAGE_BUFFER

   0x9068 constant UNSIGNED_INT_IMAGE_1D_ARRAY

   0x9069 constant UNSIGNED_INT_IMAGE_2D_ARRAY

0x906A constant UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY

0x906B constant UNSIGNED_INT_IMAGE_2D_MULTISAMPLE

0x906C constant UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY

             0x906D constant MAX_IMAGE_SAMPLES

          0x906E constant IMAGE_BINDING_FORMAT

0x90C7 constant IMAGE_FORMAT_COMPATIBILITY_TYPE

0x90C8 constant IMAGE_FORMAT_COMPATIBILITY_BY_SIZE

0x90C9 constant IMAGE_FORMAT_COMPATIBILITY_BY_CLASS

     0x90CA constant MAX_VERTEX_IMAGE_UNIFORMS

0x90CB constant MAX_TESS_CONTROL_IMAGE_UNIFORMS

0x90CC constant MAX_TESS_EVALUATION_IMAGE_UNIFORMS

   0x90CD constant MAX_GEOMETRY_IMAGE_UNIFORMS

   0x90CE constant MAX_FRAGMENT_IMAGE_UNIFORMS

   0x90CF constant MAX_COMBINED_IMAGE_UNIFORMS

    0x8E8C constant COMPRESSED_RGBA_BPTC_UNORM

0x8E8D constant COMPRESSED_SRGB_ALPHA_BPTC_UNORM

0x8E8E constant COMPRESSED_RGB_BPTC_SIGNED_FLOAT

0x8E8F constant COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT

      0x912F constant TEXTURE_IMMUTABLE_FORMAT

1 constant VERSION_4_3

 0x82E9 constant NUM_SHADING_LANGUAGE_VERSIONS

      0x874E constant VERTEX_ATTRIB_ARRAY_LONG

          0x9274 constant COMPRESSED_RGB8_ETC2

         0x9275 constant COMPRESSED_SRGB8_ETC2

0x9276 constant COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2

0x9277 constant COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2

     0x9278 constant COMPRESSED_RGBA8_ETC2_EAC

0x9279 constant COMPRESSED_SRGB8_ALPHA8_ETC2_EAC

            0x9270 constant COMPRESSED_R11_EAC

     0x9271 constant COMPRESSED_SIGNED_R11_EAC

           0x9272 constant COMPRESSED_RG11_EAC

    0x9273 constant COMPRESSED_SIGNED_RG11_EAC

 0x8D69 constant PRIMITIVE_RESTART_FIXED_INDEX

0x8D6A constant ANY_SAMPLES_PASSED_CONSERVATIVE

             0x8D6B constant MAX_ELEMENT_INDEX

                0x91B9 constant COMPUTE_SHADER

    0x91BB constant MAX_COMPUTE_UNIFORM_BLOCKS

0x91BC constant MAX_COMPUTE_TEXTURE_IMAGE_UNITS

    0x91BD constant MAX_COMPUTE_IMAGE_UNIFORMS

0x8262 constant MAX_COMPUTE_SHARED_MEMORY_SIZE

0x8263 constant MAX_COMPUTE_UNIFORM_COMPONENTS

0x8264 constant MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS

   0x8265 constant MAX_COMPUTE_ATOMIC_COUNTERS

0x8266 constant MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS

0x90EB constant MAX_COMPUTE_WORK_GROUP_INVOCATIONS

  0x91BE constant MAX_COMPUTE_WORK_GROUP_COUNT

   0x91BF constant MAX_COMPUTE_WORK_GROUP_SIZE

       0x8267 constant COMPUTE_WORK_GROUP_SIZE

0x90EC constant UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER

0x90ED constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER

      0x90EE constant DISPATCH_INDIRECT_BUFFER

0x90EF constant DISPATCH_INDIRECT_BUFFER_BINDING

            0x00000020 constant COMPUTE_SHADER_BIT

      0x8242 constant DEBUG_OUTPUT_SYNCHRONOUS

0x8243 constant DEBUG_NEXT_LOGGED_MESSAGE_LENGTH

       0x8244 constant DEBUG_CALLBACK_FUNCTION

     0x8245 constant DEBUG_CALLBACK_USER_PARAM

              0x8246 constant DEBUG_SOURCE_API

    0x8247 constant DEBUG_SOURCE_WINDOW_SYSTEM

  0x8248 constant DEBUG_SOURCE_SHADER_COMPILER

      0x8249 constant DEBUG_SOURCE_THIRD_PARTY

      0x824A constant DEBUG_SOURCE_APPLICATION

            0x824B constant DEBUG_SOURCE_OTHER

              0x824C constant DEBUG_TYPE_ERROR

0x824D constant DEBUG_TYPE_DEPRECATED_BEHAVIOR

 0x824E constant DEBUG_TYPE_UNDEFINED_BEHAVIOR

        0x824F constant DEBUG_TYPE_PORTABILITY

        0x8250 constant DEBUG_TYPE_PERFORMANCE

              0x8251 constant DEBUG_TYPE_OTHER

      0x9143 constant MAX_DEBUG_MESSAGE_LENGTH

     0x9144 constant MAX_DEBUG_LOGGED_MESSAGES

         0x9145 constant DEBUG_LOGGED_MESSAGES

           0x9146 constant DEBUG_SEVERITY_HIGH

         0x9147 constant DEBUG_SEVERITY_MEDIUM

            0x9148 constant DEBUG_SEVERITY_LOW

             0x8268 constant DEBUG_TYPE_MARKER

         0x8269 constant DEBUG_TYPE_PUSH_GROUP

          0x826A constant DEBUG_TYPE_POP_GROUP

   0x826B constant DEBUG_SEVERITY_NOTIFICATION

   0x826C constant MAX_DEBUG_GROUP_STACK_DEPTH

       0x826D constant DEBUG_GROUP_STACK_DEPTH

                        0x82E0 constant BUFFER

                        0x82E1 constant SHADER

                       0x82E2 constant PROGRAM

                         0x82E3 constant QUERY

              0x82E4 constant PROGRAM_PIPELINE

                       0x82E6 constant SAMPLER

              0x82E8 constant MAX_LABEL_LENGTH

                  0x92E0 constant DEBUG_OUTPUT

        0x00000002 constant CONTEXT_FLAG_DEBUG_BIT

         0x826E constant MAX_UNIFORM_LOCATIONS

     0x9310 constant FRAMEBUFFER_DEFAULT_WIDTH

    0x9311 constant FRAMEBUFFER_DEFAULT_HEIGHT

    0x9312 constant FRAMEBUFFER_DEFAULT_LAYERS

   0x9313 constant FRAMEBUFFER_DEFAULT_SAMPLES

0x9314 constant FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS

         0x9315 constant MAX_FRAMEBUFFER_WIDTH

        0x9316 constant MAX_FRAMEBUFFER_HEIGHT

        0x9317 constant MAX_FRAMEBUFFER_LAYERS

       0x9318 constant MAX_FRAMEBUFFER_SAMPLES

      0x826F constant INTERNALFORMAT_SUPPORTED

      0x8270 constant INTERNALFORMAT_PREFERRED

       0x8271 constant INTERNALFORMAT_RED_SIZE

     0x8272 constant INTERNALFORMAT_GREEN_SIZE

      0x8273 constant INTERNALFORMAT_BLUE_SIZE

     0x8274 constant INTERNALFORMAT_ALPHA_SIZE

     0x8275 constant INTERNALFORMAT_DEPTH_SIZE

   0x8276 constant INTERNALFORMAT_STENCIL_SIZE

    0x8277 constant INTERNALFORMAT_SHARED_SIZE

       0x8278 constant INTERNALFORMAT_RED_TYPE

     0x8279 constant INTERNALFORMAT_GREEN_TYPE

      0x827A constant INTERNALFORMAT_BLUE_TYPE

     0x827B constant INTERNALFORMAT_ALPHA_TYPE

     0x827C constant INTERNALFORMAT_DEPTH_TYPE

   0x827D constant INTERNALFORMAT_STENCIL_TYPE

                     0x827E constant MAX_WIDTH

                    0x827F constant MAX_HEIGHT

                     0x8280 constant MAX_DEPTH

                    0x8281 constant MAX_LAYERS

       0x8282 constant MAX_COMBINED_DIMENSIONS

              0x8283 constant COLOR_COMPONENTS

              0x8284 constant DEPTH_COMPONENTS

            0x8285 constant STENCIL_COMPONENTS

              0x8286 constant COLOR_RENDERABLE

              0x8287 constant DEPTH_RENDERABLE

            0x8288 constant STENCIL_RENDERABLE

        0x8289 constant FRAMEBUFFER_RENDERABLE

0x828A constant FRAMEBUFFER_RENDERABLE_LAYERED

             0x828B constant FRAMEBUFFER_BLEND

                   0x828C constant READ_PIXELS

            0x828D constant READ_PIXELS_FORMAT

              0x828E constant READ_PIXELS_TYPE

          0x828F constant TEXTURE_IMAGE_FORMAT

            0x8290 constant TEXTURE_IMAGE_TYPE

      0x8291 constant GET_TEXTURE_IMAGE_FORMAT

        0x8292 constant GET_TEXTURE_IMAGE_TYPE

                        0x8293 constant MIPMAP

        0x8294 constant MANUAL_GENERATE_MIPMAP

          0x8295 constant AUTO_GENERATE_MIPMAP

                0x8296 constant COLOR_ENCODING

                     0x8297 constant SRGB_READ

                    0x8298 constant SRGB_WRITE

                        0x829A constant FILTER

                0x829B constant VERTEX_TEXTURE

          0x829C constant TESS_CONTROL_TEXTURE

       0x829D constant TESS_EVALUATION_TEXTURE

              0x829E constant GEOMETRY_TEXTURE

              0x829F constant FRAGMENT_TEXTURE

               0x82A0 constant COMPUTE_TEXTURE

                0x82A1 constant TEXTURE_SHADOW

                0x82A2 constant TEXTURE_GATHER

         0x82A3 constant TEXTURE_GATHER_SHADOW

             0x82A4 constant SHADER_IMAGE_LOAD

            0x82A5 constant SHADER_IMAGE_STORE

           0x82A6 constant SHADER_IMAGE_ATOMIC

              0x82A7 constant IMAGE_TEXEL_SIZE

     0x82A8 constant IMAGE_COMPATIBILITY_CLASS

            0x82A9 constant IMAGE_PIXEL_FORMAT

              0x82AA constant IMAGE_PIXEL_TYPE

0x82AC constant SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST

0x82AD constant SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST

0x82AE constant SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE

0x82AF constant SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE

0x82B1 constant TEXTURE_COMPRESSED_BLOCK_WIDTH

0x82B2 constant TEXTURE_COMPRESSED_BLOCK_HEIGHT

 0x82B3 constant TEXTURE_COMPRESSED_BLOCK_SIZE

                  0x82B4 constant CLEAR_BUFFER

                  0x82B5 constant TEXTURE_VIEW

      0x82B6 constant VIEW_COMPATIBILITY_CLASS

                  0x82B7 constant FULL_SUPPORT

                0x82B8 constant CAVEAT_SUPPORT

            0x82B9 constant IMAGE_CLASS_4_X_32

            0x82BA constant IMAGE_CLASS_2_X_32

            0x82BB constant IMAGE_CLASS_1_X_32

            0x82BC constant IMAGE_CLASS_4_X_16

            0x82BD constant IMAGE_CLASS_2_X_16

            0x82BE constant IMAGE_CLASS_1_X_16

             0x82BF constant IMAGE_CLASS_4_X_8

             0x82C0 constant IMAGE_CLASS_2_X_8

             0x82C1 constant IMAGE_CLASS_1_X_8

          0x82C2 constant IMAGE_CLASS_11_11_10

        0x82C3 constant IMAGE_CLASS_10_10_10_2

           0x82C4 constant VIEW_CLASS_128_BITS

            0x82C5 constant VIEW_CLASS_96_BITS

            0x82C6 constant VIEW_CLASS_64_BITS

            0x82C7 constant VIEW_CLASS_48_BITS

            0x82C8 constant VIEW_CLASS_32_BITS

            0x82C9 constant VIEW_CLASS_24_BITS

            0x82CA constant VIEW_CLASS_16_BITS

             0x82CB constant VIEW_CLASS_8_BITS

      0x82CC constant VIEW_CLASS_S3TC_DXT1_RGB

     0x82CD constant VIEW_CLASS_S3TC_DXT1_RGBA

     0x82CE constant VIEW_CLASS_S3TC_DXT3_RGBA

     0x82CF constant VIEW_CLASS_S3TC_DXT5_RGBA

          0x82D0 constant VIEW_CLASS_RGTC1_RED

           0x82D1 constant VIEW_CLASS_RGTC2_RG

         0x82D2 constant VIEW_CLASS_BPTC_UNORM

         0x82D3 constant VIEW_CLASS_BPTC_FLOAT

                       0x92E1 constant UNIFORM

                 0x92E2 constant UNIFORM_BLOCK

                 0x92E3 constant PROGRAM_INPUT

                0x92E4 constant PROGRAM_OUTPUT

               0x92E5 constant BUFFER_VARIABLE

          0x92E6 constant SHADER_STORAGE_BLOCK

             0x92E8 constant VERTEX_SUBROUTINE

       0x92E9 constant TESS_CONTROL_SUBROUTINE

    0x92EA constant TESS_EVALUATION_SUBROUTINE

           0x92EB constant GEOMETRY_SUBROUTINE

           0x92EC constant FRAGMENT_SUBROUTINE

            0x92ED constant COMPUTE_SUBROUTINE

     0x92EE constant VERTEX_SUBROUTINE_UNIFORM

0x92EF constant TESS_CONTROL_SUBROUTINE_UNIFORM

0x92F0 constant TESS_EVALUATION_SUBROUTINE_UNIFORM

   0x92F1 constant GEOMETRY_SUBROUTINE_UNIFORM

   0x92F2 constant FRAGMENT_SUBROUTINE_UNIFORM

    0x92F3 constant COMPUTE_SUBROUTINE_UNIFORM

    0x92F4 constant TRANSFORM_FEEDBACK_VARYING

              0x92F5 constant ACTIVE_RESOURCES

               0x92F6 constant MAX_NAME_LENGTH

      0x92F7 constant MAX_NUM_ACTIVE_VARIABLES

0x92F8 constant MAX_NUM_COMPATIBLE_SUBROUTINES

                   0x92F9 constant NAME_LENGTH

                          0x92FA constant TYPE

                    0x92FB constant ARRAY_SIZE

                        0x92FC constant OFFSET

                   0x92FD constant BLOCK_INDEX

                  0x92FE constant ARRAY_STRIDE

                 0x92FF constant MATRIX_STRIDE

                  0x9300 constant IS_ROW_MAJOR

   0x9301 constant ATOMIC_COUNTER_BUFFER_INDEX

                0x9302 constant BUFFER_BINDING

              0x9303 constant BUFFER_DATA_SIZE

          0x9304 constant NUM_ACTIVE_VARIABLES

              0x9305 constant ACTIVE_VARIABLES

   0x9306 constant REFERENCED_BY_VERTEX_SHADER

0x9307 constant REFERENCED_BY_TESS_CONTROL_SHADER

0x9308 constant REFERENCED_BY_TESS_EVALUATION_SHADER

 0x9309 constant REFERENCED_BY_GEOMETRY_SHADER

 0x930A constant REFERENCED_BY_FRAGMENT_SHADER

  0x930B constant REFERENCED_BY_COMPUTE_SHADER

          0x930C constant TOP_LEVEL_ARRAY_SIZE

        0x930D constant TOP_LEVEL_ARRAY_STRIDE

                      0x930E constant LOCATION

                0x930F constant LOCATION_INDEX

                  0x92E7 constant IS_PER_PATCH

         0x90D2 constant SHADER_STORAGE_BUFFER

 0x90D3 constant SHADER_STORAGE_BUFFER_BINDING

   0x90D4 constant SHADER_STORAGE_BUFFER_START

    0x90D5 constant SHADER_STORAGE_BUFFER_SIZE

0x90D6 constant MAX_VERTEX_SHADER_STORAGE_BLOCKS

0x90D7 constant MAX_GEOMETRY_SHADER_STORAGE_BLOCKS

0x90D8 constant MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS

0x90D9 constant MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS

0x90DA constant MAX_FRAGMENT_SHADER_STORAGE_BLOCKS

0x90DB constant MAX_COMPUTE_SHADER_STORAGE_BLOCKS

0x90DC constant MAX_COMBINED_SHADER_STORAGE_BLOCKS

0x90DD constant MAX_SHADER_STORAGE_BUFFER_BINDINGS

 0x90DE constant MAX_SHADER_STORAGE_BLOCK_SIZE

0x90DF constant SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT

    0x00002000 constant SHADER_STORAGE_BARRIER_BIT

0x8F39 constant MAX_COMBINED_SHADER_OUTPUT_RESOURCES

    0x90EA constant DEPTH_STENCIL_TEXTURE_MODE

         0x919D constant TEXTURE_BUFFER_OFFSET

           0x919E constant TEXTURE_BUFFER_SIZE

0x919F constant TEXTURE_BUFFER_OFFSET_ALIGNMENT

        0x82DB constant TEXTURE_VIEW_MIN_LEVEL

       0x82DC constant TEXTURE_VIEW_NUM_LEVELS

        0x82DD constant TEXTURE_VIEW_MIN_LAYER

       0x82DE constant TEXTURE_VIEW_NUM_LAYERS

      0x82DF constant TEXTURE_IMMUTABLE_LEVELS

         0x82D4 constant VERTEX_ATTRIB_BINDING

 0x82D5 constant VERTEX_ATTRIB_RELATIVE_OFFSET

        0x82D6 constant VERTEX_BINDING_DIVISOR

         0x82D7 constant VERTEX_BINDING_OFFSET

         0x82D8 constant VERTEX_BINDING_STRIDE

0x82D9 constant MAX_VERTEX_ATTRIB_RELATIVE_OFFSET

    0x82DA constant MAX_VERTEX_ATTRIB_BINDINGS

         0x8F4F constant VERTEX_BINDING_BUFFER

1 constant VERSION_4_4

      0x82E5 constant MAX_VERTEX_ATTRIB_STRIDE

0x8221 constant PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED

        0x8C2A constant TEXTURE_BUFFER_BINDING

            0x0040 constant MAP_PERSISTENT_BIT

              0x0080 constant MAP_COHERENT_BIT

           0x0100 constant DYNAMIC_STORAGE_BIT

            0x0200 constant CLIENT_STORAGE_BIT

0x00004000 constant CLIENT_MAPPED_BUFFER_BARRIER_BIT

      0x821F constant BUFFER_IMMUTABLE_STORAGE

          0x8220 constant BUFFER_STORAGE_FLAGS

                 0x9365 constant CLEAR_TEXTURE

            0x934A constant LOCATION_COMPONENT

0x934B constant TRANSFORM_FEEDBACK_BUFFER_INDEX

0x934C constant TRANSFORM_FEEDBACK_BUFFER_STRIDE

                  0x9192 constant QUERY_BUFFER

      0x00008000 constant QUERY_BUFFER_BARRIER_BIT

          0x9193 constant QUERY_BUFFER_BINDING

          0x9194 constant QUERY_RESULT_NO_WAIT

          0x8743 constant MIRROR_CLAMP_TO_EDGE

1 constant VERSION_4_5

                  0x0507 constant CONTEXT_LOST

           0x935E constant NEGATIVE_ONE_TO_ONE

                   0x935F constant ZERO_TO_ONE

                   0x935C constant CLIP_ORIGIN

               0x935D constant CLIP_DEPTH_MODE

           0x8E17 constant QUERY_WAIT_INVERTED

        0x8E18 constant QUERY_NO_WAIT_INVERTED

 0x8E19 constant QUERY_BY_REGION_WAIT_INVERTED

0x8E1A constant QUERY_BY_REGION_NO_WAIT_INVERTED

            0x82F9 constant MAX_CULL_DISTANCES

0x82FA constant MAX_COMBINED_CLIP_AND_CULL_DISTANCES

                0x1006 constant TEXTURE_TARGET

                  0x82EA constant QUERY_TARGET

          0x8253 constant GUILTY_CONTEXT_RESET

        0x8254 constant INNOCENT_CONTEXT_RESET

         0x8255 constant UNKNOWN_CONTEXT_RESET

   0x8256 constant RESET_NOTIFICATION_STRATEGY

         0x8252 constant LOSE_CONTEXT_ON_RESET

         0x8261 constant NO_RESET_NOTIFICATION

0x00000004 constant CONTEXT_FLAG_ROBUST_ACCESS_BIT

      0x82FB constant CONTEXT_RELEASE_BEHAVIOR

0x82FC constant CONTEXT_RELEASE_BEHAVIOR_FLUSH

1 constant VERSION_4_6

   0x9551 constant SHADER_BINARY_FORMAT_SPIR_V

                 0x9552 constant SPIR_V_BINARY

              0x80EE constant PARAMETER_BUFFER

      0x80EF constant PARAMETER_BUFFER_BINDING

     0x00000008 constant CONTEXT_FLAG_NO_ERROR_BIT

            0x82EE constant VERTICES_SUBMITTED

          0x82EF constant PRIMITIVES_SUBMITTED

     0x82F0 constant VERTEX_SHADER_INVOCATIONS

   0x82F1 constant TESS_CONTROL_SHADER_PATCHES

0x82F2 constant TESS_EVALUATION_SHADER_INVOCATIONS

0x82F3 constant GEOMETRY_SHADER_PRIMITIVES_EMITTED

   0x82F4 constant FRAGMENT_SHADER_INVOCATIONS

    0x82F5 constant COMPUTE_SHADER_INVOCATIONS

     0x82F6 constant CLIPPING_INPUT_PRIMITIVES

    0x82F7 constant CLIPPING_OUTPUT_PRIMITIVES

          0x8E1B constant POLYGON_OFFSET_CLAMP

             0x9553 constant SPIR_V_EXTENSIONS

         0x9554 constant NUM_SPIR_V_EXTENSIONS

        0x84FE constant TEXTURE_MAX_ANISOTROPY

    0x84FF constant MAX_TEXTURE_MAX_ANISOTROPY

   0x82EC constant TRANSFORM_FEEDBACK_OVERFLOW

0x82ED constant TRANSFORM_FEEDBACK_STREAM_OVERFLOW

1 constant ARB_ES2_compatibility

1 constant ARB_ES3_1_compatibility

1 constant ARB_ES3_2_compatibility

    0x92BE constant PRIMITIVE_BOUNDING_BOX_ARB

0x9381 constant MULTISAMPLE_LINE_WIDTH_RANGE_ARB

0x9382 constant MULTISAMPLE_LINE_WIDTH_GRANULARITY_ARB

1 constant ARB_ES3_compatibility

1 constant ARB_arrays_of_arrays

1 constant ARB_base_instance

1 constant ARB_bindless_texture

            0x140F constant UNSIGNED_INT64_ARB

1 constant ARB_blend_func_extended

1 constant ARB_buffer_storage

1 constant ARB_cl_event

             0x8240 constant SYNC_CL_EVENT_ARB

    0x8241 constant SYNC_CL_EVENT_COMPLETE_ARB

1 constant ARB_clear_buffer_object

1 constant ARB_clear_texture

1 constant ARB_clip_control

1 constant ARB_compressed_texture_pixel_storage

1 constant ARB_compute_shader

1 constant ARB_compute_variable_group_size

0x9344 constant MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB

0x90EB constant MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB

0x9345 constant MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB

0x91BF constant MAX_COMPUTE_FIXED_GROUP_SIZE_ARB

1 constant ARB_conditional_render_inverted

1 constant ARB_conservative_depth

1 constant ARB_copy_buffer

1 constant ARB_copy_image

1 constant ARB_cull_distance

1 constant ARB_debug_output

  0x8242 constant DEBUG_OUTPUT_SYNCHRONOUS_ARB

0x8243 constant DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB

   0x8244 constant DEBUG_CALLBACK_FUNCTION_ARB

 0x8245 constant DEBUG_CALLBACK_USER_PARAM_ARB

          0x8246 constant DEBUG_SOURCE_API_ARB

0x8247 constant DEBUG_SOURCE_WINDOW_SYSTEM_ARB

0x8248 constant DEBUG_SOURCE_SHADER_COMPILER_ARB

  0x8249 constant DEBUG_SOURCE_THIRD_PARTY_ARB

  0x824A constant DEBUG_SOURCE_APPLICATION_ARB

        0x824B constant DEBUG_SOURCE_OTHER_ARB

          0x824C constant DEBUG_TYPE_ERROR_ARB

0x824D constant DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB

0x824E constant DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB

    0x824F constant DEBUG_TYPE_PORTABILITY_ARB

    0x8250 constant DEBUG_TYPE_PERFORMANCE_ARB

          0x8251 constant DEBUG_TYPE_OTHER_ARB

  0x9143 constant MAX_DEBUG_MESSAGE_LENGTH_ARB

 0x9144 constant MAX_DEBUG_LOGGED_MESSAGES_ARB

     0x9145 constant DEBUG_LOGGED_MESSAGES_ARB

       0x9146 constant DEBUG_SEVERITY_HIGH_ARB

     0x9147 constant DEBUG_SEVERITY_MEDIUM_ARB

        0x9148 constant DEBUG_SEVERITY_LOW_ARB

1 constant ARB_depth_buffer_float

1 constant ARB_depth_clamp

1 constant ARB_derivative_control

1 constant ARB_direct_state_access

1 constant ARB_draw_buffers_blend

1 constant ARB_draw_elements_base_vertex

1 constant ARB_draw_indirect

1 constant ARB_draw_instanced

1 constant ARB_enhanced_layouts

1 constant ARB_explicit_attrib_location

1 constant ARB_explicit_uniform_location

1 constant ARB_fragment_coord_conventions

1 constant ARB_fragment_layer_viewport

1 constant ARB_fragment_shader_interlock

1 constant ARB_framebuffer_no_attachments

1 constant ARB_framebuffer_object

1 constant ARB_framebuffer_sRGB

1 constant ARB_geometry_shader4

           0x000A constant LINES_ADJACENCY_ARB

      0x000B constant LINE_STRIP_ADJACENCY_ARB

       0x000C constant TRIANGLES_ADJACENCY_ARB

  0x000D constant TRIANGLE_STRIP_ADJACENCY_ARB

        0x8642 constant PROGRAM_POINT_SIZE_ARB

0x8C29 constant MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB

0x8DA7 constant FRAMEBUFFER_ATTACHMENT_LAYERED_ARB

0x8DA8 constant FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB

0x8DA9 constant FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB

           0x8DD9 constant GEOMETRY_SHADER_ARB

     0x8DDA constant GEOMETRY_VERTICES_OUT_ARB

       0x8DDB constant GEOMETRY_INPUT_TYPE_ARB

      0x8DDC constant GEOMETRY_OUTPUT_TYPE_ARB

0x8DDD constant MAX_GEOMETRY_VARYING_COMPONENTS_ARB

0x8DDE constant MAX_VERTEX_VARYING_COMPONENTS_ARB

0x8DDF constant MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB

0x8DE0 constant MAX_GEOMETRY_OUTPUT_VERTICES_ARB

0x8DE1 constant MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB

1 constant ARB_get_program_binary

1 constant ARB_get_texture_sub_image

1 constant ARB_gl_spirv

0x9551 constant SHADER_BINARY_FORMAT_SPIR_V_ARB

             0x9552 constant SPIR_V_BINARY_ARB

1 constant ARB_gpu_shader5

1 constant ARB_gpu_shader_fp64

1 constant ARB_gpu_shader_int64

                     0x140E constant INT64_ARB

                0x8FE9 constant INT64_VEC2_ARB

                0x8FEA constant INT64_VEC3_ARB

                0x8FEB constant INT64_VEC4_ARB

       0x8FF5 constant UNSIGNED_INT64_VEC2_ARB

       0x8FF6 constant UNSIGNED_INT64_VEC3_ARB

       0x8FF7 constant UNSIGNED_INT64_VEC4_ARB

1 constant ARB_half_float_vertex

1 constant ARB_imaging

1 constant ARB_indirect_parameters

          0x80EE constant PARAMETER_BUFFER_ARB

  0x80EF constant PARAMETER_BUFFER_BINDING_ARB

1 constant ARB_instanced_arrays

0x88FE constant VERTEX_ATTRIB_ARRAY_DIVISOR_ARB

1 constant ARB_internalformat_query

1 constant ARB_internalformat_query2

               0x8299 constant SRGB_DECODE_ARB

            0x9383 constant VIEW_CLASS_EAC_R11

           0x9384 constant VIEW_CLASS_EAC_RG11

           0x9385 constant VIEW_CLASS_ETC2_RGB

          0x9386 constant VIEW_CLASS_ETC2_RGBA

      0x9387 constant VIEW_CLASS_ETC2_EAC_RGBA

      0x9388 constant VIEW_CLASS_ASTC_4x4_RGBA

      0x9389 constant VIEW_CLASS_ASTC_5x4_RGBA

      0x938A constant VIEW_CLASS_ASTC_5x5_RGBA

      0x938B constant VIEW_CLASS_ASTC_6x5_RGBA

      0x938C constant VIEW_CLASS_ASTC_6x6_RGBA

      0x938D constant VIEW_CLASS_ASTC_8x5_RGBA

      0x938E constant VIEW_CLASS_ASTC_8x6_RGBA

      0x938F constant VIEW_CLASS_ASTC_8x8_RGBA

     0x9390 constant VIEW_CLASS_ASTC_10x5_RGBA

     0x9391 constant VIEW_CLASS_ASTC_10x6_RGBA

     0x9392 constant VIEW_CLASS_ASTC_10x8_RGBA

    0x9393 constant VIEW_CLASS_ASTC_10x10_RGBA

    0x9394 constant VIEW_CLASS_ASTC_12x10_RGBA

    0x9395 constant VIEW_CLASS_ASTC_12x12_RGBA

1 constant ARB_invalidate_subdata

1 constant ARB_map_buffer_alignment

1 constant ARB_map_buffer_range

1 constant ARB_multi_bind

1 constant ARB_multi_draw_indirect

1 constant ARB_occlusion_query2

1 constant ARB_parallel_shader_compile

0x91B0 constant MAX_SHADER_COMPILER_THREADS_ARB

         0x91B1 constant COMPLETION_STATUS_ARB

1 constant ARB_pipeline_statistics_query

        0x82EE constant VERTICES_SUBMITTED_ARB

      0x82EF constant PRIMITIVES_SUBMITTED_ARB

 0x82F0 constant VERTEX_SHADER_INVOCATIONS_ARB

0x82F1 constant TESS_CONTROL_SHADER_PATCHES_ARB

0x82F2 constant TESS_EVALUATION_SHADER_INVOCATIONS_ARB

0x82F3 constant GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB

0x82F4 constant FRAGMENT_SHADER_INVOCATIONS_ARB

0x82F5 constant COMPUTE_SHADER_INVOCATIONS_ARB

 0x82F6 constant CLIPPING_INPUT_PRIMITIVES_ARB

0x82F7 constant CLIPPING_OUTPUT_PRIMITIVES_ARB

1 constant ARB_pixel_buffer_object

         0x88EB constant PIXEL_PACK_BUFFER_ARB

       0x88EC constant PIXEL_UNPACK_BUFFER_ARB

 0x88ED constant PIXEL_PACK_BUFFER_BINDING_ARB

0x88EF constant PIXEL_UNPACK_BUFFER_BINDING_ARB

1 constant ARB_polygon_offset_clamp

1 constant ARB_post_depth_coverage

1 constant ARB_program_interface_query

1 constant ARB_provoking_vertex

1 constant ARB_query_buffer_object

1 constant ARB_robust_buffer_access_behavior

1 constant ARB_robustness

0x00000004 constant CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB

     0x8252 constant LOSE_CONTEXT_ON_RESET_ARB

      0x8253 constant GUILTY_CONTEXT_RESET_ARB

    0x8254 constant INNOCENT_CONTEXT_RESET_ARB

     0x8255 constant UNKNOWN_CONTEXT_RESET_ARB

0x8256 constant RESET_NOTIFICATION_STRATEGY_ARB

     0x8261 constant NO_RESET_NOTIFICATION_ARB

1 constant ARB_robustness_isolation

1 constant ARB_sample_locations

0x933D constant SAMPLE_LOCATION_SUBPIXEL_BITS_ARB

0x933E constant SAMPLE_LOCATION_PIXEL_GRID_WIDTH_ARB

0x933F constant SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_ARB

0x9340 constant PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_ARB

           0x8E50 constant SAMPLE_LOCATION_ARB

0x9341 constant PROGRAMMABLE_SAMPLE_LOCATION_ARB

0x9342 constant FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_ARB

0x9343 constant FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_ARB

1 constant ARB_sample_shading

            0x8C36 constant SAMPLE_SHADING_ARB

  0x8C37 constant MIN_SAMPLE_SHADING_VALUE_ARB

1 constant ARB_sampler_objects

1 constant ARB_seamless_cube_map

1 constant ARB_seamless_cubemap_per_texture

1 constant ARB_separate_shader_objects

1 constant ARB_shader_atomic_counter_ops

1 constant ARB_shader_atomic_counters

1 constant ARB_shader_ballot

1 constant ARB_shader_bit_encoding

1 constant ARB_shader_clock

1 constant ARB_shader_draw_parameters

1 constant ARB_shader_group_vote

1 constant ARB_shader_image_load_store

1 constant ARB_shader_image_size

1 constant ARB_shader_precision

1 constant ARB_shader_stencil_export

1 constant ARB_shader_storage_buffer_object

1 constant ARB_shader_subroutine

1 constant ARB_shader_texture_image_samples

1 constant ARB_shader_viewport_layer_array

1 constant ARB_shading_language_420pack

1 constant ARB_shading_language_include

            0x8DAE constant SHADER_INCLUDE_ARB

       0x8DE9 constant NAMED_STRING_LENGTH_ARB

         0x8DEA constant NAMED_STRING_TYPE_ARB

1 constant ARB_shading_language_packing

1 constant ARB_sparse_buffer

        0x0400 constant SPARSE_STORAGE_BIT_ARB

   0x82F8 constant SPARSE_BUFFER_PAGE_SIZE_ARB

1 constant ARB_sparse_texture

            0x91A6 constant TEXTURE_SPARSE_ARB

   0x91A7 constant VIRTUAL_PAGE_SIZE_INDEX_ARB

         0x91AA constant NUM_SPARSE_LEVELS_ARB

    0x91A8 constant NUM_VIRTUAL_PAGE_SIZES_ARB

       0x9195 constant VIRTUAL_PAGE_SIZE_X_ARB

       0x9196 constant VIRTUAL_PAGE_SIZE_Y_ARB

       0x9197 constant VIRTUAL_PAGE_SIZE_Z_ARB

   0x9198 constant MAX_SPARSE_TEXTURE_SIZE_ARB

0x9199 constant MAX_SPARSE_3D_TEXTURE_SIZE_ARB

0x919A constant MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB

0x91A9 constant SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB

1 constant ARB_sparse_texture2

1 constant ARB_sparse_texture_clamp

1 constant ARB_spirv_extensions

1 constant ARB_stencil_texturing

1 constant ARB_sync

1 constant ARB_tessellation_shader

1 constant ARB_texture_barrier

1 constant ARB_texture_border_clamp

           0x812D constant CLAMP_TO_BORDER_ARB

1 constant ARB_texture_buffer_object

            0x8C2A constant TEXTURE_BUFFER_ARB

   0x8C2B constant MAX_TEXTURE_BUFFER_SIZE_ARB

    0x8C2C constant TEXTURE_BINDING_BUFFER_ARB

0x8C2D constant TEXTURE_BUFFER_DATA_STORE_BINDING_ARB

     0x8C2E constant TEXTURE_BUFFER_FORMAT_ARB

1 constant ARB_texture_buffer_object_rgb32

1 constant ARB_texture_buffer_range

1 constant ARB_texture_compression_bptc

0x8E8C constant COMPRESSED_RGBA_BPTC_UNORM_ARB

0x8E8D constant COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB

0x8E8E constant COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB

0x8E8F constant COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB

1 constant ARB_texture_compression_rgtc

1 constant ARB_texture_cube_map_array

    0x9009 constant TEXTURE_CUBE_MAP_ARRAY_ARB

0x900A constant TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB

0x900B constant PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB

    0x900C constant SAMPLER_CUBE_MAP_ARRAY_ARB

0x900D constant SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB

0x900E constant INT_SAMPLER_CUBE_MAP_ARRAY_ARB

0x900F constant UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB

1 constant ARB_texture_filter_anisotropic

1 constant ARB_texture_filter_minmax

    0x9366 constant TEXTURE_REDUCTION_MODE_ARB

          0x9367 constant WEIGHTED_AVERAGE_ARB

1 constant ARB_texture_gather

0x8E5E constant MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB

0x8E5F constant MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB

0x8F9F constant MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB

1 constant ARB_texture_mirror_clamp_to_edge

1 constant ARB_texture_mirrored_repeat

           0x8370 constant MIRRORED_REPEAT_ARB

1 constant ARB_texture_multisample

1 constant ARB_texture_non_power_of_two

1 constant ARB_texture_query_levels

1 constant ARB_texture_query_lod

1 constant ARB_texture_rg

1 constant ARB_texture_rgb10_a2ui

1 constant ARB_texture_stencil8

1 constant ARB_texture_storage

1 constant ARB_texture_storage_multisample

1 constant ARB_texture_swizzle

1 constant ARB_texture_view

1 constant ARB_timer_query

1 constant ARB_transform_feedback2

1 constant ARB_transform_feedback3

1 constant ARB_transform_feedback_instanced

1 constant ARB_transform_feedback_overflow_query

0x82EC constant TRANSFORM_FEEDBACK_OVERFLOW_ARB

0x82ED constant TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB

1 constant ARB_uniform_buffer_object

1 constant ARB_vertex_array_bgra

1 constant ARB_vertex_array_object

1 constant ARB_vertex_attrib_64bit

1 constant ARB_vertex_attrib_binding

1 constant ARB_vertex_type_10f_11f_11f_rev

1 constant ARB_vertex_type_2_10_10_10_rev

1 constant ARB_viewport_array

1 constant KHR_blend_equation_advanced

                  0x9294 constant MULTIPLY_KHR

                    0x9295 constant SCREEN_KHR

                   0x9296 constant OVERLAY_KHR

                    0x9297 constant DARKEN_KHR

                   0x9298 constant LIGHTEN_KHR

                0x9299 constant COLORDODGE_KHR

                 0x929A constant COLORBURN_KHR

                 0x929B constant HARDLIGHT_KHR

                 0x929C constant SOFTLIGHT_KHR

                0x929E constant DIFFERENCE_KHR

                 0x92A0 constant EXCLUSION_KHR

                   0x92AD constant HSL_HUE_KHR

            0x92AE constant HSL_SATURATION_KHR

                 0x92AF constant HSL_COLOR_KHR

            0x92B0 constant HSL_LUMINOSITY_KHR

1 constant KHR_blend_equation_advanced_coherent

   0x9285 constant BLEND_ADVANCED_COHERENT_KHR

1 constant KHR_context_flush_control

1 constant KHR_debug

1 constant KHR_no_error

 0x00000008 constant CONTEXT_FLAG_NO_ERROR_BIT_KHR

1 constant KHR_parallel_shader_compile

0x91B0 constant MAX_SHADER_COMPILER_THREADS_KHR

         0x91B1 constant COMPLETION_STATUS_KHR

1 constant KHR_robust_buffer_access_behavior

1 constant KHR_robustness

         0x90F3 constant CONTEXT_ROBUST_ACCESS

1 constant KHR_shader_subgroup

             0x9532 constant SUBGROUP_SIZE_KHR

 0x9533 constant SUBGROUP_SUPPORTED_STAGES_KHR

0x9534 constant SUBGROUP_SUPPORTED_FEATURES_KHR

  0x9535 constant SUBGROUP_QUAD_ALL_STAGES_KHR

0x00000001 constant SUBGROUP_FEATURE_BASIC_BIT_KHR

 0x00000002 constant SUBGROUP_FEATURE_VOTE_BIT_KHR

0x00000004 constant SUBGROUP_FEATURE_ARITHMETIC_BIT_KHR

0x00000008 constant SUBGROUP_FEATURE_BALLOT_BIT_KHR

0x00000010 constant SUBGROUP_FEATURE_SHUFFLE_BIT_KHR

0x00000020 constant SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT_KHR

0x00000040 constant SUBGROUP_FEATURE_CLUSTERED_BIT_KHR

 0x00000080 constant SUBGROUP_FEATURE_QUAD_BIT_KHR

1 constant KHR_texture_compression_astc_hdr

  0x93B0 constant COMPRESSED_RGBA_ASTC_4x4_KHR

  0x93B1 constant COMPRESSED_RGBA_ASTC_5x4_KHR

  0x93B2 constant COMPRESSED_RGBA_ASTC_5x5_KHR

  0x93B3 constant COMPRESSED_RGBA_ASTC_6x5_KHR

  0x93B4 constant COMPRESSED_RGBA_ASTC_6x6_KHR

  0x93B5 constant COMPRESSED_RGBA_ASTC_8x5_KHR

  0x93B6 constant COMPRESSED_RGBA_ASTC_8x6_KHR

  0x93B7 constant COMPRESSED_RGBA_ASTC_8x8_KHR

 0x93B8 constant COMPRESSED_RGBA_ASTC_10x5_KHR

 0x93B9 constant COMPRESSED_RGBA_ASTC_10x6_KHR

 0x93BA constant COMPRESSED_RGBA_ASTC_10x8_KHR

0x93BB constant COMPRESSED_RGBA_ASTC_10x10_KHR

0x93BC constant COMPRESSED_RGBA_ASTC_12x10_KHR

0x93BD constant COMPRESSED_RGBA_ASTC_12x12_KHR

0x93D0 constant COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR

0x93D1 constant COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR

0x93D2 constant COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR

0x93D3 constant COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR

0x93D4 constant COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR

0x93D5 constant COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR

0x93D6 constant COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR

0x93D7 constant COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR

0x93D8 constant COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR

0x93D9 constant COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR

0x93DA constant COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR

0x93DB constant COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR

0x93DC constant COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR

0x93DD constant COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR

1 constant KHR_texture_compression_astc_ldr

1 constant KHR_texture_compression_astc_sliced_3d

1 constant AMD_framebuffer_multisample_advanced

0x91B2 constant RENDERBUFFER_STORAGE_SAMPLES_AMD

0x91B3 constant MAX_COLOR_FRAMEBUFFER_SAMPLES_AMD

0x91B4 constant MAX_COLOR_FRAMEBUFFER_STORAGE_SAMPLES_AMD

0x91B5 constant MAX_DEPTH_STENCIL_FRAMEBUFFER_SAMPLES_AMD

0x91B6 constant NUM_SUPPORTED_MULTISAMPLE_MODES_AMD

0x91B7 constant SUPPORTED_MULTISAMPLE_MODES_AMD

1 constant AMD_performance_monitor

              0x8BC0 constant COUNTER_TYPE_AMD

             0x8BC1 constant COUNTER_RANGE_AMD

            0x8BC2 constant UNSIGNED_INT64_AMD

                0x8BC3 constant PERCENTAGE_AMD

  0x8BC4 constant PERFMON_RESULT_AVAILABLE_AMD

       0x8BC5 constant PERFMON_RESULT_SIZE_AMD

            0x8BC6 constant PERFMON_RESULT_AMD

1 constant APPLE_rgb_422

                 0x8A1F constant RGB_422_APPLE

      0x85BA constant UNSIGNED_SHORT_8_8_APPLE

  0x85BB constant UNSIGNED_SHORT_8_8_REV_APPLE

             0x8A51 constant RGB_RAW_422_APPLE

1 constant EXT_EGL_image_storage

1 constant EXT_EGL_sync

1 constant EXT_debug_label

   0x8A4F constant PROGRAM_PIPELINE_OBJECT_EXT

            0x8B40 constant PROGRAM_OBJECT_EXT

             0x8B48 constant SHADER_OBJECT_EXT

             0x9151 constant BUFFER_OBJECT_EXT

              0x9153 constant QUERY_OBJECT_EXT

       0x9154 constant VERTEX_ARRAY_OBJECT_EXT

1 constant EXT_debug_marker

1 constant EXT_direct_state_access

            0x8E2D constant PROGRAM_MATRIX_EXT

  0x8E2E constant TRANSPOSE_PROGRAM_MATRIX_EXT

0x8E2F constant PROGRAM_MATRIX_STACK_DEPTH_EXT

1 constant EXT_draw_instanced

1 constant EXT_multiview_tessellation_geometry_shader

1 constant EXT_multiview_texture_multisample

1 constant EXT_multiview_timer_query

1 constant EXT_polygon_offset_clamp

      0x8E1B constant POLYGON_OFFSET_CLAMP_EXT

1 constant EXT_post_depth_coverage

1 constant EXT_raster_multisample

        0x9327 constant RASTER_MULTISAMPLE_EXT

            0x9328 constant RASTER_SAMPLES_EXT

        0x9329 constant MAX_RASTER_SAMPLES_EXT

0x932A constant RASTER_FIXED_SAMPLE_LOCATIONS_EXT

0x932B constant MULTISAMPLE_RASTERIZATION_ALLOWED_EXT

  0x932C constant EFFECTIVE_RASTER_SAMPLES_EXT

1 constant EXT_separate_shader_objects

            0x8B8D constant ACTIVE_PROGRAM_EXT

1 constant EXT_shader_framebuffer_fetch

0x8A52 constant FRAGMENT_SHADER_DISCARDS_SAMPLES_EXT

1 constant EXT_shader_framebuffer_fetch_non_coherent

1 constant EXT_shader_integer_mix

1 constant EXT_texture_compression_s3tc

  0x83F0 constant COMPRESSED_RGB_S3TC_DXT1_EXT

 0x83F1 constant COMPRESSED_RGBA_S3TC_DXT1_EXT

 0x83F2 constant COMPRESSED_RGBA_S3TC_DXT3_EXT

 0x83F3 constant COMPRESSED_RGBA_S3TC_DXT5_EXT

1 constant EXT_texture_filter_minmax

    0x9366 constant TEXTURE_REDUCTION_MODE_EXT

          0x9367 constant WEIGHTED_AVERAGE_EXT

1 constant EXT_texture_sRGB_R8

                       0x8FBD constant SR8_EXT

1 constant EXT_texture_sRGB_RG8

                      0x8FBE constant SRG8_EXT

1 constant EXT_texture_sRGB_decode

       0x8A48 constant TEXTURE_SRGB_DECODE_EXT

                    0x8A49 constant DECODE_EXT

               0x8A4A constant SKIP_DECODE_EXT

1 constant EXT_texture_shadow_lod

1 constant EXT_texture_storage

  0x912F constant TEXTURE_IMMUTABLE_FORMAT_EXT

                    0x803C constant ALPHA8_EXT

                0x8040 constant LUMINANCE8_EXT

         0x8045 constant LUMINANCE8_ALPHA8_EXT

                   0x8814 constant RGBA32F_EXT

                    0x8815 constant RGB32F_EXT

                  0x8816 constant ALPHA32F_EXT

              0x8818 constant LUMINANCE32F_EXT

        0x8819 constant LUMINANCE_ALPHA32F_EXT

                   0x881A constant RGBA16F_EXT

                    0x881B constant RGB16F_EXT

                  0x881C constant ALPHA16F_EXT

              0x881E constant LUMINANCE16F_EXT

        0x881F constant LUMINANCE_ALPHA16F_EXT

                  0x8059 constant RGB10_A2_EXT

                     0x8052 constant RGB10_EXT

                     0x93A1 constant BGRA8_EXT

                        0x8229 constant R8_EXT

                       0x822B constant RG8_EXT

                      0x822E constant R32F_EXT

                     0x8230 constant RG32F_EXT

                      0x822D constant R16F_EXT

                     0x822F constant RG16F_EXT

1 constant EXT_window_rectangles

                 0x8F10 constant INCLUSIVE_EXT

                 0x8F11 constant EXCLUSIVE_EXT

          0x8F12 constant WINDOW_RECTANGLE_EXT

     0x8F13 constant WINDOW_RECTANGLE_MODE_EXT

     0x8F14 constant MAX_WINDOW_RECTANGLES_EXT

     0x8F15 constant NUM_WINDOW_RECTANGLES_EXT

1 constant INTEL_blackhole_render

        0x83FC constant BLACKHOLE_RENDER_INTEL

1 constant INTEL_conservative_rasterization

0x83FE constant CONSERVATIVE_RASTERIZATION_INTEL

1 constant INTEL_framebuffer_CMAA

1 constant INTEL_performance_query

0x00000000 constant PERFQUERY_SINGLE_CONTEXT_INTEL

0x00000001 constant PERFQUERY_GLOBAL_CONTEXT_INTEL

          0x83FB constant PERFQUERY_WAIT_INTEL

         0x83FA constant PERFQUERY_FLUSH_INTEL

   0x83F9 constant PERFQUERY_DONOT_FLUSH_INTEL

 0x94F0 constant PERFQUERY_COUNTER_EVENT_INTEL

0x94F1 constant PERFQUERY_COUNTER_DURATION_NORM_INTEL

0x94F2 constant PERFQUERY_COUNTER_DURATION_RAW_INTEL

0x94F3 constant PERFQUERY_COUNTER_THROUGHPUT_INTEL

   0x94F4 constant PERFQUERY_COUNTER_RAW_INTEL

0x94F5 constant PERFQUERY_COUNTER_TIMESTAMP_INTEL

0x94F8 constant PERFQUERY_COUNTER_DATA_UINT32_INTEL

0x94F9 constant PERFQUERY_COUNTER_DATA_UINT64_INTEL

0x94FA constant PERFQUERY_COUNTER_DATA_FLOAT_INTEL

0x94FB constant PERFQUERY_COUNTER_DATA_DOUBLE_INTEL

0x94FC constant PERFQUERY_COUNTER_DATA_BOOL32_INTEL

0x94FD constant PERFQUERY_QUERY_NAME_LENGTH_MAX_INTEL

0x94FE constant PERFQUERY_COUNTER_NAME_LENGTH_MAX_INTEL

0x94FF constant PERFQUERY_COUNTER_DESC_LENGTH_MAX_INTEL

0x9500 constant PERFQUERY_GPA_EXTENDED_COUNTERS_INTEL

1 constant MESA_framebuffer_flip_x

       0x8BBC constant FRAMEBUFFER_FLIP_X_MESA

1 constant MESA_framebuffer_flip_y

       0x8BBB constant FRAMEBUFFER_FLIP_Y_MESA

1 constant MESA_framebuffer_swap_xy

      0x8BBD constant FRAMEBUFFER_SWAP_XY_MESA

1 constant NV_bindless_multi_draw_indirect

1 constant NV_bindless_multi_draw_indirect_count

1 constant NV_bindless_texture

1 constant NV_blend_equation_advanced

              0x9281 constant BLEND_OVERLAP_NV

    0x9280 constant BLEND_PREMULTIPLIED_SRC_NV

                       0x1905 constant BLUE_NV

                  0x929A constant COLORBURN_NV

                 0x9299 constant COLORDODGE_NV

                   0x9284 constant CONJOINT_NV

                   0x92A1 constant CONTRAST_NV

                     0x9297 constant DARKEN_NV

                 0x929E constant DIFFERENCE_NV

                   0x9283 constant DISJOINT_NV

                   0x928F constant DST_ATOP_NV

                     0x928B constant DST_IN_NV

                        0x9287 constant DST_NV

                    0x928D constant DST_OUT_NV

                   0x9289 constant DST_OVER_NV

                  0x92A0 constant EXCLUSION_NV

                      0x1904 constant GREEN_NV

                  0x929B constant HARDLIGHT_NV

                    0x92A9 constant HARDMIX_NV

                  0x92AF constant HSL_COLOR_NV

                    0x92AD constant HSL_HUE_NV

             0x92B0 constant HSL_LUMINOSITY_NV

             0x92AE constant HSL_SATURATION_NV

                 0x92B4 constant INVERT_OVG_NV

                 0x92A3 constant INVERT_RGB_NV

                    0x9298 constant LIGHTEN_NV

                 0x92A5 constant LINEARBURN_NV

                0x92A4 constant LINEARDODGE_NV

                0x92A7 constant LINEARLIGHT_NV

              0x92B3 constant MINUS_CLAMPED_NV

                      0x929F constant MINUS_NV

                   0x9294 constant MULTIPLY_NV

                    0x9296 constant OVERLAY_NV

                   0x92A8 constant PINLIGHT_NV

         0x92B2 constant PLUS_CLAMPED_ALPHA_NV

               0x92B1 constant PLUS_CLAMPED_NV

                0x9292 constant PLUS_DARKER_NV

                       0x9291 constant PLUS_NV

                        0x1903 constant RED_NV

                     0x9295 constant SCREEN_NV

                  0x929C constant SOFTLIGHT_NV

                   0x928E constant SRC_ATOP_NV

                     0x928A constant SRC_IN_NV

                        0x9286 constant SRC_NV

                    0x928C constant SRC_OUT_NV

                   0x9288 constant SRC_OVER_NV

               0x9282 constant UNCORRELATED_NV

                 0x92A6 constant VIVIDLIGHT_NV

                        0x1506 constant XOR_NV

1 constant NV_blend_equation_advanced_coherent

    0x9285 constant BLEND_ADVANCED_COHERENT_NV

1 constant NV_blend_minmax_factor

                0x901C constant FACTOR_MIN_AMD

                0x901D constant FACTOR_MAX_AMD

1 constant NV_clip_space_w_scaling

  0x937C constant VIEWPORT_POSITION_W_SCALE_NV

0x937D constant VIEWPORT_POSITION_W_SCALE_X_COEFF_NV

0x937E constant VIEWPORT_POSITION_W_SCALE_Y_COEFF_NV

1 constant NV_command_list

 0x0000 constant TERMINATE_SEQUENCE_COMMAND_NV

                0x0001 constant NOP_COMMAND_NV

      0x0002 constant DRAW_ELEMENTS_COMMAND_NV

        0x0003 constant DRAW_ARRAYS_COMMAND_NV

0x0004 constant DRAW_ELEMENTS_STRIP_COMMAND_NV

  0x0005 constant DRAW_ARRAYS_STRIP_COMMAND_NV

0x0006 constant DRAW_ELEMENTS_INSTANCED_COMMAND_NV

0x0007 constant DRAW_ARRAYS_INSTANCED_COMMAND_NV

    0x0008 constant ELEMENT_ADDRESS_COMMAND_NV

  0x0009 constant ATTRIBUTE_ADDRESS_COMMAND_NV

    0x000A constant UNIFORM_ADDRESS_COMMAND_NV

        0x000B constant BLEND_COLOR_COMMAND_NV

        0x000C constant STENCIL_REF_COMMAND_NV

         0x000D constant LINE_WIDTH_COMMAND_NV

     0x000E constant POLYGON_OFFSET_COMMAND_NV

          0x000F constant ALPHA_REF_COMMAND_NV

           0x0010 constant VIEWPORT_COMMAND_NV

            0x0011 constant SCISSOR_COMMAND_NV

         0x0012 constant FRONT_FACE_COMMAND_NV

1 constant NV_compute_shader_derivatives

1 constant NV_conditional_render

                 0x8E13 constant QUERY_WAIT_NV

              0x8E14 constant QUERY_NO_WAIT_NV

       0x8E15 constant QUERY_BY_REGION_WAIT_NV

    0x8E16 constant QUERY_BY_REGION_NO_WAIT_NV

1 constant NV_conservative_raster

 0x9346 constant CONSERVATIVE_RASTERIZATION_NV

0x9347 constant SUBPIXEL_PRECISION_BIAS_X_BITS_NV

0x9348 constant SUBPIXEL_PRECISION_BIAS_Y_BITS_NV

0x9349 constant MAX_SUBPIXEL_PRECISION_BIAS_BITS_NV

1 constant NV_conservative_raster_dilate

 0x9379 constant CONSERVATIVE_RASTER_DILATE_NV

0x937A constant CONSERVATIVE_RASTER_DILATE_RANGE_NV

0x937B constant CONSERVATIVE_RASTER_DILATE_GRANULARITY_NV

1 constant NV_conservative_raster_pre_snap

0x9550 constant CONSERVATIVE_RASTER_MODE_PRE_SNAP_NV

1 constant NV_conservative_raster_pre_snap_triangles

   0x954D constant CONSERVATIVE_RASTER_MODE_NV

0x954E constant CONSERVATIVE_RASTER_MODE_POST_SNAP_NV

0x954F constant CONSERVATIVE_RASTER_MODE_PRE_SNAP_TRIANGLES_NV

1 constant NV_conservative_raster_underestimation

1 constant NV_depth_buffer_float

         0x8DAB constant DEPTH_COMPONENT32F_NV

          0x8DAC constant DEPTH32F_STENCIL8_NV

0x8DAD constant FLOAT_32_UNSIGNED_INT_24_8_REV_NV

    0x8DAF constant DEPTH_BUFFER_FLOAT_MODE_NV

1 constant NV_draw_vulkan_image

1 constant NV_fill_rectangle

             0x933C constant FILL_RECTANGLE_NV

1 constant NV_fragment_coverage_to_color

 0x92DD constant FRAGMENT_COVERAGE_TO_COLOR_NV

    0x92DE constant FRAGMENT_COVERAGE_COLOR_NV

1 constant NV_fragment_shader_barycentric

1 constant NV_fragment_shader_interlock

1 constant NV_framebuffer_mixed_samples

  0x9331 constant COVERAGE_MODULATION_TABLE_NV

              0x8E20 constant COLOR_SAMPLES_NV

              0x932D constant DEPTH_SAMPLES_NV

            0x932E constant STENCIL_SAMPLES_NV

0x932F constant MIXED_DEPTH_SAMPLES_SUPPORTED_NV

0x9330 constant MIXED_STENCIL_SAMPLES_SUPPORTED_NV

        0x9332 constant COVERAGE_MODULATION_NV

0x9333 constant COVERAGE_MODULATION_TABLE_SIZE_NV

1 constant NV_framebuffer_multisample_coverage

0x8CAB constant RENDERBUFFER_COVERAGE_SAMPLES_NV

 0x8E10 constant RENDERBUFFER_COLOR_SAMPLES_NV

0x8E11 constant MAX_MULTISAMPLE_COVERAGE_MODES_NV

 0x8E12 constant MULTISAMPLE_COVERAGE_MODES_NV

1 constant NV_geometry_shader_passthrough

1 constant NV_gpu_shader5

                      0x140E constant INT64_NV

             0x140F constant UNSIGNED_INT64_NV

                       0x8FE0 constant INT8_NV

                  0x8FE1 constant INT8_VEC2_NV

                  0x8FE2 constant INT8_VEC3_NV

                  0x8FE3 constant INT8_VEC4_NV

                      0x8FE4 constant INT16_NV

                 0x8FE5 constant INT16_VEC2_NV

                 0x8FE6 constant INT16_VEC3_NV

                 0x8FE7 constant INT16_VEC4_NV

                 0x8FE9 constant INT64_VEC2_NV

                 0x8FEA constant INT64_VEC3_NV

                 0x8FEB constant INT64_VEC4_NV

              0x8FEC constant UNSIGNED_INT8_NV

         0x8FED constant UNSIGNED_INT8_VEC2_NV

         0x8FEE constant UNSIGNED_INT8_VEC3_NV

         0x8FEF constant UNSIGNED_INT8_VEC4_NV

             0x8FF0 constant UNSIGNED_INT16_NV

        0x8FF1 constant UNSIGNED_INT16_VEC2_NV

        0x8FF2 constant UNSIGNED_INT16_VEC3_NV

        0x8FF3 constant UNSIGNED_INT16_VEC4_NV

        0x8FF5 constant UNSIGNED_INT64_VEC2_NV

        0x8FF6 constant UNSIGNED_INT64_VEC3_NV

        0x8FF7 constant UNSIGNED_INT64_VEC4_NV

                    0x8FF8 constant FLOAT16_NV

               0x8FF9 constant FLOAT16_VEC2_NV

               0x8FFA constant FLOAT16_VEC3_NV

               0x8FFB constant FLOAT16_VEC4_NV

1 constant NV_internalformat_sample_query

               0x9371 constant MULTISAMPLES_NV

        0x9372 constant SUPERSAMPLE_SCALE_X_NV

        0x9373 constant SUPERSAMPLE_SCALE_Y_NV

                 0x9374 constant CONFORMANT_NV

1 constant NV_memory_attachment

     0x95A4 constant ATTACHED_MEMORY_OBJECT_NV

     0x95A5 constant ATTACHED_MEMORY_OFFSET_NV

0x95A6 constant MEMORY_ATTACHABLE_ALIGNMENT_NV

     0x95A7 constant MEMORY_ATTACHABLE_SIZE_NV

          0x95A8 constant MEMORY_ATTACHABLE_NV

0x95A9 constant DETACHED_MEMORY_INCARNATION_NV

          0x95AA constant DETACHED_TEXTURES_NV

           0x95AB constant DETACHED_BUFFERS_NV

      0x95AC constant MAX_DETACHED_TEXTURES_NV

       0x95AD constant MAX_DETACHED_BUFFERS_NV

1 constant NV_memory_object_sparse

1 constant NV_mesh_shader

                0x9559 constant MESH_SHADER_NV

                0x955A constant TASK_SHADER_NV

    0x8E60 constant MAX_MESH_UNIFORM_BLOCKS_NV

0x8E61 constant MAX_MESH_TEXTURE_IMAGE_UNITS_NV

    0x8E62 constant MAX_MESH_IMAGE_UNIFORMS_NV

0x8E63 constant MAX_MESH_UNIFORM_COMPONENTS_NV

0x8E64 constant MAX_MESH_ATOMIC_COUNTER_BUFFERS_NV

   0x8E65 constant MAX_MESH_ATOMIC_COUNTERS_NV

0x8E66 constant MAX_MESH_SHADER_STORAGE_BLOCKS_NV

0x8E67 constant MAX_COMBINED_MESH_UNIFORM_COMPONENTS_NV

    0x8E68 constant MAX_TASK_UNIFORM_BLOCKS_NV

0x8E69 constant MAX_TASK_TEXTURE_IMAGE_UNITS_NV

    0x8E6A constant MAX_TASK_IMAGE_UNIFORMS_NV

0x8E6B constant MAX_TASK_UNIFORM_COMPONENTS_NV

0x8E6C constant MAX_TASK_ATOMIC_COUNTER_BUFFERS_NV

   0x8E6D constant MAX_TASK_ATOMIC_COUNTERS_NV

0x8E6E constant MAX_TASK_SHADER_STORAGE_BLOCKS_NV

0x8E6F constant MAX_COMBINED_TASK_UNIFORM_COMPONENTS_NV

0x95A2 constant MAX_MESH_WORK_GROUP_INVOCATIONS_NV

0x95A3 constant MAX_TASK_WORK_GROUP_INVOCATIONS_NV

 0x9536 constant MAX_MESH_TOTAL_MEMORY_SIZE_NV

 0x9537 constant MAX_TASK_TOTAL_MEMORY_SIZE_NV

   0x9538 constant MAX_MESH_OUTPUT_VERTICES_NV

 0x9539 constant MAX_MESH_OUTPUT_PRIMITIVES_NV

      0x953A constant MAX_TASK_OUTPUT_COUNT_NV

  0x953D constant MAX_DRAW_MESH_TASKS_COUNT_NV

             0x9557 constant MAX_MESH_VIEWS_NV

0x92DF constant MESH_OUTPUT_PER_VERTEX_GRANULARITY_NV

0x9543 constant MESH_OUTPUT_PER_PRIMITIVE_GRANULARITY_NV

   0x953B constant MAX_MESH_WORK_GROUP_SIZE_NV

   0x953C constant MAX_TASK_WORK_GROUP_SIZE_NV

       0x953E constant MESH_WORK_GROUP_SIZE_NV

       0x953F constant TASK_WORK_GROUP_SIZE_NV

          0x9579 constant MESH_VERTICES_OUT_NV

        0x957A constant MESH_PRIMITIVES_OUT_NV

           0x957B constant MESH_OUTPUT_TYPE_NV

0x959C constant UNIFORM_BLOCK_REFERENCED_BY_MESH_SHADER_NV

0x959D constant UNIFORM_BLOCK_REFERENCED_BY_TASK_SHADER_NV

  0x95A0 constant REFERENCED_BY_MESH_SHADER_NV

  0x95A1 constant REFERENCED_BY_TASK_SHADER_NV

            0x00000040 constant MESH_SHADER_BIT_NV

            0x00000080 constant TASK_SHADER_BIT_NV

            0x957C constant MESH_SUBROUTINE_NV

            0x957D constant TASK_SUBROUTINE_NV

    0x957E constant MESH_SUBROUTINE_UNIFORM_NV

    0x957F constant TASK_SUBROUTINE_UNIFORM_NV

0x959E constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_MESH_SHADER_NV

0x959F constant ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TASK_SHADER_NV

1 constant NV_path_rendering

            0x9070 constant PATH_FORMAT_SVG_NV

             0x9071 constant PATH_FORMAT_PS_NV

         0x9072 constant STANDARD_FONT_NAME_NV

           0x9073 constant SYSTEM_FONT_NAME_NV

                  0x9074 constant FILE_NAME_NV

          0x9075 constant PATH_STROKE_WIDTH_NV

              0x9076 constant PATH_END_CAPS_NV

       0x9077 constant PATH_INITIAL_END_CAP_NV

      0x9078 constant PATH_TERMINAL_END_CAP_NV

            0x9079 constant PATH_JOIN_STYLE_NV

           0x907A constant PATH_MITER_LIMIT_NV

             0x907B constant PATH_DASH_CAPS_NV

      0x907C constant PATH_INITIAL_DASH_CAP_NV

     0x907D constant PATH_TERMINAL_DASH_CAP_NV

           0x907E constant PATH_DASH_OFFSET_NV

         0x907F constant PATH_CLIENT_LENGTH_NV

             0x9080 constant PATH_FILL_MODE_NV

             0x9081 constant PATH_FILL_MASK_NV

       0x9082 constant PATH_FILL_COVER_MODE_NV

     0x9083 constant PATH_STROKE_COVER_MODE_NV

           0x9084 constant PATH_STROKE_MASK_NV

                   0x9088 constant COUNT_UP_NV

                 0x9089 constant COUNT_DOWN_NV

   0x908A constant PATH_OBJECT_BOUNDING_BOX_NV

                0x908B constant CONVEX_HULL_NV

               0x908D constant BOUNDING_BOX_NV

                0x908E constant TRANSLATE_X_NV

                0x908F constant TRANSLATE_Y_NV

               0x9090 constant TRANSLATE_2D_NV

               0x9091 constant TRANSLATE_3D_NV

                  0x9092 constant AFFINE_2D_NV

                  0x9094 constant AFFINE_3D_NV

        0x9096 constant TRANSPOSE_AFFINE_2D_NV

        0x9098 constant TRANSPOSE_AFFINE_3D_NV

                       0x909A constant UTF8_NV

                      0x909B constant UTF16_NV

0x909C constant BOUNDING_BOX_OF_BOUNDING_BOXES_NV

         0x909D constant PATH_COMMAND_COUNT_NV

           0x909E constant PATH_COORD_COUNT_NV

      0x909F constant PATH_DASH_ARRAY_COUNT_NV

       0x90A0 constant PATH_COMPUTED_LENGTH_NV

     0x90A1 constant PATH_FILL_BOUNDING_BOX_NV

   0x90A2 constant PATH_STROKE_BOUNDING_BOX_NV

                     0x90A3 constant SQUARE_NV

                      0x90A4 constant ROUND_NV

                 0x90A5 constant TRIANGULAR_NV

                      0x90A6 constant BEVEL_NV

               0x90A7 constant MITER_REVERT_NV

             0x90A8 constant MITER_TRUNCATE_NV

         0x90A9 constant SKIP_MISSING_GLYPH_NV

          0x90AA constant USE_MISSING_GLYPH_NV

        0x90AB constant PATH_ERROR_POSITION_NV

       0x90AD constant ACCUM_ADJACENT_PAIRS_NV

             0x90AE constant ADJACENT_PAIRS_NV

              0x90AF constant FIRST_TO_REST_NV

              0x90B0 constant PATH_GEN_MODE_NV

             0x90B1 constant PATH_GEN_COEFF_NV

        0x90B3 constant PATH_GEN_COMPONENTS_NV

          0x90B7 constant PATH_STENCIL_FUNC_NV

           0x90B8 constant PATH_STENCIL_REF_NV

    0x90B9 constant PATH_STENCIL_VALUE_MASK_NV

0x90BD constant PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV

0x90BE constant PATH_STENCIL_DEPTH_OFFSET_UNITS_NV

      0x90BF constant PATH_COVER_DEPTH_FUNC_NV

     0x90B4 constant PATH_DASH_OFFSET_RESET_NV

             0x90B5 constant MOVE_TO_RESETS_NV

          0x90B6 constant MOVE_TO_CONTINUES_NV

                 0x00 constant CLOSE_PATH_NV

                    0x02 constant MOVE_TO_NV

           0x03 constant RELATIVE_MOVE_TO_NV

                    0x04 constant LINE_TO_NV

           0x05 constant RELATIVE_LINE_TO_NV

         0x06 constant HORIZONTAL_LINE_TO_NV

0x07 constant RELATIVE_HORIZONTAL_LINE_TO_NV

           0x08 constant VERTICAL_LINE_TO_NV

  0x09 constant RELATIVE_VERTICAL_LINE_TO_NV

         0x0A constant QUADRATIC_CURVE_TO_NV

0x0B constant RELATIVE_QUADRATIC_CURVE_TO_NV

             0x0C constant CUBIC_CURVE_TO_NV

    0x0D constant RELATIVE_CUBIC_CURVE_TO_NV

  0x0E constant SMOOTH_QUADRATIC_CURVE_TO_NV

0x0F constant RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV

      0x10 constant SMOOTH_CUBIC_CURVE_TO_NV

0x11 constant RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV

           0x12 constant SMALL_CCW_ARC_TO_NV

  0x13 constant RELATIVE_SMALL_CCW_ARC_TO_NV

            0x14 constant SMALL_CW_ARC_TO_NV

   0x15 constant RELATIVE_SMALL_CW_ARC_TO_NV

           0x16 constant LARGE_CCW_ARC_TO_NV

  0x17 constant RELATIVE_LARGE_CCW_ARC_TO_NV

            0x18 constant LARGE_CW_ARC_TO_NV

   0x19 constant RELATIVE_LARGE_CW_ARC_TO_NV

               0xF0 constant RESTART_PATH_NV

   0xF2 constant DUP_FIRST_CUBIC_CURVE_TO_NV

    0xF4 constant DUP_LAST_CUBIC_CURVE_TO_NV

                       0xF6 constant RECT_NV

        0xF8 constant CIRCULAR_CCW_ARC_TO_NV

         0xFA constant CIRCULAR_CW_ARC_TO_NV

    0xFC constant CIRCULAR_TANGENT_ARC_TO_NV

                     0xFE constant ARC_TO_NV

            0xFF constant RELATIVE_ARC_TO_NV

                   0x01 constant BOLD_BIT_NV

                 0x02 constant ITALIC_BIT_NV

            0x01 constant GLYPH_WIDTH_BIT_NV

           0x02 constant GLYPH_HEIGHT_BIT_NV

0x04 constant GLYPH_HORIZONTAL_BEARING_X_BIT_NV

0x08 constant GLYPH_HORIZONTAL_BEARING_Y_BIT_NV

0x10 constant GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV

0x20 constant GLYPH_VERTICAL_BEARING_X_BIT_NV

0x40 constant GLYPH_VERTICAL_BEARING_Y_BIT_NV

0x80 constant GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV

      0x100 constant GLYPH_HAS_KERNING_BIT_NV

      0x00010000 constant FONT_X_MIN_BOUNDS_BIT_NV

      0x00020000 constant FONT_Y_MIN_BOUNDS_BIT_NV

      0x00040000 constant FONT_X_MAX_BOUNDS_BIT_NV

      0x00080000 constant FONT_Y_MAX_BOUNDS_BIT_NV

      0x00100000 constant FONT_UNITS_PER_EM_BIT_NV

          0x00200000 constant FONT_ASCENDER_BIT_NV

         0x00400000 constant FONT_DESCENDER_BIT_NV

            0x00800000 constant FONT_HEIGHT_BIT_NV

 0x01000000 constant FONT_MAX_ADVANCE_WIDTH_BIT_NV

0x02000000 constant FONT_MAX_ADVANCE_HEIGHT_BIT_NV

0x04000000 constant FONT_UNDERLINE_POSITION_BIT_NV

0x08000000 constant FONT_UNDERLINE_THICKNESS_BIT_NV

       0x10000000 constant FONT_HAS_KERNING_BIT_NV

               0xE8 constant ROUNDED_RECT_NV

      0xE9 constant RELATIVE_ROUNDED_RECT_NV

              0xEA constant ROUNDED_RECT2_NV

     0xEB constant RELATIVE_ROUNDED_RECT2_NV

              0xEC constant ROUNDED_RECT4_NV

     0xED constant RELATIVE_ROUNDED_RECT4_NV

              0xEE constant ROUNDED_RECT8_NV

     0xEF constant RELATIVE_ROUNDED_RECT8_NV

              0xF7 constant RELATIVE_RECT_NV

      0x9368 constant FONT_GLYPHS_AVAILABLE_NV

    0x9369 constant FONT_TARGET_UNAVAILABLE_NV

           0x936A constant FONT_UNAVAILABLE_NV

        0x936B constant FONT_UNINTELLIGIBLE_NV

             0x1A constant CONIC_CURVE_TO_NV

    0x1B constant RELATIVE_CONIC_CURVE_TO_NV

 0x20000000 constant FONT_NUM_GLYPH_INDICES_BIT_NV

       0x936C constant STANDARD_FONT_FORMAT_NV

            0x1701 constant PATH_PROJECTION_NV

             0x1700 constant PATH_MODELVIEW_NV

 0x0BA3 constant PATH_MODELVIEW_STACK_DEPTH_NV

      0x0BA6 constant PATH_MODELVIEW_MATRIX_NV

0x0D36 constant PATH_MAX_MODELVIEW_STACK_DEPTH_NV

0x84E3 constant PATH_TRANSPOSE_MODELVIEW_MATRIX_NV

0x0BA4 constant PATH_PROJECTION_STACK_DEPTH_NV

     0x0BA7 constant PATH_PROJECTION_MATRIX_NV

0x0D38 constant PATH_MAX_PROJECTION_STACK_DEPTH_NV

0x84E4 constant PATH_TRANSPOSE_PROJECTION_MATRIX_NV

             0x936D constant FRAGMENT_INPUT_NV

1 constant NV_path_rendering_shared_edge

                0xC0 constant SHARED_EDGE_NV

1 constant NV_primitive_shading_rate

0x95B1 constant SHADING_RATE_IMAGE_PER_PRIMITIVE_NV

0x95B2 constant SHADING_RATE_IMAGE_PALETTE_COUNT_NV

1 constant NV_representative_fragment_test

0x937F constant REPRESENTATIVE_FRAGMENT_TEST_NV

1 constant NV_sample_locations

0x933D constant SAMPLE_LOCATION_SUBPIXEL_BITS_NV

0x933E constant SAMPLE_LOCATION_PIXEL_GRID_WIDTH_NV

0x933F constant SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_NV

0x9340 constant PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_NV

            0x8E50 constant SAMPLE_LOCATION_NV

0x9341 constant PROGRAMMABLE_SAMPLE_LOCATION_NV

0x9342 constant FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_NV

0x9343 constant FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_NV

1 constant NV_sample_mask_override_coverage

1 constant NV_scissor_exclusive

     0x9555 constant SCISSOR_TEST_EXCLUSIVE_NV

      0x9556 constant SCISSOR_BOX_EXCLUSIVE_NV

1 constant NV_shader_atomic_counters

1 constant NV_shader_atomic_float

1 constant NV_shader_atomic_float64

1 constant NV_shader_atomic_fp16_vector

1 constant NV_shader_atomic_int64

1 constant NV_shader_buffer_load

         0x8F1D constant BUFFER_GPU_ADDRESS_NV

                0x8F34 constant GPU_ADDRESS_NV

  0x8F35 constant MAX_SHADER_BUFFER_ADDRESS_NV

1 constant NV_shader_buffer_store

0x00000010 constant SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV

1 constant NV_shader_subgroup_partitioned

0x00000100 constant SUBGROUP_FEATURE_PARTITIONED_BIT_NV

1 constant NV_shader_texture_footprint

1 constant NV_shader_thread_group

                  0x9339 constant WARP_SIZE_NV

               0x933A constant WARPS_PER_SM_NV

                   0x933B constant SM_COUNT_NV

1 constant NV_shader_thread_shuffle

1 constant NV_shading_rate_image

         0x9563 constant SHADING_RATE_IMAGE_NV

0x9564 constant SHADING_RATE_NO_INVOCATIONS_NV

0x9565 constant SHADING_RATE_1_INVOCATION_PER_PIXEL_NV

0x9566 constant SHADING_RATE_1_INVOCATION_PER_1X2_PIXELS_NV

0x9567 constant SHADING_RATE_1_INVOCATION_PER_2X1_PIXELS_NV

0x9568 constant SHADING_RATE_1_INVOCATION_PER_2X2_PIXELS_NV

0x9569 constant SHADING_RATE_1_INVOCATION_PER_2X4_PIXELS_NV

0x956A constant SHADING_RATE_1_INVOCATION_PER_4X2_PIXELS_NV

0x956B constant SHADING_RATE_1_INVOCATION_PER_4X4_PIXELS_NV

0x956C constant SHADING_RATE_2_INVOCATIONS_PER_PIXEL_NV

0x956D constant SHADING_RATE_4_INVOCATIONS_PER_PIXEL_NV

0x956E constant SHADING_RATE_8_INVOCATIONS_PER_PIXEL_NV

0x956F constant SHADING_RATE_16_INVOCATIONS_PER_PIXEL_NV

 0x955B constant SHADING_RATE_IMAGE_BINDING_NV

0x955C constant SHADING_RATE_IMAGE_TEXEL_WIDTH_NV

0x955D constant SHADING_RATE_IMAGE_TEXEL_HEIGHT_NV

0x955E constant SHADING_RATE_IMAGE_PALETTE_SIZE_NV

0x955F constant MAX_COARSE_FRAGMENT_SAMPLES_NV

0x95AE constant SHADING_RATE_SAMPLE_ORDER_DEFAULT_NV

0x95AF constant SHADING_RATE_SAMPLE_ORDER_PIXEL_MAJOR_NV

0x95B0 constant SHADING_RATE_SAMPLE_ORDER_SAMPLE_MAJOR_NV

1 constant NV_stereo_view_rendering

1 constant NV_texture_barrier

1 constant NV_texture_rectangle_compressed

1 constant NV_uniform_buffer_unified_memory

     0x936E constant UNIFORM_BUFFER_UNIFIED_NV

     0x936F constant UNIFORM_BUFFER_ADDRESS_NV

      0x9370 constant UNIFORM_BUFFER_LENGTH_NV

1 constant NV_vertex_attrib_integer_64bit

1 constant NV_vertex_buffer_unified_memory

0x8F1E constant VERTEX_ATTRIB_ARRAY_UNIFIED_NV

      0x8F1F constant ELEMENT_ARRAY_UNIFIED_NV

0x8F20 constant VERTEX_ATTRIB_ARRAY_ADDRESS_NV

       0x8F21 constant VERTEX_ARRAY_ADDRESS_NV

       0x8F22 constant NORMAL_ARRAY_ADDRESS_NV

        0x8F23 constant COLOR_ARRAY_ADDRESS_NV

        0x8F24 constant INDEX_ARRAY_ADDRESS_NV

0x8F25 constant TEXTURE_COORD_ARRAY_ADDRESS_NV

    0x8F26 constant EDGE_FLAG_ARRAY_ADDRESS_NV

0x8F27 constant SECONDARY_COLOR_ARRAY_ADDRESS_NV

    0x8F28 constant FOG_COORD_ARRAY_ADDRESS_NV

      0x8F29 constant ELEMENT_ARRAY_ADDRESS_NV

 0x8F2A constant VERTEX_ATTRIB_ARRAY_LENGTH_NV

        0x8F2B constant VERTEX_ARRAY_LENGTH_NV

        0x8F2C constant NORMAL_ARRAY_LENGTH_NV

         0x8F2D constant COLOR_ARRAY_LENGTH_NV

         0x8F2E constant INDEX_ARRAY_LENGTH_NV

 0x8F2F constant TEXTURE_COORD_ARRAY_LENGTH_NV

     0x8F30 constant EDGE_FLAG_ARRAY_LENGTH_NV

0x8F31 constant SECONDARY_COLOR_ARRAY_LENGTH_NV

     0x8F32 constant FOG_COORD_ARRAY_LENGTH_NV

       0x8F33 constant ELEMENT_ARRAY_LENGTH_NV

      0x8F40 constant DRAW_INDIRECT_UNIFIED_NV

      0x8F41 constant DRAW_INDIRECT_ADDRESS_NV

       0x8F42 constant DRAW_INDIRECT_LENGTH_NV

1 constant NV_viewport_array2

1 constant NV_viewport_swizzle

0x9350 constant VIEWPORT_SWIZZLE_POSITIVE_X_NV

0x9351 constant VIEWPORT_SWIZZLE_NEGATIVE_X_NV

0x9352 constant VIEWPORT_SWIZZLE_POSITIVE_Y_NV

0x9353 constant VIEWPORT_SWIZZLE_NEGATIVE_Y_NV

0x9354 constant VIEWPORT_SWIZZLE_POSITIVE_Z_NV

0x9355 constant VIEWPORT_SWIZZLE_NEGATIVE_Z_NV

0x9356 constant VIEWPORT_SWIZZLE_POSITIVE_W_NV

0x9357 constant VIEWPORT_SWIZZLE_NEGATIVE_W_NV

         0x9358 constant VIEWPORT_SWIZZLE_X_NV

         0x9359 constant VIEWPORT_SWIZZLE_Y_NV

         0x935A constant VIEWPORT_SWIZZLE_Z_NV

         0x935B constant VIEWPORT_SWIZZLE_W_NV

1 constant OVR_multiview

0x9630 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR

0x9632 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR

                 0x9631 constant MAX_VIEWS_OVR

0x9633 constant FRAMEBUFFER_INCOMPLETE_VIEW_TARGETS_OVR

1 constant OVR_multiview2

	// GLAPI void APIENTRY CullFace (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glCullFace" sys optional CullFace 

	// GLAPI void APIENTRY FrontFace (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glFrontFace" sys optional FrontFace 

	// GLAPI void APIENTRY Hint (GLenum target,GLenum mode)
	 ( Z32:_target Z32:_mode ) "libGL.so.1" "glHint" sys optional Hint 

	// GLAPI void APIENTRY LineWidth (GLfloat width)
	 ( Real:_width ) "libGL.so.1" "glLineWidth" sys optional LineWidth 

	// GLAPI void APIENTRY PointSize (GLfloat size)
	 ( Real:_size ) "libGL.so.1" "glPointSize" sys optional PointSize 

	// GLAPI void APIENTRY PolygonMode (GLenum face,GLenum mode)
	 ( Z32:_face Z32:_mode ) "libGL.so.1" "glPolygonMode" sys optional PolygonMode 

	// GLAPI void APIENTRY Scissor (GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glScissor" sys optional Scissor 

	// GLAPI void APIENTRY TexParameterf (GLenum target,GLenum pname,GLfloat param)
	 ( Z32:_target Z32:_pname Real:_param ) "libGL.so.1" "glTexParameterf" sys optional TexParameterf 

	// GLAPI void APIENTRY TexParameterfv (GLenum target,GLenum pname,const GLfloat * params)
	 ( Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glTexParameterfv" sys optional TexParameterfv 

	// GLAPI void APIENTRY TexParameteri (GLenum target,GLenum pname,GLint param)
	 ( Z32:_target Z32:_pname Z32:_param ) "libGL.so.1" "glTexParameteri" sys optional TexParameteri 

	// GLAPI void APIENTRY TexParameteriv (GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glTexParameteriv" sys optional TexParameteriv 

	// GLAPI void APIENTRY TexImage1D (GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTexImage1D" sys optional TexImage1D 

	// GLAPI void APIENTRY TexImage2D (GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTexImage2D" sys optional TexImage2D 

	// GLAPI void APIENTRY DrawBuffer (GLenum buf)
	 ( Z32:_buf ) "libGL.so.1" "glDrawBuffer" sys optional DrawBuffer 

	// GLAPI void APIENTRY Clear (GLbitfield mask)
	 ( Z32:_mask ) "libGL.so.1" "glClear" sys optional Clear 

	// GLAPI void APIENTRY ClearColor (GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
	 ( Real:_red Real:_green Real:_blue Real:_alpha ) "libGL.so.1" "glClearColor" sys optional ClearColor 

	// GLAPI void APIENTRY ClearStencil (GLint s)
	 ( Z32:_s ) "libGL.so.1" "glClearStencil" sys optional ClearStencil 

	// GLAPI void APIENTRY ClearDepth (GLdouble depth)
	 ( Double32:_depth ) "libGL.so.1" "glClearDepth" sys optional ClearDepth 

	// GLAPI void APIENTRY StencilMask (GLuint mask)
	 ( Z32:_mask ) "libGL.so.1" "glStencilMask" sys optional StencilMask 

	// GLAPI void APIENTRY ColorMask (GLboolean red,GLboolean green,GLboolean blue,GLboolean alpha)
	 ( Bit:_red Bit:_green Bit:_blue Bit:_alpha ) "libGL.so.1" "glColorMask" sys optional ColorMask 

	// GLAPI void APIENTRY DepthMask (GLboolean flag)
	 ( Bit:_flag ) "libGL.so.1" "glDepthMask" sys optional DepthMask 

	// GLAPI void APIENTRY Disable (GLenum cap)
	 ( Z32:_cap ) "libGL.so.1" "glDisable" sys optional Disable 

	// GLAPI void APIENTRY Enable (GLenum cap)
	 ( Z32:_cap ) "libGL.so.1" "glEnable" sys optional Enable 

	// GLAPI void APIENTRY Finish (void)
	 ( ) "libGL.so.1" "glFinish" sys optional Finish 

	// GLAPI void APIENTRY Flush (void)
	 ( ) "libGL.so.1" "glFlush" sys optional Flush 

	// GLAPI void APIENTRY BlendFunc (GLenum sfactor,GLenum dfactor)
	 ( Z32:_sfactor Z32:_dfactor ) "libGL.so.1" "glBlendFunc" sys optional BlendFunc 

	// GLAPI void APIENTRY LogicOp (GLenum opcode)
	 ( Z32:_opcode ) "libGL.so.1" "glLogicOp" sys optional LogicOp 

	// GLAPI void APIENTRY StencilFunc (GLenum func,GLint ref,GLuint mask)
	 ( Z32:_func Z32:_ref Z32:_mask ) "libGL.so.1" "glStencilFunc" sys optional StencilFunc 

	// GLAPI void APIENTRY StencilOp (GLenum fail,GLenum zfail,GLenum zpass)
	 ( Z32:_fail Z32:_zfail Z32:_zpass ) "libGL.so.1" "glStencilOp" sys optional StencilOp 

	// GLAPI void APIENTRY DepthFunc (GLenum func)
	 ( Z32:_func ) "libGL.so.1" "glDepthFunc" sys optional DepthFunc 

	// GLAPI void APIENTRY PixelStoref (GLenum pname,GLfloat param)
	 ( Z32:_pname Real:_param ) "libGL.so.1" "glPixelStoref" sys optional PixelStoref 

	// GLAPI void APIENTRY PixelStorei (GLenum pname,GLint param)
	 ( Z32:_pname Z32:_param ) "libGL.so.1" "glPixelStorei" sys optional PixelStorei 

	// GLAPI void APIENTRY ReadBuffer (GLenum src)
	 ( Z32:_src ) "libGL.so.1" "glReadBuffer" sys optional ReadBuffer 

	// GLAPI void APIENTRY ReadPixels (GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,void * pixels)
	 ( Z32:_x Z32:_y Z32:_width Z32:_height Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glReadPixels" sys optional ReadPixels 

	// GLAPI void APIENTRY GetBooleanv (GLenum pname,GLboolean * data)
	 ( Z32:_pname Bit*:_data ) "libGL.so.1" "glGetBooleanv" sys optional GetBooleanv 

	// GLAPI void APIENTRY GetDoublev (GLenum pname,GLdouble * data)
	 ( Z32:_pname Double32*:_data ) "libGL.so.1" "glGetDoublev" sys optional GetDoublev 

	// GLAPI GLenum APIENTRY GetError (void)
	 (  -> Z32 ) "libGL.so.1" "glGetError" sys optional GetError 

	// GLAPI void APIENTRY GetFloatv (GLenum pname,GLfloat * data)
	 ( Z32:_pname Real*:_data ) "libGL.so.1" "glGetFloatv" sys optional GetFloatv 

	// GLAPI void APIENTRY GetIntegerv (GLenum pname,GLint * data)
	 ( Z32:_pname Z32*:_data ) "libGL.so.1" "glGetIntegerv" sys optional GetIntegerv 

	// GLAPI const GLubyte * APIENTRY GetString (GLenum name)
	 ( Z32:_name  -> String ) "libGL.so.1" "glGetString" sys optional GetString 

	// GLAPI void APIENTRY GetTexImage (GLenum target,GLint level,GLenum format,GLenum type,void * pixels)
	 ( Z32:_target Z32:_level Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glGetTexImage" sys optional GetTexImage 

	// GLAPI void APIENTRY GetTexParameterfv (GLenum target,GLenum pname,GLfloat * params)
	 ( Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glGetTexParameterfv" sys optional GetTexParameterfv 

	// GLAPI void APIENTRY GetTexParameteriv (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTexParameteriv" sys optional GetTexParameteriv 

	// GLAPI void APIENTRY GetTexLevelParameterfv (GLenum target,GLint level,GLenum pname,GLfloat * params)
	 ( Z32:_target Z32:_level Z32:_pname Real*:_params ) "libGL.so.1" "glGetTexLevelParameterfv" sys optional GetTexLevelParameterfv 

	// GLAPI void APIENTRY GetTexLevelParameteriv (GLenum target,GLint level,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_level Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTexLevelParameteriv" sys optional GetTexLevelParameteriv 

	// GLAPI GLboolean APIENTRY IsEnabled (GLenum cap)
	 ( Z32:_cap  -> Bit ) "libGL.so.1" "glIsEnabled" sys optional IsEnabled 

	// GLAPI void APIENTRY DepthRange (GLdouble n,GLdouble f)
	 ( Double32:_n Double32:_f ) "libGL.so.1" "glDepthRange" sys optional DepthRange 

	// GLAPI void APIENTRY Viewport (GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glViewport" sys optional Viewport 

	// GLAPI void APIENTRY DrawArrays (GLenum mode,GLint first,GLsizei count)
	 ( Z32:_mode Z32:_first Z32:_count ) "libGL.so.1" "glDrawArrays" sys optional DrawArrays 

	// GLAPI void APIENTRY DrawElements (GLenum mode,GLsizei count,GLenum type,const void * indices)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices ) "libGL.so.1" "glDrawElements" sys optional DrawElements 

	// GLAPI void APIENTRY GetPointerv (GLenum pname,void ** params)
	 ( Z32:_pname Any**:_params ) "libGL.so.1" "glGetPointerv" sys optional GetPointerv 

	// GLAPI void APIENTRY PolygonOffset (GLfloat factor,GLfloat units)
	 ( Real:_factor Real:_units ) "libGL.so.1" "glPolygonOffset" sys optional PolygonOffset 

	// GLAPI void APIENTRY CopyTexImage1D (GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_x Z32:_y Z32:_width Z32:_border ) "libGL.so.1" "glCopyTexImage1D" sys optional CopyTexImage1D 

	// GLAPI void APIENTRY CopyTexImage2D (GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_x Z32:_y Z32:_width Z32:_height Z32:_border ) "libGL.so.1" "glCopyTexImage2D" sys optional CopyTexImage2D 

	// GLAPI void APIENTRY CopyTexSubImage1D (GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_x Z32:_y Z32:_width ) "libGL.so.1" "glCopyTexSubImage1D" sys optional CopyTexSubImage1D 

	// GLAPI void APIENTRY CopyTexSubImage2D (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyTexSubImage2D" sys optional CopyTexSubImage2D 

	// GLAPI void APIENTRY TexSubImage1D (GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTexSubImage1D" sys optional TexSubImage1D 

	// GLAPI void APIENTRY TexSubImage2D (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTexSubImage2D" sys optional TexSubImage2D 

	// GLAPI void APIENTRY BindTexture (GLenum target,GLuint texture)
	 ( Z32:_target Z32:_texture ) "libGL.so.1" "glBindTexture" sys optional BindTexture 

	// GLAPI void APIENTRY DeleteTextures (GLsizei n,const GLuint * textures)
	 ( Z32:_n Z32*:_textures ) "libGL.so.1" "glDeleteTextures" sys optional DeleteTextures 

	// GLAPI void APIENTRY GenTextures (GLsizei n,GLuint * textures)
	 ( Z32:_n Z32*:_textures ) "libGL.so.1" "glGenTextures" sys optional GenTextures 

	// GLAPI GLboolean APIENTRY IsTexture (GLuint texture)
	 ( Z32:_texture  -> Bit ) "libGL.so.1" "glIsTexture" sys optional IsTexture 

	// GLAPI void APIENTRY DrawRangeElements (GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const void * indices)
	 ( Z32:_mode Z32:_start Z32:_end Z32:_count Z32:_type Any*:_indices ) "libGL.so.1" "glDrawRangeElements" sys optional DrawRangeElements 

	// GLAPI void APIENTRY TexImage3D (GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_depth Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTexImage3D" sys optional TexImage3D 

	// GLAPI void APIENTRY TexSubImage3D (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTexSubImage3D" sys optional TexSubImage3D 

	// GLAPI void APIENTRY CopyTexSubImage3D (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyTexSubImage3D" sys optional CopyTexSubImage3D 

	// GLAPI void APIENTRY ActiveTexture (GLenum texture)
	 ( Z32:_texture ) "libGL.so.1" "glActiveTexture" sys optional ActiveTexture 

	// GLAPI void APIENTRY SampleCoverage (GLfloat value,GLboolean invert)
	 ( Real:_value Bit:_invert ) "libGL.so.1" "glSampleCoverage" sys optional SampleCoverage 

	// GLAPI void APIENTRY CompressedTexImage3D (GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const void * data)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_depth Z32:_border Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTexImage3D" sys optional CompressedTexImage3D 

	// GLAPI void APIENTRY CompressedTexImage2D (GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const void * data)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_border Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTexImage2D" sys optional CompressedTexImage2D 

	// GLAPI void APIENTRY CompressedTexImage1D (GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const void * data)
	 ( Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_border Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTexImage1D" sys optional CompressedTexImage1D 

	// GLAPI void APIENTRY CompressedTexSubImage3D (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const void * data)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTexSubImage3D" sys optional CompressedTexSubImage3D 

	// GLAPI void APIENTRY CompressedTexSubImage2D (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const void * data)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTexSubImage2D" sys optional CompressedTexSubImage2D 

	// GLAPI void APIENTRY CompressedTexSubImage1D (GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const void * data)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTexSubImage1D" sys optional CompressedTexSubImage1D 

	// GLAPI void APIENTRY GetCompressedTexImage (GLenum target,GLint level,void * img)
	 ( Z32:_target Z32:_level Any*:_img ) "libGL.so.1" "glGetCompressedTexImage" sys optional GetCompressedTexImage 

	// GLAPI void APIENTRY BlendFuncSeparate (GLenum sfactorRGB,GLenum dfactorRGB,GLenum sfactorAlpha,GLenum dfactorAlpha)
	 ( Z32:_sfactorRGB Z32:_dfactorRGB Z32:_sfactorAlpha Z32:_dfactorAlpha ) "libGL.so.1" "glBlendFuncSeparate" sys optional BlendFuncSeparate 

	// GLAPI void APIENTRY MultiDrawArrays (GLenum mode,const GLint * first,const GLsizei * count,GLsizei drawcount)
	 ( Z32:_mode Z32*:_first Z32*:_count Z32:_drawcount ) "libGL.so.1" "glMultiDrawArrays" sys optional MultiDrawArrays 

	// GLAPI void APIENTRY MultiDrawElements (GLenum mode,const GLsizei * count,GLenum type,const void * const * indices,GLsizei drawcount)
	 ( Z32:_mode Z32*:_count Z32:_type Any**:_indices Z32:_drawcount ) "libGL.so.1" "glMultiDrawElements" sys optional MultiDrawElements 

	// GLAPI void APIENTRY PointParameterf (GLenum pname,GLfloat param)
	 ( Z32:_pname Real:_param ) "libGL.so.1" "glPointParameterf" sys optional PointParameterf 

	// GLAPI void APIENTRY PointParameterfv (GLenum pname,const GLfloat * params)
	 ( Z32:_pname Real*:_params ) "libGL.so.1" "glPointParameterfv" sys optional PointParameterfv 

	// GLAPI void APIENTRY PointParameteri (GLenum pname,GLint param)
	 ( Z32:_pname Z32:_param ) "libGL.so.1" "glPointParameteri" sys optional PointParameteri 

	// GLAPI void APIENTRY PointParameteriv (GLenum pname,const GLint * params)
	 ( Z32:_pname Z32*:_params ) "libGL.so.1" "glPointParameteriv" sys optional PointParameteriv 

	// GLAPI void APIENTRY BlendColor (GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
	 ( Real:_red Real:_green Real:_blue Real:_alpha ) "libGL.so.1" "glBlendColor" sys optional BlendColor 

	// GLAPI void APIENTRY BlendEquation (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glBlendEquation" sys optional BlendEquation 

	// GLAPI void APIENTRY GenQueries (GLsizei n,GLuint * ids)
	 ( Z32:_n Z32*:_ids ) "libGL.so.1" "glGenQueries" sys optional GenQueries 

	// GLAPI void APIENTRY DeleteQueries (GLsizei n,const GLuint * ids)
	 ( Z32:_n Z32*:_ids ) "libGL.so.1" "glDeleteQueries" sys optional DeleteQueries 

	// GLAPI GLboolean APIENTRY IsQuery (GLuint id)
	 ( Z32:_id  -> Bit ) "libGL.so.1" "glIsQuery" sys optional IsQuery 

	// GLAPI void APIENTRY BeginQuery (GLenum target,GLuint id)
	 ( Z32:_target Z32:_id ) "libGL.so.1" "glBeginQuery" sys optional BeginQuery 

	// GLAPI void APIENTRY EndQuery (GLenum target)
	 ( Z32:_target ) "libGL.so.1" "glEndQuery" sys optional EndQuery 

	// GLAPI void APIENTRY GetQueryiv (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetQueryiv" sys optional GetQueryiv 

	// GLAPI void APIENTRY GetQueryObjectiv (GLuint id,GLenum pname,GLint * params)
	 ( Z32:_id Z32:_pname Z32*:_params ) "libGL.so.1" "glGetQueryObjectiv" sys optional GetQueryObjectiv 

	// GLAPI void APIENTRY GetQueryObjectuiv (GLuint id,GLenum pname,GLuint * params)
	 ( Z32:_id Z32:_pname Z32*:_params ) "libGL.so.1" "glGetQueryObjectuiv" sys optional GetQueryObjectuiv 

	// GLAPI void APIENTRY BindBuffer (GLenum target,GLuint buffer)
	 ( Z32:_target Z32:_buffer ) "libGL.so.1" "glBindBuffer" sys optional BindBuffer 

	// GLAPI void APIENTRY DeleteBuffers (GLsizei n,const GLuint * buffers)
	 ( Z32:_n Z32*:_buffers ) "libGL.so.1" "glDeleteBuffers" sys optional DeleteBuffers 

	// GLAPI void APIENTRY GenBuffers (GLsizei n,GLuint * buffers)
	 ( Z32:_n Z32*:_buffers ) "libGL.so.1" "glGenBuffers" sys optional GenBuffers 

	// GLAPI GLboolean APIENTRY IsBuffer (GLuint buffer)
	 ( Z32:_buffer  -> Bit ) "libGL.so.1" "glIsBuffer" sys optional IsBuffer 

	// GLAPI void APIENTRY BufferData (GLenum target,GLsizeiptr size,const void * data,GLenum usage)
	 ( Z32:_target Z32:_size Any*:_data Z32:_usage ) "libGL.so.1" "glBufferData" sys optional BufferData 

	// GLAPI void APIENTRY BufferSubData (GLenum target,GLintptr offset,GLsizeiptr size,const void * data)
	 ( Z32:_target Any&:_offset Z32:_size Any*:_data ) "libGL.so.1" "glBufferSubData" sys optional BufferSubData 

	// GLAPI void APIENTRY GetBufferSubData (GLenum target,GLintptr offset,GLsizeiptr size,void * data)
	 ( Z32:_target Any&:_offset Z32:_size Any*:_data ) "libGL.so.1" "glGetBufferSubData" sys optional GetBufferSubData 

	// GLAPI void * APIENTRY MapBuffer (GLenum target,GLenum access)
	 ( Z32:_target Z32:_access  -> Any* ) "libGL.so.1" "glMapBuffer" sys optional MapBuffer 

	// GLAPI GLboolean APIENTRY UnmapBuffer (GLenum target)
	 ( Z32:_target  -> Bit ) "libGL.so.1" "glUnmapBuffer" sys optional UnmapBuffer 

	// GLAPI void APIENTRY GetBufferParameteriv (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetBufferParameteriv" sys optional GetBufferParameteriv 

	// GLAPI void APIENTRY GetBufferPointerv (GLenum target,GLenum pname,void ** params)
	 ( Z32:_target Z32:_pname Any**:_params ) "libGL.so.1" "glGetBufferPointerv" sys optional GetBufferPointerv 

	// GLAPI void APIENTRY BlendEquationSeparate (GLenum modeRGB,GLenum modeAlpha)
	 ( Z32:_modeRGB Z32:_modeAlpha ) "libGL.so.1" "glBlendEquationSeparate" sys optional BlendEquationSeparate 

	// GLAPI void APIENTRY DrawBuffers (GLsizei n,const GLenum * bufs)
	 ( Z32:_n Z32*:_bufs ) "libGL.so.1" "glDrawBuffers" sys optional DrawBuffers 

	// GLAPI void APIENTRY StencilOpSeparate (GLenum face,GLenum sfail,GLenum dpfail,GLenum dppass)
	 ( Z32:_face Z32:_sfail Z32:_dpfail Z32:_dppass ) "libGL.so.1" "glStencilOpSeparate" sys optional StencilOpSeparate 

	// GLAPI void APIENTRY StencilFuncSeparate (GLenum face,GLenum func,GLint ref,GLuint mask)
	 ( Z32:_face Z32:_func Z32:_ref Z32:_mask ) "libGL.so.1" "glStencilFuncSeparate" sys optional StencilFuncSeparate 

	// GLAPI void APIENTRY StencilMaskSeparate (GLenum face,GLuint mask)
	 ( Z32:_face Z32:_mask ) "libGL.so.1" "glStencilMaskSeparate" sys optional StencilMaskSeparate 

	// GLAPI void APIENTRY AttachShader (GLuint program,GLuint shader)
	 ( Z32:_program Z32:_shader ) "libGL.so.1" "glAttachShader" sys optional AttachShader 

	// GLAPI void APIENTRY BindAttribLocation (GLuint program,GLuint index,const GLchar * name)
	 ( Z32:_program Z32:_index String:_name ) "libGL.so.1" "glBindAttribLocation" sys optional BindAttribLocation 

	// GLAPI void APIENTRY CompileShader (GLuint shader)
	 ( Z32:_shader ) "libGL.so.1" "glCompileShader" sys optional CompileShader 

	// GLAPI GLuint APIENTRY CreateProgram (void)
	 (  -> Z32 ) "libGL.so.1" "glCreateProgram" sys optional CreateProgram 

	// GLAPI GLuint APIENTRY CreateShader (GLenum type)
	 ( Z32:_type  -> Z32 ) "libGL.so.1" "glCreateShader" sys optional CreateShader 

	// GLAPI void APIENTRY DeleteProgram (GLuint program)
	 ( Z32:_program ) "libGL.so.1" "glDeleteProgram" sys optional DeleteProgram 

	// GLAPI void APIENTRY DeleteShader (GLuint shader)
	 ( Z32:_shader ) "libGL.so.1" "glDeleteShader" sys optional DeleteShader 

	// GLAPI void APIENTRY DetachShader (GLuint program,GLuint shader)
	 ( Z32:_program Z32:_shader ) "libGL.so.1" "glDetachShader" sys optional DetachShader 

	// GLAPI void APIENTRY DisableVertexAttribArray (GLuint index)
	 ( Z32:_index ) "libGL.so.1" "glDisableVertexAttribArray" sys optional DisableVertexAttribArray 

	// GLAPI void APIENTRY EnableVertexAttribArray (GLuint index)
	 ( Z32:_index ) "libGL.so.1" "glEnableVertexAttribArray" sys optional EnableVertexAttribArray 

	// GLAPI void APIENTRY GetActiveAttrib (GLuint program,GLuint index,GLsizei bufSize,GLsizei * length,GLint * size,GLenum * type,GLchar * name)
	 ( Z32:_program Z32:_index Z32:_bufSize Z32*:_length Z32*:_size Z32*:_type String:_name ) "libGL.so.1" "glGetActiveAttrib" sys optional GetActiveAttrib 

	// GLAPI void APIENTRY GetActiveUniform (GLuint program,GLuint index,GLsizei bufSize,GLsizei * length,GLint * size,GLenum * type,GLchar * name)
	 ( Z32:_program Z32:_index Z32:_bufSize Z32*:_length Z32*:_size Z32*:_type String:_name ) "libGL.so.1" "glGetActiveUniform" sys optional GetActiveUniform 

	// GLAPI void APIENTRY GetAttachedShaders (GLuint program,GLsizei maxCount,GLsizei * count,GLuint * shaders)
	 ( Z32:_program Z32:_maxCount Z32*:_count Z32*:_shaders ) "libGL.so.1" "glGetAttachedShaders" sys optional GetAttachedShaders 

	// GLAPI GLint APIENTRY GetAttribLocation (GLuint program,const GLchar * name)
	 ( Z32:_program String:_name  -> Z32 ) "libGL.so.1" "glGetAttribLocation" sys optional GetAttribLocation 

	// GLAPI void APIENTRY GetProgramiv (GLuint program,GLenum pname,GLint * params)
	 ( Z32:_program Z32:_pname Z32*:_params ) "libGL.so.1" "glGetProgramiv" sys optional GetProgramiv 

	// GLAPI void APIENTRY GetProgramInfoLog (GLuint program,GLsizei bufSize,GLsizei * length,GLchar * infoLog)
	 ( Z32:_program Z32:_bufSize Z32*:_length String:_infoLog ) "libGL.so.1" "glGetProgramInfoLog" sys optional GetProgramInfoLog 

	// GLAPI void APIENTRY GetShaderiv (GLuint shader,GLenum pname,GLint * params)
	 ( Z32:_shader Z32:_pname Z32*:_params ) "libGL.so.1" "glGetShaderiv" sys optional GetShaderiv 

	// GLAPI void APIENTRY GetShaderInfoLog (GLuint shader,GLsizei bufSize,GLsizei * length,GLchar * infoLog)
	 ( Z32:_shader Z32:_bufSize Z32*:_length String:_infoLog ) "libGL.so.1" "glGetShaderInfoLog" sys optional GetShaderInfoLog 

	// GLAPI void APIENTRY GetShaderSource (GLuint shader,GLsizei bufSize,GLsizei * length,GLchar * source)
	 ( Z32:_shader Z32:_bufSize Z32*:_length String:_source ) "libGL.so.1" "glGetShaderSource" sys optional GetShaderSource 

	// GLAPI GLint APIENTRY GetUniformLocation (GLuint program,const GLchar * name)
	 ( Z32:_program String:_name  -> Z32 ) "libGL.so.1" "glGetUniformLocation" sys optional GetUniformLocation 

	// GLAPI void APIENTRY GetUniformfv (GLuint program,GLint location,GLfloat * params)
	 ( Z32:_program Z32:_location Real*:_params ) "libGL.so.1" "glGetUniformfv" sys optional GetUniformfv 

	// GLAPI void APIENTRY GetUniformiv (GLuint program,GLint location,GLint * params)
	 ( Z32:_program Z32:_location Z32*:_params ) "libGL.so.1" "glGetUniformiv" sys optional GetUniformiv 

	// GLAPI void APIENTRY GetVertexAttribdv (GLuint index,GLenum pname,GLdouble * params)
	 ( Z32:_index Z32:_pname Double32*:_params ) "libGL.so.1" "glGetVertexAttribdv" sys optional GetVertexAttribdv 

	// GLAPI void APIENTRY GetVertexAttribfv (GLuint index,GLenum pname,GLfloat * params)
	 ( Z32:_index Z32:_pname Real*:_params ) "libGL.so.1" "glGetVertexAttribfv" sys optional GetVertexAttribfv 

	// GLAPI void APIENTRY GetVertexAttribiv (GLuint index,GLenum pname,GLint * params)
	 ( Z32:_index Z32:_pname Z32*:_params ) "libGL.so.1" "glGetVertexAttribiv" sys optional GetVertexAttribiv 

	// GLAPI void APIENTRY GetVertexAttribPointerv (GLuint index,GLenum pname,void ** pointer)
	 ( Z32:_index Z32:_pname Any**:_pointer ) "libGL.so.1" "glGetVertexAttribPointerv" sys optional GetVertexAttribPointerv 

	// GLAPI GLboolean APIENTRY IsProgram (GLuint program)
	 ( Z32:_program  -> Bit ) "libGL.so.1" "glIsProgram" sys optional IsProgram 

	// GLAPI GLboolean APIENTRY IsShader (GLuint shader)
	 ( Z32:_shader  -> Bit ) "libGL.so.1" "glIsShader" sys optional IsShader 

	// GLAPI void APIENTRY LinkProgram (GLuint program)
	 ( Z32:_program ) "libGL.so.1" "glLinkProgram" sys optional LinkProgram 

	// GLAPI void APIENTRY ShaderSource (GLuint shader,GLsizei count,const GLchar * const * string,const GLint * length)
	 ( Z32:_shader Z32:_count String*:_string Z32*:_length ) "libGL.so.1" "glShaderSource" sys optional ShaderSource 

	// GLAPI void APIENTRY UseProgram (GLuint program)
	 ( Z32:_program ) "libGL.so.1" "glUseProgram" sys optional UseProgram 

	// GLAPI void APIENTRY Uniform1f (GLint location,GLfloat v0)
	 ( Z32:_location Real:_v0 ) "libGL.so.1" "glUniform1f" sys optional Uniform1f 

	// GLAPI void APIENTRY Uniform2f (GLint location,GLfloat v0,GLfloat v1)
	 ( Z32:_location Real:_v0 Real:_v1 ) "libGL.so.1" "glUniform2f" sys optional Uniform2f 

	// GLAPI void APIENTRY Uniform3f (GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
	 ( Z32:_location Real:_v0 Real:_v1 Real:_v2 ) "libGL.so.1" "glUniform3f" sys optional Uniform3f 

	// GLAPI void APIENTRY Uniform4f (GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
	 ( Z32:_location Real:_v0 Real:_v1 Real:_v2 Real:_v3 ) "libGL.so.1" "glUniform4f" sys optional Uniform4f 

	// GLAPI void APIENTRY Uniform1i (GLint location,GLint v0)
	 ( Z32:_location Z32:_v0 ) "libGL.so.1" "glUniform1i" sys optional Uniform1i 

	// GLAPI void APIENTRY Uniform2i (GLint location,GLint v0,GLint v1)
	 ( Z32:_location Z32:_v0 Z32:_v1 ) "libGL.so.1" "glUniform2i" sys optional Uniform2i 

	// GLAPI void APIENTRY Uniform3i (GLint location,GLint v0,GLint v1,GLint v2)
	 ( Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 ) "libGL.so.1" "glUniform3i" sys optional Uniform3i 

	// GLAPI void APIENTRY Uniform4i (GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
	 ( Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 Z32:_v3 ) "libGL.so.1" "glUniform4i" sys optional Uniform4i 

	// GLAPI void APIENTRY Uniform1fv (GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glUniform1fv" sys optional Uniform1fv 

	// GLAPI void APIENTRY Uniform2fv (GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glUniform2fv" sys optional Uniform2fv 

	// GLAPI void APIENTRY Uniform3fv (GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glUniform3fv" sys optional Uniform3fv 

	// GLAPI void APIENTRY Uniform4fv (GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glUniform4fv" sys optional Uniform4fv 

	// GLAPI void APIENTRY Uniform1iv (GLint location,GLsizei count,const GLint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform1iv" sys optional Uniform1iv 

	// GLAPI void APIENTRY Uniform2iv (GLint location,GLsizei count,const GLint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform2iv" sys optional Uniform2iv 

	// GLAPI void APIENTRY Uniform3iv (GLint location,GLsizei count,const GLint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform3iv" sys optional Uniform3iv 

	// GLAPI void APIENTRY Uniform4iv (GLint location,GLsizei count,const GLint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform4iv" sys optional Uniform4iv 

	// GLAPI void APIENTRY UniformMatrix2fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix2fv" sys optional UniformMatrix2fv 

	// GLAPI void APIENTRY UniformMatrix3fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix3fv" sys optional UniformMatrix3fv 

	// GLAPI void APIENTRY UniformMatrix4fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix4fv" sys optional UniformMatrix4fv 

	// GLAPI void APIENTRY ValidateProgram (GLuint program)
	 ( Z32:_program ) "libGL.so.1" "glValidateProgram" sys optional ValidateProgram 

	// GLAPI void APIENTRY VertexAttrib1d (GLuint index,GLdouble x)
	 ( Z32:_index Double32:_x ) "libGL.so.1" "glVertexAttrib1d" sys optional VertexAttrib1d 

	// GLAPI void APIENTRY VertexAttrib1dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttrib1dv" sys optional VertexAttrib1dv 

	// GLAPI void APIENTRY VertexAttrib1f (GLuint index,GLfloat x)
	 ( Z32:_index Real:_x ) "libGL.so.1" "glVertexAttrib1f" sys optional VertexAttrib1f 

	// GLAPI void APIENTRY VertexAttrib1fv (GLuint index,const GLfloat * v)
	 ( Z32:_index Real*:_v ) "libGL.so.1" "glVertexAttrib1fv" sys optional VertexAttrib1fv 

	// GLAPI void APIENTRY VertexAttrib1s (GLuint index,GLshort x)
	 ( Z32:_index Z32:_x ) "libGL.so.1" "glVertexAttrib1s" sys optional VertexAttrib1s 

	// GLAPI void APIENTRY VertexAttrib1sv (GLuint index,const GLshort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib1sv" sys optional VertexAttrib1sv 

	// GLAPI void APIENTRY VertexAttrib2d (GLuint index,GLdouble x,GLdouble y)
	 ( Z32:_index Double32:_x Double32:_y ) "libGL.so.1" "glVertexAttrib2d" sys optional VertexAttrib2d 

	// GLAPI void APIENTRY VertexAttrib2dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttrib2dv" sys optional VertexAttrib2dv 

	// GLAPI void APIENTRY VertexAttrib2f (GLuint index,GLfloat x,GLfloat y)
	 ( Z32:_index Real:_x Real:_y ) "libGL.so.1" "glVertexAttrib2f" sys optional VertexAttrib2f 

	// GLAPI void APIENTRY VertexAttrib2fv (GLuint index,const GLfloat * v)
	 ( Z32:_index Real*:_v ) "libGL.so.1" "glVertexAttrib2fv" sys optional VertexAttrib2fv 

	// GLAPI void APIENTRY VertexAttrib2s (GLuint index,GLshort x,GLshort y)
	 ( Z32:_index Z32:_x Z32:_y ) "libGL.so.1" "glVertexAttrib2s" sys optional VertexAttrib2s 

	// GLAPI void APIENTRY VertexAttrib2sv (GLuint index,const GLshort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib2sv" sys optional VertexAttrib2sv 

	// GLAPI void APIENTRY VertexAttrib3d (GLuint index,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_index Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glVertexAttrib3d" sys optional VertexAttrib3d 

	// GLAPI void APIENTRY VertexAttrib3dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttrib3dv" sys optional VertexAttrib3dv 

	// GLAPI void APIENTRY VertexAttrib3f (GLuint index,GLfloat x,GLfloat y,GLfloat z)
	 ( Z32:_index Real:_x Real:_y Real:_z ) "libGL.so.1" "glVertexAttrib3f" sys optional VertexAttrib3f 

	// GLAPI void APIENTRY VertexAttrib3fv (GLuint index,const GLfloat * v)
	 ( Z32:_index Real*:_v ) "libGL.so.1" "glVertexAttrib3fv" sys optional VertexAttrib3fv 

	// GLAPI void APIENTRY VertexAttrib3s (GLuint index,GLshort x,GLshort y,GLshort z)
	 ( Z32:_index Z32:_x Z32:_y Z32:_z ) "libGL.so.1" "glVertexAttrib3s" sys optional VertexAttrib3s 

	// GLAPI void APIENTRY VertexAttrib3sv (GLuint index,const GLshort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib3sv" sys optional VertexAttrib3sv 

	// GLAPI void APIENTRY VertexAttrib4Nbv (GLuint index,const GLbyte * v)
	 ( Z32:_index Byte*:_v ) "libGL.so.1" "glVertexAttrib4Nbv" sys optional VertexAttrib4Nbv 

	// GLAPI void APIENTRY VertexAttrib4Niv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4Niv" sys optional VertexAttrib4Niv 

	// GLAPI void APIENTRY VertexAttrib4Nsv (GLuint index,const GLshort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4Nsv" sys optional VertexAttrib4Nsv 

	// GLAPI void APIENTRY VertexAttrib4Nub (GLuint index,GLubyte x,GLubyte y,GLubyte z,GLubyte w)
	 ( Z32:_index Byte:_x Byte:_y Byte:_z Byte:_w ) "libGL.so.1" "glVertexAttrib4Nub" sys optional VertexAttrib4Nub 

	// GLAPI void APIENTRY VertexAttrib4Nubv (GLuint index,const GLubyte * v)
	 ( Z32:_index String:_v ) "libGL.so.1" "glVertexAttrib4Nubv" sys optional VertexAttrib4Nubv 

	// GLAPI void APIENTRY VertexAttrib4Nuiv (GLuint index,const GLuint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4Nuiv" sys optional VertexAttrib4Nuiv 

	// GLAPI void APIENTRY VertexAttrib4Nusv (GLuint index,const GLushort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4Nusv" sys optional VertexAttrib4Nusv 

	// GLAPI void APIENTRY VertexAttrib4bv (GLuint index,const GLbyte * v)
	 ( Z32:_index Byte*:_v ) "libGL.so.1" "glVertexAttrib4bv" sys optional VertexAttrib4bv 

	// GLAPI void APIENTRY VertexAttrib4d (GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
	 ( Z32:_index Double32:_x Double32:_y Double32:_z Double32:_w ) "libGL.so.1" "glVertexAttrib4d" sys optional VertexAttrib4d 

	// GLAPI void APIENTRY VertexAttrib4dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttrib4dv" sys optional VertexAttrib4dv 

	// GLAPI void APIENTRY VertexAttrib4f (GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w)
	 ( Z32:_index Real:_x Real:_y Real:_z Real:_w ) "libGL.so.1" "glVertexAttrib4f" sys optional VertexAttrib4f 

	// GLAPI void APIENTRY VertexAttrib4fv (GLuint index,const GLfloat * v)
	 ( Z32:_index Real*:_v ) "libGL.so.1" "glVertexAttrib4fv" sys optional VertexAttrib4fv 

	// GLAPI void APIENTRY VertexAttrib4iv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4iv" sys optional VertexAttrib4iv 

	// GLAPI void APIENTRY VertexAttrib4s (GLuint index,GLshort x,GLshort y,GLshort z,GLshort w)
	 ( Z32:_index Z32:_x Z32:_y Z32:_z Z32:_w ) "libGL.so.1" "glVertexAttrib4s" sys optional VertexAttrib4s 

	// GLAPI void APIENTRY VertexAttrib4sv (GLuint index,const GLshort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4sv" sys optional VertexAttrib4sv 

	// GLAPI void APIENTRY VertexAttrib4ubv (GLuint index,const GLubyte * v)
	 ( Z32:_index String:_v ) "libGL.so.1" "glVertexAttrib4ubv" sys optional VertexAttrib4ubv 

	// GLAPI void APIENTRY VertexAttrib4uiv (GLuint index,const GLuint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4uiv" sys optional VertexAttrib4uiv 

	// GLAPI void APIENTRY VertexAttrib4usv (GLuint index,const GLushort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttrib4usv" sys optional VertexAttrib4usv 

	// GLAPI void APIENTRY VertexAttribPointer (GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const void * pointer)
	( Z32:_index Z32:_size Z32:_type Bit:_normalized Z32:_stride Size32:_pointer ) "libGL.so.1" "glVertexAttribPointer" sys optional VertexAttribPointer
	// GLAPI void APIENTRY UniformMatrix2x3fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix2x3fv" sys optional UniformMatrix2x3fv 

	// GLAPI void APIENTRY UniformMatrix3x2fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix3x2fv" sys optional UniformMatrix3x2fv 

	// GLAPI void APIENTRY UniformMatrix2x4fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix2x4fv" sys optional UniformMatrix2x4fv 

	// GLAPI void APIENTRY UniformMatrix4x2fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix4x2fv" sys optional UniformMatrix4x2fv 

	// GLAPI void APIENTRY UniformMatrix3x4fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix3x4fv" sys optional UniformMatrix3x4fv 

	// GLAPI void APIENTRY UniformMatrix4x3fv (GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glUniformMatrix4x3fv" sys optional UniformMatrix4x3fv 

	// GLAPI void APIENTRY ColorMaski (GLuint index,GLboolean r,GLboolean g,GLboolean b,GLboolean a)
	 ( Z32:_index Bit:_r Bit:_g Bit:_b Bit:_a ) "libGL.so.1" "glColorMaski" sys optional ColorMaski 

	// GLAPI void APIENTRY GetBooleani_v (GLenum target,GLuint index,GLboolean * data)
	 ( Z32:_target Z32:_index Bit*:_data ) "libGL.so.1" "glGetBooleani_v" sys optional GetBooleani_v 

	// GLAPI void APIENTRY GetIntegeri_v (GLenum target,GLuint index,GLint * data)
	 ( Z32:_target Z32:_index Z32*:_data ) "libGL.so.1" "glGetIntegeri_v" sys optional GetIntegeri_v 

	// GLAPI void APIENTRY Enablei (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index ) "libGL.so.1" "glEnablei" sys optional Enablei 

	// GLAPI void APIENTRY Disablei (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index ) "libGL.so.1" "glDisablei" sys optional Disablei 

	// GLAPI GLboolean APIENTRY IsEnabledi (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index  -> Bit ) "libGL.so.1" "glIsEnabledi" sys optional IsEnabledi 

	// GLAPI void APIENTRY BeginTransformFeedback (GLenum primitiveMode)
	 ( Z32:_primitiveMode ) "libGL.so.1" "glBeginTransformFeedback" sys optional BeginTransformFeedback 

	// GLAPI void APIENTRY EndTransformFeedback (void)
	 ( ) "libGL.so.1" "glEndTransformFeedback" sys optional EndTransformFeedback 

	// GLAPI void APIENTRY BindBufferRange (GLenum target,GLuint index,GLuint buffer,GLintptr offset,GLsizeiptr size)
	 ( Z32:_target Z32:_index Z32:_buffer Any&:_offset Z32:_size ) "libGL.so.1" "glBindBufferRange" sys optional BindBufferRange 

	// GLAPI void APIENTRY BindBufferBase (GLenum target,GLuint index,GLuint buffer)
	 ( Z32:_target Z32:_index Z32:_buffer ) "libGL.so.1" "glBindBufferBase" sys optional BindBufferBase 

	// GLAPI void APIENTRY TransformFeedbackVaryings (GLuint program,GLsizei count,const GLchar * const * varyings,GLenum bufferMode)
	 ( Z32:_program Z32:_count String*:_varyings Z32:_bufferMode ) "libGL.so.1" "glTransformFeedbackVaryings" sys optional TransformFeedbackVaryings 

	// GLAPI void APIENTRY GetTransformFeedbackVarying (GLuint program,GLuint index,GLsizei bufSize,GLsizei * length,GLsizei * size,GLenum * type,GLchar * name)
	 ( Z32:_program Z32:_index Z32:_bufSize Z32*:_length Z32*:_size Z32*:_type String:_name ) "libGL.so.1" "glGetTransformFeedbackVarying" sys optional GetTransformFeedbackVarying 

	// GLAPI void APIENTRY ClampColor (GLenum target,GLenum clamp)
	 ( Z32:_target Z32:_clamp ) "libGL.so.1" "glClampColor" sys optional ClampColor 

	// GLAPI void APIENTRY BeginConditionalRender (GLuint id,GLenum mode)
	 ( Z32:_id Z32:_mode ) "libGL.so.1" "glBeginConditionalRender" sys optional BeginConditionalRender 

	// GLAPI void APIENTRY EndConditionalRender (void)
	 ( ) "libGL.so.1" "glEndConditionalRender" sys optional EndConditionalRender 

	// GLAPI void APIENTRY VertexAttribIPointer (GLuint index,GLint size,GLenum type,GLsizei stride,const void * pointer)
	 ( Z32:_index Z32:_size Z32:_type Z32:_stride Any*:_pointer ) "libGL.so.1" "glVertexAttribIPointer" sys optional VertexAttribIPointer 

	// GLAPI void APIENTRY GetVertexAttribIiv (GLuint index,GLenum pname,GLint * params)
	 ( Z32:_index Z32:_pname Z32*:_params ) "libGL.so.1" "glGetVertexAttribIiv" sys optional GetVertexAttribIiv 

	// GLAPI void APIENTRY GetVertexAttribIuiv (GLuint index,GLenum pname,GLuint * params)
	 ( Z32:_index Z32:_pname Z32*:_params ) "libGL.so.1" "glGetVertexAttribIuiv" sys optional GetVertexAttribIuiv 

	// GLAPI void APIENTRY VertexAttribI1i (GLuint index,GLint x)
	 ( Z32:_index Z32:_x ) "libGL.so.1" "glVertexAttribI1i" sys optional VertexAttribI1i 

	// GLAPI void APIENTRY VertexAttribI2i (GLuint index,GLint x,GLint y)
	 ( Z32:_index Z32:_x Z32:_y ) "libGL.so.1" "glVertexAttribI2i" sys optional VertexAttribI2i 

	// GLAPI void APIENTRY VertexAttribI3i (GLuint index,GLint x,GLint y,GLint z)
	 ( Z32:_index Z32:_x Z32:_y Z32:_z ) "libGL.so.1" "glVertexAttribI3i" sys optional VertexAttribI3i 

	// GLAPI void APIENTRY VertexAttribI4i (GLuint index,GLint x,GLint y,GLint z,GLint w)
	 ( Z32:_index Z32:_x Z32:_y Z32:_z Z32:_w ) "libGL.so.1" "glVertexAttribI4i" sys optional VertexAttribI4i 

	// GLAPI void APIENTRY VertexAttribI1ui (GLuint index,GLuint x)
	 ( Z32:_index Z32:_x ) "libGL.so.1" "glVertexAttribI1ui" sys optional VertexAttribI1ui 

	// GLAPI void APIENTRY VertexAttribI2ui (GLuint index,GLuint x,GLuint y)
	 ( Z32:_index Z32:_x Z32:_y ) "libGL.so.1" "glVertexAttribI2ui" sys optional VertexAttribI2ui 

	// GLAPI void APIENTRY VertexAttribI3ui (GLuint index,GLuint x,GLuint y,GLuint z)
	 ( Z32:_index Z32:_x Z32:_y Z32:_z ) "libGL.so.1" "glVertexAttribI3ui" sys optional VertexAttribI3ui 

	// GLAPI void APIENTRY VertexAttribI4ui (GLuint index,GLuint x,GLuint y,GLuint z,GLuint w)
	 ( Z32:_index Z32:_x Z32:_y Z32:_z Z32:_w ) "libGL.so.1" "glVertexAttribI4ui" sys optional VertexAttribI4ui 

	// GLAPI void APIENTRY VertexAttribI1iv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI1iv" sys optional VertexAttribI1iv 

	// GLAPI void APIENTRY VertexAttribI2iv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI2iv" sys optional VertexAttribI2iv 

	// GLAPI void APIENTRY VertexAttribI3iv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI3iv" sys optional VertexAttribI3iv 

	// GLAPI void APIENTRY VertexAttribI4iv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI4iv" sys optional VertexAttribI4iv 

	// GLAPI void APIENTRY VertexAttribI1uiv (GLuint index,const GLuint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI1uiv" sys optional VertexAttribI1uiv 

	// GLAPI void APIENTRY VertexAttribI2uiv (GLuint index,const GLuint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI2uiv" sys optional VertexAttribI2uiv 

	// GLAPI void APIENTRY VertexAttribI3uiv (GLuint index,const GLuint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI3uiv" sys optional VertexAttribI3uiv 

	// GLAPI void APIENTRY VertexAttribI4uiv (GLuint index,const GLuint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI4uiv" sys optional VertexAttribI4uiv 

	// GLAPI void APIENTRY VertexAttribI4bv (GLuint index,const GLbyte * v)
	 ( Z32:_index Byte*:_v ) "libGL.so.1" "glVertexAttribI4bv" sys optional VertexAttribI4bv 

	// GLAPI void APIENTRY VertexAttribI4sv (GLuint index,const GLshort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI4sv" sys optional VertexAttribI4sv 

	// GLAPI void APIENTRY VertexAttribI4ubv (GLuint index,const GLubyte * v)
	 ( Z32:_index String:_v ) "libGL.so.1" "glVertexAttribI4ubv" sys optional VertexAttribI4ubv 

	// GLAPI void APIENTRY VertexAttribI4usv (GLuint index,const GLushort * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glVertexAttribI4usv" sys optional VertexAttribI4usv 

	// GLAPI void APIENTRY GetUniformuiv (GLuint program,GLint location,GLuint * params)
	 ( Z32:_program Z32:_location Z32*:_params ) "libGL.so.1" "glGetUniformuiv" sys optional GetUniformuiv 

	// GLAPI void APIENTRY BindFragDataLocation (GLuint program,GLuint color,const GLchar * name)
	 ( Z32:_program Z32:_color String:_name ) "libGL.so.1" "glBindFragDataLocation" sys optional BindFragDataLocation 

	// GLAPI GLint APIENTRY GetFragDataLocation (GLuint program,const GLchar * name)
	 ( Z32:_program String:_name  -> Z32 ) "libGL.so.1" "glGetFragDataLocation" sys optional GetFragDataLocation 

	// GLAPI void APIENTRY Uniform1ui (GLint location,GLuint v0)
	 ( Z32:_location Z32:_v0 ) "libGL.so.1" "glUniform1ui" sys optional Uniform1ui 

	// GLAPI void APIENTRY Uniform2ui (GLint location,GLuint v0,GLuint v1)
	 ( Z32:_location Z32:_v0 Z32:_v1 ) "libGL.so.1" "glUniform2ui" sys optional Uniform2ui 

	// GLAPI void APIENTRY Uniform3ui (GLint location,GLuint v0,GLuint v1,GLuint v2)
	 ( Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 ) "libGL.so.1" "glUniform3ui" sys optional Uniform3ui 

	// GLAPI void APIENTRY Uniform4ui (GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3)
	 ( Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 Z32:_v3 ) "libGL.so.1" "glUniform4ui" sys optional Uniform4ui 

	// GLAPI void APIENTRY Uniform1uiv (GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform1uiv" sys optional Uniform1uiv 

	// GLAPI void APIENTRY Uniform2uiv (GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform2uiv" sys optional Uniform2uiv 

	// GLAPI void APIENTRY Uniform3uiv (GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform3uiv" sys optional Uniform3uiv 

	// GLAPI void APIENTRY Uniform4uiv (GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glUniform4uiv" sys optional Uniform4uiv 

	// GLAPI void APIENTRY TexParameterIiv (GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glTexParameterIiv" sys optional TexParameterIiv 

	// GLAPI void APIENTRY TexParameterIuiv (GLenum target,GLenum pname,const GLuint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glTexParameterIuiv" sys optional TexParameterIuiv 

	// GLAPI void APIENTRY GetTexParameterIiv (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTexParameterIiv" sys optional GetTexParameterIiv 

	// GLAPI void APIENTRY GetTexParameterIuiv (GLenum target,GLenum pname,GLuint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTexParameterIuiv" sys optional GetTexParameterIuiv 

	// GLAPI void APIENTRY ClearBufferiv (GLenum buffer,GLint drawbuffer,const GLint * value)
	 ( Z32:_buffer Z32:_drawbuffer Z32*:_value ) "libGL.so.1" "glClearBufferiv" sys optional ClearBufferiv 

	// GLAPI void APIENTRY ClearBufferuiv (GLenum buffer,GLint drawbuffer,const GLuint * value)
	 ( Z32:_buffer Z32:_drawbuffer Z32*:_value ) "libGL.so.1" "glClearBufferuiv" sys optional ClearBufferuiv 

	// GLAPI void APIENTRY ClearBufferfv (GLenum buffer,GLint drawbuffer,const GLfloat * value)
	 ( Z32:_buffer Z32:_drawbuffer Real*:_value ) "libGL.so.1" "glClearBufferfv" sys optional ClearBufferfv 

	// GLAPI void APIENTRY ClearBufferfi (GLenum buffer,GLint drawbuffer,GLfloat depth,GLint stencil)
	 ( Z32:_buffer Z32:_drawbuffer Real:_depth Z32:_stencil ) "libGL.so.1" "glClearBufferfi" sys optional ClearBufferfi 

	// GLAPI const GLubyte * APIENTRY GetStringi (GLenum name,GLuint index)
	 ( Z32:_name Z32:_index  -> String ) "libGL.so.1" "glGetStringi" sys optional GetStringi 

	// GLAPI GLboolean APIENTRY IsRenderbuffer (GLuint renderbuffer)
	 ( Z32:_renderbuffer  -> Bit ) "libGL.so.1" "glIsRenderbuffer" sys optional IsRenderbuffer 

	// GLAPI void APIENTRY BindRenderbuffer (GLenum target,GLuint renderbuffer)
	 ( Z32:_target Z32:_renderbuffer ) "libGL.so.1" "glBindRenderbuffer" sys optional BindRenderbuffer 

	// GLAPI void APIENTRY DeleteRenderbuffers (GLsizei n,const GLuint * renderbuffers)
	 ( Z32:_n Z32*:_renderbuffers ) "libGL.so.1" "glDeleteRenderbuffers" sys optional DeleteRenderbuffers 

	// GLAPI void APIENTRY GenRenderbuffers (GLsizei n,GLuint * renderbuffers)
	 ( Z32:_n Z32*:_renderbuffers ) "libGL.so.1" "glGenRenderbuffers" sys optional GenRenderbuffers 

	// GLAPI void APIENTRY RenderbufferStorage (GLenum target,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glRenderbufferStorage" sys optional RenderbufferStorage 

	// GLAPI void APIENTRY GetRenderbufferParameteriv (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetRenderbufferParameteriv" sys optional GetRenderbufferParameteriv 

	// GLAPI GLboolean APIENTRY IsFramebuffer (GLuint framebuffer)
	 ( Z32:_framebuffer  -> Bit ) "libGL.so.1" "glIsFramebuffer" sys optional IsFramebuffer 

	// GLAPI void APIENTRY BindFramebuffer (GLenum target,GLuint framebuffer)
	 ( Z32:_target Z32:_framebuffer ) "libGL.so.1" "glBindFramebuffer" sys optional BindFramebuffer 

	// GLAPI void APIENTRY DeleteFramebuffers (GLsizei n,const GLuint * framebuffers)
	 ( Z32:_n Z32*:_framebuffers ) "libGL.so.1" "glDeleteFramebuffers" sys optional DeleteFramebuffers 

	// GLAPI void APIENTRY GenFramebuffers (GLsizei n,GLuint * framebuffers)
	 ( Z32:_n Z32*:_framebuffers ) "libGL.so.1" "glGenFramebuffers" sys optional GenFramebuffers 

	// GLAPI GLenum APIENTRY CheckFramebufferStatus (GLenum target)
	 ( Z32:_target  -> Z32 ) "libGL.so.1" "glCheckFramebufferStatus" sys optional CheckFramebufferStatus 

	// GLAPI void APIENTRY FramebufferTexture1D (GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
	 ( Z32:_target Z32:_attachment Z32:_textarget Z32:_texture Z32:_level ) "libGL.so.1" "glFramebufferTexture1D" sys optional FramebufferTexture1D 

	// GLAPI void APIENTRY FramebufferTexture2D (GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
	 ( Z32:_target Z32:_attachment Z32:_textarget Z32:_texture Z32:_level ) "libGL.so.1" "glFramebufferTexture2D" sys optional FramebufferTexture2D 

	// GLAPI void APIENTRY FramebufferTexture3D (GLenum target,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset)
	 ( Z32:_target Z32:_attachment Z32:_textarget Z32:_texture Z32:_level Z32:_zoffset ) "libGL.so.1" "glFramebufferTexture3D" sys optional FramebufferTexture3D 

	// GLAPI void APIENTRY FramebufferRenderbuffer (GLenum target,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer)
	 ( Z32:_target Z32:_attachment Z32:_renderbuffertarget Z32:_renderbuffer ) "libGL.so.1" "glFramebufferRenderbuffer" sys optional FramebufferRenderbuffer 

	// GLAPI void APIENTRY GetFramebufferAttachmentParameteriv (GLenum target,GLenum attachment,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_attachment Z32:_pname Z32*:_params ) "libGL.so.1" "glGetFramebufferAttachmentParameteriv" sys optional GetFramebufferAttachmentParameteriv 

	// GLAPI void APIENTRY GenerateMipmap (GLenum target)
	 ( Z32:_target ) "libGL.so.1" "glGenerateMipmap" sys optional GenerateMipmap 

	// GLAPI void APIENTRY BlitFramebuffer (GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter)
	 ( Z32:_srcX0 Z32:_srcY0 Z32:_srcX1 Z32:_srcY1 Z32:_dstX0 Z32:_dstY0 Z32:_dstX1 Z32:_dstY1 Z32:_mask Z32:_filter ) "libGL.so.1" "glBlitFramebuffer" sys optional BlitFramebuffer 

	// GLAPI void APIENTRY RenderbufferStorageMultisample (GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glRenderbufferStorageMultisample" sys optional RenderbufferStorageMultisample 

	// GLAPI void APIENTRY FramebufferTextureLayer (GLenum target,GLenum attachment,GLuint texture,GLint level,GLint layer)
	 ( Z32:_target Z32:_attachment Z32:_texture Z32:_level Z32:_layer ) "libGL.so.1" "glFramebufferTextureLayer" sys optional FramebufferTextureLayer 

	// GLAPI void * APIENTRY MapBufferRange (GLenum target,GLintptr offset,GLsizeiptr length,GLbitfield access)
	 ( Z32:_target Any&:_offset Z32:_length Z32:_access  -> Any* ) "libGL.so.1" "glMapBufferRange" sys optional MapBufferRange 

	// GLAPI void APIENTRY FlushMappedBufferRange (GLenum target,GLintptr offset,GLsizeiptr length)
	 ( Z32:_target Any&:_offset Z32:_length ) "libGL.so.1" "glFlushMappedBufferRange" sys optional FlushMappedBufferRange 

	// GLAPI void APIENTRY BindVertexArray (GLuint array)
	 ( Z32:_array ) "libGL.so.1" "glBindVertexArray" sys optional BindVertexArray 

	// GLAPI void APIENTRY DeleteVertexArrays (GLsizei n,const GLuint * arrays)
	 ( Z32:_n Z32*:_arrays ) "libGL.so.1" "glDeleteVertexArrays" sys optional DeleteVertexArrays 

	// GLAPI void APIENTRY GenVertexArrays (GLsizei n,GLuint * arrays)
	 ( Z32:_n Z32*:_arrays ) "libGL.so.1" "glGenVertexArrays" sys optional GenVertexArrays 

	// GLAPI GLboolean APIENTRY IsVertexArray (GLuint array)
	 ( Z32:_array  -> Bit ) "libGL.so.1" "glIsVertexArray" sys optional IsVertexArray 

	// GLAPI void APIENTRY DrawArraysInstanced (GLenum mode,GLint first,GLsizei count,GLsizei instancecount)
	 ( Z32:_mode Z32:_first Z32:_count Z32:_instancecount ) "libGL.so.1" "glDrawArraysInstanced" sys optional DrawArraysInstanced 

	// GLAPI void APIENTRY DrawElementsInstanced (GLenum mode,GLsizei count,GLenum type,const void * indices,GLsizei instancecount)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_instancecount ) "libGL.so.1" "glDrawElementsInstanced" sys optional DrawElementsInstanced 

	// GLAPI void APIENTRY TexBuffer (GLenum target,GLenum internalformat,GLuint buffer)
	 ( Z32:_target Z32:_internalformat Z32:_buffer ) "libGL.so.1" "glTexBuffer" sys optional TexBuffer 

	// GLAPI void APIENTRY PrimitiveRestartIndex (GLuint index)
	 ( Z32:_index ) "libGL.so.1" "glPrimitiveRestartIndex" sys optional PrimitiveRestartIndex 

	// GLAPI void APIENTRY CopyBufferSubData (GLenum readTarget,GLenum writeTarget,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size)
	 ( Z32:_readTarget Z32:_writeTarget Any&:_readOffset Any&:_writeOffset Z32:_size ) "libGL.so.1" "glCopyBufferSubData" sys optional CopyBufferSubData 

	// GLAPI void APIENTRY GetUniformIndices (GLuint program,GLsizei uniformCount,const GLchar * const * uniformNames,GLuint * uniformIndices)
	 ( Z32:_program Z32:_uniformCount String*:_uniformNames Z32*:_uniformIndices ) "libGL.so.1" "glGetUniformIndices" sys optional GetUniformIndices 

	// GLAPI void APIENTRY GetActiveUniformsiv (GLuint program,GLsizei uniformCount,const GLuint * uniformIndices,GLenum pname,GLint * params)
	 ( Z32:_program Z32:_uniformCount Z32*:_uniformIndices Z32:_pname Z32*:_params ) "libGL.so.1" "glGetActiveUniformsiv" sys optional GetActiveUniformsiv 

	// GLAPI void APIENTRY GetActiveUniformName (GLuint program,GLuint uniformIndex,GLsizei bufSize,GLsizei * length,GLchar * uniformName)
	 ( Z32:_program Z32:_uniformIndex Z32:_bufSize Z32*:_length String:_uniformName ) "libGL.so.1" "glGetActiveUniformName" sys optional GetActiveUniformName 

	// GLAPI GLuint APIENTRY GetUniformBlockIndex (GLuint program,const GLchar * uniformBlockName)
	 ( Z32:_program String:_uniformBlockName  -> Z32 ) "libGL.so.1" "glGetUniformBlockIndex" sys optional GetUniformBlockIndex 

	// GLAPI void APIENTRY GetActiveUniformBlockiv (GLuint program,GLuint uniformBlockIndex,GLenum pname,GLint * params)
	 ( Z32:_program Z32:_uniformBlockIndex Z32:_pname Z32*:_params ) "libGL.so.1" "glGetActiveUniformBlockiv" sys optional GetActiveUniformBlockiv 

	// GLAPI void APIENTRY GetActiveUniformBlockName (GLuint program,GLuint uniformBlockIndex,GLsizei bufSize,GLsizei * length,GLchar * uniformBlockName)
	 ( Z32:_program Z32:_uniformBlockIndex Z32:_bufSize Z32*:_length String:_uniformBlockName ) "libGL.so.1" "glGetActiveUniformBlockName" sys optional GetActiveUniformBlockName 

	// GLAPI void APIENTRY UniformBlockBinding (GLuint program,GLuint uniformBlockIndex,GLuint uniformBlockBinding)
	 ( Z32:_program Z32:_uniformBlockIndex Z32:_uniformBlockBinding ) "libGL.so.1" "glUniformBlockBinding" sys optional UniformBlockBinding 

	// GLAPI void APIENTRY DrawElementsBaseVertex (GLenum mode,GLsizei count,GLenum type,const void * indices,GLint basevertex)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_basevertex ) "libGL.so.1" "glDrawElementsBaseVertex" sys optional DrawElementsBaseVertex 

	// GLAPI void APIENTRY DrawRangeElementsBaseVertex (GLenum mode,GLuint start,GLuint end,GLsizei count,GLenum type,const void * indices,GLint basevertex)
	 ( Z32:_mode Z32:_start Z32:_end Z32:_count Z32:_type Any*:_indices Z32:_basevertex ) "libGL.so.1" "glDrawRangeElementsBaseVertex" sys optional DrawRangeElementsBaseVertex 

	// GLAPI void APIENTRY DrawElementsInstancedBaseVertex (GLenum mode,GLsizei count,GLenum type,const void * indices,GLsizei instancecount,GLint basevertex)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_instancecount Z32:_basevertex ) "libGL.so.1" "glDrawElementsInstancedBaseVertex" sys optional DrawElementsInstancedBaseVertex 

	// GLAPI void APIENTRY MultiDrawElementsBaseVertex (GLenum mode,const GLsizei * count,GLenum type,const void * const * indices,GLsizei drawcount,const GLint * basevertex)
	 ( Z32:_mode Z32*:_count Z32:_type Any**:_indices Z32:_drawcount Z32*:_basevertex ) "libGL.so.1" "glMultiDrawElementsBaseVertex" sys optional MultiDrawElementsBaseVertex 

	// GLAPI void APIENTRY ProvokingVertex (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glProvokingVertex" sys optional ProvokingVertex 

	// GLAPI GLsync APIENTRY FenceSync (GLenum condition,GLbitfield flags)
	 ( Z32:_condition Z32:_flags  -> Any& ) "libGL.so.1" "glFenceSync" sys optional FenceSync 

	// GLAPI GLboolean APIENTRY IsSync (GLsync sync)
	 ( Any&:_sync  -> Bit ) "libGL.so.1" "glIsSync" sys optional IsSync 

	// GLAPI void APIENTRY DeleteSync (GLsync sync)
	 ( Any&:_sync ) "libGL.so.1" "glDeleteSync" sys optional DeleteSync 

	// GLAPI GLenum APIENTRY ClientWaitSync (GLsync sync,GLbitfield flags,GLuint64 timeout)
	 ( Any&:_sync Z32:_flags Any&:_timeout  -> Z32 ) "libGL.so.1" "glClientWaitSync" sys optional ClientWaitSync 

	// GLAPI void APIENTRY WaitSync (GLsync sync,GLbitfield flags,GLuint64 timeout)
	 ( Any&:_sync Z32:_flags Any&:_timeout ) "libGL.so.1" "glWaitSync" sys optional WaitSync 

	// GLAPI void APIENTRY GetInteger64v (GLenum pname,GLint64 * data)
	 ( Z32:_pname Any&*:_data ) "libGL.so.1" "glGetInteger64v" sys optional GetInteger64v 

	// GLAPI void APIENTRY GetSynciv (GLsync sync,GLenum pname,GLsizei count,GLsizei * length,GLint * values)
	 ( Any&:_sync Z32:_pname Z32:_count Z32*:_length Z32*:_values ) "libGL.so.1" "glGetSynciv" sys optional GetSynciv 

	// GLAPI void APIENTRY GetInteger64i_v (GLenum target,GLuint index,GLint64 * data)
	 ( Z32:_target Z32:_index Any&*:_data ) "libGL.so.1" "glGetInteger64i_v" sys optional GetInteger64i_v 

	// GLAPI void APIENTRY GetBufferParameteri64v (GLenum target,GLenum pname,GLint64 * params)
	 ( Z32:_target Z32:_pname Any&*:_params ) "libGL.so.1" "glGetBufferParameteri64v" sys optional GetBufferParameteri64v 

	// GLAPI void APIENTRY FramebufferTexture (GLenum target,GLenum attachment,GLuint texture,GLint level)
	 ( Z32:_target Z32:_attachment Z32:_texture Z32:_level ) "libGL.so.1" "glFramebufferTexture" sys optional FramebufferTexture 

	// GLAPI void APIENTRY TexImage2DMultisample (GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations)
	 ( Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height Bit:_fixedsamplelocations ) "libGL.so.1" "glTexImage2DMultisample" sys optional TexImage2DMultisample 

	// GLAPI void APIENTRY TexImage3DMultisample (GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations)
	 ( Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height Z32:_depth Bit:_fixedsamplelocations ) "libGL.so.1" "glTexImage3DMultisample" sys optional TexImage3DMultisample 

	// GLAPI void APIENTRY GetMultisamplefv (GLenum pname,GLuint index,GLfloat * val)
	 ( Z32:_pname Z32:_index Real*:_val ) "libGL.so.1" "glGetMultisamplefv" sys optional GetMultisamplefv 

	// GLAPI void APIENTRY SampleMaski (GLuint maskNumber,GLbitfield mask)
	 ( Z32:_maskNumber Z32:_mask ) "libGL.so.1" "glSampleMaski" sys optional SampleMaski 

	// GLAPI void APIENTRY BindFragDataLocationIndexed (GLuint program,GLuint colorNumber,GLuint index,const GLchar * name)
	 ( Z32:_program Z32:_colorNumber Z32:_index String:_name ) "libGL.so.1" "glBindFragDataLocationIndexed" sys optional BindFragDataLocationIndexed 

	// GLAPI GLint APIENTRY GetFragDataIndex (GLuint program,const GLchar * name)
	 ( Z32:_program String:_name  -> Z32 ) "libGL.so.1" "glGetFragDataIndex" sys optional GetFragDataIndex 

	// GLAPI void APIENTRY GenSamplers (GLsizei count,GLuint * samplers)
	 ( Z32:_count Z32*:_samplers ) "libGL.so.1" "glGenSamplers" sys optional GenSamplers 

	// GLAPI void APIENTRY DeleteSamplers (GLsizei count,const GLuint * samplers)
	 ( Z32:_count Z32*:_samplers ) "libGL.so.1" "glDeleteSamplers" sys optional DeleteSamplers 

	// GLAPI GLboolean APIENTRY IsSampler (GLuint sampler)
	 ( Z32:_sampler  -> Bit ) "libGL.so.1" "glIsSampler" sys optional IsSampler 

	// GLAPI void APIENTRY BindSampler (GLuint unit,GLuint sampler)
	 ( Z32:_unit Z32:_sampler ) "libGL.so.1" "glBindSampler" sys optional BindSampler 

	// GLAPI void APIENTRY SamplerParameteri (GLuint sampler,GLenum pname,GLint param)
	 ( Z32:_sampler Z32:_pname Z32:_param ) "libGL.so.1" "glSamplerParameteri" sys optional SamplerParameteri 

	// GLAPI void APIENTRY SamplerParameteriv (GLuint sampler,GLenum pname,const GLint * param)
	 ( Z32:_sampler Z32:_pname Z32*:_param ) "libGL.so.1" "glSamplerParameteriv" sys optional SamplerParameteriv 

	// GLAPI void APIENTRY SamplerParameterf (GLuint sampler,GLenum pname,GLfloat param)
	 ( Z32:_sampler Z32:_pname Real:_param ) "libGL.so.1" "glSamplerParameterf" sys optional SamplerParameterf 

	// GLAPI void APIENTRY SamplerParameterfv (GLuint sampler,GLenum pname,const GLfloat * param)
	 ( Z32:_sampler Z32:_pname Real*:_param ) "libGL.so.1" "glSamplerParameterfv" sys optional SamplerParameterfv 

	// GLAPI void APIENTRY SamplerParameterIiv (GLuint sampler,GLenum pname,const GLint * param)
	 ( Z32:_sampler Z32:_pname Z32*:_param ) "libGL.so.1" "glSamplerParameterIiv" sys optional SamplerParameterIiv 

	// GLAPI void APIENTRY SamplerParameterIuiv (GLuint sampler,GLenum pname,const GLuint * param)
	 ( Z32:_sampler Z32:_pname Z32*:_param ) "libGL.so.1" "glSamplerParameterIuiv" sys optional SamplerParameterIuiv 

	// GLAPI void APIENTRY GetSamplerParameteriv (GLuint sampler,GLenum pname,GLint * params)
	 ( Z32:_sampler Z32:_pname Z32*:_params ) "libGL.so.1" "glGetSamplerParameteriv" sys optional GetSamplerParameteriv 

	// GLAPI void APIENTRY GetSamplerParameterIiv (GLuint sampler,GLenum pname,GLint * params)
	 ( Z32:_sampler Z32:_pname Z32*:_params ) "libGL.so.1" "glGetSamplerParameterIiv" sys optional GetSamplerParameterIiv 

	// GLAPI void APIENTRY GetSamplerParameterfv (GLuint sampler,GLenum pname,GLfloat * params)
	 ( Z32:_sampler Z32:_pname Real*:_params ) "libGL.so.1" "glGetSamplerParameterfv" sys optional GetSamplerParameterfv 

	// GLAPI void APIENTRY GetSamplerParameterIuiv (GLuint sampler,GLenum pname,GLuint * params)
	 ( Z32:_sampler Z32:_pname Z32*:_params ) "libGL.so.1" "glGetSamplerParameterIuiv" sys optional GetSamplerParameterIuiv 

	// GLAPI void APIENTRY QueryCounter (GLuint id,GLenum target)
	 ( Z32:_id Z32:_target ) "libGL.so.1" "glQueryCounter" sys optional QueryCounter 

	// GLAPI void APIENTRY GetQueryObjecti64v (GLuint id,GLenum pname,GLint64 * params)
	 ( Z32:_id Z32:_pname Any&*:_params ) "libGL.so.1" "glGetQueryObjecti64v" sys optional GetQueryObjecti64v 

	// GLAPI void APIENTRY GetQueryObjectui64v (GLuint id,GLenum pname,GLuint64 * params)
	 ( Z32:_id Z32:_pname Any&*:_params ) "libGL.so.1" "glGetQueryObjectui64v" sys optional GetQueryObjectui64v 

	// GLAPI void APIENTRY VertexAttribDivisor (GLuint index,GLuint divisor)
	 ( Z32:_index Z32:_divisor ) "libGL.so.1" "glVertexAttribDivisor" sys optional VertexAttribDivisor 

	// GLAPI void APIENTRY VertexAttribP1ui (GLuint index,GLenum type,GLboolean normalized,GLuint value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32:_value ) "libGL.so.1" "glVertexAttribP1ui" sys optional VertexAttribP1ui 

	// GLAPI void APIENTRY VertexAttribP1uiv (GLuint index,GLenum type,GLboolean normalized,const GLuint * value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32*:_value ) "libGL.so.1" "glVertexAttribP1uiv" sys optional VertexAttribP1uiv 

	// GLAPI void APIENTRY VertexAttribP2ui (GLuint index,GLenum type,GLboolean normalized,GLuint value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32:_value ) "libGL.so.1" "glVertexAttribP2ui" sys optional VertexAttribP2ui 

	// GLAPI void APIENTRY VertexAttribP2uiv (GLuint index,GLenum type,GLboolean normalized,const GLuint * value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32*:_value ) "libGL.so.1" "glVertexAttribP2uiv" sys optional VertexAttribP2uiv 

	// GLAPI void APIENTRY VertexAttribP3ui (GLuint index,GLenum type,GLboolean normalized,GLuint value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32:_value ) "libGL.so.1" "glVertexAttribP3ui" sys optional VertexAttribP3ui 

	// GLAPI void APIENTRY VertexAttribP3uiv (GLuint index,GLenum type,GLboolean normalized,const GLuint * value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32*:_value ) "libGL.so.1" "glVertexAttribP3uiv" sys optional VertexAttribP3uiv 

	// GLAPI void APIENTRY VertexAttribP4ui (GLuint index,GLenum type,GLboolean normalized,GLuint value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32:_value ) "libGL.so.1" "glVertexAttribP4ui" sys optional VertexAttribP4ui 

	// GLAPI void APIENTRY VertexAttribP4uiv (GLuint index,GLenum type,GLboolean normalized,const GLuint * value)
	 ( Z32:_index Z32:_type Bit:_normalized Z32*:_value ) "libGL.so.1" "glVertexAttribP4uiv" sys optional VertexAttribP4uiv 

	// GLAPI void APIENTRY MinSampleShading (GLfloat value)
	 ( Real:_value ) "libGL.so.1" "glMinSampleShading" sys optional MinSampleShading 

	// GLAPI void APIENTRY BlendEquationi (GLuint buf,GLenum mode)
	 ( Z32:_buf Z32:_mode ) "libGL.so.1" "glBlendEquationi" sys optional BlendEquationi 

	// GLAPI void APIENTRY BlendEquationSeparatei (GLuint buf,GLenum modeRGB,GLenum modeAlpha)
	 ( Z32:_buf Z32:_modeRGB Z32:_modeAlpha ) "libGL.so.1" "glBlendEquationSeparatei" sys optional BlendEquationSeparatei 

	// GLAPI void APIENTRY BlendFunci (GLuint buf,GLenum src,GLenum dst)
	 ( Z32:_buf Z32:_src Z32:_dst ) "libGL.so.1" "glBlendFunci" sys optional BlendFunci 

	// GLAPI void APIENTRY BlendFuncSeparatei (GLuint buf,GLenum srcRGB,GLenum dstRGB,GLenum srcAlpha,GLenum dstAlpha)
	 ( Z32:_buf Z32:_srcRGB Z32:_dstRGB Z32:_srcAlpha Z32:_dstAlpha ) "libGL.so.1" "glBlendFuncSeparatei" sys optional BlendFuncSeparatei 

	// GLAPI void APIENTRY DrawArraysIndirect (GLenum mode,const void * indirect)
	 ( Z32:_mode Any*:_indirect ) "libGL.so.1" "glDrawArraysIndirect" sys optional DrawArraysIndirect 

	// GLAPI void APIENTRY DrawElementsIndirect (GLenum mode,GLenum type,const void * indirect)
	 ( Z32:_mode Z32:_type Any*:_indirect ) "libGL.so.1" "glDrawElementsIndirect" sys optional DrawElementsIndirect 

	// GLAPI void APIENTRY Uniform1d (GLint location,GLdouble x)
	 ( Z32:_location Double32:_x ) "libGL.so.1" "glUniform1d" sys optional Uniform1d 

	// GLAPI void APIENTRY Uniform2d (GLint location,GLdouble x,GLdouble y)
	 ( Z32:_location Double32:_x Double32:_y ) "libGL.so.1" "glUniform2d" sys optional Uniform2d 

	// GLAPI void APIENTRY Uniform3d (GLint location,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_location Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glUniform3d" sys optional Uniform3d 

	// GLAPI void APIENTRY Uniform4d (GLint location,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
	 ( Z32:_location Double32:_x Double32:_y Double32:_z Double32:_w ) "libGL.so.1" "glUniform4d" sys optional Uniform4d 

	// GLAPI void APIENTRY Uniform1dv (GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glUniform1dv" sys optional Uniform1dv 

	// GLAPI void APIENTRY Uniform2dv (GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glUniform2dv" sys optional Uniform2dv 

	// GLAPI void APIENTRY Uniform3dv (GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glUniform3dv" sys optional Uniform3dv 

	// GLAPI void APIENTRY Uniform4dv (GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glUniform4dv" sys optional Uniform4dv 

	// GLAPI void APIENTRY UniformMatrix2dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix2dv" sys optional UniformMatrix2dv 

	// GLAPI void APIENTRY UniformMatrix3dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix3dv" sys optional UniformMatrix3dv 

	// GLAPI void APIENTRY UniformMatrix4dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix4dv" sys optional UniformMatrix4dv 

	// GLAPI void APIENTRY UniformMatrix2x3dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix2x3dv" sys optional UniformMatrix2x3dv 

	// GLAPI void APIENTRY UniformMatrix2x4dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix2x4dv" sys optional UniformMatrix2x4dv 

	// GLAPI void APIENTRY UniformMatrix3x2dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix3x2dv" sys optional UniformMatrix3x2dv 

	// GLAPI void APIENTRY UniformMatrix3x4dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix3x4dv" sys optional UniformMatrix3x4dv 

	// GLAPI void APIENTRY UniformMatrix4x2dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix4x2dv" sys optional UniformMatrix4x2dv 

	// GLAPI void APIENTRY UniformMatrix4x3dv (GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glUniformMatrix4x3dv" sys optional UniformMatrix4x3dv 

	// GLAPI void APIENTRY GetUniformdv (GLuint program,GLint location,GLdouble * params)
	 ( Z32:_program Z32:_location Double32*:_params ) "libGL.so.1" "glGetUniformdv" sys optional GetUniformdv 

	// GLAPI GLint APIENTRY GetSubroutineUniformLocation (GLuint program,GLenum shadertype,const GLchar * name)
	 ( Z32:_program Z32:_shadertype String:_name  -> Z32 ) "libGL.so.1" "glGetSubroutineUniformLocation" sys optional GetSubroutineUniformLocation 

	// GLAPI GLuint APIENTRY GetSubroutineIndex (GLuint program,GLenum shadertype,const GLchar * name)
	 ( Z32:_program Z32:_shadertype String:_name  -> Z32 ) "libGL.so.1" "glGetSubroutineIndex" sys optional GetSubroutineIndex 

	// GLAPI void APIENTRY GetActiveSubroutineUniformiv (GLuint program,GLenum shadertype,GLuint index,GLenum pname,GLint * values)
	 ( Z32:_program Z32:_shadertype Z32:_index Z32:_pname Z32*:_values ) "libGL.so.1" "glGetActiveSubroutineUniformiv" sys optional GetActiveSubroutineUniformiv 

	// GLAPI void APIENTRY GetActiveSubroutineUniformName (GLuint program,GLenum shadertype,GLuint index,GLsizei bufSize,GLsizei * length,GLchar * name)
	 ( Z32:_program Z32:_shadertype Z32:_index Z32:_bufSize Z32*:_length String:_name ) "libGL.so.1" "glGetActiveSubroutineUniformName" sys optional GetActiveSubroutineUniformName 

	// GLAPI void APIENTRY GetActiveSubroutineName (GLuint program,GLenum shadertype,GLuint index,GLsizei bufSize,GLsizei * length,GLchar * name)
	 ( Z32:_program Z32:_shadertype Z32:_index Z32:_bufSize Z32*:_length String:_name ) "libGL.so.1" "glGetActiveSubroutineName" sys optional GetActiveSubroutineName 

	// GLAPI void APIENTRY UniformSubroutinesuiv (GLenum shadertype,GLsizei count,const GLuint * indices)
	 ( Z32:_shadertype Z32:_count Z32*:_indices ) "libGL.so.1" "glUniformSubroutinesuiv" sys optional UniformSubroutinesuiv 

	// GLAPI void APIENTRY GetUniformSubroutineuiv (GLenum shadertype,GLint location,GLuint * params)
	 ( Z32:_shadertype Z32:_location Z32*:_params ) "libGL.so.1" "glGetUniformSubroutineuiv" sys optional GetUniformSubroutineuiv 

	// GLAPI void APIENTRY GetProgramStageiv (GLuint program,GLenum shadertype,GLenum pname,GLint * values)
	 ( Z32:_program Z32:_shadertype Z32:_pname Z32*:_values ) "libGL.so.1" "glGetProgramStageiv" sys optional GetProgramStageiv 

	// GLAPI void APIENTRY PatchParameteri (GLenum pname,GLint value)
	 ( Z32:_pname Z32:_value ) "libGL.so.1" "glPatchParameteri" sys optional PatchParameteri 

	// GLAPI void APIENTRY PatchParameterfv (GLenum pname,const GLfloat * values)
	 ( Z32:_pname Real*:_values ) "libGL.so.1" "glPatchParameterfv" sys optional PatchParameterfv 

	// GLAPI void APIENTRY BindTransformFeedback (GLenum target,GLuint id)
	 ( Z32:_target Z32:_id ) "libGL.so.1" "glBindTransformFeedback" sys optional BindTransformFeedback 

	// GLAPI void APIENTRY DeleteTransformFeedbacks (GLsizei n,const GLuint * ids)
	 ( Z32:_n Z32*:_ids ) "libGL.so.1" "glDeleteTransformFeedbacks" sys optional DeleteTransformFeedbacks 

	// GLAPI void APIENTRY GenTransformFeedbacks (GLsizei n,GLuint * ids)
	 ( Z32:_n Z32*:_ids ) "libGL.so.1" "glGenTransformFeedbacks" sys optional GenTransformFeedbacks 

	// GLAPI GLboolean APIENTRY IsTransformFeedback (GLuint id)
	 ( Z32:_id  -> Bit ) "libGL.so.1" "glIsTransformFeedback" sys optional IsTransformFeedback 

	// GLAPI void APIENTRY PauseTransformFeedback (void)
	 ( ) "libGL.so.1" "glPauseTransformFeedback" sys optional PauseTransformFeedback 

	// GLAPI void APIENTRY ResumeTransformFeedback (void)
	 ( ) "libGL.so.1" "glResumeTransformFeedback" sys optional ResumeTransformFeedback 

	// GLAPI void APIENTRY DrawTransformFeedback (GLenum mode,GLuint id)
	 ( Z32:_mode Z32:_id ) "libGL.so.1" "glDrawTransformFeedback" sys optional DrawTransformFeedback 

	// GLAPI void APIENTRY DrawTransformFeedbackStream (GLenum mode,GLuint id,GLuint stream)
	 ( Z32:_mode Z32:_id Z32:_stream ) "libGL.so.1" "glDrawTransformFeedbackStream" sys optional DrawTransformFeedbackStream 

	// GLAPI void APIENTRY BeginQueryIndexed (GLenum target,GLuint index,GLuint id)
	 ( Z32:_target Z32:_index Z32:_id ) "libGL.so.1" "glBeginQueryIndexed" sys optional BeginQueryIndexed 

	// GLAPI void APIENTRY EndQueryIndexed (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index ) "libGL.so.1" "glEndQueryIndexed" sys optional EndQueryIndexed 

	// GLAPI void APIENTRY GetQueryIndexediv (GLenum target,GLuint index,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_index Z32:_pname Z32*:_params ) "libGL.so.1" "glGetQueryIndexediv" sys optional GetQueryIndexediv 

	// GLAPI void APIENTRY ReleaseShaderCompiler (void)
	 ( ) "libGL.so.1" "glReleaseShaderCompiler" sys optional ReleaseShaderCompiler 

	// GLAPI void APIENTRY ShaderBinary (GLsizei count,const GLuint * shaders,GLenum binaryFormat,const void * binary,GLsizei length)
	 ( Z32:_count Z32*:_shaders Z32:_binaryFormat Any*:_binary Z32:_length ) "libGL.so.1" "glShaderBinary" sys optional ShaderBinary 

	// GLAPI void APIENTRY GetShaderPrecisionFormat (GLenum shadertype,GLenum precisiontype,GLint * range,GLint * precision)
	 ( Z32:_shadertype Z32:_precisiontype Z32*:_range Z32*:_precision ) "libGL.so.1" "glGetShaderPrecisionFormat" sys optional GetShaderPrecisionFormat 

	// GLAPI void APIENTRY DepthRangef (GLfloat n,GLfloat f)
	 ( Real:_n Real:_f ) "libGL.so.1" "glDepthRangef" sys optional DepthRangef 

	// GLAPI void APIENTRY ClearDepthf (GLfloat d)
	 ( Real:_d ) "libGL.so.1" "glClearDepthf" sys optional ClearDepthf 

	// GLAPI void APIENTRY GetProgramBinary (GLuint program,GLsizei bufSize,GLsizei * length,GLenum * binaryFormat,void * binary)
	 ( Z32:_program Z32:_bufSize Z32*:_length Z32*:_binaryFormat Any*:_binary ) "libGL.so.1" "glGetProgramBinary" sys optional GetProgramBinary 

	// GLAPI void APIENTRY ProgramBinary (GLuint program,GLenum binaryFormat,const void * binary,GLsizei length)
	 ( Z32:_program Z32:_binaryFormat Any*:_binary Z32:_length ) "libGL.so.1" "glProgramBinary" sys optional ProgramBinary 

	// GLAPI void APIENTRY ProgramParameteri (GLuint program,GLenum pname,GLint value)
	 ( Z32:_program Z32:_pname Z32:_value ) "libGL.so.1" "glProgramParameteri" sys optional ProgramParameteri 

	// GLAPI void APIENTRY UseProgramStages (GLuint pipeline,GLbitfield stages,GLuint program)
	 ( Z32:_pipeline Z32:_stages Z32:_program ) "libGL.so.1" "glUseProgramStages" sys optional UseProgramStages 

	// GLAPI void APIENTRY ActiveShaderProgram (GLuint pipeline,GLuint program)
	 ( Z32:_pipeline Z32:_program ) "libGL.so.1" "glActiveShaderProgram" sys optional ActiveShaderProgram 

	// GLAPI GLuint APIENTRY CreateShaderProgramv (GLenum type,GLsizei count,const GLchar * const * strings)
	 ( Z32:_type Z32:_count String*:_strings  -> Z32 ) "libGL.so.1" "glCreateShaderProgramv" sys optional CreateShaderProgramv 

	// GLAPI void APIENTRY BindProgramPipeline (GLuint pipeline)
	 ( Z32:_pipeline ) "libGL.so.1" "glBindProgramPipeline" sys optional BindProgramPipeline 

	// GLAPI void APIENTRY DeleteProgramPipelines (GLsizei n,const GLuint * pipelines)
	 ( Z32:_n Z32*:_pipelines ) "libGL.so.1" "glDeleteProgramPipelines" sys optional DeleteProgramPipelines 

	// GLAPI void APIENTRY GenProgramPipelines (GLsizei n,GLuint * pipelines)
	 ( Z32:_n Z32*:_pipelines ) "libGL.so.1" "glGenProgramPipelines" sys optional GenProgramPipelines 

	// GLAPI GLboolean APIENTRY IsProgramPipeline (GLuint pipeline)
	 ( Z32:_pipeline  -> Bit ) "libGL.so.1" "glIsProgramPipeline" sys optional IsProgramPipeline 

	// GLAPI void APIENTRY GetProgramPipelineiv (GLuint pipeline,GLenum pname,GLint * params)
	 ( Z32:_pipeline Z32:_pname Z32*:_params ) "libGL.so.1" "glGetProgramPipelineiv" sys optional GetProgramPipelineiv 

	// GLAPI void APIENTRY ProgramUniform1i (GLuint program,GLint location,GLint v0)
	 ( Z32:_program Z32:_location Z32:_v0 ) "libGL.so.1" "glProgramUniform1i" sys optional ProgramUniform1i 

	// GLAPI void APIENTRY ProgramUniform1iv (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform1iv" sys optional ProgramUniform1iv 

	// GLAPI void APIENTRY ProgramUniform1f (GLuint program,GLint location,GLfloat v0)
	 ( Z32:_program Z32:_location Real:_v0 ) "libGL.so.1" "glProgramUniform1f" sys optional ProgramUniform1f 

	// GLAPI void APIENTRY ProgramUniform1fv (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform1fv" sys optional ProgramUniform1fv 

	// GLAPI void APIENTRY ProgramUniform1d (GLuint program,GLint location,GLdouble v0)
	 ( Z32:_program Z32:_location Double32:_v0 ) "libGL.so.1" "glProgramUniform1d" sys optional ProgramUniform1d 

	// GLAPI void APIENTRY ProgramUniform1dv (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform1dv" sys optional ProgramUniform1dv 

	// GLAPI void APIENTRY ProgramUniform1ui (GLuint program,GLint location,GLuint v0)
	 ( Z32:_program Z32:_location Z32:_v0 ) "libGL.so.1" "glProgramUniform1ui" sys optional ProgramUniform1ui 

	// GLAPI void APIENTRY ProgramUniform1uiv (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform1uiv" sys optional ProgramUniform1uiv 

	// GLAPI void APIENTRY ProgramUniform2i (GLuint program,GLint location,GLint v0,GLint v1)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 ) "libGL.so.1" "glProgramUniform2i" sys optional ProgramUniform2i 

	// GLAPI void APIENTRY ProgramUniform2iv (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform2iv" sys optional ProgramUniform2iv 

	// GLAPI void APIENTRY ProgramUniform2f (GLuint program,GLint location,GLfloat v0,GLfloat v1)
	 ( Z32:_program Z32:_location Real:_v0 Real:_v1 ) "libGL.so.1" "glProgramUniform2f" sys optional ProgramUniform2f 

	// GLAPI void APIENTRY ProgramUniform2fv (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform2fv" sys optional ProgramUniform2fv 

	// GLAPI void APIENTRY ProgramUniform2d (GLuint program,GLint location,GLdouble v0,GLdouble v1)
	 ( Z32:_program Z32:_location Double32:_v0 Double32:_v1 ) "libGL.so.1" "glProgramUniform2d" sys optional ProgramUniform2d 

	// GLAPI void APIENTRY ProgramUniform2dv (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform2dv" sys optional ProgramUniform2dv 

	// GLAPI void APIENTRY ProgramUniform2ui (GLuint program,GLint location,GLuint v0,GLuint v1)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 ) "libGL.so.1" "glProgramUniform2ui" sys optional ProgramUniform2ui 

	// GLAPI void APIENTRY ProgramUniform2uiv (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform2uiv" sys optional ProgramUniform2uiv 

	// GLAPI void APIENTRY ProgramUniform3i (GLuint program,GLint location,GLint v0,GLint v1,GLint v2)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 ) "libGL.so.1" "glProgramUniform3i" sys optional ProgramUniform3i 

	// GLAPI void APIENTRY ProgramUniform3iv (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform3iv" sys optional ProgramUniform3iv 

	// GLAPI void APIENTRY ProgramUniform3f (GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
	 ( Z32:_program Z32:_location Real:_v0 Real:_v1 Real:_v2 ) "libGL.so.1" "glProgramUniform3f" sys optional ProgramUniform3f 

	// GLAPI void APIENTRY ProgramUniform3fv (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform3fv" sys optional ProgramUniform3fv 

	// GLAPI void APIENTRY ProgramUniform3d (GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2)
	 ( Z32:_program Z32:_location Double32:_v0 Double32:_v1 Double32:_v2 ) "libGL.so.1" "glProgramUniform3d" sys optional ProgramUniform3d 

	// GLAPI void APIENTRY ProgramUniform3dv (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform3dv" sys optional ProgramUniform3dv 

	// GLAPI void APIENTRY ProgramUniform3ui (GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 ) "libGL.so.1" "glProgramUniform3ui" sys optional ProgramUniform3ui 

	// GLAPI void APIENTRY ProgramUniform3uiv (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform3uiv" sys optional ProgramUniform3uiv 

	// GLAPI void APIENTRY ProgramUniform4i (GLuint program,GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 Z32:_v3 ) "libGL.so.1" "glProgramUniform4i" sys optional ProgramUniform4i 

	// GLAPI void APIENTRY ProgramUniform4iv (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform4iv" sys optional ProgramUniform4iv 

	// GLAPI void APIENTRY ProgramUniform4f (GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
	 ( Z32:_program Z32:_location Real:_v0 Real:_v1 Real:_v2 Real:_v3 ) "libGL.so.1" "glProgramUniform4f" sys optional ProgramUniform4f 

	// GLAPI void APIENTRY ProgramUniform4fv (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform4fv" sys optional ProgramUniform4fv 

	// GLAPI void APIENTRY ProgramUniform4d (GLuint program,GLint location,GLdouble v0,GLdouble v1,GLdouble v2,GLdouble v3)
	 ( Z32:_program Z32:_location Double32:_v0 Double32:_v1 Double32:_v2 Double32:_v3 ) "libGL.so.1" "glProgramUniform4d" sys optional ProgramUniform4d 

	// GLAPI void APIENTRY ProgramUniform4dv (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform4dv" sys optional ProgramUniform4dv 

	// GLAPI void APIENTRY ProgramUniform4ui (GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 Z32:_v3 ) "libGL.so.1" "glProgramUniform4ui" sys optional ProgramUniform4ui 

	// GLAPI void APIENTRY ProgramUniform4uiv (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform4uiv" sys optional ProgramUniform4uiv 

	// GLAPI void APIENTRY ProgramUniformMatrix2fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix2fv" sys optional ProgramUniformMatrix2fv 

	// GLAPI void APIENTRY ProgramUniformMatrix3fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix3fv" sys optional ProgramUniformMatrix3fv 

	// GLAPI void APIENTRY ProgramUniformMatrix4fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix4fv" sys optional ProgramUniformMatrix4fv 

	// GLAPI void APIENTRY ProgramUniformMatrix2dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix2dv" sys optional ProgramUniformMatrix2dv 

	// GLAPI void APIENTRY ProgramUniformMatrix3dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix3dv" sys optional ProgramUniformMatrix3dv 

	// GLAPI void APIENTRY ProgramUniformMatrix4dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix4dv" sys optional ProgramUniformMatrix4dv 

	// GLAPI void APIENTRY ProgramUniformMatrix2x3fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix2x3fv" sys optional ProgramUniformMatrix2x3fv 

	// GLAPI void APIENTRY ProgramUniformMatrix3x2fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix3x2fv" sys optional ProgramUniformMatrix3x2fv 

	// GLAPI void APIENTRY ProgramUniformMatrix2x4fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix2x4fv" sys optional ProgramUniformMatrix2x4fv 

	// GLAPI void APIENTRY ProgramUniformMatrix4x2fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix4x2fv" sys optional ProgramUniformMatrix4x2fv 

	// GLAPI void APIENTRY ProgramUniformMatrix3x4fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix3x4fv" sys optional ProgramUniformMatrix3x4fv 

	// GLAPI void APIENTRY ProgramUniformMatrix4x3fv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix4x3fv" sys optional ProgramUniformMatrix4x3fv 

	// GLAPI void APIENTRY ProgramUniformMatrix2x3dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix2x3dv" sys optional ProgramUniformMatrix2x3dv 

	// GLAPI void APIENTRY ProgramUniformMatrix3x2dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix3x2dv" sys optional ProgramUniformMatrix3x2dv 

	// GLAPI void APIENTRY ProgramUniformMatrix2x4dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix2x4dv" sys optional ProgramUniformMatrix2x4dv 

	// GLAPI void APIENTRY ProgramUniformMatrix4x2dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix4x2dv" sys optional ProgramUniformMatrix4x2dv 

	// GLAPI void APIENTRY ProgramUniformMatrix3x4dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix3x4dv" sys optional ProgramUniformMatrix3x4dv 

	// GLAPI void APIENTRY ProgramUniformMatrix4x3dv (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix4x3dv" sys optional ProgramUniformMatrix4x3dv 

	// GLAPI void APIENTRY ValidateProgramPipeline (GLuint pipeline)
	 ( Z32:_pipeline ) "libGL.so.1" "glValidateProgramPipeline" sys optional ValidateProgramPipeline 

	// GLAPI void APIENTRY GetProgramPipelineInfoLog (GLuint pipeline,GLsizei bufSize,GLsizei * length,GLchar * infoLog)
	 ( Z32:_pipeline Z32:_bufSize Z32*:_length String:_infoLog ) "libGL.so.1" "glGetProgramPipelineInfoLog" sys optional GetProgramPipelineInfoLog 

	// GLAPI void APIENTRY VertexAttribL1d (GLuint index,GLdouble x)
	 ( Z32:_index Double32:_x ) "libGL.so.1" "glVertexAttribL1d" sys optional VertexAttribL1d 

	// GLAPI void APIENTRY VertexAttribL2d (GLuint index,GLdouble x,GLdouble y)
	 ( Z32:_index Double32:_x Double32:_y ) "libGL.so.1" "glVertexAttribL2d" sys optional VertexAttribL2d 

	// GLAPI void APIENTRY VertexAttribL3d (GLuint index,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_index Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glVertexAttribL3d" sys optional VertexAttribL3d 

	// GLAPI void APIENTRY VertexAttribL4d (GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
	 ( Z32:_index Double32:_x Double32:_y Double32:_z Double32:_w ) "libGL.so.1" "glVertexAttribL4d" sys optional VertexAttribL4d 

	// GLAPI void APIENTRY VertexAttribL1dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttribL1dv" sys optional VertexAttribL1dv 

	// GLAPI void APIENTRY VertexAttribL2dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttribL2dv" sys optional VertexAttribL2dv 

	// GLAPI void APIENTRY VertexAttribL3dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttribL3dv" sys optional VertexAttribL3dv 

	// GLAPI void APIENTRY VertexAttribL4dv (GLuint index,const GLdouble * v)
	 ( Z32:_index Double32*:_v ) "libGL.so.1" "glVertexAttribL4dv" sys optional VertexAttribL4dv 

	// GLAPI void APIENTRY VertexAttribLPointer (GLuint index,GLint size,GLenum type,GLsizei stride,const void * pointer)
	 ( Z32:_index Z32:_size Z32:_type Z32:_stride Any*:_pointer ) "libGL.so.1" "glVertexAttribLPointer" sys optional VertexAttribLPointer 

	// GLAPI void APIENTRY GetVertexAttribLdv (GLuint index,GLenum pname,GLdouble * params)
	 ( Z32:_index Z32:_pname Double32*:_params ) "libGL.so.1" "glGetVertexAttribLdv" sys optional GetVertexAttribLdv 

	// GLAPI void APIENTRY ViewportArrayv (GLuint first,GLsizei count,const GLfloat * v)
	 ( Z32:_first Z32:_count Real*:_v ) "libGL.so.1" "glViewportArrayv" sys optional ViewportArrayv 

	// GLAPI void APIENTRY ViewportIndexedf (GLuint index,GLfloat x,GLfloat y,GLfloat w,GLfloat h)
	 ( Z32:_index Real:_x Real:_y Real:_w Real:_h ) "libGL.so.1" "glViewportIndexedf" sys optional ViewportIndexedf 

	// GLAPI void APIENTRY ViewportIndexedfv (GLuint index,const GLfloat * v)
	 ( Z32:_index Real*:_v ) "libGL.so.1" "glViewportIndexedfv" sys optional ViewportIndexedfv 

	// GLAPI void APIENTRY ScissorArrayv (GLuint first,GLsizei count,const GLint * v)
	 ( Z32:_first Z32:_count Z32*:_v ) "libGL.so.1" "glScissorArrayv" sys optional ScissorArrayv 

	// GLAPI void APIENTRY ScissorIndexed (GLuint index,GLint left,GLint bottom,GLsizei width,GLsizei height)
	 ( Z32:_index Z32:_left Z32:_bottom Z32:_width Z32:_height ) "libGL.so.1" "glScissorIndexed" sys optional ScissorIndexed 

	// GLAPI void APIENTRY ScissorIndexedv (GLuint index,const GLint * v)
	 ( Z32:_index Z32*:_v ) "libGL.so.1" "glScissorIndexedv" sys optional ScissorIndexedv 

	// GLAPI void APIENTRY DepthRangeArrayv (GLuint first,GLsizei count,const GLdouble * v)
	 ( Z32:_first Z32:_count Double32*:_v ) "libGL.so.1" "glDepthRangeArrayv" sys optional DepthRangeArrayv 

	// GLAPI void APIENTRY DepthRangeIndexed (GLuint index,GLdouble n,GLdouble f)
	 ( Z32:_index Double32:_n Double32:_f ) "libGL.so.1" "glDepthRangeIndexed" sys optional DepthRangeIndexed 

	// GLAPI void APIENTRY GetFloati_v (GLenum target,GLuint index,GLfloat * data)
	 ( Z32:_target Z32:_index Real*:_data ) "libGL.so.1" "glGetFloati_v" sys optional GetFloati_v 

	// GLAPI void APIENTRY GetDoublei_v (GLenum target,GLuint index,GLdouble * data)
	 ( Z32:_target Z32:_index Double32*:_data ) "libGL.so.1" "glGetDoublei_v" sys optional GetDoublei_v 

	// GLAPI void APIENTRY DrawArraysInstancedBaseInstance (GLenum mode,GLint first,GLsizei count,GLsizei instancecount,GLuint baseinstance)
	 ( Z32:_mode Z32:_first Z32:_count Z32:_instancecount Z32:_baseinstance ) "libGL.so.1" "glDrawArraysInstancedBaseInstance" sys optional DrawArraysInstancedBaseInstance 

	// GLAPI void APIENTRY DrawElementsInstancedBaseInstance (GLenum mode,GLsizei count,GLenum type,const void * indices,GLsizei instancecount,GLuint baseinstance)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_instancecount Z32:_baseinstance ) "libGL.so.1" "glDrawElementsInstancedBaseInstance" sys optional DrawElementsInstancedBaseInstance 

	// GLAPI void APIENTRY DrawElementsInstancedBaseVertexBaseInstance (GLenum mode,GLsizei count,GLenum type,const void * indices,GLsizei instancecount,GLint basevertex,GLuint baseinstance)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_instancecount Z32:_basevertex Z32:_baseinstance ) "libGL.so.1" "glDrawElementsInstancedBaseVertexBaseInstance" sys optional DrawElementsInstancedBaseVertexBaseInstance 

	// GLAPI void APIENTRY GetInternalformativ (GLenum target,GLenum internalformat,GLenum pname,GLsizei count,GLint * params)
	 ( Z32:_target Z32:_internalformat Z32:_pname Z32:_count Z32*:_params ) "libGL.so.1" "glGetInternalformativ" sys optional GetInternalformativ 

	// GLAPI void APIENTRY GetActiveAtomicCounterBufferiv (GLuint program,GLuint bufferIndex,GLenum pname,GLint * params)
	 ( Z32:_program Z32:_bufferIndex Z32:_pname Z32*:_params ) "libGL.so.1" "glGetActiveAtomicCounterBufferiv" sys optional GetActiveAtomicCounterBufferiv 

	// GLAPI void APIENTRY BindImageTexture (GLuint unit,GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum access,GLenum format)
	 ( Z32:_unit Z32:_texture Z32:_level Bit:_layered Z32:_layer Z32:_access Z32:_format ) "libGL.so.1" "glBindImageTexture" sys optional BindImageTexture 

	// GLAPI void APIENTRY MemoryBarrier (GLbitfield barriers)
	 ( Z32:_barriers ) "libGL.so.1" "glMemoryBarrier" sys optional MemoryBarrier 

	// GLAPI void APIENTRY TexStorage1D (GLenum target,GLsizei levels,GLenum internalformat,GLsizei width)
	 ( Z32:_target Z32:_levels Z32:_internalformat Z32:_width ) "libGL.so.1" "glTexStorage1D" sys optional TexStorage1D 

	// GLAPI void APIENTRY TexStorage2D (GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_levels Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glTexStorage2D" sys optional TexStorage2D 

	// GLAPI void APIENTRY TexStorage3D (GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth)
	 ( Z32:_target Z32:_levels Z32:_internalformat Z32:_width Z32:_height Z32:_depth ) "libGL.so.1" "glTexStorage3D" sys optional TexStorage3D 

	// GLAPI void APIENTRY DrawTransformFeedbackInstanced (GLenum mode,GLuint id,GLsizei instancecount)
	 ( Z32:_mode Z32:_id Z32:_instancecount ) "libGL.so.1" "glDrawTransformFeedbackInstanced" sys optional DrawTransformFeedbackInstanced 

	// GLAPI void APIENTRY DrawTransformFeedbackStreamInstanced (GLenum mode,GLuint id,GLuint stream,GLsizei instancecount)
	 ( Z32:_mode Z32:_id Z32:_stream Z32:_instancecount ) "libGL.so.1" "glDrawTransformFeedbackStreamInstanced" sys optional DrawTransformFeedbackStreamInstanced 

	// GLAPI void APIENTRY ClearBufferData (GLenum target,GLenum internalformat,GLenum format,GLenum type,const void * data)
	 ( Z32:_target Z32:_internalformat Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearBufferData" sys optional ClearBufferData 

	// GLAPI void APIENTRY ClearBufferSubData (GLenum target,GLenum internalformat,GLintptr offset,GLsizeiptr size,GLenum format,GLenum type,const void * data)
	 ( Z32:_target Z32:_internalformat Any&:_offset Z32:_size Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearBufferSubData" sys optional ClearBufferSubData 

	// GLAPI void APIENTRY DispatchCompute (GLuint num_groups_x,GLuint num_groups_y,GLuint num_groups_z)
	 ( Z32:_num_groups_x Z32:_num_groups_y Z32:_num_groups_z ) "libGL.so.1" "glDispatchCompute" sys optional DispatchCompute 

	// GLAPI void APIENTRY DispatchComputeIndirect (GLintptr indirect)
	 ( Any&:_indirect ) "libGL.so.1" "glDispatchComputeIndirect" sys optional DispatchComputeIndirect 

	// GLAPI void APIENTRY CopyImageSubData (GLuint srcName,GLenum srcTarget,GLint srcLevel,GLint srcX,GLint srcY,GLint srcZ,GLuint dstName,GLenum dstTarget,GLint dstLevel,GLint dstX,GLint dstY,GLint dstZ,GLsizei srcWidth,GLsizei srcHeight,GLsizei srcDepth)
	 ( Z32:_srcName Z32:_srcTarget Z32:_srcLevel Z32:_srcX Z32:_srcY Z32:_srcZ Z32:_dstName Z32:_dstTarget Z32:_dstLevel Z32:_dstX Z32:_dstY Z32:_dstZ Z32:_srcWidth Z32:_srcHeight Z32:_srcDepth ) "libGL.so.1" "glCopyImageSubData" sys optional CopyImageSubData 

	// GLAPI void APIENTRY FramebufferParameteri (GLenum target,GLenum pname,GLint param)
	 ( Z32:_target Z32:_pname Z32:_param ) "libGL.so.1" "glFramebufferParameteri" sys optional FramebufferParameteri 

	// GLAPI void APIENTRY GetFramebufferParameteriv (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetFramebufferParameteriv" sys optional GetFramebufferParameteriv 

	// GLAPI void APIENTRY GetInternalformati64v (GLenum target,GLenum internalformat,GLenum pname,GLsizei count,GLint64 * params)
	 ( Z32:_target Z32:_internalformat Z32:_pname Z32:_count Any&*:_params ) "libGL.so.1" "glGetInternalformati64v" sys optional GetInternalformati64v 

	// GLAPI void APIENTRY InvalidateTexSubImage (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth ) "libGL.so.1" "glInvalidateTexSubImage" sys optional InvalidateTexSubImage 

	// GLAPI void APIENTRY InvalidateTexImage (GLuint texture,GLint level)
	 ( Z32:_texture Z32:_level ) "libGL.so.1" "glInvalidateTexImage" sys optional InvalidateTexImage 

	// GLAPI void APIENTRY InvalidateBufferSubData (GLuint buffer,GLintptr offset,GLsizeiptr length)
	 ( Z32:_buffer Any&:_offset Z32:_length ) "libGL.so.1" "glInvalidateBufferSubData" sys optional InvalidateBufferSubData 

	// GLAPI void APIENTRY InvalidateBufferData (GLuint buffer)
	 ( Z32:_buffer ) "libGL.so.1" "glInvalidateBufferData" sys optional InvalidateBufferData 

	// GLAPI void APIENTRY InvalidateFramebuffer (GLenum target,GLsizei numAttachments,const GLenum * attachments)
	 ( Z32:_target Z32:_numAttachments Z32*:_attachments ) "libGL.so.1" "glInvalidateFramebuffer" sys optional InvalidateFramebuffer 

	// GLAPI void APIENTRY InvalidateSubFramebuffer (GLenum target,GLsizei numAttachments,const GLenum * attachments,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_numAttachments Z32*:_attachments Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glInvalidateSubFramebuffer" sys optional InvalidateSubFramebuffer 

	// GLAPI void APIENTRY MultiDrawArraysIndirect (GLenum mode,const void * indirect,GLsizei drawcount,GLsizei stride)
	 ( Z32:_mode Any*:_indirect Z32:_drawcount Z32:_stride ) "libGL.so.1" "glMultiDrawArraysIndirect" sys optional MultiDrawArraysIndirect 

	// GLAPI void APIENTRY MultiDrawElementsIndirect (GLenum mode,GLenum type,const void * indirect,GLsizei drawcount,GLsizei stride)
	 ( Z32:_mode Z32:_type Any*:_indirect Z32:_drawcount Z32:_stride ) "libGL.so.1" "glMultiDrawElementsIndirect" sys optional MultiDrawElementsIndirect 

	// GLAPI void APIENTRY GetProgramInterfaceiv (GLuint program,GLenum programInterface,GLenum pname,GLint * params)
	 ( Z32:_program Z32:_programInterface Z32:_pname Z32*:_params ) "libGL.so.1" "glGetProgramInterfaceiv" sys optional GetProgramInterfaceiv 

	// GLAPI GLuint APIENTRY GetProgramResourceIndex (GLuint program,GLenum programInterface,const GLchar * name)
	 ( Z32:_program Z32:_programInterface String:_name  -> Z32 ) "libGL.so.1" "glGetProgramResourceIndex" sys optional GetProgramResourceIndex 

	// GLAPI void APIENTRY GetProgramResourceName (GLuint program,GLenum programInterface,GLuint index,GLsizei bufSize,GLsizei * length,GLchar * name)
	 ( Z32:_program Z32:_programInterface Z32:_index Z32:_bufSize Z32*:_length String:_name ) "libGL.so.1" "glGetProgramResourceName" sys optional GetProgramResourceName 

	// GLAPI void APIENTRY GetProgramResourceiv (GLuint program,GLenum programInterface,GLuint index,GLsizei propCount,const GLenum * props,GLsizei count,GLsizei * length,GLint * params)
	 ( Z32:_program Z32:_programInterface Z32:_index Z32:_propCount Z32*:_props Z32:_count Z32*:_length Z32*:_params ) "libGL.so.1" "glGetProgramResourceiv" sys optional GetProgramResourceiv 

	// GLAPI GLint APIENTRY GetProgramResourceLocation (GLuint program,GLenum programInterface,const GLchar * name)
	 ( Z32:_program Z32:_programInterface String:_name  -> Z32 ) "libGL.so.1" "glGetProgramResourceLocation" sys optional GetProgramResourceLocation 

	// GLAPI GLint APIENTRY GetProgramResourceLocationIndex (GLuint program,GLenum programInterface,const GLchar * name)
	 ( Z32:_program Z32:_programInterface String:_name  -> Z32 ) "libGL.so.1" "glGetProgramResourceLocationIndex" sys optional GetProgramResourceLocationIndex 

	// GLAPI void APIENTRY ShaderStorageBlockBinding (GLuint program,GLuint storageBlockIndex,GLuint storageBlockBinding)
	 ( Z32:_program Z32:_storageBlockIndex Z32:_storageBlockBinding ) "libGL.so.1" "glShaderStorageBlockBinding" sys optional ShaderStorageBlockBinding 

	// GLAPI void APIENTRY TexBufferRange (GLenum target,GLenum internalformat,GLuint buffer,GLintptr offset,GLsizeiptr size)
	 ( Z32:_target Z32:_internalformat Z32:_buffer Any&:_offset Z32:_size ) "libGL.so.1" "glTexBufferRange" sys optional TexBufferRange 

	// GLAPI void APIENTRY TexStorage2DMultisample (GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations)
	 ( Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height Bit:_fixedsamplelocations ) "libGL.so.1" "glTexStorage2DMultisample" sys optional TexStorage2DMultisample 

	// GLAPI void APIENTRY TexStorage3DMultisample (GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations)
	 ( Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height Z32:_depth Bit:_fixedsamplelocations ) "libGL.so.1" "glTexStorage3DMultisample" sys optional TexStorage3DMultisample 

	// GLAPI void APIENTRY TextureView (GLuint texture,GLenum target,GLuint origtexture,GLenum internalformat,GLuint minlevel,GLuint numlevels,GLuint minlayer,GLuint numlayers)
	 ( Z32:_texture Z32:_target Z32:_origtexture Z32:_internalformat Z32:_minlevel Z32:_numlevels Z32:_minlayer Z32:_numlayers ) "libGL.so.1" "glTextureView" sys optional TextureView 

	// GLAPI void APIENTRY BindVertexBuffer (GLuint bindingindex,GLuint buffer,GLintptr offset,GLsizei stride)
	 ( Z32:_bindingindex Z32:_buffer Any&:_offset Z32:_stride ) "libGL.so.1" "glBindVertexBuffer" sys optional BindVertexBuffer 

	// GLAPI void APIENTRY VertexAttribFormat (GLuint attribindex,GLint size,GLenum type,GLboolean normalized,GLuint relativeoffset)
	 ( Z32:_attribindex Z32:_size Z32:_type Bit:_normalized Z32:_relativeoffset ) "libGL.so.1" "glVertexAttribFormat" sys optional VertexAttribFormat 

	// GLAPI void APIENTRY VertexAttribIFormat (GLuint attribindex,GLint size,GLenum type,GLuint relativeoffset)
	 ( Z32:_attribindex Z32:_size Z32:_type Z32:_relativeoffset ) "libGL.so.1" "glVertexAttribIFormat" sys optional VertexAttribIFormat 

	// GLAPI void APIENTRY VertexAttribLFormat (GLuint attribindex,GLint size,GLenum type,GLuint relativeoffset)
	 ( Z32:_attribindex Z32:_size Z32:_type Z32:_relativeoffset ) "libGL.so.1" "glVertexAttribLFormat" sys optional VertexAttribLFormat 

	// GLAPI void APIENTRY VertexAttribBinding (GLuint attribindex,GLuint bindingindex)
	 ( Z32:_attribindex Z32:_bindingindex ) "libGL.so.1" "glVertexAttribBinding" sys optional VertexAttribBinding 

	// GLAPI void APIENTRY VertexBindingDivisor (GLuint bindingindex,GLuint divisor)
	 ( Z32:_bindingindex Z32:_divisor ) "libGL.so.1" "glVertexBindingDivisor" sys optional VertexBindingDivisor 

	// GLAPI void APIENTRY DebugMessageControl (GLenum source,GLenum type,GLenum severity,GLsizei count,const GLuint * ids,GLboolean enabled)
	 ( Z32:_source Z32:_type Z32:_severity Z32:_count Z32*:_ids Bit:_enabled ) "libGL.so.1" "glDebugMessageControl" sys optional DebugMessageControl 

	// GLAPI void APIENTRY DebugMessageInsert (GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar * buf)
	 ( Z32:_source Z32:_type Z32:_id Z32:_severity Z32:_length String:_buf ) "libGL.so.1" "glDebugMessageInsert" sys optional DebugMessageInsert 

	// GLAPI void APIENTRY DebugMessageCallback (GLDEBUGPROC callback,const void * userParam)
	// GLAPI GLuint APIENTRY GetDebugMessageLog (GLuint count,GLsizei bufSize,GLenum * sources,GLenum * types,GLuint * ids,GLenum * severities,GLsizei * lengths,GLchar * messageLog)
	 ( Z32:_count Z32:_bufSize Z32*:_sources Z32*:_types Z32*:_ids Z32*:_severities Z32*:_lengths String:_messageLog  -> Z32 ) "libGL.so.1" "glGetDebugMessageLog" sys optional GetDebugMessageLog 

	// GLAPI void APIENTRY PushDebugGroup (GLenum source,GLuint id,GLsizei length,const GLchar * message)
	 ( Z32:_source Z32:_id Z32:_length String:_message ) "libGL.so.1" "glPushDebugGroup" sys optional PushDebugGroup 

	// GLAPI void APIENTRY PopDebugGroup (void)
	 ( ) "libGL.so.1" "glPopDebugGroup" sys optional PopDebugGroup 

	// GLAPI void APIENTRY ObjectLabel (GLenum identifier,GLuint name,GLsizei length,const GLchar * label)
	 ( Z32:_identifier Z32:_name Z32:_length String:_label ) "libGL.so.1" "glObjectLabel" sys optional ObjectLabel 

	// GLAPI void APIENTRY GetObjectLabel (GLenum identifier,GLuint name,GLsizei bufSize,GLsizei * length,GLchar * label)
	 ( Z32:_identifier Z32:_name Z32:_bufSize Z32*:_length String:_label ) "libGL.so.1" "glGetObjectLabel" sys optional GetObjectLabel 

	// GLAPI void APIENTRY ObjectPtrLabel (const void * ptr,GLsizei length,const GLchar * label)
	 ( Any*:_ptr Z32:_length String:_label ) "libGL.so.1" "glObjectPtrLabel" sys optional ObjectPtrLabel 

	// GLAPI void APIENTRY GetObjectPtrLabel (const void * ptr,GLsizei bufSize,GLsizei * length,GLchar * label)
	 ( Any*:_ptr Z32:_bufSize Z32*:_length String:_label ) "libGL.so.1" "glGetObjectPtrLabel" sys optional GetObjectPtrLabel 

	// GLAPI void APIENTRY BufferStorage (GLenum target,GLsizeiptr size,const void * data,GLbitfield flags)
	 ( Z32:_target Z32:_size Any*:_data Z32:_flags ) "libGL.so.1" "glBufferStorage" sys optional BufferStorage 

	// GLAPI void APIENTRY ClearTexImage (GLuint texture,GLint level,GLenum format,GLenum type,const void * data)
	 ( Z32:_texture Z32:_level Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearTexImage" sys optional ClearTexImage 

	// GLAPI void APIENTRY ClearTexSubImage (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const void * data)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearTexSubImage" sys optional ClearTexSubImage 

	// GLAPI void APIENTRY BindBuffersBase (GLenum target,GLuint first,GLsizei count,const GLuint * buffers)
	 ( Z32:_target Z32:_first Z32:_count Z32*:_buffers ) "libGL.so.1" "glBindBuffersBase" sys optional BindBuffersBase 

	// GLAPI void APIENTRY BindBuffersRange (GLenum target,GLuint first,GLsizei count,const GLuint * buffers,const GLintptr * offsets,const GLsizeiptr * sizes)
	 ( Z32:_target Z32:_first Z32:_count Z32*:_buffers Any&*:_offsets Z32*:_sizes ) "libGL.so.1" "glBindBuffersRange" sys optional BindBuffersRange 

	// GLAPI void APIENTRY BindTextures (GLuint first,GLsizei count,const GLuint * textures)
	 ( Z32:_first Z32:_count Z32*:_textures ) "libGL.so.1" "glBindTextures" sys optional BindTextures 

	// GLAPI void APIENTRY BindSamplers (GLuint first,GLsizei count,const GLuint * samplers)
	 ( Z32:_first Z32:_count Z32*:_samplers ) "libGL.so.1" "glBindSamplers" sys optional BindSamplers 

	// GLAPI void APIENTRY BindImageTextures (GLuint first,GLsizei count,const GLuint * textures)
	 ( Z32:_first Z32:_count Z32*:_textures ) "libGL.so.1" "glBindImageTextures" sys optional BindImageTextures 

	// GLAPI void APIENTRY BindVertexBuffers (GLuint first,GLsizei count,const GLuint * buffers,const GLintptr * offsets,const GLsizei * strides)
	 ( Z32:_first Z32:_count Z32*:_buffers Any&*:_offsets Z32*:_strides ) "libGL.so.1" "glBindVertexBuffers" sys optional BindVertexBuffers 

	// GLAPI void APIENTRY ClipControl (GLenum origin,GLenum depth)
	 ( Z32:_origin Z32:_depth ) "libGL.so.1" "glClipControl" sys optional ClipControl 

	// GLAPI void APIENTRY CreateTransformFeedbacks (GLsizei n,GLuint * ids)
	 ( Z32:_n Z32*:_ids ) "libGL.so.1" "glCreateTransformFeedbacks" sys optional CreateTransformFeedbacks 

	// GLAPI void APIENTRY TransformFeedbackBufferBase (GLuint xfb,GLuint index,GLuint buffer)
	 ( Z32:_xfb Z32:_index Z32:_buffer ) "libGL.so.1" "glTransformFeedbackBufferBase" sys optional TransformFeedbackBufferBase 

	// GLAPI void APIENTRY TransformFeedbackBufferRange (GLuint xfb,GLuint index,GLuint buffer,GLintptr offset,GLsizeiptr size)
	 ( Z32:_xfb Z32:_index Z32:_buffer Any&:_offset Z32:_size ) "libGL.so.1" "glTransformFeedbackBufferRange" sys optional TransformFeedbackBufferRange 

	// GLAPI void APIENTRY GetTransformFeedbackiv (GLuint xfb,GLenum pname,GLint * param)
	 ( Z32:_xfb Z32:_pname Z32*:_param ) "libGL.so.1" "glGetTransformFeedbackiv" sys optional GetTransformFeedbackiv 

	// GLAPI void APIENTRY GetTransformFeedbacki_v (GLuint xfb,GLenum pname,GLuint index,GLint * param)
	 ( Z32:_xfb Z32:_pname Z32:_index Z32*:_param ) "libGL.so.1" "glGetTransformFeedbacki_v" sys optional GetTransformFeedbacki_v 

	// GLAPI void APIENTRY GetTransformFeedbacki64_v (GLuint xfb,GLenum pname,GLuint index,GLint64 * param)
	 ( Z32:_xfb Z32:_pname Z32:_index Any&*:_param ) "libGL.so.1" "glGetTransformFeedbacki64_v" sys optional GetTransformFeedbacki64_v 

	// GLAPI void APIENTRY CreateBuffers (GLsizei n,GLuint * buffers)
	 ( Z32:_n Z32*:_buffers ) "libGL.so.1" "glCreateBuffers" sys optional CreateBuffers 

	// GLAPI void APIENTRY NamedBufferStorage (GLuint buffer,GLsizeiptr size,const void * data,GLbitfield flags)
	 ( Z32:_buffer Z32:_size Any*:_data Z32:_flags ) "libGL.so.1" "glNamedBufferStorage" sys optional NamedBufferStorage 

	// GLAPI void APIENTRY NamedBufferData (GLuint buffer,GLsizeiptr size,const void * data,GLenum usage)
	 ( Z32:_buffer Z32:_size Any*:_data Z32:_usage ) "libGL.so.1" "glNamedBufferData" sys optional NamedBufferData 

	// GLAPI void APIENTRY NamedBufferSubData (GLuint buffer,GLintptr offset,GLsizeiptr size,const void * data)
	 ( Z32:_buffer Any&:_offset Z32:_size Any*:_data ) "libGL.so.1" "glNamedBufferSubData" sys optional NamedBufferSubData 

	// GLAPI void APIENTRY CopyNamedBufferSubData (GLuint readBuffer,GLuint writeBuffer,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size)
	 ( Z32:_readBuffer Z32:_writeBuffer Any&:_readOffset Any&:_writeOffset Z32:_size ) "libGL.so.1" "glCopyNamedBufferSubData" sys optional CopyNamedBufferSubData 

	// GLAPI void APIENTRY ClearNamedBufferData (GLuint buffer,GLenum internalformat,GLenum format,GLenum type,const void * data)
	 ( Z32:_buffer Z32:_internalformat Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearNamedBufferData" sys optional ClearNamedBufferData 

	// GLAPI void APIENTRY ClearNamedBufferSubData (GLuint buffer,GLenum internalformat,GLintptr offset,GLsizeiptr size,GLenum format,GLenum type,const void * data)
	 ( Z32:_buffer Z32:_internalformat Any&:_offset Z32:_size Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearNamedBufferSubData" sys optional ClearNamedBufferSubData 

	// GLAPI void * APIENTRY MapNamedBuffer (GLuint buffer,GLenum access)
	 ( Z32:_buffer Z32:_access  -> Any* ) "libGL.so.1" "glMapNamedBuffer" sys optional MapNamedBuffer 

	// GLAPI void * APIENTRY MapNamedBufferRange (GLuint buffer,GLintptr offset,GLsizeiptr length,GLbitfield access)
	 ( Z32:_buffer Any&:_offset Z32:_length Z32:_access  -> Any* ) "libGL.so.1" "glMapNamedBufferRange" sys optional MapNamedBufferRange 

	// GLAPI GLboolean APIENTRY UnmapNamedBuffer (GLuint buffer)
	 ( Z32:_buffer  -> Bit ) "libGL.so.1" "glUnmapNamedBuffer" sys optional UnmapNamedBuffer 

	// GLAPI void APIENTRY FlushMappedNamedBufferRange (GLuint buffer,GLintptr offset,GLsizeiptr length)
	 ( Z32:_buffer Any&:_offset Z32:_length ) "libGL.so.1" "glFlushMappedNamedBufferRange" sys optional FlushMappedNamedBufferRange 

	// GLAPI void APIENTRY GetNamedBufferParameteriv (GLuint buffer,GLenum pname,GLint * params)
	 ( Z32:_buffer Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedBufferParameteriv" sys optional GetNamedBufferParameteriv 

	// GLAPI void APIENTRY GetNamedBufferParameteri64v (GLuint buffer,GLenum pname,GLint64 * params)
	 ( Z32:_buffer Z32:_pname Any&*:_params ) "libGL.so.1" "glGetNamedBufferParameteri64v" sys optional GetNamedBufferParameteri64v 

	// GLAPI void APIENTRY GetNamedBufferPointerv (GLuint buffer,GLenum pname,void ** params)
	 ( Z32:_buffer Z32:_pname Any**:_params ) "libGL.so.1" "glGetNamedBufferPointerv" sys optional GetNamedBufferPointerv 

	// GLAPI void APIENTRY GetNamedBufferSubData (GLuint buffer,GLintptr offset,GLsizeiptr size,void * data)
	 ( Z32:_buffer Any&:_offset Z32:_size Any*:_data ) "libGL.so.1" "glGetNamedBufferSubData" sys optional GetNamedBufferSubData 

	// GLAPI void APIENTRY CreateFramebuffers (GLsizei n,GLuint * framebuffers)
	 ( Z32:_n Z32*:_framebuffers ) "libGL.so.1" "glCreateFramebuffers" sys optional CreateFramebuffers 

	// GLAPI void APIENTRY NamedFramebufferRenderbuffer (GLuint framebuffer,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer)
	 ( Z32:_framebuffer Z32:_attachment Z32:_renderbuffertarget Z32:_renderbuffer ) "libGL.so.1" "glNamedFramebufferRenderbuffer" sys optional NamedFramebufferRenderbuffer 

	// GLAPI void APIENTRY NamedFramebufferParameteri (GLuint framebuffer,GLenum pname,GLint param)
	 ( Z32:_framebuffer Z32:_pname Z32:_param ) "libGL.so.1" "glNamedFramebufferParameteri" sys optional NamedFramebufferParameteri 

	// GLAPI void APIENTRY NamedFramebufferTexture (GLuint framebuffer,GLenum attachment,GLuint texture,GLint level)
	 ( Z32:_framebuffer Z32:_attachment Z32:_texture Z32:_level ) "libGL.so.1" "glNamedFramebufferTexture" sys optional NamedFramebufferTexture 

	// GLAPI void APIENTRY NamedFramebufferTextureLayer (GLuint framebuffer,GLenum attachment,GLuint texture,GLint level,GLint layer)
	 ( Z32:_framebuffer Z32:_attachment Z32:_texture Z32:_level Z32:_layer ) "libGL.so.1" "glNamedFramebufferTextureLayer" sys optional NamedFramebufferTextureLayer 

	// GLAPI void APIENTRY NamedFramebufferDrawBuffer (GLuint framebuffer,GLenum buf)
	 ( Z32:_framebuffer Z32:_buf ) "libGL.so.1" "glNamedFramebufferDrawBuffer" sys optional NamedFramebufferDrawBuffer 

	// GLAPI void APIENTRY NamedFramebufferDrawBuffers (GLuint framebuffer,GLsizei n,const GLenum * bufs)
	 ( Z32:_framebuffer Z32:_n Z32*:_bufs ) "libGL.so.1" "glNamedFramebufferDrawBuffers" sys optional NamedFramebufferDrawBuffers 

	// GLAPI void APIENTRY NamedFramebufferReadBuffer (GLuint framebuffer,GLenum src)
	 ( Z32:_framebuffer Z32:_src ) "libGL.so.1" "glNamedFramebufferReadBuffer" sys optional NamedFramebufferReadBuffer 

	// GLAPI void APIENTRY InvalidateNamedFramebufferData (GLuint framebuffer,GLsizei numAttachments,const GLenum * attachments)
	 ( Z32:_framebuffer Z32:_numAttachments Z32*:_attachments ) "libGL.so.1" "glInvalidateNamedFramebufferData" sys optional InvalidateNamedFramebufferData 

	// GLAPI void APIENTRY InvalidateNamedFramebufferSubData (GLuint framebuffer,GLsizei numAttachments,const GLenum * attachments,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_framebuffer Z32:_numAttachments Z32*:_attachments Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glInvalidateNamedFramebufferSubData" sys optional InvalidateNamedFramebufferSubData 

	// GLAPI void APIENTRY ClearNamedFramebufferiv (GLuint framebuffer,GLenum buffer,GLint drawbuffer,const GLint * value)
	 ( Z32:_framebuffer Z32:_buffer Z32:_drawbuffer Z32*:_value ) "libGL.so.1" "glClearNamedFramebufferiv" sys optional ClearNamedFramebufferiv 

	// GLAPI void APIENTRY ClearNamedFramebufferuiv (GLuint framebuffer,GLenum buffer,GLint drawbuffer,const GLuint * value)
	 ( Z32:_framebuffer Z32:_buffer Z32:_drawbuffer Z32*:_value ) "libGL.so.1" "glClearNamedFramebufferuiv" sys optional ClearNamedFramebufferuiv 

	// GLAPI void APIENTRY ClearNamedFramebufferfv (GLuint framebuffer,GLenum buffer,GLint drawbuffer,const GLfloat * value)
	 ( Z32:_framebuffer Z32:_buffer Z32:_drawbuffer Real*:_value ) "libGL.so.1" "glClearNamedFramebufferfv" sys optional ClearNamedFramebufferfv 

	// GLAPI void APIENTRY ClearNamedFramebufferfi (GLuint framebuffer,GLenum buffer,GLint drawbuffer,GLfloat depth,GLint stencil)
	 ( Z32:_framebuffer Z32:_buffer Z32:_drawbuffer Real:_depth Z32:_stencil ) "libGL.so.1" "glClearNamedFramebufferfi" sys optional ClearNamedFramebufferfi 

	// GLAPI void APIENTRY BlitNamedFramebuffer (GLuint readFramebuffer,GLuint drawFramebuffer,GLint srcX0,GLint srcY0,GLint srcX1,GLint srcY1,GLint dstX0,GLint dstY0,GLint dstX1,GLint dstY1,GLbitfield mask,GLenum filter)
	 ( Z32:_readFramebuffer Z32:_drawFramebuffer Z32:_srcX0 Z32:_srcY0 Z32:_srcX1 Z32:_srcY1 Z32:_dstX0 Z32:_dstY0 Z32:_dstX1 Z32:_dstY1 Z32:_mask Z32:_filter ) "libGL.so.1" "glBlitNamedFramebuffer" sys optional BlitNamedFramebuffer 

	// GLAPI GLenum APIENTRY CheckNamedFramebufferStatus (GLuint framebuffer,GLenum target)
	 ( Z32:_framebuffer Z32:_target  -> Z32 ) "libGL.so.1" "glCheckNamedFramebufferStatus" sys optional CheckNamedFramebufferStatus 

	// GLAPI void APIENTRY GetNamedFramebufferParameteriv (GLuint framebuffer,GLenum pname,GLint * param)
	 ( Z32:_framebuffer Z32:_pname Z32*:_param ) "libGL.so.1" "glGetNamedFramebufferParameteriv" sys optional GetNamedFramebufferParameteriv 

	// GLAPI void APIENTRY GetNamedFramebufferAttachmentParameteriv (GLuint framebuffer,GLenum attachment,GLenum pname,GLint * params)
	 ( Z32:_framebuffer Z32:_attachment Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedFramebufferAttachmentParameteriv" sys optional GetNamedFramebufferAttachmentParameteriv 

	// GLAPI void APIENTRY CreateRenderbuffers (GLsizei n,GLuint * renderbuffers)
	 ( Z32:_n Z32*:_renderbuffers ) "libGL.so.1" "glCreateRenderbuffers" sys optional CreateRenderbuffers 

	// GLAPI void APIENTRY NamedRenderbufferStorage (GLuint renderbuffer,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_renderbuffer Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glNamedRenderbufferStorage" sys optional NamedRenderbufferStorage 

	// GLAPI void APIENTRY NamedRenderbufferStorageMultisample (GLuint renderbuffer,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_renderbuffer Z32:_samples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glNamedRenderbufferStorageMultisample" sys optional NamedRenderbufferStorageMultisample 

	// GLAPI void APIENTRY GetNamedRenderbufferParameteriv (GLuint renderbuffer,GLenum pname,GLint * params)
	 ( Z32:_renderbuffer Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedRenderbufferParameteriv" sys optional GetNamedRenderbufferParameteriv 

	// GLAPI void APIENTRY CreateTextures (GLenum target,GLsizei n,GLuint * textures)
	 ( Z32:_target Z32:_n Z32*:_textures ) "libGL.so.1" "glCreateTextures" sys optional CreateTextures 

	// GLAPI void APIENTRY TextureBuffer (GLuint texture,GLenum internalformat,GLuint buffer)
	 ( Z32:_texture Z32:_internalformat Z32:_buffer ) "libGL.so.1" "glTextureBuffer" sys optional TextureBuffer 

	// GLAPI void APIENTRY TextureBufferRange (GLuint texture,GLenum internalformat,GLuint buffer,GLintptr offset,GLsizeiptr size)
	 ( Z32:_texture Z32:_internalformat Z32:_buffer Any&:_offset Z32:_size ) "libGL.so.1" "glTextureBufferRange" sys optional TextureBufferRange 

	// GLAPI void APIENTRY TextureStorage1D (GLuint texture,GLsizei levels,GLenum internalformat,GLsizei width)
	 ( Z32:_texture Z32:_levels Z32:_internalformat Z32:_width ) "libGL.so.1" "glTextureStorage1D" sys optional TextureStorage1D 

	// GLAPI void APIENTRY TextureStorage2D (GLuint texture,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_texture Z32:_levels Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glTextureStorage2D" sys optional TextureStorage2D 

	// GLAPI void APIENTRY TextureStorage3D (GLuint texture,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth)
	 ( Z32:_texture Z32:_levels Z32:_internalformat Z32:_width Z32:_height Z32:_depth ) "libGL.so.1" "glTextureStorage3D" sys optional TextureStorage3D 

	// GLAPI void APIENTRY TextureStorage2DMultisample (GLuint texture,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations)
	 ( Z32:_texture Z32:_samples Z32:_internalformat Z32:_width Z32:_height Bit:_fixedsamplelocations ) "libGL.so.1" "glTextureStorage2DMultisample" sys optional TextureStorage2DMultisample 

	// GLAPI void APIENTRY TextureStorage3DMultisample (GLuint texture,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations)
	 ( Z32:_texture Z32:_samples Z32:_internalformat Z32:_width Z32:_height Z32:_depth Bit:_fixedsamplelocations ) "libGL.so.1" "glTextureStorage3DMultisample" sys optional TextureStorage3DMultisample 

	// GLAPI void APIENTRY TextureSubImage1D (GLuint texture,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureSubImage1D" sys optional TextureSubImage1D 

	// GLAPI void APIENTRY TextureSubImage2D (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureSubImage2D" sys optional TextureSubImage2D 

	// GLAPI void APIENTRY TextureSubImage3D (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureSubImage3D" sys optional TextureSubImage3D 

	// GLAPI void APIENTRY CompressedTextureSubImage1D (GLuint texture,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const void * data)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTextureSubImage1D" sys optional CompressedTextureSubImage1D 

	// GLAPI void APIENTRY CompressedTextureSubImage2D (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const void * data)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTextureSubImage2D" sys optional CompressedTextureSubImage2D 

	// GLAPI void APIENTRY CompressedTextureSubImage3D (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const void * data)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_imageSize Any*:_data ) "libGL.so.1" "glCompressedTextureSubImage3D" sys optional CompressedTextureSubImage3D 

	// GLAPI void APIENTRY CopyTextureSubImage1D (GLuint texture,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_x Z32:_y Z32:_width ) "libGL.so.1" "glCopyTextureSubImage1D" sys optional CopyTextureSubImage1D 

	// GLAPI void APIENTRY CopyTextureSubImage2D (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyTextureSubImage2D" sys optional CopyTextureSubImage2D 

	// GLAPI void APIENTRY CopyTextureSubImage3D (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyTextureSubImage3D" sys optional CopyTextureSubImage3D 

	// GLAPI void APIENTRY TextureParameterf (GLuint texture,GLenum pname,GLfloat param)
	 ( Z32:_texture Z32:_pname Real:_param ) "libGL.so.1" "glTextureParameterf" sys optional TextureParameterf 

	// GLAPI void APIENTRY TextureParameterfv (GLuint texture,GLenum pname,const GLfloat * param)
	 ( Z32:_texture Z32:_pname Real*:_param ) "libGL.so.1" "glTextureParameterfv" sys optional TextureParameterfv 

	// GLAPI void APIENTRY TextureParameteri (GLuint texture,GLenum pname,GLint param)
	 ( Z32:_texture Z32:_pname Z32:_param ) "libGL.so.1" "glTextureParameteri" sys optional TextureParameteri 

	// GLAPI void APIENTRY TextureParameterIiv (GLuint texture,GLenum pname,const GLint * params)
	 ( Z32:_texture Z32:_pname Z32*:_params ) "libGL.so.1" "glTextureParameterIiv" sys optional TextureParameterIiv 

	// GLAPI void APIENTRY TextureParameterIuiv (GLuint texture,GLenum pname,const GLuint * params)
	 ( Z32:_texture Z32:_pname Z32*:_params ) "libGL.so.1" "glTextureParameterIuiv" sys optional TextureParameterIuiv 

	// GLAPI void APIENTRY TextureParameteriv (GLuint texture,GLenum pname,const GLint * param)
	 ( Z32:_texture Z32:_pname Z32*:_param ) "libGL.so.1" "glTextureParameteriv" sys optional TextureParameteriv 

	// GLAPI void APIENTRY GenerateTextureMipmap (GLuint texture)
	 ( Z32:_texture ) "libGL.so.1" "glGenerateTextureMipmap" sys optional GenerateTextureMipmap 

	// GLAPI void APIENTRY BindTextureUnit (GLuint unit,GLuint texture)
	 ( Z32:_unit Z32:_texture ) "libGL.so.1" "glBindTextureUnit" sys optional BindTextureUnit 

	// GLAPI void APIENTRY GetTextureImage (GLuint texture,GLint level,GLenum format,GLenum type,GLsizei bufSize,void * pixels)
	 ( Z32:_texture Z32:_level Z32:_format Z32:_type Z32:_bufSize Any*:_pixels ) "libGL.so.1" "glGetTextureImage" sys optional GetTextureImage 

	// GLAPI void APIENTRY GetCompressedTextureImage (GLuint texture,GLint level,GLsizei bufSize,void * pixels)
	 ( Z32:_texture Z32:_level Z32:_bufSize Any*:_pixels ) "libGL.so.1" "glGetCompressedTextureImage" sys optional GetCompressedTextureImage 

	// GLAPI void APIENTRY GetTextureLevelParameterfv (GLuint texture,GLint level,GLenum pname,GLfloat * params)
	 ( Z32:_texture Z32:_level Z32:_pname Real*:_params ) "libGL.so.1" "glGetTextureLevelParameterfv" sys optional GetTextureLevelParameterfv 

	// GLAPI void APIENTRY GetTextureLevelParameteriv (GLuint texture,GLint level,GLenum pname,GLint * params)
	 ( Z32:_texture Z32:_level Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureLevelParameteriv" sys optional GetTextureLevelParameteriv 

	// GLAPI void APIENTRY GetTextureParameterfv (GLuint texture,GLenum pname,GLfloat * params)
	 ( Z32:_texture Z32:_pname Real*:_params ) "libGL.so.1" "glGetTextureParameterfv" sys optional GetTextureParameterfv 

	// GLAPI void APIENTRY GetTextureParameterIiv (GLuint texture,GLenum pname,GLint * params)
	 ( Z32:_texture Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureParameterIiv" sys optional GetTextureParameterIiv 

	// GLAPI void APIENTRY GetTextureParameterIuiv (GLuint texture,GLenum pname,GLuint * params)
	 ( Z32:_texture Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureParameterIuiv" sys optional GetTextureParameterIuiv 

	// GLAPI void APIENTRY GetTextureParameteriv (GLuint texture,GLenum pname,GLint * params)
	 ( Z32:_texture Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureParameteriv" sys optional GetTextureParameteriv 

	// GLAPI void APIENTRY CreateVertexArrays (GLsizei n,GLuint * arrays)
	 ( Z32:_n Z32*:_arrays ) "libGL.so.1" "glCreateVertexArrays" sys optional CreateVertexArrays 

	// GLAPI void APIENTRY DisableVertexArrayAttrib (GLuint vaobj,GLuint index)
	 ( Z32:_vaobj Z32:_index ) "libGL.so.1" "glDisableVertexArrayAttrib" sys optional DisableVertexArrayAttrib 

	// GLAPI void APIENTRY EnableVertexArrayAttrib (GLuint vaobj,GLuint index)
	 ( Z32:_vaobj Z32:_index ) "libGL.so.1" "glEnableVertexArrayAttrib" sys optional EnableVertexArrayAttrib 

	// GLAPI void APIENTRY VertexArrayElementBuffer (GLuint vaobj,GLuint buffer)
	 ( Z32:_vaobj Z32:_buffer ) "libGL.so.1" "glVertexArrayElementBuffer" sys optional VertexArrayElementBuffer 

	// GLAPI void APIENTRY VertexArrayVertexBuffer (GLuint vaobj,GLuint bindingindex,GLuint buffer,GLintptr offset,GLsizei stride)
	 ( Z32:_vaobj Z32:_bindingindex Z32:_buffer Any&:_offset Z32:_stride ) "libGL.so.1" "glVertexArrayVertexBuffer" sys optional VertexArrayVertexBuffer 

	// GLAPI void APIENTRY VertexArrayVertexBuffers (GLuint vaobj,GLuint first,GLsizei count,const GLuint * buffers,const GLintptr * offsets,const GLsizei * strides)
	 ( Z32:_vaobj Z32:_first Z32:_count Z32*:_buffers Any&*:_offsets Z32*:_strides ) "libGL.so.1" "glVertexArrayVertexBuffers" sys optional VertexArrayVertexBuffers 

	// GLAPI void APIENTRY VertexArrayAttribBinding (GLuint vaobj,GLuint attribindex,GLuint bindingindex)
	 ( Z32:_vaobj Z32:_attribindex Z32:_bindingindex ) "libGL.so.1" "glVertexArrayAttribBinding" sys optional VertexArrayAttribBinding 

	// GLAPI void APIENTRY VertexArrayAttribFormat (GLuint vaobj,GLuint attribindex,GLint size,GLenum type,GLboolean normalized,GLuint relativeoffset)
	 ( Z32:_vaobj Z32:_attribindex Z32:_size Z32:_type Bit:_normalized Z32:_relativeoffset ) "libGL.so.1" "glVertexArrayAttribFormat" sys optional VertexArrayAttribFormat 

	// GLAPI void APIENTRY VertexArrayAttribIFormat (GLuint vaobj,GLuint attribindex,GLint size,GLenum type,GLuint relativeoffset)
	 ( Z32:_vaobj Z32:_attribindex Z32:_size Z32:_type Z32:_relativeoffset ) "libGL.so.1" "glVertexArrayAttribIFormat" sys optional VertexArrayAttribIFormat 

	// GLAPI void APIENTRY VertexArrayAttribLFormat (GLuint vaobj,GLuint attribindex,GLint size,GLenum type,GLuint relativeoffset)
	 ( Z32:_vaobj Z32:_attribindex Z32:_size Z32:_type Z32:_relativeoffset ) "libGL.so.1" "glVertexArrayAttribLFormat" sys optional VertexArrayAttribLFormat 

	// GLAPI void APIENTRY VertexArrayBindingDivisor (GLuint vaobj,GLuint bindingindex,GLuint divisor)
	 ( Z32:_vaobj Z32:_bindingindex Z32:_divisor ) "libGL.so.1" "glVertexArrayBindingDivisor" sys optional VertexArrayBindingDivisor 

	// GLAPI void APIENTRY GetVertexArrayiv (GLuint vaobj,GLenum pname,GLint * param)
	 ( Z32:_vaobj Z32:_pname Z32*:_param ) "libGL.so.1" "glGetVertexArrayiv" sys optional GetVertexArrayiv 

	// GLAPI void APIENTRY GetVertexArrayIndexediv (GLuint vaobj,GLuint index,GLenum pname,GLint * param)
	 ( Z32:_vaobj Z32:_index Z32:_pname Z32*:_param ) "libGL.so.1" "glGetVertexArrayIndexediv" sys optional GetVertexArrayIndexediv 

	// GLAPI void APIENTRY GetVertexArrayIndexed64iv (GLuint vaobj,GLuint index,GLenum pname,GLint64 * param)
	 ( Z32:_vaobj Z32:_index Z32:_pname Any&*:_param ) "libGL.so.1" "glGetVertexArrayIndexed64iv" sys optional GetVertexArrayIndexed64iv 

	// GLAPI void APIENTRY CreateSamplers (GLsizei n,GLuint * samplers)
	 ( Z32:_n Z32*:_samplers ) "libGL.so.1" "glCreateSamplers" sys optional CreateSamplers 

	// GLAPI void APIENTRY CreateProgramPipelines (GLsizei n,GLuint * pipelines)
	 ( Z32:_n Z32*:_pipelines ) "libGL.so.1" "glCreateProgramPipelines" sys optional CreateProgramPipelines 

	// GLAPI void APIENTRY CreateQueries (GLenum target,GLsizei n,GLuint * ids)
	 ( Z32:_target Z32:_n Z32*:_ids ) "libGL.so.1" "glCreateQueries" sys optional CreateQueries 

	// GLAPI void APIENTRY GetQueryBufferObjecti64v (GLuint id,GLuint buffer,GLenum pname,GLintptr offset)
	 ( Z32:_id Z32:_buffer Z32:_pname Any&:_offset ) "libGL.so.1" "glGetQueryBufferObjecti64v" sys optional GetQueryBufferObjecti64v 

	// GLAPI void APIENTRY GetQueryBufferObjectiv (GLuint id,GLuint buffer,GLenum pname,GLintptr offset)
	 ( Z32:_id Z32:_buffer Z32:_pname Any&:_offset ) "libGL.so.1" "glGetQueryBufferObjectiv" sys optional GetQueryBufferObjectiv 

	// GLAPI void APIENTRY GetQueryBufferObjectui64v (GLuint id,GLuint buffer,GLenum pname,GLintptr offset)
	 ( Z32:_id Z32:_buffer Z32:_pname Any&:_offset ) "libGL.so.1" "glGetQueryBufferObjectui64v" sys optional GetQueryBufferObjectui64v 

	// GLAPI void APIENTRY GetQueryBufferObjectuiv (GLuint id,GLuint buffer,GLenum pname,GLintptr offset)
	 ( Z32:_id Z32:_buffer Z32:_pname Any&:_offset ) "libGL.so.1" "glGetQueryBufferObjectuiv" sys optional GetQueryBufferObjectuiv 

	// GLAPI void APIENTRY MemoryBarrierByRegion (GLbitfield barriers)
	 ( Z32:_barriers ) "libGL.so.1" "glMemoryBarrierByRegion" sys optional MemoryBarrierByRegion 

	// GLAPI void APIENTRY GetTextureSubImage (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,GLsizei bufSize,void * pixels)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_type Z32:_bufSize Any*:_pixels ) "libGL.so.1" "glGetTextureSubImage" sys optional GetTextureSubImage 

	// GLAPI void APIENTRY GetCompressedTextureSubImage (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLsizei bufSize,void * pixels)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_bufSize Any*:_pixels ) "libGL.so.1" "glGetCompressedTextureSubImage" sys optional GetCompressedTextureSubImage 

	// GLAPI GLenum APIENTRY GetGraphicsResetStatus (void)
	 (  -> Z32 ) "libGL.so.1" "glGetGraphicsResetStatus" sys optional GetGraphicsResetStatus 

	// GLAPI void APIENTRY GetnCompressedTexImage (GLenum target,GLint lod,GLsizei bufSize,void * pixels)
	 ( Z32:_target Z32:_lod Z32:_bufSize Any*:_pixels ) "libGL.so.1" "glGetnCompressedTexImage" sys optional GetnCompressedTexImage 

	// GLAPI void APIENTRY GetnTexImage (GLenum target,GLint level,GLenum format,GLenum type,GLsizei bufSize,void * pixels)
	 ( Z32:_target Z32:_level Z32:_format Z32:_type Z32:_bufSize Any*:_pixels ) "libGL.so.1" "glGetnTexImage" sys optional GetnTexImage 

	// GLAPI void APIENTRY GetnUniformdv (GLuint program,GLint location,GLsizei bufSize,GLdouble * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Double32*:_params ) "libGL.so.1" "glGetnUniformdv" sys optional GetnUniformdv 

	// GLAPI void APIENTRY GetnUniformfv (GLuint program,GLint location,GLsizei bufSize,GLfloat * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Real*:_params ) "libGL.so.1" "glGetnUniformfv" sys optional GetnUniformfv 

	// GLAPI void APIENTRY GetnUniformiv (GLuint program,GLint location,GLsizei bufSize,GLint * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Z32*:_params ) "libGL.so.1" "glGetnUniformiv" sys optional GetnUniformiv 

	// GLAPI void APIENTRY GetnUniformuiv (GLuint program,GLint location,GLsizei bufSize,GLuint * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Z32*:_params ) "libGL.so.1" "glGetnUniformuiv" sys optional GetnUniformuiv 

	// GLAPI void APIENTRY ReadnPixels (GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLsizei bufSize,void * data)
	 ( Z32:_x Z32:_y Z32:_width Z32:_height Z32:_format Z32:_type Z32:_bufSize Any*:_data ) "libGL.so.1" "glReadnPixels" sys optional ReadnPixels 

	// GLAPI void APIENTRY TextureBarrier (void)
	 ( ) "libGL.so.1" "glTextureBarrier" sys optional TextureBarrier 

	// GLAPI void APIENTRY SpecializeShader (GLuint shader,const GLchar * pEntryPoint,GLuint numSpecializationConstants,const GLuint * pConstantIndex,const GLuint * pConstantValue)
	 ( Z32:_shader String:_pEntryPoint Z32:_numSpecializationConstants Z32*:_pConstantIndex Z32*:_pConstantValue ) "libGL.so.1" "glSpecializeShader" sys optional SpecializeShader 

	// GLAPI void APIENTRY MultiDrawArraysIndirectCount (GLenum mode,const void * indirect,GLintptr drawcount,GLsizei maxdrawcount,GLsizei stride)
	 ( Z32:_mode Any*:_indirect Any&:_drawcount Z32:_maxdrawcount Z32:_stride ) "libGL.so.1" "glMultiDrawArraysIndirectCount" sys optional MultiDrawArraysIndirectCount 

	// GLAPI void APIENTRY MultiDrawElementsIndirectCount (GLenum mode,GLenum type,const void * indirect,GLintptr drawcount,GLsizei maxdrawcount,GLsizei stride)
	 ( Z32:_mode Z32:_type Any*:_indirect Any&:_drawcount Z32:_maxdrawcount Z32:_stride ) "libGL.so.1" "glMultiDrawElementsIndirectCount" sys optional MultiDrawElementsIndirectCount 

	// GLAPI void APIENTRY PolygonOffsetClamp (GLfloat factor,GLfloat units,GLfloat clamp)
	 ( Real:_factor Real:_units Real:_clamp ) "libGL.so.1" "glPolygonOffsetClamp" sys optional PolygonOffsetClamp 

	// GLAPI void APIENTRY PrimitiveBoundingBoxARB (GLfloat minX,GLfloat minY,GLfloat minZ,GLfloat minW,GLfloat maxX,GLfloat maxY,GLfloat maxZ,GLfloat maxW)
	 ( Real:_minX Real:_minY Real:_minZ Real:_minW Real:_maxX Real:_maxY Real:_maxZ Real:_maxW ) "libGL.so.1" "glPrimitiveBoundingBoxARB" sys optional PrimitiveBoundingBoxARB 

	// GLAPI GLuint64 APIENTRY GetTextureHandleARB (GLuint texture)
	 ( Z32:_texture  -> Any& ) "libGL.so.1" "glGetTextureHandleARB" sys optional GetTextureHandleARB 

	// GLAPI GLuint64 APIENTRY GetTextureSamplerHandleARB (GLuint texture,GLuint sampler)
	 ( Z32:_texture Z32:_sampler  -> Any& ) "libGL.so.1" "glGetTextureSamplerHandleARB" sys optional GetTextureSamplerHandleARB 

	// GLAPI void APIENTRY MakeTextureHandleResidentARB (GLuint64 handle)
	 ( Any&:_handle ) "libGL.so.1" "glMakeTextureHandleResidentARB" sys optional MakeTextureHandleResidentARB 

	// GLAPI void APIENTRY MakeTextureHandleNonResidentARB (GLuint64 handle)
	 ( Any&:_handle ) "libGL.so.1" "glMakeTextureHandleNonResidentARB" sys optional MakeTextureHandleNonResidentARB 

	// GLAPI GLuint64 APIENTRY GetImageHandleARB (GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum format)
	 ( Z32:_texture Z32:_level Bit:_layered Z32:_layer Z32:_format  -> Any& ) "libGL.so.1" "glGetImageHandleARB" sys optional GetImageHandleARB 

	// GLAPI void APIENTRY MakeImageHandleResidentARB (GLuint64 handle,GLenum access)
	 ( Any&:_handle Z32:_access ) "libGL.so.1" "glMakeImageHandleResidentARB" sys optional MakeImageHandleResidentARB 

	// GLAPI void APIENTRY MakeImageHandleNonResidentARB (GLuint64 handle)
	 ( Any&:_handle ) "libGL.so.1" "glMakeImageHandleNonResidentARB" sys optional MakeImageHandleNonResidentARB 

	// GLAPI void APIENTRY UniformHandleui64ARB (GLint location,GLuint64 value)
	 ( Z32:_location Any&:_value ) "libGL.so.1" "glUniformHandleui64ARB" sys optional UniformHandleui64ARB 

	// GLAPI void APIENTRY UniformHandleui64vARB (GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniformHandleui64vARB" sys optional UniformHandleui64vARB 

	// GLAPI void APIENTRY ProgramUniformHandleui64ARB (GLuint program,GLint location,GLuint64 value)
	 ( Z32:_program Z32:_location Any&:_value ) "libGL.so.1" "glProgramUniformHandleui64ARB" sys optional ProgramUniformHandleui64ARB 

	// GLAPI void APIENTRY ProgramUniformHandleui64vARB (GLuint program,GLint location,GLsizei count,const GLuint64 * values)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_values ) "libGL.so.1" "glProgramUniformHandleui64vARB" sys optional ProgramUniformHandleui64vARB 

	// GLAPI GLboolean APIENTRY IsTextureHandleResidentARB (GLuint64 handle)
	 ( Any&:_handle  -> Bit ) "libGL.so.1" "glIsTextureHandleResidentARB" sys optional IsTextureHandleResidentARB 

	// GLAPI GLboolean APIENTRY IsImageHandleResidentARB (GLuint64 handle)
	 ( Any&:_handle  -> Bit ) "libGL.so.1" "glIsImageHandleResidentARB" sys optional IsImageHandleResidentARB 

	// GLAPI void APIENTRY VertexAttribL1ui64ARB (GLuint index,GLuint64EXT x)
	 ( Z32:_index Any&:_x ) "libGL.so.1" "glVertexAttribL1ui64ARB" sys optional VertexAttribL1ui64ARB 

	// GLAPI void APIENTRY VertexAttribL1ui64vARB (GLuint index,const GLuint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL1ui64vARB" sys optional VertexAttribL1ui64vARB 

	// GLAPI void APIENTRY GetVertexAttribLui64vARB (GLuint index,GLenum pname,GLuint64EXT * params)
	 ( Z32:_index Z32:_pname Any&*:_params ) "libGL.so.1" "glGetVertexAttribLui64vARB" sys optional GetVertexAttribLui64vARB 

	// GLAPI GLsync APIENTRY CreateSyncFromCLeventARB (struct _cl_context * context,struct _cl_event * event,GLbitfield flags)
	// GLAPI void APIENTRY DispatchComputeGroupSizeARB (GLuint num_groups_x,GLuint num_groups_y,GLuint num_groups_z,GLuint group_size_x,GLuint group_size_y,GLuint group_size_z)
	 ( Z32:_num_groups_x Z32:_num_groups_y Z32:_num_groups_z Z32:_group_size_x Z32:_group_size_y Z32:_group_size_z ) "libGL.so.1" "glDispatchComputeGroupSizeARB" sys optional DispatchComputeGroupSizeARB 

	// GLAPI void APIENTRY DebugMessageControlARB (GLenum source,GLenum type,GLenum severity,GLsizei count,const GLuint * ids,GLboolean enabled)
	 ( Z32:_source Z32:_type Z32:_severity Z32:_count Z32*:_ids Bit:_enabled ) "libGL.so.1" "glDebugMessageControlARB" sys optional DebugMessageControlARB 

	// GLAPI void APIENTRY DebugMessageInsertARB (GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar * buf)
	 ( Z32:_source Z32:_type Z32:_id Z32:_severity Z32:_length String:_buf ) "libGL.so.1" "glDebugMessageInsertARB" sys optional DebugMessageInsertARB 

	// GLAPI void APIENTRY DebugMessageCallbackARB (GLDEBUGPROCARB callback,const void * userParam)
	// GLAPI GLuint APIENTRY GetDebugMessageLogARB (GLuint count,GLsizei bufSize,GLenum * sources,GLenum * types,GLuint * ids,GLenum * severities,GLsizei * lengths,GLchar * messageLog)
	 ( Z32:_count Z32:_bufSize Z32*:_sources Z32*:_types Z32*:_ids Z32*:_severities Z32*:_lengths String:_messageLog  -> Z32 ) "libGL.so.1" "glGetDebugMessageLogARB" sys optional GetDebugMessageLogARB 

	// GLAPI void APIENTRY BlendEquationiARB (GLuint buf,GLenum mode)
	 ( Z32:_buf Z32:_mode ) "libGL.so.1" "glBlendEquationiARB" sys optional BlendEquationiARB 

	// GLAPI void APIENTRY BlendEquationSeparateiARB (GLuint buf,GLenum modeRGB,GLenum modeAlpha)
	 ( Z32:_buf Z32:_modeRGB Z32:_modeAlpha ) "libGL.so.1" "glBlendEquationSeparateiARB" sys optional BlendEquationSeparateiARB 

	// GLAPI void APIENTRY BlendFunciARB (GLuint buf,GLenum src,GLenum dst)
	 ( Z32:_buf Z32:_src Z32:_dst ) "libGL.so.1" "glBlendFunciARB" sys optional BlendFunciARB 

	// GLAPI void APIENTRY BlendFuncSeparateiARB (GLuint buf,GLenum srcRGB,GLenum dstRGB,GLenum srcAlpha,GLenum dstAlpha)
	 ( Z32:_buf Z32:_srcRGB Z32:_dstRGB Z32:_srcAlpha Z32:_dstAlpha ) "libGL.so.1" "glBlendFuncSeparateiARB" sys optional BlendFuncSeparateiARB 

	// GLAPI void APIENTRY DrawArraysInstancedARB (GLenum mode,GLint first,GLsizei count,GLsizei primcount)
	 ( Z32:_mode Z32:_first Z32:_count Z32:_primcount ) "libGL.so.1" "glDrawArraysInstancedARB" sys optional DrawArraysInstancedARB 

	// GLAPI void APIENTRY DrawElementsInstancedARB (GLenum mode,GLsizei count,GLenum type,const void * indices,GLsizei primcount)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_primcount ) "libGL.so.1" "glDrawElementsInstancedARB" sys optional DrawElementsInstancedARB 

	// GLAPI void APIENTRY ProgramParameteriARB (GLuint program,GLenum pname,GLint value)
	 ( Z32:_program Z32:_pname Z32:_value ) "libGL.so.1" "glProgramParameteriARB" sys optional ProgramParameteriARB 

	// GLAPI void APIENTRY FramebufferTextureARB (GLenum target,GLenum attachment,GLuint texture,GLint level)
	 ( Z32:_target Z32:_attachment Z32:_texture Z32:_level ) "libGL.so.1" "glFramebufferTextureARB" sys optional FramebufferTextureARB 

	// GLAPI void APIENTRY FramebufferTextureLayerARB (GLenum target,GLenum attachment,GLuint texture,GLint level,GLint layer)
	 ( Z32:_target Z32:_attachment Z32:_texture Z32:_level Z32:_layer ) "libGL.so.1" "glFramebufferTextureLayerARB" sys optional FramebufferTextureLayerARB 

	// GLAPI void APIENTRY FramebufferTextureFaceARB (GLenum target,GLenum attachment,GLuint texture,GLint level,GLenum face)
	 ( Z32:_target Z32:_attachment Z32:_texture Z32:_level Z32:_face ) "libGL.so.1" "glFramebufferTextureFaceARB" sys optional FramebufferTextureFaceARB 

	// GLAPI void APIENTRY SpecializeShaderARB (GLuint shader,const GLchar * pEntryPoint,GLuint numSpecializationConstants,const GLuint * pConstantIndex,const GLuint * pConstantValue)
	 ( Z32:_shader String:_pEntryPoint Z32:_numSpecializationConstants Z32*:_pConstantIndex Z32*:_pConstantValue ) "libGL.so.1" "glSpecializeShaderARB" sys optional SpecializeShaderARB 

	// GLAPI void APIENTRY Uniform1i64ARB (GLint location,GLint64 x)
	 ( Z32:_location Any&:_x ) "libGL.so.1" "glUniform1i64ARB" sys optional Uniform1i64ARB 

	// GLAPI void APIENTRY Uniform2i64ARB (GLint location,GLint64 x,GLint64 y)
	 ( Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glUniform2i64ARB" sys optional Uniform2i64ARB 

	// GLAPI void APIENTRY Uniform3i64ARB (GLint location,GLint64 x,GLint64 y,GLint64 z)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glUniform3i64ARB" sys optional Uniform3i64ARB 

	// GLAPI void APIENTRY Uniform4i64ARB (GLint location,GLint64 x,GLint64 y,GLint64 z,GLint64 w)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glUniform4i64ARB" sys optional Uniform4i64ARB 

	// GLAPI void APIENTRY Uniform1i64vARB (GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform1i64vARB" sys optional Uniform1i64vARB 

	// GLAPI void APIENTRY Uniform2i64vARB (GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform2i64vARB" sys optional Uniform2i64vARB 

	// GLAPI void APIENTRY Uniform3i64vARB (GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform3i64vARB" sys optional Uniform3i64vARB 

	// GLAPI void APIENTRY Uniform4i64vARB (GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform4i64vARB" sys optional Uniform4i64vARB 

	// GLAPI void APIENTRY Uniform1ui64ARB (GLint location,GLuint64 x)
	 ( Z32:_location Any&:_x ) "libGL.so.1" "glUniform1ui64ARB" sys optional Uniform1ui64ARB 

	// GLAPI void APIENTRY Uniform2ui64ARB (GLint location,GLuint64 x,GLuint64 y)
	 ( Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glUniform2ui64ARB" sys optional Uniform2ui64ARB 

	// GLAPI void APIENTRY Uniform3ui64ARB (GLint location,GLuint64 x,GLuint64 y,GLuint64 z)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glUniform3ui64ARB" sys optional Uniform3ui64ARB 

	// GLAPI void APIENTRY Uniform4ui64ARB (GLint location,GLuint64 x,GLuint64 y,GLuint64 z,GLuint64 w)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glUniform4ui64ARB" sys optional Uniform4ui64ARB 

	// GLAPI void APIENTRY Uniform1ui64vARB (GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform1ui64vARB" sys optional Uniform1ui64vARB 

	// GLAPI void APIENTRY Uniform2ui64vARB (GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform2ui64vARB" sys optional Uniform2ui64vARB 

	// GLAPI void APIENTRY Uniform3ui64vARB (GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform3ui64vARB" sys optional Uniform3ui64vARB 

	// GLAPI void APIENTRY Uniform4ui64vARB (GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform4ui64vARB" sys optional Uniform4ui64vARB 

	// GLAPI void APIENTRY GetUniformi64vARB (GLuint program,GLint location,GLint64 * params)
	 ( Z32:_program Z32:_location Any&*:_params ) "libGL.so.1" "glGetUniformi64vARB" sys optional GetUniformi64vARB 

	// GLAPI void APIENTRY GetUniformui64vARB (GLuint program,GLint location,GLuint64 * params)
	 ( Z32:_program Z32:_location Any&*:_params ) "libGL.so.1" "glGetUniformui64vARB" sys optional GetUniformui64vARB 

	// GLAPI void APIENTRY GetnUniformi64vARB (GLuint program,GLint location,GLsizei bufSize,GLint64 * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Any&*:_params ) "libGL.so.1" "glGetnUniformi64vARB" sys optional GetnUniformi64vARB 

	// GLAPI void APIENTRY GetnUniformui64vARB (GLuint program,GLint location,GLsizei bufSize,GLuint64 * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Any&*:_params ) "libGL.so.1" "glGetnUniformui64vARB" sys optional GetnUniformui64vARB 

	// GLAPI void APIENTRY ProgramUniform1i64ARB (GLuint program,GLint location,GLint64 x)
	 ( Z32:_program Z32:_location Any&:_x ) "libGL.so.1" "glProgramUniform1i64ARB" sys optional ProgramUniform1i64ARB 

	// GLAPI void APIENTRY ProgramUniform2i64ARB (GLuint program,GLint location,GLint64 x,GLint64 y)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glProgramUniform2i64ARB" sys optional ProgramUniform2i64ARB 

	// GLAPI void APIENTRY ProgramUniform3i64ARB (GLuint program,GLint location,GLint64 x,GLint64 y,GLint64 z)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glProgramUniform3i64ARB" sys optional ProgramUniform3i64ARB 

	// GLAPI void APIENTRY ProgramUniform4i64ARB (GLuint program,GLint location,GLint64 x,GLint64 y,GLint64 z,GLint64 w)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glProgramUniform4i64ARB" sys optional ProgramUniform4i64ARB 

	// GLAPI void APIENTRY ProgramUniform1i64vARB (GLuint program,GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform1i64vARB" sys optional ProgramUniform1i64vARB 

	// GLAPI void APIENTRY ProgramUniform2i64vARB (GLuint program,GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform2i64vARB" sys optional ProgramUniform2i64vARB 

	// GLAPI void APIENTRY ProgramUniform3i64vARB (GLuint program,GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform3i64vARB" sys optional ProgramUniform3i64vARB 

	// GLAPI void APIENTRY ProgramUniform4i64vARB (GLuint program,GLint location,GLsizei count,const GLint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform4i64vARB" sys optional ProgramUniform4i64vARB 

	// GLAPI void APIENTRY ProgramUniform1ui64ARB (GLuint program,GLint location,GLuint64 x)
	 ( Z32:_program Z32:_location Any&:_x ) "libGL.so.1" "glProgramUniform1ui64ARB" sys optional ProgramUniform1ui64ARB 

	// GLAPI void APIENTRY ProgramUniform2ui64ARB (GLuint program,GLint location,GLuint64 x,GLuint64 y)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glProgramUniform2ui64ARB" sys optional ProgramUniform2ui64ARB 

	// GLAPI void APIENTRY ProgramUniform3ui64ARB (GLuint program,GLint location,GLuint64 x,GLuint64 y,GLuint64 z)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glProgramUniform3ui64ARB" sys optional ProgramUniform3ui64ARB 

	// GLAPI void APIENTRY ProgramUniform4ui64ARB (GLuint program,GLint location,GLuint64 x,GLuint64 y,GLuint64 z,GLuint64 w)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glProgramUniform4ui64ARB" sys optional ProgramUniform4ui64ARB 

	// GLAPI void APIENTRY ProgramUniform1ui64vARB (GLuint program,GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform1ui64vARB" sys optional ProgramUniform1ui64vARB 

	// GLAPI void APIENTRY ProgramUniform2ui64vARB (GLuint program,GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform2ui64vARB" sys optional ProgramUniform2ui64vARB 

	// GLAPI void APIENTRY ProgramUniform3ui64vARB (GLuint program,GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform3ui64vARB" sys optional ProgramUniform3ui64vARB 

	// GLAPI void APIENTRY ProgramUniform4ui64vARB (GLuint program,GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform4ui64vARB" sys optional ProgramUniform4ui64vARB 

	// GLAPI void APIENTRY MultiDrawArraysIndirectCountARB (GLenum mode,const void * indirect,GLintptr drawcount,GLsizei maxdrawcount,GLsizei stride)
	 ( Z32:_mode Any*:_indirect Any&:_drawcount Z32:_maxdrawcount Z32:_stride ) "libGL.so.1" "glMultiDrawArraysIndirectCountARB" sys optional MultiDrawArraysIndirectCountARB 

	// GLAPI void APIENTRY MultiDrawElementsIndirectCountARB (GLenum mode,GLenum type,const void * indirect,GLintptr drawcount,GLsizei maxdrawcount,GLsizei stride)
	 ( Z32:_mode Z32:_type Any*:_indirect Any&:_drawcount Z32:_maxdrawcount Z32:_stride ) "libGL.so.1" "glMultiDrawElementsIndirectCountARB" sys optional MultiDrawElementsIndirectCountARB 

	// GLAPI void APIENTRY VertexAttribDivisorARB (GLuint index,GLuint divisor)
	 ( Z32:_index Z32:_divisor ) "libGL.so.1" "glVertexAttribDivisorARB" sys optional VertexAttribDivisorARB 

	// GLAPI void APIENTRY MaxShaderCompilerThreadsARB (GLuint count)
	 ( Z32:_count ) "libGL.so.1" "glMaxShaderCompilerThreadsARB" sys optional MaxShaderCompilerThreadsARB 

	// GLAPI GLenum APIENTRY GetGraphicsResetStatusARB (void)
	 (  -> Z32 ) "libGL.so.1" "glGetGraphicsResetStatusARB" sys optional GetGraphicsResetStatusARB 

	// GLAPI void APIENTRY GetnTexImageARB (GLenum target,GLint level,GLenum format,GLenum type,GLsizei bufSize,void * img)
	 ( Z32:_target Z32:_level Z32:_format Z32:_type Z32:_bufSize Any*:_img ) "libGL.so.1" "glGetnTexImageARB" sys optional GetnTexImageARB 

	// GLAPI void APIENTRY ReadnPixelsARB (GLint x,GLint y,GLsizei width,GLsizei height,GLenum format,GLenum type,GLsizei bufSize,void * data)
	 ( Z32:_x Z32:_y Z32:_width Z32:_height Z32:_format Z32:_type Z32:_bufSize Any*:_data ) "libGL.so.1" "glReadnPixelsARB" sys optional ReadnPixelsARB 

	// GLAPI void APIENTRY GetnCompressedTexImageARB (GLenum target,GLint lod,GLsizei bufSize,void * img)
	 ( Z32:_target Z32:_lod Z32:_bufSize Any*:_img ) "libGL.so.1" "glGetnCompressedTexImageARB" sys optional GetnCompressedTexImageARB 

	// GLAPI void APIENTRY GetnUniformfvARB (GLuint program,GLint location,GLsizei bufSize,GLfloat * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Real*:_params ) "libGL.so.1" "glGetnUniformfvARB" sys optional GetnUniformfvARB 

	// GLAPI void APIENTRY GetnUniformivARB (GLuint program,GLint location,GLsizei bufSize,GLint * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Z32*:_params ) "libGL.so.1" "glGetnUniformivARB" sys optional GetnUniformivARB 

	// GLAPI void APIENTRY GetnUniformuivARB (GLuint program,GLint location,GLsizei bufSize,GLuint * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Z32*:_params ) "libGL.so.1" "glGetnUniformuivARB" sys optional GetnUniformuivARB 

	// GLAPI void APIENTRY GetnUniformdvARB (GLuint program,GLint location,GLsizei bufSize,GLdouble * params)
	 ( Z32:_program Z32:_location Z32:_bufSize Double32*:_params ) "libGL.so.1" "glGetnUniformdvARB" sys optional GetnUniformdvARB 

	// GLAPI void APIENTRY FramebufferSampleLocationsfvARB (GLenum target,GLuint start,GLsizei count,const GLfloat * v)
	 ( Z32:_target Z32:_start Z32:_count Real*:_v ) "libGL.so.1" "glFramebufferSampleLocationsfvARB" sys optional FramebufferSampleLocationsfvARB 

	// GLAPI void APIENTRY NamedFramebufferSampleLocationsfvARB (GLuint framebuffer,GLuint start,GLsizei count,const GLfloat * v)
	 ( Z32:_framebuffer Z32:_start Z32:_count Real*:_v ) "libGL.so.1" "glNamedFramebufferSampleLocationsfvARB" sys optional NamedFramebufferSampleLocationsfvARB 

	// GLAPI void APIENTRY EvaluateDepthValuesARB (void)
	 ( ) "libGL.so.1" "glEvaluateDepthValuesARB" sys optional EvaluateDepthValuesARB 

	// GLAPI void APIENTRY MinSampleShadingARB (GLfloat value)
	 ( Real:_value ) "libGL.so.1" "glMinSampleShadingARB" sys optional MinSampleShadingARB 

	// GLAPI void APIENTRY NamedStringARB (GLenum type,GLint namelen,const GLchar * name,GLint stringlen,const GLchar * string)
	 ( Z32:_type Z32:_namelen String:_name Z32:_stringlen String:_string ) "libGL.so.1" "glNamedStringARB" sys optional NamedStringARB 

	// GLAPI void APIENTRY DeleteNamedStringARB (GLint namelen,const GLchar * name)
	 ( Z32:_namelen String:_name ) "libGL.so.1" "glDeleteNamedStringARB" sys optional DeleteNamedStringARB 

	// GLAPI void APIENTRY CompileShaderIncludeARB (GLuint shader,GLsizei count,const GLchar * const * path,const GLint * length)
	 ( Z32:_shader Z32:_count String*:_path Z32*:_length ) "libGL.so.1" "glCompileShaderIncludeARB" sys optional CompileShaderIncludeARB 

	// GLAPI GLboolean APIENTRY IsNamedStringARB (GLint namelen,const GLchar * name)
	 ( Z32:_namelen String:_name  -> Bit ) "libGL.so.1" "glIsNamedStringARB" sys optional IsNamedStringARB 

	// GLAPI void APIENTRY GetNamedStringARB (GLint namelen,const GLchar * name,GLsizei bufSize,GLint * stringlen,GLchar * string)
	 ( Z32:_namelen String:_name Z32:_bufSize Z32*:_stringlen String:_string ) "libGL.so.1" "glGetNamedStringARB" sys optional GetNamedStringARB 

	// GLAPI void APIENTRY GetNamedStringivARB (GLint namelen,const GLchar * name,GLenum pname,GLint * params)
	 ( Z32:_namelen String:_name Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedStringivARB" sys optional GetNamedStringivARB 

	// GLAPI void APIENTRY BufferPageCommitmentARB (GLenum target,GLintptr offset,GLsizeiptr size,GLboolean commit)
	 ( Z32:_target Any&:_offset Z32:_size Bit:_commit ) "libGL.so.1" "glBufferPageCommitmentARB" sys optional BufferPageCommitmentARB 

	// GLAPI void APIENTRY NamedBufferPageCommitmentEXT (GLuint buffer,GLintptr offset,GLsizeiptr size,GLboolean commit)
	 ( Z32:_buffer Any&:_offset Z32:_size Bit:_commit ) "libGL.so.1" "glNamedBufferPageCommitmentEXT" sys optional NamedBufferPageCommitmentEXT 

	// GLAPI void APIENTRY NamedBufferPageCommitmentARB (GLuint buffer,GLintptr offset,GLsizeiptr size,GLboolean commit)
	 ( Z32:_buffer Any&:_offset Z32:_size Bit:_commit ) "libGL.so.1" "glNamedBufferPageCommitmentARB" sys optional NamedBufferPageCommitmentARB 

	// GLAPI void APIENTRY TexPageCommitmentARB (GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLboolean commit)
	 ( Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Bit:_commit ) "libGL.so.1" "glTexPageCommitmentARB" sys optional TexPageCommitmentARB 

	// GLAPI void APIENTRY TexBufferARB (GLenum target,GLenum internalformat,GLuint buffer)
	 ( Z32:_target Z32:_internalformat Z32:_buffer ) "libGL.so.1" "glTexBufferARB" sys optional TexBufferARB 

	// GLAPI void APIENTRY DepthRangeArraydvNV (GLuint first,GLsizei count,const GLdouble * v)
	 ( Z32:_first Z32:_count Double32*:_v ) "libGL.so.1" "glDepthRangeArraydvNV" sys optional DepthRangeArraydvNV 

	// GLAPI void APIENTRY DepthRangeIndexeddNV (GLuint index,GLdouble n,GLdouble f)
	 ( Z32:_index Double32:_n Double32:_f ) "libGL.so.1" "glDepthRangeIndexeddNV" sys optional DepthRangeIndexeddNV 

	// GLAPI void APIENTRY BlendBarrierKHR (void)
	 ( ) "libGL.so.1" "glBlendBarrierKHR" sys optional BlendBarrierKHR 

	// GLAPI void APIENTRY MaxShaderCompilerThreadsKHR (GLuint count)
	 ( Z32:_count ) "libGL.so.1" "glMaxShaderCompilerThreadsKHR" sys optional MaxShaderCompilerThreadsKHR 

	// GLAPI void APIENTRY RenderbufferStorageMultisampleAdvancedAMD (GLenum target,GLsizei samples,GLsizei storageSamples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_samples Z32:_storageSamples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glRenderbufferStorageMultisampleAdvancedAMD" sys optional RenderbufferStorageMultisampleAdvancedAMD 

	// GLAPI void APIENTRY NamedRenderbufferStorageMultisampleAdvancedAMD (GLuint renderbuffer,GLsizei samples,GLsizei storageSamples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_renderbuffer Z32:_samples Z32:_storageSamples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glNamedRenderbufferStorageMultisampleAdvancedAMD" sys optional NamedRenderbufferStorageMultisampleAdvancedAMD 

	// GLAPI void APIENTRY GetPerfMonitorGroupsAMD (GLint * numGroups,GLsizei groupsSize,GLuint * groups)
	 ( Z32*:_numGroups Z32:_groupsSize Z32*:_groups ) "libGL.so.1" "glGetPerfMonitorGroupsAMD" sys optional GetPerfMonitorGroupsAMD 

	// GLAPI void APIENTRY GetPerfMonitorCountersAMD (GLuint group,GLint * numCounters,GLint * maxActiveCounters,GLsizei counterSize,GLuint * counters)
	 ( Z32:_group Z32*:_numCounters Z32*:_maxActiveCounters Z32:_counterSize Z32*:_counters ) "libGL.so.1" "glGetPerfMonitorCountersAMD" sys optional GetPerfMonitorCountersAMD 

	// GLAPI void APIENTRY GetPerfMonitorGroupStringAMD (GLuint group,GLsizei bufSize,GLsizei * length,GLchar * groupString)
	 ( Z32:_group Z32:_bufSize Z32*:_length String:_groupString ) "libGL.so.1" "glGetPerfMonitorGroupStringAMD" sys optional GetPerfMonitorGroupStringAMD 

	// GLAPI void APIENTRY GetPerfMonitorCounterStringAMD (GLuint group,GLuint counter,GLsizei bufSize,GLsizei * length,GLchar * counterString)
	 ( Z32:_group Z32:_counter Z32:_bufSize Z32*:_length String:_counterString ) "libGL.so.1" "glGetPerfMonitorCounterStringAMD" sys optional GetPerfMonitorCounterStringAMD 

	// GLAPI void APIENTRY GetPerfMonitorCounterInfoAMD (GLuint group,GLuint counter,GLenum pname,void * data)
	 ( Z32:_group Z32:_counter Z32:_pname Any*:_data ) "libGL.so.1" "glGetPerfMonitorCounterInfoAMD" sys optional GetPerfMonitorCounterInfoAMD 

	// GLAPI void APIENTRY GenPerfMonitorsAMD (GLsizei n,GLuint * monitors)
	 ( Z32:_n Z32*:_monitors ) "libGL.so.1" "glGenPerfMonitorsAMD" sys optional GenPerfMonitorsAMD 

	// GLAPI void APIENTRY DeletePerfMonitorsAMD (GLsizei n,GLuint * monitors)
	 ( Z32:_n Z32*:_monitors ) "libGL.so.1" "glDeletePerfMonitorsAMD" sys optional DeletePerfMonitorsAMD 

	// GLAPI void APIENTRY SelectPerfMonitorCountersAMD (GLuint monitor,GLboolean enable,GLuint group,GLint numCounters,GLuint * counterList)
	 ( Z32:_monitor Bit:_enable Z32:_group Z32:_numCounters Z32*:_counterList ) "libGL.so.1" "glSelectPerfMonitorCountersAMD" sys optional SelectPerfMonitorCountersAMD 

	// GLAPI void APIENTRY BeginPerfMonitorAMD (GLuint monitor)
	 ( Z32:_monitor ) "libGL.so.1" "glBeginPerfMonitorAMD" sys optional BeginPerfMonitorAMD 

	// GLAPI void APIENTRY EndPerfMonitorAMD (GLuint monitor)
	 ( Z32:_monitor ) "libGL.so.1" "glEndPerfMonitorAMD" sys optional EndPerfMonitorAMD 

	// GLAPI void APIENTRY GetPerfMonitorCounterDataAMD (GLuint monitor,GLenum pname,GLsizei dataSize,GLuint * data,GLint * bytesWritten)
	 ( Z32:_monitor Z32:_pname Z32:_dataSize Z32*:_data Z32*:_bytesWritten ) "libGL.so.1" "glGetPerfMonitorCounterDataAMD" sys optional GetPerfMonitorCounterDataAMD 

	// GLAPI void APIENTRY EGLImageTargetTexStorageEXT (GLenum target,GLeglImageOES image,const GLint * attrib_list)
	// GLAPI void APIENTRY EGLImageTargetTextureStorageEXT (GLuint texture,GLeglImageOES image,const GLint * attrib_list)
	// GLAPI void APIENTRY LabelObjectEXT (GLenum type,GLuint object,GLsizei length,const GLchar * label)
	 ( Z32:_type Z32:_object Z32:_length String:_label ) "libGL.so.1" "glLabelObjectEXT" sys optional LabelObjectEXT 

	// GLAPI void APIENTRY GetObjectLabelEXT (GLenum type,GLuint object,GLsizei bufSize,GLsizei * length,GLchar * label)
	 ( Z32:_type Z32:_object Z32:_bufSize Z32*:_length String:_label ) "libGL.so.1" "glGetObjectLabelEXT" sys optional GetObjectLabelEXT 

	// GLAPI void APIENTRY InsertEventMarkerEXT (GLsizei length,const GLchar * marker)
	 ( Z32:_length String:_marker ) "libGL.so.1" "glInsertEventMarkerEXT" sys optional InsertEventMarkerEXT 

	// GLAPI void APIENTRY PushGroupMarkerEXT (GLsizei length,const GLchar * marker)
	 ( Z32:_length String:_marker ) "libGL.so.1" "glPushGroupMarkerEXT" sys optional PushGroupMarkerEXT 

	// GLAPI void APIENTRY PopGroupMarkerEXT (void)
	 ( ) "libGL.so.1" "glPopGroupMarkerEXT" sys optional PopGroupMarkerEXT 

	// GLAPI void APIENTRY MatrixLoadfEXT (GLenum mode,const GLfloat * m)
	 ( Z32:_mode Real*:_m ) "libGL.so.1" "glMatrixLoadfEXT" sys optional MatrixLoadfEXT 

	// GLAPI void APIENTRY MatrixLoaddEXT (GLenum mode,const GLdouble * m)
	 ( Z32:_mode Double32*:_m ) "libGL.so.1" "glMatrixLoaddEXT" sys optional MatrixLoaddEXT 

	// GLAPI void APIENTRY MatrixMultfEXT (GLenum mode,const GLfloat * m)
	 ( Z32:_mode Real*:_m ) "libGL.so.1" "glMatrixMultfEXT" sys optional MatrixMultfEXT 

	// GLAPI void APIENTRY MatrixMultdEXT (GLenum mode,const GLdouble * m)
	 ( Z32:_mode Double32*:_m ) "libGL.so.1" "glMatrixMultdEXT" sys optional MatrixMultdEXT 

	// GLAPI void APIENTRY MatrixLoadIdentityEXT (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glMatrixLoadIdentityEXT" sys optional MatrixLoadIdentityEXT 

	// GLAPI void APIENTRY MatrixRotatefEXT (GLenum mode,GLfloat angle,GLfloat x,GLfloat y,GLfloat z)
	 ( Z32:_mode Real:_angle Real:_x Real:_y Real:_z ) "libGL.so.1" "glMatrixRotatefEXT" sys optional MatrixRotatefEXT 

	// GLAPI void APIENTRY MatrixRotatedEXT (GLenum mode,GLdouble angle,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_mode Double32:_angle Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glMatrixRotatedEXT" sys optional MatrixRotatedEXT 

	// GLAPI void APIENTRY MatrixScalefEXT (GLenum mode,GLfloat x,GLfloat y,GLfloat z)
	 ( Z32:_mode Real:_x Real:_y Real:_z ) "libGL.so.1" "glMatrixScalefEXT" sys optional MatrixScalefEXT 

	// GLAPI void APIENTRY MatrixScaledEXT (GLenum mode,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_mode Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glMatrixScaledEXT" sys optional MatrixScaledEXT 

	// GLAPI void APIENTRY MatrixTranslatefEXT (GLenum mode,GLfloat x,GLfloat y,GLfloat z)
	 ( Z32:_mode Real:_x Real:_y Real:_z ) "libGL.so.1" "glMatrixTranslatefEXT" sys optional MatrixTranslatefEXT 

	// GLAPI void APIENTRY MatrixTranslatedEXT (GLenum mode,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_mode Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glMatrixTranslatedEXT" sys optional MatrixTranslatedEXT 

	// GLAPI void APIENTRY MatrixFrustumEXT (GLenum mode,GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
	 ( Z32:_mode Double32:_left Double32:_right Double32:_bottom Double32:_top Double32:_zNear Double32:_zFar ) "libGL.so.1" "glMatrixFrustumEXT" sys optional MatrixFrustumEXT 

	// GLAPI void APIENTRY MatrixOrthoEXT (GLenum mode,GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
	 ( Z32:_mode Double32:_left Double32:_right Double32:_bottom Double32:_top Double32:_zNear Double32:_zFar ) "libGL.so.1" "glMatrixOrthoEXT" sys optional MatrixOrthoEXT 

	// GLAPI void APIENTRY MatrixPopEXT (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glMatrixPopEXT" sys optional MatrixPopEXT 

	// GLAPI void APIENTRY MatrixPushEXT (GLenum mode)
	 ( Z32:_mode ) "libGL.so.1" "glMatrixPushEXT" sys optional MatrixPushEXT 

	// GLAPI void APIENTRY ClientAttribDefaultEXT (GLbitfield mask)
	 ( Z32:_mask ) "libGL.so.1" "glClientAttribDefaultEXT" sys optional ClientAttribDefaultEXT 

	// GLAPI void APIENTRY PushClientAttribDefaultEXT (GLbitfield mask)
	 ( Z32:_mask ) "libGL.so.1" "glPushClientAttribDefaultEXT" sys optional PushClientAttribDefaultEXT 

	// GLAPI void APIENTRY TextureParameterfEXT (GLuint texture,GLenum target,GLenum pname,GLfloat param)
	 ( Z32:_texture Z32:_target Z32:_pname Real:_param ) "libGL.so.1" "glTextureParameterfEXT" sys optional TextureParameterfEXT 

	// GLAPI void APIENTRY TextureParameterfvEXT (GLuint texture,GLenum target,GLenum pname,const GLfloat * params)
	 ( Z32:_texture Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glTextureParameterfvEXT" sys optional TextureParameterfvEXT 

	// GLAPI void APIENTRY TextureParameteriEXT (GLuint texture,GLenum target,GLenum pname,GLint param)
	 ( Z32:_texture Z32:_target Z32:_pname Z32:_param ) "libGL.so.1" "glTextureParameteriEXT" sys optional TextureParameteriEXT 

	// GLAPI void APIENTRY TextureParameterivEXT (GLuint texture,GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_texture Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glTextureParameterivEXT" sys optional TextureParameterivEXT 

	// GLAPI void APIENTRY TextureImage1DEXT (GLuint texture,GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureImage1DEXT" sys optional TextureImage1DEXT 

	// GLAPI void APIENTRY TextureImage2DEXT (GLuint texture,GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureImage2DEXT" sys optional TextureImage2DEXT 

	// GLAPI void APIENTRY TextureSubImage1DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureSubImage1DEXT" sys optional TextureSubImage1DEXT 

	// GLAPI void APIENTRY TextureSubImage2DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureSubImage2DEXT" sys optional TextureSubImage2DEXT 

	// GLAPI void APIENTRY CopyTextureImage1DEXT (GLuint texture,GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_x Z32:_y Z32:_width Z32:_border ) "libGL.so.1" "glCopyTextureImage1DEXT" sys optional CopyTextureImage1DEXT 

	// GLAPI void APIENTRY CopyTextureImage2DEXT (GLuint texture,GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_x Z32:_y Z32:_width Z32:_height Z32:_border ) "libGL.so.1" "glCopyTextureImage2DEXT" sys optional CopyTextureImage2DEXT 

	// GLAPI void APIENTRY CopyTextureSubImage1DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_x Z32:_y Z32:_width ) "libGL.so.1" "glCopyTextureSubImage1DEXT" sys optional CopyTextureSubImage1DEXT 

	// GLAPI void APIENTRY CopyTextureSubImage2DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyTextureSubImage2DEXT" sys optional CopyTextureSubImage2DEXT 

	// GLAPI void APIENTRY GetTextureImageEXT (GLuint texture,GLenum target,GLint level,GLenum format,GLenum type,void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glGetTextureImageEXT" sys optional GetTextureImageEXT 

	// GLAPI void APIENTRY GetTextureParameterfvEXT (GLuint texture,GLenum target,GLenum pname,GLfloat * params)
	 ( Z32:_texture Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glGetTextureParameterfvEXT" sys optional GetTextureParameterfvEXT 

	// GLAPI void APIENTRY GetTextureParameterivEXT (GLuint texture,GLenum target,GLenum pname,GLint * params)
	 ( Z32:_texture Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureParameterivEXT" sys optional GetTextureParameterivEXT 

	// GLAPI void APIENTRY GetTextureLevelParameterfvEXT (GLuint texture,GLenum target,GLint level,GLenum pname,GLfloat * params)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_pname Real*:_params ) "libGL.so.1" "glGetTextureLevelParameterfvEXT" sys optional GetTextureLevelParameterfvEXT 

	// GLAPI void APIENTRY GetTextureLevelParameterivEXT (GLuint texture,GLenum target,GLint level,GLenum pname,GLint * params)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureLevelParameterivEXT" sys optional GetTextureLevelParameterivEXT 

	// GLAPI void APIENTRY TextureImage3DEXT (GLuint texture,GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_depth Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureImage3DEXT" sys optional TextureImage3DEXT 

	// GLAPI void APIENTRY TextureSubImage3DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glTextureSubImage3DEXT" sys optional TextureSubImage3DEXT 

	// GLAPI void APIENTRY CopyTextureSubImage3DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyTextureSubImage3DEXT" sys optional CopyTextureSubImage3DEXT 

	// GLAPI void APIENTRY BindMultiTextureEXT (GLenum texunit,GLenum target,GLuint texture)
	 ( Z32:_texunit Z32:_target Z32:_texture ) "libGL.so.1" "glBindMultiTextureEXT" sys optional BindMultiTextureEXT 

	// GLAPI void APIENTRY MultiTexCoordPointerEXT (GLenum texunit,GLint size,GLenum type,GLsizei stride,const void * pointer)
	 ( Z32:_texunit Z32:_size Z32:_type Z32:_stride Any*:_pointer ) "libGL.so.1" "glMultiTexCoordPointerEXT" sys optional MultiTexCoordPointerEXT 

	// GLAPI void APIENTRY MultiTexEnvfEXT (GLenum texunit,GLenum target,GLenum pname,GLfloat param)
	 ( Z32:_texunit Z32:_target Z32:_pname Real:_param ) "libGL.so.1" "glMultiTexEnvfEXT" sys optional MultiTexEnvfEXT 

	// GLAPI void APIENTRY MultiTexEnvfvEXT (GLenum texunit,GLenum target,GLenum pname,const GLfloat * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glMultiTexEnvfvEXT" sys optional MultiTexEnvfvEXT 

	// GLAPI void APIENTRY MultiTexEnviEXT (GLenum texunit,GLenum target,GLenum pname,GLint param)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32:_param ) "libGL.so.1" "glMultiTexEnviEXT" sys optional MultiTexEnviEXT 

	// GLAPI void APIENTRY MultiTexEnvivEXT (GLenum texunit,GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glMultiTexEnvivEXT" sys optional MultiTexEnvivEXT 

	// GLAPI void APIENTRY MultiTexGendEXT (GLenum texunit,GLenum coord,GLenum pname,GLdouble param)
	 ( Z32:_texunit Z32:_coord Z32:_pname Double32:_param ) "libGL.so.1" "glMultiTexGendEXT" sys optional MultiTexGendEXT 

	// GLAPI void APIENTRY MultiTexGendvEXT (GLenum texunit,GLenum coord,GLenum pname,const GLdouble * params)
	 ( Z32:_texunit Z32:_coord Z32:_pname Double32*:_params ) "libGL.so.1" "glMultiTexGendvEXT" sys optional MultiTexGendvEXT 

	// GLAPI void APIENTRY MultiTexGenfEXT (GLenum texunit,GLenum coord,GLenum pname,GLfloat param)
	 ( Z32:_texunit Z32:_coord Z32:_pname Real:_param ) "libGL.so.1" "glMultiTexGenfEXT" sys optional MultiTexGenfEXT 

	// GLAPI void APIENTRY MultiTexGenfvEXT (GLenum texunit,GLenum coord,GLenum pname,const GLfloat * params)
	 ( Z32:_texunit Z32:_coord Z32:_pname Real*:_params ) "libGL.so.1" "glMultiTexGenfvEXT" sys optional MultiTexGenfvEXT 

	// GLAPI void APIENTRY MultiTexGeniEXT (GLenum texunit,GLenum coord,GLenum pname,GLint param)
	 ( Z32:_texunit Z32:_coord Z32:_pname Z32:_param ) "libGL.so.1" "glMultiTexGeniEXT" sys optional MultiTexGeniEXT 

	// GLAPI void APIENTRY MultiTexGenivEXT (GLenum texunit,GLenum coord,GLenum pname,const GLint * params)
	 ( Z32:_texunit Z32:_coord Z32:_pname Z32*:_params ) "libGL.so.1" "glMultiTexGenivEXT" sys optional MultiTexGenivEXT 

	// GLAPI void APIENTRY GetMultiTexEnvfvEXT (GLenum texunit,GLenum target,GLenum pname,GLfloat * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glGetMultiTexEnvfvEXT" sys optional GetMultiTexEnvfvEXT 

	// GLAPI void APIENTRY GetMultiTexEnvivEXT (GLenum texunit,GLenum target,GLenum pname,GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetMultiTexEnvivEXT" sys optional GetMultiTexEnvivEXT 

	// GLAPI void APIENTRY GetMultiTexGendvEXT (GLenum texunit,GLenum coord,GLenum pname,GLdouble * params)
	 ( Z32:_texunit Z32:_coord Z32:_pname Double32*:_params ) "libGL.so.1" "glGetMultiTexGendvEXT" sys optional GetMultiTexGendvEXT 

	// GLAPI void APIENTRY GetMultiTexGenfvEXT (GLenum texunit,GLenum coord,GLenum pname,GLfloat * params)
	 ( Z32:_texunit Z32:_coord Z32:_pname Real*:_params ) "libGL.so.1" "glGetMultiTexGenfvEXT" sys optional GetMultiTexGenfvEXT 

	// GLAPI void APIENTRY GetMultiTexGenivEXT (GLenum texunit,GLenum coord,GLenum pname,GLint * params)
	 ( Z32:_texunit Z32:_coord Z32:_pname Z32*:_params ) "libGL.so.1" "glGetMultiTexGenivEXT" sys optional GetMultiTexGenivEXT 

	// GLAPI void APIENTRY MultiTexParameteriEXT (GLenum texunit,GLenum target,GLenum pname,GLint param)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32:_param ) "libGL.so.1" "glMultiTexParameteriEXT" sys optional MultiTexParameteriEXT 

	// GLAPI void APIENTRY MultiTexParameterivEXT (GLenum texunit,GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glMultiTexParameterivEXT" sys optional MultiTexParameterivEXT 

	// GLAPI void APIENTRY MultiTexParameterfEXT (GLenum texunit,GLenum target,GLenum pname,GLfloat param)
	 ( Z32:_texunit Z32:_target Z32:_pname Real:_param ) "libGL.so.1" "glMultiTexParameterfEXT" sys optional MultiTexParameterfEXT 

	// GLAPI void APIENTRY MultiTexParameterfvEXT (GLenum texunit,GLenum target,GLenum pname,const GLfloat * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glMultiTexParameterfvEXT" sys optional MultiTexParameterfvEXT 

	// GLAPI void APIENTRY MultiTexImage1DEXT (GLenum texunit,GLenum target,GLint level,GLint internalformat,GLsizei width,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glMultiTexImage1DEXT" sys optional MultiTexImage1DEXT 

	// GLAPI void APIENTRY MultiTexImage2DEXT (GLenum texunit,GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glMultiTexImage2DEXT" sys optional MultiTexImage2DEXT 

	// GLAPI void APIENTRY MultiTexSubImage1DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glMultiTexSubImage1DEXT" sys optional MultiTexSubImage1DEXT 

	// GLAPI void APIENTRY MultiTexSubImage2DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glMultiTexSubImage2DEXT" sys optional MultiTexSubImage2DEXT 

	// GLAPI void APIENTRY CopyMultiTexImage1DEXT (GLenum texunit,GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLint border)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_x Z32:_y Z32:_width Z32:_border ) "libGL.so.1" "glCopyMultiTexImage1DEXT" sys optional CopyMultiTexImage1DEXT 

	// GLAPI void APIENTRY CopyMultiTexImage2DEXT (GLenum texunit,GLenum target,GLint level,GLenum internalformat,GLint x,GLint y,GLsizei width,GLsizei height,GLint border)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_x Z32:_y Z32:_width Z32:_height Z32:_border ) "libGL.so.1" "glCopyMultiTexImage2DEXT" sys optional CopyMultiTexImage2DEXT 

	// GLAPI void APIENTRY CopyMultiTexSubImage1DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint x,GLint y,GLsizei width)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_x Z32:_y Z32:_width ) "libGL.so.1" "glCopyMultiTexSubImage1DEXT" sys optional CopyMultiTexSubImage1DEXT 

	// GLAPI void APIENTRY CopyMultiTexSubImage2DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyMultiTexSubImage2DEXT" sys optional CopyMultiTexSubImage2DEXT 

	// GLAPI void APIENTRY GetMultiTexImageEXT (GLenum texunit,GLenum target,GLint level,GLenum format,GLenum type,void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glGetMultiTexImageEXT" sys optional GetMultiTexImageEXT 

	// GLAPI void APIENTRY GetMultiTexParameterfvEXT (GLenum texunit,GLenum target,GLenum pname,GLfloat * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Real*:_params ) "libGL.so.1" "glGetMultiTexParameterfvEXT" sys optional GetMultiTexParameterfvEXT 

	// GLAPI void APIENTRY GetMultiTexParameterivEXT (GLenum texunit,GLenum target,GLenum pname,GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetMultiTexParameterivEXT" sys optional GetMultiTexParameterivEXT 

	// GLAPI void APIENTRY GetMultiTexLevelParameterfvEXT (GLenum texunit,GLenum target,GLint level,GLenum pname,GLfloat * params)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_pname Real*:_params ) "libGL.so.1" "glGetMultiTexLevelParameterfvEXT" sys optional GetMultiTexLevelParameterfvEXT 

	// GLAPI void APIENTRY GetMultiTexLevelParameterivEXT (GLenum texunit,GLenum target,GLint level,GLenum pname,GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_pname Z32*:_params ) "libGL.so.1" "glGetMultiTexLevelParameterivEXT" sys optional GetMultiTexLevelParameterivEXT 

	// GLAPI void APIENTRY MultiTexImage3DEXT (GLenum texunit,GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_depth Z32:_border Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glMultiTexImage3DEXT" sys optional MultiTexImage3DEXT 

	// GLAPI void APIENTRY MultiTexSubImage3DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLenum type,const void * pixels)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_type Any*:_pixels ) "libGL.so.1" "glMultiTexSubImage3DEXT" sys optional MultiTexSubImage3DEXT 

	// GLAPI void APIENTRY CopyMultiTexSubImage3DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glCopyMultiTexSubImage3DEXT" sys optional CopyMultiTexSubImage3DEXT 

	// GLAPI void APIENTRY EnableClientStateIndexedEXT (GLenum array,GLuint index)
	 ( Z32:_array Z32:_index ) "libGL.so.1" "glEnableClientStateIndexedEXT" sys optional EnableClientStateIndexedEXT 

	// GLAPI void APIENTRY DisableClientStateIndexedEXT (GLenum array,GLuint index)
	 ( Z32:_array Z32:_index ) "libGL.so.1" "glDisableClientStateIndexedEXT" sys optional DisableClientStateIndexedEXT 

	// GLAPI void APIENTRY GetFloatIndexedvEXT (GLenum target,GLuint index,GLfloat * data)
	 ( Z32:_target Z32:_index Real*:_data ) "libGL.so.1" "glGetFloatIndexedvEXT" sys optional GetFloatIndexedvEXT 

	// GLAPI void APIENTRY GetDoubleIndexedvEXT (GLenum target,GLuint index,GLdouble * data)
	 ( Z32:_target Z32:_index Double32*:_data ) "libGL.so.1" "glGetDoubleIndexedvEXT" sys optional GetDoubleIndexedvEXT 

	// GLAPI void APIENTRY GetPointerIndexedvEXT (GLenum target,GLuint index,void ** data)
	 ( Z32:_target Z32:_index Any**:_data ) "libGL.so.1" "glGetPointerIndexedvEXT" sys optional GetPointerIndexedvEXT 

	// GLAPI void APIENTRY EnableIndexedEXT (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index ) "libGL.so.1" "glEnableIndexedEXT" sys optional EnableIndexedEXT 

	// GLAPI void APIENTRY DisableIndexedEXT (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index ) "libGL.so.1" "glDisableIndexedEXT" sys optional DisableIndexedEXT 

	// GLAPI GLboolean APIENTRY IsEnabledIndexedEXT (GLenum target,GLuint index)
	 ( Z32:_target Z32:_index  -> Bit ) "libGL.so.1" "glIsEnabledIndexedEXT" sys optional IsEnabledIndexedEXT 

	// GLAPI void APIENTRY GetIntegerIndexedvEXT (GLenum target,GLuint index,GLint * data)
	 ( Z32:_target Z32:_index Z32*:_data ) "libGL.so.1" "glGetIntegerIndexedvEXT" sys optional GetIntegerIndexedvEXT 

	// GLAPI void APIENTRY GetBooleanIndexedvEXT (GLenum target,GLuint index,GLboolean * data)
	 ( Z32:_target Z32:_index Bit*:_data ) "libGL.so.1" "glGetBooleanIndexedvEXT" sys optional GetBooleanIndexedvEXT 

	// GLAPI void APIENTRY CompressedTextureImage3DEXT (GLuint texture,GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const void * bits)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_depth Z32:_border Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedTextureImage3DEXT" sys optional CompressedTextureImage3DEXT 

	// GLAPI void APIENTRY CompressedTextureImage2DEXT (GLuint texture,GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const void * bits)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_border Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedTextureImage2DEXT" sys optional CompressedTextureImage2DEXT 

	// GLAPI void APIENTRY CompressedTextureImage1DEXT (GLuint texture,GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const void * bits)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_border Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedTextureImage1DEXT" sys optional CompressedTextureImage1DEXT 

	// GLAPI void APIENTRY CompressedTextureSubImage3DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const void * bits)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedTextureSubImage3DEXT" sys optional CompressedTextureSubImage3DEXT 

	// GLAPI void APIENTRY CompressedTextureSubImage2DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const void * bits)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedTextureSubImage2DEXT" sys optional CompressedTextureSubImage2DEXT 

	// GLAPI void APIENTRY CompressedTextureSubImage1DEXT (GLuint texture,GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const void * bits)
	 ( Z32:_texture Z32:_target Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedTextureSubImage1DEXT" sys optional CompressedTextureSubImage1DEXT 

	// GLAPI void APIENTRY GetCompressedTextureImageEXT (GLuint texture,GLenum target,GLint lod,void * img)
	 ( Z32:_texture Z32:_target Z32:_lod Any*:_img ) "libGL.so.1" "glGetCompressedTextureImageEXT" sys optional GetCompressedTextureImageEXT 

	// GLAPI void APIENTRY CompressedMultiTexImage3DEXT (GLenum texunit,GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLint border,GLsizei imageSize,const void * bits)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_depth Z32:_border Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedMultiTexImage3DEXT" sys optional CompressedMultiTexImage3DEXT 

	// GLAPI void APIENTRY CompressedMultiTexImage2DEXT (GLenum texunit,GLenum target,GLint level,GLenum internalformat,GLsizei width,GLsizei height,GLint border,GLsizei imageSize,const void * bits)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_height Z32:_border Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedMultiTexImage2DEXT" sys optional CompressedMultiTexImage2DEXT 

	// GLAPI void APIENTRY CompressedMultiTexImage1DEXT (GLenum texunit,GLenum target,GLint level,GLenum internalformat,GLsizei width,GLint border,GLsizei imageSize,const void * bits)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_internalformat Z32:_width Z32:_border Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedMultiTexImage1DEXT" sys optional CompressedMultiTexImage1DEXT 

	// GLAPI void APIENTRY CompressedMultiTexSubImage3DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLenum format,GLsizei imageSize,const void * bits)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_format Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedMultiTexSubImage3DEXT" sys optional CompressedMultiTexSubImage3DEXT 

	// GLAPI void APIENTRY CompressedMultiTexSubImage2DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLint yoffset,GLsizei width,GLsizei height,GLenum format,GLsizei imageSize,const void * bits)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_yoffset Z32:_width Z32:_height Z32:_format Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedMultiTexSubImage2DEXT" sys optional CompressedMultiTexSubImage2DEXT 

	// GLAPI void APIENTRY CompressedMultiTexSubImage1DEXT (GLenum texunit,GLenum target,GLint level,GLint xoffset,GLsizei width,GLenum format,GLsizei imageSize,const void * bits)
	 ( Z32:_texunit Z32:_target Z32:_level Z32:_xoffset Z32:_width Z32:_format Z32:_imageSize Any*:_bits ) "libGL.so.1" "glCompressedMultiTexSubImage1DEXT" sys optional CompressedMultiTexSubImage1DEXT 

	// GLAPI void APIENTRY GetCompressedMultiTexImageEXT (GLenum texunit,GLenum target,GLint lod,void * img)
	 ( Z32:_texunit Z32:_target Z32:_lod Any*:_img ) "libGL.so.1" "glGetCompressedMultiTexImageEXT" sys optional GetCompressedMultiTexImageEXT 

	// GLAPI void APIENTRY MatrixLoadTransposefEXT (GLenum mode,const GLfloat * m)
	 ( Z32:_mode Real*:_m ) "libGL.so.1" "glMatrixLoadTransposefEXT" sys optional MatrixLoadTransposefEXT 

	// GLAPI void APIENTRY MatrixLoadTransposedEXT (GLenum mode,const GLdouble * m)
	 ( Z32:_mode Double32*:_m ) "libGL.so.1" "glMatrixLoadTransposedEXT" sys optional MatrixLoadTransposedEXT 

	// GLAPI void APIENTRY MatrixMultTransposefEXT (GLenum mode,const GLfloat * m)
	 ( Z32:_mode Real*:_m ) "libGL.so.1" "glMatrixMultTransposefEXT" sys optional MatrixMultTransposefEXT 

	// GLAPI void APIENTRY MatrixMultTransposedEXT (GLenum mode,const GLdouble * m)
	 ( Z32:_mode Double32*:_m ) "libGL.so.1" "glMatrixMultTransposedEXT" sys optional MatrixMultTransposedEXT 

	// GLAPI void APIENTRY NamedBufferDataEXT (GLuint buffer,GLsizeiptr size,const void * data,GLenum usage)
	 ( Z32:_buffer Z32:_size Any*:_data Z32:_usage ) "libGL.so.1" "glNamedBufferDataEXT" sys optional NamedBufferDataEXT 

	// GLAPI void APIENTRY NamedBufferSubDataEXT (GLuint buffer,GLintptr offset,GLsizeiptr size,const void * data)
	 ( Z32:_buffer Any&:_offset Z32:_size Any*:_data ) "libGL.so.1" "glNamedBufferSubDataEXT" sys optional NamedBufferSubDataEXT 

	// GLAPI void * APIENTRY MapNamedBufferEXT (GLuint buffer,GLenum access)
	 ( Z32:_buffer Z32:_access  -> Any* ) "libGL.so.1" "glMapNamedBufferEXT" sys optional MapNamedBufferEXT 

	// GLAPI GLboolean APIENTRY UnmapNamedBufferEXT (GLuint buffer)
	 ( Z32:_buffer  -> Bit ) "libGL.so.1" "glUnmapNamedBufferEXT" sys optional UnmapNamedBufferEXT 

	// GLAPI void APIENTRY GetNamedBufferParameterivEXT (GLuint buffer,GLenum pname,GLint * params)
	 ( Z32:_buffer Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedBufferParameterivEXT" sys optional GetNamedBufferParameterivEXT 

	// GLAPI void APIENTRY GetNamedBufferPointervEXT (GLuint buffer,GLenum pname,void ** params)
	 ( Z32:_buffer Z32:_pname Any**:_params ) "libGL.so.1" "glGetNamedBufferPointervEXT" sys optional GetNamedBufferPointervEXT 

	// GLAPI void APIENTRY GetNamedBufferSubDataEXT (GLuint buffer,GLintptr offset,GLsizeiptr size,void * data)
	 ( Z32:_buffer Any&:_offset Z32:_size Any*:_data ) "libGL.so.1" "glGetNamedBufferSubDataEXT" sys optional GetNamedBufferSubDataEXT 

	// GLAPI void APIENTRY ProgramUniform1fEXT (GLuint program,GLint location,GLfloat v0)
	 ( Z32:_program Z32:_location Real:_v0 ) "libGL.so.1" "glProgramUniform1fEXT" sys optional ProgramUniform1fEXT 

	// GLAPI void APIENTRY ProgramUniform2fEXT (GLuint program,GLint location,GLfloat v0,GLfloat v1)
	 ( Z32:_program Z32:_location Real:_v0 Real:_v1 ) "libGL.so.1" "glProgramUniform2fEXT" sys optional ProgramUniform2fEXT 

	// GLAPI void APIENTRY ProgramUniform3fEXT (GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2)
	 ( Z32:_program Z32:_location Real:_v0 Real:_v1 Real:_v2 ) "libGL.so.1" "glProgramUniform3fEXT" sys optional ProgramUniform3fEXT 

	// GLAPI void APIENTRY ProgramUniform4fEXT (GLuint program,GLint location,GLfloat v0,GLfloat v1,GLfloat v2,GLfloat v3)
	 ( Z32:_program Z32:_location Real:_v0 Real:_v1 Real:_v2 Real:_v3 ) "libGL.so.1" "glProgramUniform4fEXT" sys optional ProgramUniform4fEXT 

	// GLAPI void APIENTRY ProgramUniform1iEXT (GLuint program,GLint location,GLint v0)
	 ( Z32:_program Z32:_location Z32:_v0 ) "libGL.so.1" "glProgramUniform1iEXT" sys optional ProgramUniform1iEXT 

	// GLAPI void APIENTRY ProgramUniform2iEXT (GLuint program,GLint location,GLint v0,GLint v1)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 ) "libGL.so.1" "glProgramUniform2iEXT" sys optional ProgramUniform2iEXT 

	// GLAPI void APIENTRY ProgramUniform3iEXT (GLuint program,GLint location,GLint v0,GLint v1,GLint v2)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 ) "libGL.so.1" "glProgramUniform3iEXT" sys optional ProgramUniform3iEXT 

	// GLAPI void APIENTRY ProgramUniform4iEXT (GLuint program,GLint location,GLint v0,GLint v1,GLint v2,GLint v3)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 Z32:_v3 ) "libGL.so.1" "glProgramUniform4iEXT" sys optional ProgramUniform4iEXT 

	// GLAPI void APIENTRY ProgramUniform1fvEXT (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform1fvEXT" sys optional ProgramUniform1fvEXT 

	// GLAPI void APIENTRY ProgramUniform2fvEXT (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform2fvEXT" sys optional ProgramUniform2fvEXT 

	// GLAPI void APIENTRY ProgramUniform3fvEXT (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform3fvEXT" sys optional ProgramUniform3fvEXT 

	// GLAPI void APIENTRY ProgramUniform4fvEXT (GLuint program,GLint location,GLsizei count,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Real*:_value ) "libGL.so.1" "glProgramUniform4fvEXT" sys optional ProgramUniform4fvEXT 

	// GLAPI void APIENTRY ProgramUniform1ivEXT (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform1ivEXT" sys optional ProgramUniform1ivEXT 

	// GLAPI void APIENTRY ProgramUniform2ivEXT (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform2ivEXT" sys optional ProgramUniform2ivEXT 

	// GLAPI void APIENTRY ProgramUniform3ivEXT (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform3ivEXT" sys optional ProgramUniform3ivEXT 

	// GLAPI void APIENTRY ProgramUniform4ivEXT (GLuint program,GLint location,GLsizei count,const GLint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform4ivEXT" sys optional ProgramUniform4ivEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix2fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix2fvEXT" sys optional ProgramUniformMatrix2fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix3fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix3fvEXT" sys optional ProgramUniformMatrix3fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix4fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix4fvEXT" sys optional ProgramUniformMatrix4fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix2x3fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix2x3fvEXT" sys optional ProgramUniformMatrix2x3fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix3x2fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix3x2fvEXT" sys optional ProgramUniformMatrix3x2fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix2x4fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix2x4fvEXT" sys optional ProgramUniformMatrix2x4fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix4x2fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix4x2fvEXT" sys optional ProgramUniformMatrix4x2fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix3x4fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix3x4fvEXT" sys optional ProgramUniformMatrix3x4fvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix4x3fvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLfloat * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Real*:_value ) "libGL.so.1" "glProgramUniformMatrix4x3fvEXT" sys optional ProgramUniformMatrix4x3fvEXT 

	// GLAPI void APIENTRY TextureBufferEXT (GLuint texture,GLenum target,GLenum internalformat,GLuint buffer)
	 ( Z32:_texture Z32:_target Z32:_internalformat Z32:_buffer ) "libGL.so.1" "glTextureBufferEXT" sys optional TextureBufferEXT 

	// GLAPI void APIENTRY MultiTexBufferEXT (GLenum texunit,GLenum target,GLenum internalformat,GLuint buffer)
	 ( Z32:_texunit Z32:_target Z32:_internalformat Z32:_buffer ) "libGL.so.1" "glMultiTexBufferEXT" sys optional MultiTexBufferEXT 

	// GLAPI void APIENTRY TextureParameterIivEXT (GLuint texture,GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_texture Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glTextureParameterIivEXT" sys optional TextureParameterIivEXT 

	// GLAPI void APIENTRY TextureParameterIuivEXT (GLuint texture,GLenum target,GLenum pname,const GLuint * params)
	 ( Z32:_texture Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glTextureParameterIuivEXT" sys optional TextureParameterIuivEXT 

	// GLAPI void APIENTRY GetTextureParameterIivEXT (GLuint texture,GLenum target,GLenum pname,GLint * params)
	 ( Z32:_texture Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureParameterIivEXT" sys optional GetTextureParameterIivEXT 

	// GLAPI void APIENTRY GetTextureParameterIuivEXT (GLuint texture,GLenum target,GLenum pname,GLuint * params)
	 ( Z32:_texture Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetTextureParameterIuivEXT" sys optional GetTextureParameterIuivEXT 

	// GLAPI void APIENTRY MultiTexParameterIivEXT (GLenum texunit,GLenum target,GLenum pname,const GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glMultiTexParameterIivEXT" sys optional MultiTexParameterIivEXT 

	// GLAPI void APIENTRY MultiTexParameterIuivEXT (GLenum texunit,GLenum target,GLenum pname,const GLuint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glMultiTexParameterIuivEXT" sys optional MultiTexParameterIuivEXT 

	// GLAPI void APIENTRY GetMultiTexParameterIivEXT (GLenum texunit,GLenum target,GLenum pname,GLint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetMultiTexParameterIivEXT" sys optional GetMultiTexParameterIivEXT 

	// GLAPI void APIENTRY GetMultiTexParameterIuivEXT (GLenum texunit,GLenum target,GLenum pname,GLuint * params)
	 ( Z32:_texunit Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetMultiTexParameterIuivEXT" sys optional GetMultiTexParameterIuivEXT 

	// GLAPI void APIENTRY ProgramUniform1uiEXT (GLuint program,GLint location,GLuint v0)
	 ( Z32:_program Z32:_location Z32:_v0 ) "libGL.so.1" "glProgramUniform1uiEXT" sys optional ProgramUniform1uiEXT 

	// GLAPI void APIENTRY ProgramUniform2uiEXT (GLuint program,GLint location,GLuint v0,GLuint v1)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 ) "libGL.so.1" "glProgramUniform2uiEXT" sys optional ProgramUniform2uiEXT 

	// GLAPI void APIENTRY ProgramUniform3uiEXT (GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 ) "libGL.so.1" "glProgramUniform3uiEXT" sys optional ProgramUniform3uiEXT 

	// GLAPI void APIENTRY ProgramUniform4uiEXT (GLuint program,GLint location,GLuint v0,GLuint v1,GLuint v2,GLuint v3)
	 ( Z32:_program Z32:_location Z32:_v0 Z32:_v1 Z32:_v2 Z32:_v3 ) "libGL.so.1" "glProgramUniform4uiEXT" sys optional ProgramUniform4uiEXT 

	// GLAPI void APIENTRY ProgramUniform1uivEXT (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform1uivEXT" sys optional ProgramUniform1uivEXT 

	// GLAPI void APIENTRY ProgramUniform2uivEXT (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform2uivEXT" sys optional ProgramUniform2uivEXT 

	// GLAPI void APIENTRY ProgramUniform3uivEXT (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform3uivEXT" sys optional ProgramUniform3uivEXT 

	// GLAPI void APIENTRY ProgramUniform4uivEXT (GLuint program,GLint location,GLsizei count,const GLuint * value)
	 ( Z32:_program Z32:_location Z32:_count Z32*:_value ) "libGL.so.1" "glProgramUniform4uivEXT" sys optional ProgramUniform4uivEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameters4fvEXT (GLuint program,GLenum target,GLuint index,GLsizei count,const GLfloat * params)
	 ( Z32:_program Z32:_target Z32:_index Z32:_count Real*:_params ) "libGL.so.1" "glNamedProgramLocalParameters4fvEXT" sys optional NamedProgramLocalParameters4fvEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameterI4iEXT (GLuint program,GLenum target,GLuint index,GLint x,GLint y,GLint z,GLint w)
	 ( Z32:_program Z32:_target Z32:_index Z32:_x Z32:_y Z32:_z Z32:_w ) "libGL.so.1" "glNamedProgramLocalParameterI4iEXT" sys optional NamedProgramLocalParameterI4iEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameterI4ivEXT (GLuint program,GLenum target,GLuint index,const GLint * params)
	 ( Z32:_program Z32:_target Z32:_index Z32*:_params ) "libGL.so.1" "glNamedProgramLocalParameterI4ivEXT" sys optional NamedProgramLocalParameterI4ivEXT 

	// GLAPI void APIENTRY NamedProgramLocalParametersI4ivEXT (GLuint program,GLenum target,GLuint index,GLsizei count,const GLint * params)
	 ( Z32:_program Z32:_target Z32:_index Z32:_count Z32*:_params ) "libGL.so.1" "glNamedProgramLocalParametersI4ivEXT" sys optional NamedProgramLocalParametersI4ivEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameterI4uiEXT (GLuint program,GLenum target,GLuint index,GLuint x,GLuint y,GLuint z,GLuint w)
	 ( Z32:_program Z32:_target Z32:_index Z32:_x Z32:_y Z32:_z Z32:_w ) "libGL.so.1" "glNamedProgramLocalParameterI4uiEXT" sys optional NamedProgramLocalParameterI4uiEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameterI4uivEXT (GLuint program,GLenum target,GLuint index,const GLuint * params)
	 ( Z32:_program Z32:_target Z32:_index Z32*:_params ) "libGL.so.1" "glNamedProgramLocalParameterI4uivEXT" sys optional NamedProgramLocalParameterI4uivEXT 

	// GLAPI void APIENTRY NamedProgramLocalParametersI4uivEXT (GLuint program,GLenum target,GLuint index,GLsizei count,const GLuint * params)
	 ( Z32:_program Z32:_target Z32:_index Z32:_count Z32*:_params ) "libGL.so.1" "glNamedProgramLocalParametersI4uivEXT" sys optional NamedProgramLocalParametersI4uivEXT 

	// GLAPI void APIENTRY GetNamedProgramLocalParameterIivEXT (GLuint program,GLenum target,GLuint index,GLint * params)
	 ( Z32:_program Z32:_target Z32:_index Z32*:_params ) "libGL.so.1" "glGetNamedProgramLocalParameterIivEXT" sys optional GetNamedProgramLocalParameterIivEXT 

	// GLAPI void APIENTRY GetNamedProgramLocalParameterIuivEXT (GLuint program,GLenum target,GLuint index,GLuint * params)
	 ( Z32:_program Z32:_target Z32:_index Z32*:_params ) "libGL.so.1" "glGetNamedProgramLocalParameterIuivEXT" sys optional GetNamedProgramLocalParameterIuivEXT 

	// GLAPI void APIENTRY EnableClientStateiEXT (GLenum array,GLuint index)
	 ( Z32:_array Z32:_index ) "libGL.so.1" "glEnableClientStateiEXT" sys optional EnableClientStateiEXT 

	// GLAPI void APIENTRY DisableClientStateiEXT (GLenum array,GLuint index)
	 ( Z32:_array Z32:_index ) "libGL.so.1" "glDisableClientStateiEXT" sys optional DisableClientStateiEXT 

	// GLAPI void APIENTRY GetFloati_vEXT (GLenum pname,GLuint index,GLfloat * params)
	 ( Z32:_pname Z32:_index Real*:_params ) "libGL.so.1" "glGetFloati_vEXT" sys optional GetFloati_vEXT 

	// GLAPI void APIENTRY GetDoublei_vEXT (GLenum pname,GLuint index,GLdouble * params)
	 ( Z32:_pname Z32:_index Double32*:_params ) "libGL.so.1" "glGetDoublei_vEXT" sys optional GetDoublei_vEXT 

	// GLAPI void APIENTRY GetPointeri_vEXT (GLenum pname,GLuint index,void ** params)
	 ( Z32:_pname Z32:_index Any**:_params ) "libGL.so.1" "glGetPointeri_vEXT" sys optional GetPointeri_vEXT 

	// GLAPI void APIENTRY NamedProgramStringEXT (GLuint program,GLenum target,GLenum format,GLsizei len,const void * string)
	 ( Z32:_program Z32:_target Z32:_format Z32:_len Any*:_string ) "libGL.so.1" "glNamedProgramStringEXT" sys optional NamedProgramStringEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameter4dEXT (GLuint program,GLenum target,GLuint index,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
	 ( Z32:_program Z32:_target Z32:_index Double32:_x Double32:_y Double32:_z Double32:_w ) "libGL.so.1" "glNamedProgramLocalParameter4dEXT" sys optional NamedProgramLocalParameter4dEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameter4dvEXT (GLuint program,GLenum target,GLuint index,const GLdouble * params)
	 ( Z32:_program Z32:_target Z32:_index Double32*:_params ) "libGL.so.1" "glNamedProgramLocalParameter4dvEXT" sys optional NamedProgramLocalParameter4dvEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameter4fEXT (GLuint program,GLenum target,GLuint index,GLfloat x,GLfloat y,GLfloat z,GLfloat w)
	 ( Z32:_program Z32:_target Z32:_index Real:_x Real:_y Real:_z Real:_w ) "libGL.so.1" "glNamedProgramLocalParameter4fEXT" sys optional NamedProgramLocalParameter4fEXT 

	// GLAPI void APIENTRY NamedProgramLocalParameter4fvEXT (GLuint program,GLenum target,GLuint index,const GLfloat * params)
	 ( Z32:_program Z32:_target Z32:_index Real*:_params ) "libGL.so.1" "glNamedProgramLocalParameter4fvEXT" sys optional NamedProgramLocalParameter4fvEXT 

	// GLAPI void APIENTRY GetNamedProgramLocalParameterdvEXT (GLuint program,GLenum target,GLuint index,GLdouble * params)
	 ( Z32:_program Z32:_target Z32:_index Double32*:_params ) "libGL.so.1" "glGetNamedProgramLocalParameterdvEXT" sys optional GetNamedProgramLocalParameterdvEXT 

	// GLAPI void APIENTRY GetNamedProgramLocalParameterfvEXT (GLuint program,GLenum target,GLuint index,GLfloat * params)
	 ( Z32:_program Z32:_target Z32:_index Real*:_params ) "libGL.so.1" "glGetNamedProgramLocalParameterfvEXT" sys optional GetNamedProgramLocalParameterfvEXT 

	// GLAPI void APIENTRY GetNamedProgramivEXT (GLuint program,GLenum target,GLenum pname,GLint * params)
	 ( Z32:_program Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedProgramivEXT" sys optional GetNamedProgramivEXT 

	// GLAPI void APIENTRY GetNamedProgramStringEXT (GLuint program,GLenum target,GLenum pname,void * string)
	 ( Z32:_program Z32:_target Z32:_pname Any*:_string ) "libGL.so.1" "glGetNamedProgramStringEXT" sys optional GetNamedProgramStringEXT 

	// GLAPI void APIENTRY NamedRenderbufferStorageEXT (GLuint renderbuffer,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_renderbuffer Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glNamedRenderbufferStorageEXT" sys optional NamedRenderbufferStorageEXT 

	// GLAPI void APIENTRY GetNamedRenderbufferParameterivEXT (GLuint renderbuffer,GLenum pname,GLint * params)
	 ( Z32:_renderbuffer Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedRenderbufferParameterivEXT" sys optional GetNamedRenderbufferParameterivEXT 

	// GLAPI void APIENTRY NamedRenderbufferStorageMultisampleEXT (GLuint renderbuffer,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_renderbuffer Z32:_samples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glNamedRenderbufferStorageMultisampleEXT" sys optional NamedRenderbufferStorageMultisampleEXT 

	// GLAPI void APIENTRY NamedRenderbufferStorageMultisampleCoverageEXT (GLuint renderbuffer,GLsizei coverageSamples,GLsizei colorSamples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_renderbuffer Z32:_coverageSamples Z32:_colorSamples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glNamedRenderbufferStorageMultisampleCoverageEXT" sys optional NamedRenderbufferStorageMultisampleCoverageEXT 

	// GLAPI GLenum APIENTRY CheckNamedFramebufferStatusEXT (GLuint framebuffer,GLenum target)
	 ( Z32:_framebuffer Z32:_target  -> Z32 ) "libGL.so.1" "glCheckNamedFramebufferStatusEXT" sys optional CheckNamedFramebufferStatusEXT 

	// GLAPI void APIENTRY NamedFramebufferTexture1DEXT (GLuint framebuffer,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
	 ( Z32:_framebuffer Z32:_attachment Z32:_textarget Z32:_texture Z32:_level ) "libGL.so.1" "glNamedFramebufferTexture1DEXT" sys optional NamedFramebufferTexture1DEXT 

	// GLAPI void APIENTRY NamedFramebufferTexture2DEXT (GLuint framebuffer,GLenum attachment,GLenum textarget,GLuint texture,GLint level)
	 ( Z32:_framebuffer Z32:_attachment Z32:_textarget Z32:_texture Z32:_level ) "libGL.so.1" "glNamedFramebufferTexture2DEXT" sys optional NamedFramebufferTexture2DEXT 

	// GLAPI void APIENTRY NamedFramebufferTexture3DEXT (GLuint framebuffer,GLenum attachment,GLenum textarget,GLuint texture,GLint level,GLint zoffset)
	 ( Z32:_framebuffer Z32:_attachment Z32:_textarget Z32:_texture Z32:_level Z32:_zoffset ) "libGL.so.1" "glNamedFramebufferTexture3DEXT" sys optional NamedFramebufferTexture3DEXT 

	// GLAPI void APIENTRY NamedFramebufferRenderbufferEXT (GLuint framebuffer,GLenum attachment,GLenum renderbuffertarget,GLuint renderbuffer)
	 ( Z32:_framebuffer Z32:_attachment Z32:_renderbuffertarget Z32:_renderbuffer ) "libGL.so.1" "glNamedFramebufferRenderbufferEXT" sys optional NamedFramebufferRenderbufferEXT 

	// GLAPI void APIENTRY GetNamedFramebufferAttachmentParameterivEXT (GLuint framebuffer,GLenum attachment,GLenum pname,GLint * params)
	 ( Z32:_framebuffer Z32:_attachment Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedFramebufferAttachmentParameterivEXT" sys optional GetNamedFramebufferAttachmentParameterivEXT 

	// GLAPI void APIENTRY GenerateTextureMipmapEXT (GLuint texture,GLenum target)
	 ( Z32:_texture Z32:_target ) "libGL.so.1" "glGenerateTextureMipmapEXT" sys optional GenerateTextureMipmapEXT 

	// GLAPI void APIENTRY GenerateMultiTexMipmapEXT (GLenum texunit,GLenum target)
	 ( Z32:_texunit Z32:_target ) "libGL.so.1" "glGenerateMultiTexMipmapEXT" sys optional GenerateMultiTexMipmapEXT 

	// GLAPI void APIENTRY FramebufferDrawBufferEXT (GLuint framebuffer,GLenum mode)
	 ( Z32:_framebuffer Z32:_mode ) "libGL.so.1" "glFramebufferDrawBufferEXT" sys optional FramebufferDrawBufferEXT 

	// GLAPI void APIENTRY FramebufferDrawBuffersEXT (GLuint framebuffer,GLsizei n,const GLenum * bufs)
	 ( Z32:_framebuffer Z32:_n Z32*:_bufs ) "libGL.so.1" "glFramebufferDrawBuffersEXT" sys optional FramebufferDrawBuffersEXT 

	// GLAPI void APIENTRY FramebufferReadBufferEXT (GLuint framebuffer,GLenum mode)
	 ( Z32:_framebuffer Z32:_mode ) "libGL.so.1" "glFramebufferReadBufferEXT" sys optional FramebufferReadBufferEXT 

	// GLAPI void APIENTRY GetFramebufferParameterivEXT (GLuint framebuffer,GLenum pname,GLint * params)
	 ( Z32:_framebuffer Z32:_pname Z32*:_params ) "libGL.so.1" "glGetFramebufferParameterivEXT" sys optional GetFramebufferParameterivEXT 

	// GLAPI void APIENTRY NamedCopyBufferSubDataEXT (GLuint readBuffer,GLuint writeBuffer,GLintptr readOffset,GLintptr writeOffset,GLsizeiptr size)
	 ( Z32:_readBuffer Z32:_writeBuffer Any&:_readOffset Any&:_writeOffset Z32:_size ) "libGL.so.1" "glNamedCopyBufferSubDataEXT" sys optional NamedCopyBufferSubDataEXT 

	// GLAPI void APIENTRY NamedFramebufferTextureEXT (GLuint framebuffer,GLenum attachment,GLuint texture,GLint level)
	 ( Z32:_framebuffer Z32:_attachment Z32:_texture Z32:_level ) "libGL.so.1" "glNamedFramebufferTextureEXT" sys optional NamedFramebufferTextureEXT 

	// GLAPI void APIENTRY NamedFramebufferTextureLayerEXT (GLuint framebuffer,GLenum attachment,GLuint texture,GLint level,GLint layer)
	 ( Z32:_framebuffer Z32:_attachment Z32:_texture Z32:_level Z32:_layer ) "libGL.so.1" "glNamedFramebufferTextureLayerEXT" sys optional NamedFramebufferTextureLayerEXT 

	// GLAPI void APIENTRY NamedFramebufferTextureFaceEXT (GLuint framebuffer,GLenum attachment,GLuint texture,GLint level,GLenum face)
	 ( Z32:_framebuffer Z32:_attachment Z32:_texture Z32:_level Z32:_face ) "libGL.so.1" "glNamedFramebufferTextureFaceEXT" sys optional NamedFramebufferTextureFaceEXT 

	// GLAPI void APIENTRY TextureRenderbufferEXT (GLuint texture,GLenum target,GLuint renderbuffer)
	 ( Z32:_texture Z32:_target Z32:_renderbuffer ) "libGL.so.1" "glTextureRenderbufferEXT" sys optional TextureRenderbufferEXT 

	// GLAPI void APIENTRY MultiTexRenderbufferEXT (GLenum texunit,GLenum target,GLuint renderbuffer)
	 ( Z32:_texunit Z32:_target Z32:_renderbuffer ) "libGL.so.1" "glMultiTexRenderbufferEXT" sys optional MultiTexRenderbufferEXT 

	// GLAPI void APIENTRY VertexArrayVertexOffsetEXT (GLuint vaobj,GLuint buffer,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayVertexOffsetEXT" sys optional VertexArrayVertexOffsetEXT 

	// GLAPI void APIENTRY VertexArrayColorOffsetEXT (GLuint vaobj,GLuint buffer,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayColorOffsetEXT" sys optional VertexArrayColorOffsetEXT 

	// GLAPI void APIENTRY VertexArrayEdgeFlagOffsetEXT (GLuint vaobj,GLuint buffer,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayEdgeFlagOffsetEXT" sys optional VertexArrayEdgeFlagOffsetEXT 

	// GLAPI void APIENTRY VertexArrayIndexOffsetEXT (GLuint vaobj,GLuint buffer,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayIndexOffsetEXT" sys optional VertexArrayIndexOffsetEXT 

	// GLAPI void APIENTRY VertexArrayNormalOffsetEXT (GLuint vaobj,GLuint buffer,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayNormalOffsetEXT" sys optional VertexArrayNormalOffsetEXT 

	// GLAPI void APIENTRY VertexArrayTexCoordOffsetEXT (GLuint vaobj,GLuint buffer,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayTexCoordOffsetEXT" sys optional VertexArrayTexCoordOffsetEXT 

	// GLAPI void APIENTRY VertexArrayMultiTexCoordOffsetEXT (GLuint vaobj,GLuint buffer,GLenum texunit,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_texunit Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayMultiTexCoordOffsetEXT" sys optional VertexArrayMultiTexCoordOffsetEXT 

	// GLAPI void APIENTRY VertexArrayFogCoordOffsetEXT (GLuint vaobj,GLuint buffer,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayFogCoordOffsetEXT" sys optional VertexArrayFogCoordOffsetEXT 

	// GLAPI void APIENTRY VertexArraySecondaryColorOffsetEXT (GLuint vaobj,GLuint buffer,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArraySecondaryColorOffsetEXT" sys optional VertexArraySecondaryColorOffsetEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribOffsetEXT (GLuint vaobj,GLuint buffer,GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_index Z32:_size Z32:_type Bit:_normalized Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayVertexAttribOffsetEXT" sys optional VertexArrayVertexAttribOffsetEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribIOffsetEXT (GLuint vaobj,GLuint buffer,GLuint index,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_index Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayVertexAttribIOffsetEXT" sys optional VertexArrayVertexAttribIOffsetEXT 

	// GLAPI void APIENTRY EnableVertexArrayEXT (GLuint vaobj,GLenum array)
	 ( Z32:_vaobj Z32:_array ) "libGL.so.1" "glEnableVertexArrayEXT" sys optional EnableVertexArrayEXT 

	// GLAPI void APIENTRY DisableVertexArrayEXT (GLuint vaobj,GLenum array)
	 ( Z32:_vaobj Z32:_array ) "libGL.so.1" "glDisableVertexArrayEXT" sys optional DisableVertexArrayEXT 

	// GLAPI void APIENTRY EnableVertexArrayAttribEXT (GLuint vaobj,GLuint index)
	 ( Z32:_vaobj Z32:_index ) "libGL.so.1" "glEnableVertexArrayAttribEXT" sys optional EnableVertexArrayAttribEXT 

	// GLAPI void APIENTRY DisableVertexArrayAttribEXT (GLuint vaobj,GLuint index)
	 ( Z32:_vaobj Z32:_index ) "libGL.so.1" "glDisableVertexArrayAttribEXT" sys optional DisableVertexArrayAttribEXT 

	// GLAPI void APIENTRY GetVertexArrayIntegervEXT (GLuint vaobj,GLenum pname,GLint * param)
	 ( Z32:_vaobj Z32:_pname Z32*:_param ) "libGL.so.1" "glGetVertexArrayIntegervEXT" sys optional GetVertexArrayIntegervEXT 

	// GLAPI void APIENTRY GetVertexArrayPointervEXT (GLuint vaobj,GLenum pname,void ** param)
	 ( Z32:_vaobj Z32:_pname Any**:_param ) "libGL.so.1" "glGetVertexArrayPointervEXT" sys optional GetVertexArrayPointervEXT 

	// GLAPI void APIENTRY GetVertexArrayIntegeri_vEXT (GLuint vaobj,GLuint index,GLenum pname,GLint * param)
	 ( Z32:_vaobj Z32:_index Z32:_pname Z32*:_param ) "libGL.so.1" "glGetVertexArrayIntegeri_vEXT" sys optional GetVertexArrayIntegeri_vEXT 

	// GLAPI void APIENTRY GetVertexArrayPointeri_vEXT (GLuint vaobj,GLuint index,GLenum pname,void ** param)
	 ( Z32:_vaobj Z32:_index Z32:_pname Any**:_param ) "libGL.so.1" "glGetVertexArrayPointeri_vEXT" sys optional GetVertexArrayPointeri_vEXT 

	// GLAPI void * APIENTRY MapNamedBufferRangeEXT (GLuint buffer,GLintptr offset,GLsizeiptr length,GLbitfield access)
	 ( Z32:_buffer Any&:_offset Z32:_length Z32:_access  -> Any* ) "libGL.so.1" "glMapNamedBufferRangeEXT" sys optional MapNamedBufferRangeEXT 

	// GLAPI void APIENTRY FlushMappedNamedBufferRangeEXT (GLuint buffer,GLintptr offset,GLsizeiptr length)
	 ( Z32:_buffer Any&:_offset Z32:_length ) "libGL.so.1" "glFlushMappedNamedBufferRangeEXT" sys optional FlushMappedNamedBufferRangeEXT 

	// GLAPI void APIENTRY NamedBufferStorageEXT (GLuint buffer,GLsizeiptr size,const void * data,GLbitfield flags)
	 ( Z32:_buffer Z32:_size Any*:_data Z32:_flags ) "libGL.so.1" "glNamedBufferStorageEXT" sys optional NamedBufferStorageEXT 

	// GLAPI void APIENTRY ClearNamedBufferDataEXT (GLuint buffer,GLenum internalformat,GLenum format,GLenum type,const void * data)
	 ( Z32:_buffer Z32:_internalformat Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearNamedBufferDataEXT" sys optional ClearNamedBufferDataEXT 

	// GLAPI void APIENTRY ClearNamedBufferSubDataEXT (GLuint buffer,GLenum internalformat,GLsizeiptr offset,GLsizeiptr size,GLenum format,GLenum type,const void * data)
	 ( Z32:_buffer Z32:_internalformat Z32:_offset Z32:_size Z32:_format Z32:_type Any*:_data ) "libGL.so.1" "glClearNamedBufferSubDataEXT" sys optional ClearNamedBufferSubDataEXT 

	// GLAPI void APIENTRY NamedFramebufferParameteriEXT (GLuint framebuffer,GLenum pname,GLint param)
	 ( Z32:_framebuffer Z32:_pname Z32:_param ) "libGL.so.1" "glNamedFramebufferParameteriEXT" sys optional NamedFramebufferParameteriEXT 

	// GLAPI void APIENTRY GetNamedFramebufferParameterivEXT (GLuint framebuffer,GLenum pname,GLint * params)
	 ( Z32:_framebuffer Z32:_pname Z32*:_params ) "libGL.so.1" "glGetNamedFramebufferParameterivEXT" sys optional GetNamedFramebufferParameterivEXT 

	// GLAPI void APIENTRY ProgramUniform1dEXT (GLuint program,GLint location,GLdouble x)
	 ( Z32:_program Z32:_location Double32:_x ) "libGL.so.1" "glProgramUniform1dEXT" sys optional ProgramUniform1dEXT 

	// GLAPI void APIENTRY ProgramUniform2dEXT (GLuint program,GLint location,GLdouble x,GLdouble y)
	 ( Z32:_program Z32:_location Double32:_x Double32:_y ) "libGL.so.1" "glProgramUniform2dEXT" sys optional ProgramUniform2dEXT 

	// GLAPI void APIENTRY ProgramUniform3dEXT (GLuint program,GLint location,GLdouble x,GLdouble y,GLdouble z)
	 ( Z32:_program Z32:_location Double32:_x Double32:_y Double32:_z ) "libGL.so.1" "glProgramUniform3dEXT" sys optional ProgramUniform3dEXT 

	// GLAPI void APIENTRY ProgramUniform4dEXT (GLuint program,GLint location,GLdouble x,GLdouble y,GLdouble z,GLdouble w)
	 ( Z32:_program Z32:_location Double32:_x Double32:_y Double32:_z Double32:_w ) "libGL.so.1" "glProgramUniform4dEXT" sys optional ProgramUniform4dEXT 

	// GLAPI void APIENTRY ProgramUniform1dvEXT (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform1dvEXT" sys optional ProgramUniform1dvEXT 

	// GLAPI void APIENTRY ProgramUniform2dvEXT (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform2dvEXT" sys optional ProgramUniform2dvEXT 

	// GLAPI void APIENTRY ProgramUniform3dvEXT (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform3dvEXT" sys optional ProgramUniform3dvEXT 

	// GLAPI void APIENTRY ProgramUniform4dvEXT (GLuint program,GLint location,GLsizei count,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Double32*:_value ) "libGL.so.1" "glProgramUniform4dvEXT" sys optional ProgramUniform4dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix2dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix2dvEXT" sys optional ProgramUniformMatrix2dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix3dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix3dvEXT" sys optional ProgramUniformMatrix3dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix4dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix4dvEXT" sys optional ProgramUniformMatrix4dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix2x3dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix2x3dvEXT" sys optional ProgramUniformMatrix2x3dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix2x4dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix2x4dvEXT" sys optional ProgramUniformMatrix2x4dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix3x2dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix3x2dvEXT" sys optional ProgramUniformMatrix3x2dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix3x4dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix3x4dvEXT" sys optional ProgramUniformMatrix3x4dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix4x2dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix4x2dvEXT" sys optional ProgramUniformMatrix4x2dvEXT 

	// GLAPI void APIENTRY ProgramUniformMatrix4x3dvEXT (GLuint program,GLint location,GLsizei count,GLboolean transpose,const GLdouble * value)
	 ( Z32:_program Z32:_location Z32:_count Bit:_transpose Double32*:_value ) "libGL.so.1" "glProgramUniformMatrix4x3dvEXT" sys optional ProgramUniformMatrix4x3dvEXT 

	// GLAPI void APIENTRY TextureBufferRangeEXT (GLuint texture,GLenum target,GLenum internalformat,GLuint buffer,GLintptr offset,GLsizeiptr size)
	 ( Z32:_texture Z32:_target Z32:_internalformat Z32:_buffer Any&:_offset Z32:_size ) "libGL.so.1" "glTextureBufferRangeEXT" sys optional TextureBufferRangeEXT 

	// GLAPI void APIENTRY TextureStorage1DEXT (GLuint texture,GLenum target,GLsizei levels,GLenum internalformat,GLsizei width)
	 ( Z32:_texture Z32:_target Z32:_levels Z32:_internalformat Z32:_width ) "libGL.so.1" "glTextureStorage1DEXT" sys optional TextureStorage1DEXT 

	// GLAPI void APIENTRY TextureStorage2DEXT (GLuint texture,GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_texture Z32:_target Z32:_levels Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glTextureStorage2DEXT" sys optional TextureStorage2DEXT 

	// GLAPI void APIENTRY TextureStorage3DEXT (GLuint texture,GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth)
	 ( Z32:_texture Z32:_target Z32:_levels Z32:_internalformat Z32:_width Z32:_height Z32:_depth ) "libGL.so.1" "glTextureStorage3DEXT" sys optional TextureStorage3DEXT 

	// GLAPI void APIENTRY TextureStorage2DMultisampleEXT (GLuint texture,GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLboolean fixedsamplelocations)
	 ( Z32:_texture Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height Bit:_fixedsamplelocations ) "libGL.so.1" "glTextureStorage2DMultisampleEXT" sys optional TextureStorage2DMultisampleEXT 

	// GLAPI void APIENTRY TextureStorage3DMultisampleEXT (GLuint texture,GLenum target,GLsizei samples,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth,GLboolean fixedsamplelocations)
	 ( Z32:_texture Z32:_target Z32:_samples Z32:_internalformat Z32:_width Z32:_height Z32:_depth Bit:_fixedsamplelocations ) "libGL.so.1" "glTextureStorage3DMultisampleEXT" sys optional TextureStorage3DMultisampleEXT 

	// GLAPI void APIENTRY VertexArrayBindVertexBufferEXT (GLuint vaobj,GLuint bindingindex,GLuint buffer,GLintptr offset,GLsizei stride)
	 ( Z32:_vaobj Z32:_bindingindex Z32:_buffer Any&:_offset Z32:_stride ) "libGL.so.1" "glVertexArrayBindVertexBufferEXT" sys optional VertexArrayBindVertexBufferEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribFormatEXT (GLuint vaobj,GLuint attribindex,GLint size,GLenum type,GLboolean normalized,GLuint relativeoffset)
	 ( Z32:_vaobj Z32:_attribindex Z32:_size Z32:_type Bit:_normalized Z32:_relativeoffset ) "libGL.so.1" "glVertexArrayVertexAttribFormatEXT" sys optional VertexArrayVertexAttribFormatEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribIFormatEXT (GLuint vaobj,GLuint attribindex,GLint size,GLenum type,GLuint relativeoffset)
	 ( Z32:_vaobj Z32:_attribindex Z32:_size Z32:_type Z32:_relativeoffset ) "libGL.so.1" "glVertexArrayVertexAttribIFormatEXT" sys optional VertexArrayVertexAttribIFormatEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribLFormatEXT (GLuint vaobj,GLuint attribindex,GLint size,GLenum type,GLuint relativeoffset)
	 ( Z32:_vaobj Z32:_attribindex Z32:_size Z32:_type Z32:_relativeoffset ) "libGL.so.1" "glVertexArrayVertexAttribLFormatEXT" sys optional VertexArrayVertexAttribLFormatEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribBindingEXT (GLuint vaobj,GLuint attribindex,GLuint bindingindex)
	 ( Z32:_vaobj Z32:_attribindex Z32:_bindingindex ) "libGL.so.1" "glVertexArrayVertexAttribBindingEXT" sys optional VertexArrayVertexAttribBindingEXT 

	// GLAPI void APIENTRY VertexArrayVertexBindingDivisorEXT (GLuint vaobj,GLuint bindingindex,GLuint divisor)
	 ( Z32:_vaobj Z32:_bindingindex Z32:_divisor ) "libGL.so.1" "glVertexArrayVertexBindingDivisorEXT" sys optional VertexArrayVertexBindingDivisorEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribLOffsetEXT (GLuint vaobj,GLuint buffer,GLuint index,GLint size,GLenum type,GLsizei stride,GLintptr offset)
	 ( Z32:_vaobj Z32:_buffer Z32:_index Z32:_size Z32:_type Z32:_stride Any&:_offset ) "libGL.so.1" "glVertexArrayVertexAttribLOffsetEXT" sys optional VertexArrayVertexAttribLOffsetEXT 

	// GLAPI void APIENTRY TexturePageCommitmentEXT (GLuint texture,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLboolean commit)
	 ( Z32:_texture Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Bit:_commit ) "libGL.so.1" "glTexturePageCommitmentEXT" sys optional TexturePageCommitmentEXT 

	// GLAPI void APIENTRY VertexArrayVertexAttribDivisorEXT (GLuint vaobj,GLuint index,GLuint divisor)
	 ( Z32:_vaobj Z32:_index Z32:_divisor ) "libGL.so.1" "glVertexArrayVertexAttribDivisorEXT" sys optional VertexArrayVertexAttribDivisorEXT 

	// GLAPI void APIENTRY DrawArraysInstancedEXT (GLenum mode,GLint start,GLsizei count,GLsizei primcount)
	 ( Z32:_mode Z32:_start Z32:_count Z32:_primcount ) "libGL.so.1" "glDrawArraysInstancedEXT" sys optional DrawArraysInstancedEXT 

	// GLAPI void APIENTRY DrawElementsInstancedEXT (GLenum mode,GLsizei count,GLenum type,const void * indices,GLsizei primcount)
	 ( Z32:_mode Z32:_count Z32:_type Any*:_indices Z32:_primcount ) "libGL.so.1" "glDrawElementsInstancedEXT" sys optional DrawElementsInstancedEXT 

	// GLAPI void APIENTRY PolygonOffsetClampEXT (GLfloat factor,GLfloat units,GLfloat clamp)
	 ( Real:_factor Real:_units Real:_clamp ) "libGL.so.1" "glPolygonOffsetClampEXT" sys optional PolygonOffsetClampEXT 

	// GLAPI void APIENTRY RasterSamplesEXT (GLuint samples,GLboolean fixedsamplelocations)
	 ( Z32:_samples Bit:_fixedsamplelocations ) "libGL.so.1" "glRasterSamplesEXT" sys optional RasterSamplesEXT 

	// GLAPI void APIENTRY UseShaderProgramEXT (GLenum type,GLuint program)
	 ( Z32:_type Z32:_program ) "libGL.so.1" "glUseShaderProgramEXT" sys optional UseShaderProgramEXT 

	// GLAPI void APIENTRY ActiveProgramEXT (GLuint program)
	 ( Z32:_program ) "libGL.so.1" "glActiveProgramEXT" sys optional ActiveProgramEXT 

	// GLAPI GLuint APIENTRY CreateShaderProgramEXT (GLenum type,const GLchar * string)
	 ( Z32:_type String:_string  -> Z32 ) "libGL.so.1" "glCreateShaderProgramEXT" sys optional CreateShaderProgramEXT 

	// GLAPI void APIENTRY FramebufferFetchBarrierEXT (void)
	 ( ) "libGL.so.1" "glFramebufferFetchBarrierEXT" sys optional FramebufferFetchBarrierEXT 

	// GLAPI void APIENTRY TexStorage1DEXT (GLenum target,GLsizei levels,GLenum internalformat,GLsizei width)
	 ( Z32:_target Z32:_levels Z32:_internalformat Z32:_width ) "libGL.so.1" "glTexStorage1DEXT" sys optional TexStorage1DEXT 

	// GLAPI void APIENTRY TexStorage2DEXT (GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_levels Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glTexStorage2DEXT" sys optional TexStorage2DEXT 

	// GLAPI void APIENTRY TexStorage3DEXT (GLenum target,GLsizei levels,GLenum internalformat,GLsizei width,GLsizei height,GLsizei depth)
	 ( Z32:_target Z32:_levels Z32:_internalformat Z32:_width Z32:_height Z32:_depth ) "libGL.so.1" "glTexStorage3DEXT" sys optional TexStorage3DEXT 

	// GLAPI void APIENTRY WindowRectanglesEXT (GLenum mode,GLsizei count,const GLint * box)
	 ( Z32:_mode Z32:_count Z32*:_box ) "libGL.so.1" "glWindowRectanglesEXT" sys optional WindowRectanglesEXT 

	// GLAPI void APIENTRY ApplyFramebufferAttachmentCMAAINTEL (void)
	 ( ) "libGL.so.1" "glApplyFramebufferAttachmentCMAAINTEL" sys optional ApplyFramebufferAttachmentCMAAINTEL 

	// GLAPI void APIENTRY BeginPerfQueryINTEL (GLuint queryHandle)
	 ( Z32:_queryHandle ) "libGL.so.1" "glBeginPerfQueryINTEL" sys optional BeginPerfQueryINTEL 

	// GLAPI void APIENTRY CreatePerfQueryINTEL (GLuint queryId,GLuint * queryHandle)
	 ( Z32:_queryId Z32*:_queryHandle ) "libGL.so.1" "glCreatePerfQueryINTEL" sys optional CreatePerfQueryINTEL 

	// GLAPI void APIENTRY DeletePerfQueryINTEL (GLuint queryHandle)
	 ( Z32:_queryHandle ) "libGL.so.1" "glDeletePerfQueryINTEL" sys optional DeletePerfQueryINTEL 

	// GLAPI void APIENTRY EndPerfQueryINTEL (GLuint queryHandle)
	 ( Z32:_queryHandle ) "libGL.so.1" "glEndPerfQueryINTEL" sys optional EndPerfQueryINTEL 

	// GLAPI void APIENTRY GetFirstPerfQueryIdINTEL (GLuint * queryId)
	 ( Z32*:_queryId ) "libGL.so.1" "glGetFirstPerfQueryIdINTEL" sys optional GetFirstPerfQueryIdINTEL 

	// GLAPI void APIENTRY GetNextPerfQueryIdINTEL (GLuint queryId,GLuint * nextQueryId)
	 ( Z32:_queryId Z32*:_nextQueryId ) "libGL.so.1" "glGetNextPerfQueryIdINTEL" sys optional GetNextPerfQueryIdINTEL 

	// GLAPI void APIENTRY GetPerfCounterInfoINTEL (GLuint queryId,GLuint counterId,GLuint counterNameLength,GLchar * counterName,GLuint counterDescLength,GLchar * counterDesc,GLuint * counterOffset,GLuint * counterDataSize,GLuint * counterTypeEnum,GLuint * counterDataTypeEnum,GLuint64 * rawCounterMaxValue)
	 ( Z32:_queryId Z32:_counterId Z32:_counterNameLength String:_counterName Z32:_counterDescLength String:_counterDesc Z32*:_counterOffset Z32*:_counterDataSize Z32*:_counterTypeEnum Z32*:_counterDataTypeEnum Any&*:_rawCounterMaxValue ) "libGL.so.1" "glGetPerfCounterInfoINTEL" sys optional GetPerfCounterInfoINTEL 

	// GLAPI void APIENTRY GetPerfQueryDataINTEL (GLuint queryHandle,GLuint flags,GLsizei dataSize,void * data,GLuint * bytesWritten)
	 ( Z32:_queryHandle Z32:_flags Z32:_dataSize Any*:_data Z32*:_bytesWritten ) "libGL.so.1" "glGetPerfQueryDataINTEL" sys optional GetPerfQueryDataINTEL 

	// GLAPI void APIENTRY GetPerfQueryIdByNameINTEL (GLchar * queryName,GLuint * queryId)
	 ( String:_queryName Z32*:_queryId ) "libGL.so.1" "glGetPerfQueryIdByNameINTEL" sys optional GetPerfQueryIdByNameINTEL 

	// GLAPI void APIENTRY GetPerfQueryInfoINTEL (GLuint queryId,GLuint queryNameLength,GLchar * queryName,GLuint * dataSize,GLuint * noCounters,GLuint * noInstances,GLuint * capsMask)
	 ( Z32:_queryId Z32:_queryNameLength String:_queryName Z32*:_dataSize Z32*:_noCounters Z32*:_noInstances Z32*:_capsMask ) "libGL.so.1" "glGetPerfQueryInfoINTEL" sys optional GetPerfQueryInfoINTEL 

	// GLAPI void APIENTRY FramebufferParameteriMESA (GLenum target,GLenum pname,GLint param)
	 ( Z32:_target Z32:_pname Z32:_param ) "libGL.so.1" "glFramebufferParameteriMESA" sys optional FramebufferParameteriMESA 

	// GLAPI void APIENTRY GetFramebufferParameterivMESA (GLenum target,GLenum pname,GLint * params)
	 ( Z32:_target Z32:_pname Z32*:_params ) "libGL.so.1" "glGetFramebufferParameterivMESA" sys optional GetFramebufferParameterivMESA 

	// GLAPI void APIENTRY MultiDrawArraysIndirectBindlessNV (GLenum mode,const void * indirect,GLsizei drawCount,GLsizei stride,GLint vertexBufferCount)
	 ( Z32:_mode Any*:_indirect Z32:_drawCount Z32:_stride Z32:_vertexBufferCount ) "libGL.so.1" "glMultiDrawArraysIndirectBindlessNV" sys optional MultiDrawArraysIndirectBindlessNV 

	// GLAPI void APIENTRY MultiDrawElementsIndirectBindlessNV (GLenum mode,GLenum type,const void * indirect,GLsizei drawCount,GLsizei stride,GLint vertexBufferCount)
	 ( Z32:_mode Z32:_type Any*:_indirect Z32:_drawCount Z32:_stride Z32:_vertexBufferCount ) "libGL.so.1" "glMultiDrawElementsIndirectBindlessNV" sys optional MultiDrawElementsIndirectBindlessNV 

	// GLAPI void APIENTRY MultiDrawArraysIndirectBindlessCountNV (GLenum mode,const void * indirect,GLsizei drawCount,GLsizei maxDrawCount,GLsizei stride,GLint vertexBufferCount)
	 ( Z32:_mode Any*:_indirect Z32:_drawCount Z32:_maxDrawCount Z32:_stride Z32:_vertexBufferCount ) "libGL.so.1" "glMultiDrawArraysIndirectBindlessCountNV" sys optional MultiDrawArraysIndirectBindlessCountNV 

	// GLAPI void APIENTRY MultiDrawElementsIndirectBindlessCountNV (GLenum mode,GLenum type,const void * indirect,GLsizei drawCount,GLsizei maxDrawCount,GLsizei stride,GLint vertexBufferCount)
	 ( Z32:_mode Z32:_type Any*:_indirect Z32:_drawCount Z32:_maxDrawCount Z32:_stride Z32:_vertexBufferCount ) "libGL.so.1" "glMultiDrawElementsIndirectBindlessCountNV" sys optional MultiDrawElementsIndirectBindlessCountNV 

	// GLAPI GLuint64 APIENTRY GetTextureHandleNV (GLuint texture)
	 ( Z32:_texture  -> Any& ) "libGL.so.1" "glGetTextureHandleNV" sys optional GetTextureHandleNV 

	// GLAPI GLuint64 APIENTRY GetTextureSamplerHandleNV (GLuint texture,GLuint sampler)
	 ( Z32:_texture Z32:_sampler  -> Any& ) "libGL.so.1" "glGetTextureSamplerHandleNV" sys optional GetTextureSamplerHandleNV 

	// GLAPI void APIENTRY MakeTextureHandleResidentNV (GLuint64 handle)
	 ( Any&:_handle ) "libGL.so.1" "glMakeTextureHandleResidentNV" sys optional MakeTextureHandleResidentNV 

	// GLAPI void APIENTRY MakeTextureHandleNonResidentNV (GLuint64 handle)
	 ( Any&:_handle ) "libGL.so.1" "glMakeTextureHandleNonResidentNV" sys optional MakeTextureHandleNonResidentNV 

	// GLAPI GLuint64 APIENTRY GetImageHandleNV (GLuint texture,GLint level,GLboolean layered,GLint layer,GLenum format)
	 ( Z32:_texture Z32:_level Bit:_layered Z32:_layer Z32:_format  -> Any& ) "libGL.so.1" "glGetImageHandleNV" sys optional GetImageHandleNV 

	// GLAPI void APIENTRY MakeImageHandleResidentNV (GLuint64 handle,GLenum access)
	 ( Any&:_handle Z32:_access ) "libGL.so.1" "glMakeImageHandleResidentNV" sys optional MakeImageHandleResidentNV 

	// GLAPI void APIENTRY MakeImageHandleNonResidentNV (GLuint64 handle)
	 ( Any&:_handle ) "libGL.so.1" "glMakeImageHandleNonResidentNV" sys optional MakeImageHandleNonResidentNV 

	// GLAPI void APIENTRY UniformHandleui64NV (GLint location,GLuint64 value)
	 ( Z32:_location Any&:_value ) "libGL.so.1" "glUniformHandleui64NV" sys optional UniformHandleui64NV 

	// GLAPI void APIENTRY UniformHandleui64vNV (GLint location,GLsizei count,const GLuint64 * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniformHandleui64vNV" sys optional UniformHandleui64vNV 

	// GLAPI void APIENTRY ProgramUniformHandleui64NV (GLuint program,GLint location,GLuint64 value)
	 ( Z32:_program Z32:_location Any&:_value ) "libGL.so.1" "glProgramUniformHandleui64NV" sys optional ProgramUniformHandleui64NV 

	// GLAPI void APIENTRY ProgramUniformHandleui64vNV (GLuint program,GLint location,GLsizei count,const GLuint64 * values)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_values ) "libGL.so.1" "glProgramUniformHandleui64vNV" sys optional ProgramUniformHandleui64vNV 

	// GLAPI GLboolean APIENTRY IsTextureHandleResidentNV (GLuint64 handle)
	 ( Any&:_handle  -> Bit ) "libGL.so.1" "glIsTextureHandleResidentNV" sys optional IsTextureHandleResidentNV 

	// GLAPI GLboolean APIENTRY IsImageHandleResidentNV (GLuint64 handle)
	 ( Any&:_handle  -> Bit ) "libGL.so.1" "glIsImageHandleResidentNV" sys optional IsImageHandleResidentNV 

	// GLAPI void APIENTRY BlendParameteriNV (GLenum pname,GLint value)
	 ( Z32:_pname Z32:_value ) "libGL.so.1" "glBlendParameteriNV" sys optional BlendParameteriNV 

	// GLAPI void APIENTRY BlendBarrierNV (void)
	 ( ) "libGL.so.1" "glBlendBarrierNV" sys optional BlendBarrierNV 

	// GLAPI void APIENTRY ViewportPositionWScaleNV (GLuint index,GLfloat xcoeff,GLfloat ycoeff)
	 ( Z32:_index Real:_xcoeff Real:_ycoeff ) "libGL.so.1" "glViewportPositionWScaleNV" sys optional ViewportPositionWScaleNV 

	// GLAPI void APIENTRY CreateStatesNV (GLsizei n,GLuint * states)
	 ( Z32:_n Z32*:_states ) "libGL.so.1" "glCreateStatesNV" sys optional CreateStatesNV 

	// GLAPI void APIENTRY DeleteStatesNV (GLsizei n,const GLuint * states)
	 ( Z32:_n Z32*:_states ) "libGL.so.1" "glDeleteStatesNV" sys optional DeleteStatesNV 

	// GLAPI GLboolean APIENTRY IsStateNV (GLuint state)
	 ( Z32:_state  -> Bit ) "libGL.so.1" "glIsStateNV" sys optional IsStateNV 

	// GLAPI void APIENTRY StateCaptureNV (GLuint state,GLenum mode)
	 ( Z32:_state Z32:_mode ) "libGL.so.1" "glStateCaptureNV" sys optional StateCaptureNV 

	// GLAPI GLuint APIENTRY GetCommandHeaderNV (GLenum tokenID,GLuint size)
	 ( Z32:_tokenID Z32:_size  -> Z32 ) "libGL.so.1" "glGetCommandHeaderNV" sys optional GetCommandHeaderNV 

	// GLAPI GLushort APIENTRY GetStageIndexNV (GLenum shadertype)
	 ( Z32:_shadertype  -> Z32 ) "libGL.so.1" "glGetStageIndexNV" sys optional GetStageIndexNV 

	// GLAPI void APIENTRY DrawCommandsNV (GLenum primitiveMode,GLuint buffer,const GLintptr * indirects,const GLsizei * sizes,GLuint count)
	 ( Z32:_primitiveMode Z32:_buffer Any&*:_indirects Z32*:_sizes Z32:_count ) "libGL.so.1" "glDrawCommandsNV" sys optional DrawCommandsNV 

	// GLAPI void APIENTRY DrawCommandsAddressNV (GLenum primitiveMode,const GLuint64 * indirects,const GLsizei * sizes,GLuint count)
	 ( Z32:_primitiveMode Any&*:_indirects Z32*:_sizes Z32:_count ) "libGL.so.1" "glDrawCommandsAddressNV" sys optional DrawCommandsAddressNV 

	// GLAPI void APIENTRY DrawCommandsStatesNV (GLuint buffer,const GLintptr * indirects,const GLsizei * sizes,const GLuint * states,const GLuint * fbos,GLuint count)
	 ( Z32:_buffer Any&*:_indirects Z32*:_sizes Z32*:_states Z32*:_fbos Z32:_count ) "libGL.so.1" "glDrawCommandsStatesNV" sys optional DrawCommandsStatesNV 

	// GLAPI void APIENTRY DrawCommandsStatesAddressNV (const GLuint64 * indirects,const GLsizei * sizes,const GLuint * states,const GLuint * fbos,GLuint count)
	 ( Any&*:_indirects Z32*:_sizes Z32*:_states Z32*:_fbos Z32:_count ) "libGL.so.1" "glDrawCommandsStatesAddressNV" sys optional DrawCommandsStatesAddressNV 

	// GLAPI void APIENTRY CreateCommandListsNV (GLsizei n,GLuint * lists)
	 ( Z32:_n Z32*:_lists ) "libGL.so.1" "glCreateCommandListsNV" sys optional CreateCommandListsNV 

	// GLAPI void APIENTRY DeleteCommandListsNV (GLsizei n,const GLuint * lists)
	 ( Z32:_n Z32*:_lists ) "libGL.so.1" "glDeleteCommandListsNV" sys optional DeleteCommandListsNV 

	// GLAPI GLboolean APIENTRY IsCommandListNV (GLuint list)
	 ( Z32:_list  -> Bit ) "libGL.so.1" "glIsCommandListNV" sys optional IsCommandListNV 

	// GLAPI void APIENTRY ListDrawCommandsStatesClientNV (GLuint list,GLuint segment,const void ** indirects,const GLsizei * sizes,const GLuint * states,const GLuint * fbos,GLuint count)
	 ( Z32:_list Z32:_segment Any**:_indirects Z32*:_sizes Z32*:_states Z32*:_fbos Z32:_count ) "libGL.so.1" "glListDrawCommandsStatesClientNV" sys optional ListDrawCommandsStatesClientNV 

	// GLAPI void APIENTRY CommandListSegmentsNV (GLuint list,GLuint segments)
	 ( Z32:_list Z32:_segments ) "libGL.so.1" "glCommandListSegmentsNV" sys optional CommandListSegmentsNV 

	// GLAPI void APIENTRY CompileCommandListNV (GLuint list)
	 ( Z32:_list ) "libGL.so.1" "glCompileCommandListNV" sys optional CompileCommandListNV 

	// GLAPI void APIENTRY CallCommandListNV (GLuint list)
	 ( Z32:_list ) "libGL.so.1" "glCallCommandListNV" sys optional CallCommandListNV 

	// GLAPI void APIENTRY BeginConditionalRenderNV (GLuint id,GLenum mode)
	 ( Z32:_id Z32:_mode ) "libGL.so.1" "glBeginConditionalRenderNV" sys optional BeginConditionalRenderNV 

	// GLAPI void APIENTRY EndConditionalRenderNV (void)
	 ( ) "libGL.so.1" "glEndConditionalRenderNV" sys optional EndConditionalRenderNV 

	// GLAPI void APIENTRY SubpixelPrecisionBiasNV (GLuint xbits,GLuint ybits)
	 ( Z32:_xbits Z32:_ybits ) "libGL.so.1" "glSubpixelPrecisionBiasNV" sys optional SubpixelPrecisionBiasNV 

	// GLAPI void APIENTRY ConservativeRasterParameterfNV (GLenum pname,GLfloat value)
	 ( Z32:_pname Real:_value ) "libGL.so.1" "glConservativeRasterParameterfNV" sys optional ConservativeRasterParameterfNV 

	// GLAPI void APIENTRY ConservativeRasterParameteriNV (GLenum pname,GLint param)
	 ( Z32:_pname Z32:_param ) "libGL.so.1" "glConservativeRasterParameteriNV" sys optional ConservativeRasterParameteriNV 

	// GLAPI void APIENTRY DepthRangedNV (GLdouble zNear,GLdouble zFar)
	 ( Double32:_zNear Double32:_zFar ) "libGL.so.1" "glDepthRangedNV" sys optional DepthRangedNV 

	// GLAPI void APIENTRY ClearDepthdNV (GLdouble depth)
	 ( Double32:_depth ) "libGL.so.1" "glClearDepthdNV" sys optional ClearDepthdNV 

	// GLAPI void APIENTRY DepthBoundsdNV (GLdouble zmin,GLdouble zmax)
	 ( Double32:_zmin Double32:_zmax ) "libGL.so.1" "glDepthBoundsdNV" sys optional DepthBoundsdNV 

	// GLAPI void APIENTRY DrawVkImageNV (GLuint64 vkImage,GLuint sampler,GLfloat x0,GLfloat y0,GLfloat x1,GLfloat y1,GLfloat z,GLfloat s0,GLfloat t0,GLfloat s1,GLfloat t1)
	 ( Any&:_vkImage Z32:_sampler Real:_x0 Real:_y0 Real:_x1 Real:_y1 Real:_z Real:_s0 Real:_t0 Real:_s1 Real:_t1 ) "libGL.so.1" "glDrawVkImageNV" sys optional DrawVkImageNV 

	// GLAPI GLVULKANPROCNV APIENTRY GetVkProcAddrNV (const GLchar * name)
	// GLAPI void APIENTRY WaitVkSemaphoreNV (GLuint64 vkSemaphore)
	 ( Any&:_vkSemaphore ) "libGL.so.1" "glWaitVkSemaphoreNV" sys optional WaitVkSemaphoreNV 

	// GLAPI void APIENTRY SignalVkSemaphoreNV (GLuint64 vkSemaphore)
	 ( Any&:_vkSemaphore ) "libGL.so.1" "glSignalVkSemaphoreNV" sys optional SignalVkSemaphoreNV 

	// GLAPI void APIENTRY SignalVkFenceNV (GLuint64 vkFence)
	 ( Any&:_vkFence ) "libGL.so.1" "glSignalVkFenceNV" sys optional SignalVkFenceNV 

	// GLAPI void APIENTRY FragmentCoverageColorNV (GLuint color)
	 ( Z32:_color ) "libGL.so.1" "glFragmentCoverageColorNV" sys optional FragmentCoverageColorNV 

	// GLAPI void APIENTRY CoverageModulationTableNV (GLsizei n,const GLfloat * v)
	 ( Z32:_n Real*:_v ) "libGL.so.1" "glCoverageModulationTableNV" sys optional CoverageModulationTableNV 

	// GLAPI void APIENTRY GetCoverageModulationTableNV (GLsizei bufSize,GLfloat * v)
	 ( Z32:_bufSize Real*:_v ) "libGL.so.1" "glGetCoverageModulationTableNV" sys optional GetCoverageModulationTableNV 

	// GLAPI void APIENTRY CoverageModulationNV (GLenum components)
	 ( Z32:_components ) "libGL.so.1" "glCoverageModulationNV" sys optional CoverageModulationNV 

	// GLAPI void APIENTRY RenderbufferStorageMultisampleCoverageNV (GLenum target,GLsizei coverageSamples,GLsizei colorSamples,GLenum internalformat,GLsizei width,GLsizei height)
	 ( Z32:_target Z32:_coverageSamples Z32:_colorSamples Z32:_internalformat Z32:_width Z32:_height ) "libGL.so.1" "glRenderbufferStorageMultisampleCoverageNV" sys optional RenderbufferStorageMultisampleCoverageNV 

	// GLAPI void APIENTRY Uniform1i64NV (GLint location,GLint64EXT x)
	 ( Z32:_location Any&:_x ) "libGL.so.1" "glUniform1i64NV" sys optional Uniform1i64NV 

	// GLAPI void APIENTRY Uniform2i64NV (GLint location,GLint64EXT x,GLint64EXT y)
	 ( Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glUniform2i64NV" sys optional Uniform2i64NV 

	// GLAPI void APIENTRY Uniform3i64NV (GLint location,GLint64EXT x,GLint64EXT y,GLint64EXT z)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glUniform3i64NV" sys optional Uniform3i64NV 

	// GLAPI void APIENTRY Uniform4i64NV (GLint location,GLint64EXT x,GLint64EXT y,GLint64EXT z,GLint64EXT w)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glUniform4i64NV" sys optional Uniform4i64NV 

	// GLAPI void APIENTRY Uniform1i64vNV (GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform1i64vNV" sys optional Uniform1i64vNV 

	// GLAPI void APIENTRY Uniform2i64vNV (GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform2i64vNV" sys optional Uniform2i64vNV 

	// GLAPI void APIENTRY Uniform3i64vNV (GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform3i64vNV" sys optional Uniform3i64vNV 

	// GLAPI void APIENTRY Uniform4i64vNV (GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform4i64vNV" sys optional Uniform4i64vNV 

	// GLAPI void APIENTRY Uniform1ui64NV (GLint location,GLuint64EXT x)
	 ( Z32:_location Any&:_x ) "libGL.so.1" "glUniform1ui64NV" sys optional Uniform1ui64NV 

	// GLAPI void APIENTRY Uniform2ui64NV (GLint location,GLuint64EXT x,GLuint64EXT y)
	 ( Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glUniform2ui64NV" sys optional Uniform2ui64NV 

	// GLAPI void APIENTRY Uniform3ui64NV (GLint location,GLuint64EXT x,GLuint64EXT y,GLuint64EXT z)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glUniform3ui64NV" sys optional Uniform3ui64NV 

	// GLAPI void APIENTRY Uniform4ui64NV (GLint location,GLuint64EXT x,GLuint64EXT y,GLuint64EXT z,GLuint64EXT w)
	 ( Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glUniform4ui64NV" sys optional Uniform4ui64NV 

	// GLAPI void APIENTRY Uniform1ui64vNV (GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform1ui64vNV" sys optional Uniform1ui64vNV 

	// GLAPI void APIENTRY Uniform2ui64vNV (GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform2ui64vNV" sys optional Uniform2ui64vNV 

	// GLAPI void APIENTRY Uniform3ui64vNV (GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform3ui64vNV" sys optional Uniform3ui64vNV 

	// GLAPI void APIENTRY Uniform4ui64vNV (GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniform4ui64vNV" sys optional Uniform4ui64vNV 

	// GLAPI void APIENTRY GetUniformi64vNV (GLuint program,GLint location,GLint64EXT * params)
	 ( Z32:_program Z32:_location Any&*:_params ) "libGL.so.1" "glGetUniformi64vNV" sys optional GetUniformi64vNV 

	// GLAPI void APIENTRY ProgramUniform1i64NV (GLuint program,GLint location,GLint64EXT x)
	 ( Z32:_program Z32:_location Any&:_x ) "libGL.so.1" "glProgramUniform1i64NV" sys optional ProgramUniform1i64NV 

	// GLAPI void APIENTRY ProgramUniform2i64NV (GLuint program,GLint location,GLint64EXT x,GLint64EXT y)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glProgramUniform2i64NV" sys optional ProgramUniform2i64NV 

	// GLAPI void APIENTRY ProgramUniform3i64NV (GLuint program,GLint location,GLint64EXT x,GLint64EXT y,GLint64EXT z)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glProgramUniform3i64NV" sys optional ProgramUniform3i64NV 

	// GLAPI void APIENTRY ProgramUniform4i64NV (GLuint program,GLint location,GLint64EXT x,GLint64EXT y,GLint64EXT z,GLint64EXT w)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glProgramUniform4i64NV" sys optional ProgramUniform4i64NV 

	// GLAPI void APIENTRY ProgramUniform1i64vNV (GLuint program,GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform1i64vNV" sys optional ProgramUniform1i64vNV 

	// GLAPI void APIENTRY ProgramUniform2i64vNV (GLuint program,GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform2i64vNV" sys optional ProgramUniform2i64vNV 

	// GLAPI void APIENTRY ProgramUniform3i64vNV (GLuint program,GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform3i64vNV" sys optional ProgramUniform3i64vNV 

	// GLAPI void APIENTRY ProgramUniform4i64vNV (GLuint program,GLint location,GLsizei count,const GLint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform4i64vNV" sys optional ProgramUniform4i64vNV 

	// GLAPI void APIENTRY ProgramUniform1ui64NV (GLuint program,GLint location,GLuint64EXT x)
	 ( Z32:_program Z32:_location Any&:_x ) "libGL.so.1" "glProgramUniform1ui64NV" sys optional ProgramUniform1ui64NV 

	// GLAPI void APIENTRY ProgramUniform2ui64NV (GLuint program,GLint location,GLuint64EXT x,GLuint64EXT y)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y ) "libGL.so.1" "glProgramUniform2ui64NV" sys optional ProgramUniform2ui64NV 

	// GLAPI void APIENTRY ProgramUniform3ui64NV (GLuint program,GLint location,GLuint64EXT x,GLuint64EXT y,GLuint64EXT z)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glProgramUniform3ui64NV" sys optional ProgramUniform3ui64NV 

	// GLAPI void APIENTRY ProgramUniform4ui64NV (GLuint program,GLint location,GLuint64EXT x,GLuint64EXT y,GLuint64EXT z,GLuint64EXT w)
	 ( Z32:_program Z32:_location Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glProgramUniform4ui64NV" sys optional ProgramUniform4ui64NV 

	// GLAPI void APIENTRY ProgramUniform1ui64vNV (GLuint program,GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform1ui64vNV" sys optional ProgramUniform1ui64vNV 

	// GLAPI void APIENTRY ProgramUniform2ui64vNV (GLuint program,GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform2ui64vNV" sys optional ProgramUniform2ui64vNV 

	// GLAPI void APIENTRY ProgramUniform3ui64vNV (GLuint program,GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform3ui64vNV" sys optional ProgramUniform3ui64vNV 

	// GLAPI void APIENTRY ProgramUniform4ui64vNV (GLuint program,GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniform4ui64vNV" sys optional ProgramUniform4ui64vNV 

	// GLAPI void APIENTRY GetInternalformatSampleivNV (GLenum target,GLenum internalformat,GLsizei samples,GLenum pname,GLsizei count,GLint * params)
	 ( Z32:_target Z32:_internalformat Z32:_samples Z32:_pname Z32:_count Z32*:_params ) "libGL.so.1" "glGetInternalformatSampleivNV" sys optional GetInternalformatSampleivNV 

	// GLAPI void APIENTRY GetMemoryObjectDetachedResourcesuivNV (GLuint memory,GLenum pname,GLint first,GLsizei count,GLuint * params)
	 ( Z32:_memory Z32:_pname Z32:_first Z32:_count Z32*:_params ) "libGL.so.1" "glGetMemoryObjectDetachedResourcesuivNV" sys optional GetMemoryObjectDetachedResourcesuivNV 

	// GLAPI void APIENTRY ResetMemoryObjectParameterNV (GLuint memory,GLenum pname)
	 ( Z32:_memory Z32:_pname ) "libGL.so.1" "glResetMemoryObjectParameterNV" sys optional ResetMemoryObjectParameterNV 

	// GLAPI void APIENTRY TexAttachMemoryNV (GLenum target,GLuint memory,GLuint64 offset)
	 ( Z32:_target Z32:_memory Any&:_offset ) "libGL.so.1" "glTexAttachMemoryNV" sys optional TexAttachMemoryNV 

	// GLAPI void APIENTRY BufferAttachMemoryNV (GLenum target,GLuint memory,GLuint64 offset)
	 ( Z32:_target Z32:_memory Any&:_offset ) "libGL.so.1" "glBufferAttachMemoryNV" sys optional BufferAttachMemoryNV 

	// GLAPI void APIENTRY TextureAttachMemoryNV (GLuint texture,GLuint memory,GLuint64 offset)
	 ( Z32:_texture Z32:_memory Any&:_offset ) "libGL.so.1" "glTextureAttachMemoryNV" sys optional TextureAttachMemoryNV 

	// GLAPI void APIENTRY NamedBufferAttachMemoryNV (GLuint buffer,GLuint memory,GLuint64 offset)
	 ( Z32:_buffer Z32:_memory Any&:_offset ) "libGL.so.1" "glNamedBufferAttachMemoryNV" sys optional NamedBufferAttachMemoryNV 

	// GLAPI void APIENTRY BufferPageCommitmentMemNV (GLenum target,GLintptr offset,GLsizeiptr size,GLuint memory,GLuint64 memOffset,GLboolean commit)
	 ( Z32:_target Any&:_offset Z32:_size Z32:_memory Any&:_memOffset Bit:_commit ) "libGL.so.1" "glBufferPageCommitmentMemNV" sys optional BufferPageCommitmentMemNV 

	// GLAPI void APIENTRY TexPageCommitmentMemNV (GLenum target,GLint layer,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLuint memory,GLuint64 offset,GLboolean commit)
	 ( Z32:_target Z32:_layer Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_memory Any&:_offset Bit:_commit ) "libGL.so.1" "glTexPageCommitmentMemNV" sys optional TexPageCommitmentMemNV 

	// GLAPI void APIENTRY NamedBufferPageCommitmentMemNV (GLuint buffer,GLintptr offset,GLsizeiptr size,GLuint memory,GLuint64 memOffset,GLboolean commit)
	 ( Z32:_buffer Any&:_offset Z32:_size Z32:_memory Any&:_memOffset Bit:_commit ) "libGL.so.1" "glNamedBufferPageCommitmentMemNV" sys optional NamedBufferPageCommitmentMemNV 

	// GLAPI void APIENTRY TexturePageCommitmentMemNV (GLuint texture,GLint layer,GLint level,GLint xoffset,GLint yoffset,GLint zoffset,GLsizei width,GLsizei height,GLsizei depth,GLuint memory,GLuint64 offset,GLboolean commit)
	 ( Z32:_texture Z32:_layer Z32:_level Z32:_xoffset Z32:_yoffset Z32:_zoffset Z32:_width Z32:_height Z32:_depth Z32:_memory Any&:_offset Bit:_commit ) "libGL.so.1" "glTexturePageCommitmentMemNV" sys optional TexturePageCommitmentMemNV 

	// GLAPI void APIENTRY DrawMeshTasksNV (GLuint first,GLuint count)
	 ( Z32:_first Z32:_count ) "libGL.so.1" "glDrawMeshTasksNV" sys optional DrawMeshTasksNV 

	// GLAPI void APIENTRY DrawMeshTasksIndirectNV (GLintptr indirect)
	 ( Any&:_indirect ) "libGL.so.1" "glDrawMeshTasksIndirectNV" sys optional DrawMeshTasksIndirectNV 

	// GLAPI void APIENTRY MultiDrawMeshTasksIndirectNV (GLintptr indirect,GLsizei drawcount,GLsizei stride)
	 ( Any&:_indirect Z32:_drawcount Z32:_stride ) "libGL.so.1" "glMultiDrawMeshTasksIndirectNV" sys optional MultiDrawMeshTasksIndirectNV 

	// GLAPI void APIENTRY MultiDrawMeshTasksIndirectCountNV (GLintptr indirect,GLintptr drawcount,GLsizei maxdrawcount,GLsizei stride)
	 ( Any&:_indirect Any&:_drawcount Z32:_maxdrawcount Z32:_stride ) "libGL.so.1" "glMultiDrawMeshTasksIndirectCountNV" sys optional MultiDrawMeshTasksIndirectCountNV 

	// GLAPI GLuint APIENTRY GenPathsNV (GLsizei range)
	 ( Z32:_range  -> Z32 ) "libGL.so.1" "glGenPathsNV" sys optional GenPathsNV 

	// GLAPI void APIENTRY DeletePathsNV (GLuint path,GLsizei range)
	 ( Z32:_path Z32:_range ) "libGL.so.1" "glDeletePathsNV" sys optional DeletePathsNV 

	// GLAPI GLboolean APIENTRY IsPathNV (GLuint path)
	 ( Z32:_path  -> Bit ) "libGL.so.1" "glIsPathNV" sys optional IsPathNV 

	// GLAPI void APIENTRY PathCommandsNV (GLuint path,GLsizei numCommands,const GLubyte * commands,GLsizei numCoords,GLenum coordType,const void * coords)
	 ( Z32:_path Z32:_numCommands String:_commands Z32:_numCoords Z32:_coordType Any*:_coords ) "libGL.so.1" "glPathCommandsNV" sys optional PathCommandsNV 

	// GLAPI void APIENTRY PathCoordsNV (GLuint path,GLsizei numCoords,GLenum coordType,const void * coords)
	 ( Z32:_path Z32:_numCoords Z32:_coordType Any*:_coords ) "libGL.so.1" "glPathCoordsNV" sys optional PathCoordsNV 

	// GLAPI void APIENTRY PathSubCommandsNV (GLuint path,GLsizei commandStart,GLsizei commandsToDelete,GLsizei numCommands,const GLubyte * commands,GLsizei numCoords,GLenum coordType,const void * coords)
	 ( Z32:_path Z32:_commandStart Z32:_commandsToDelete Z32:_numCommands String:_commands Z32:_numCoords Z32:_coordType Any*:_coords ) "libGL.so.1" "glPathSubCommandsNV" sys optional PathSubCommandsNV 

	// GLAPI void APIENTRY PathSubCoordsNV (GLuint path,GLsizei coordStart,GLsizei numCoords,GLenum coordType,const void * coords)
	 ( Z32:_path Z32:_coordStart Z32:_numCoords Z32:_coordType Any*:_coords ) "libGL.so.1" "glPathSubCoordsNV" sys optional PathSubCoordsNV 

	// GLAPI void APIENTRY PathStringNV (GLuint path,GLenum format,GLsizei length,const void * pathString)
	 ( Z32:_path Z32:_format Z32:_length Any*:_pathString ) "libGL.so.1" "glPathStringNV" sys optional PathStringNV 

	// GLAPI void APIENTRY PathGlyphsNV (GLuint firstPathName,GLenum fontTarget,const void * fontName,GLbitfield fontStyle,GLsizei numGlyphs,GLenum type,const void * charcodes,GLenum handleMissingGlyphs,GLuint pathParameterTemplate,GLfloat emScale)
	 ( Z32:_firstPathName Z32:_fontTarget Any*:_fontName Z32:_fontStyle Z32:_numGlyphs Z32:_type Any*:_charcodes Z32:_handleMissingGlyphs Z32:_pathParameterTemplate Real:_emScale ) "libGL.so.1" "glPathGlyphsNV" sys optional PathGlyphsNV 

	// GLAPI void APIENTRY PathGlyphRangeNV (GLuint firstPathName,GLenum fontTarget,const void * fontName,GLbitfield fontStyle,GLuint firstGlyph,GLsizei numGlyphs,GLenum handleMissingGlyphs,GLuint pathParameterTemplate,GLfloat emScale)
	 ( Z32:_firstPathName Z32:_fontTarget Any*:_fontName Z32:_fontStyle Z32:_firstGlyph Z32:_numGlyphs Z32:_handleMissingGlyphs Z32:_pathParameterTemplate Real:_emScale ) "libGL.so.1" "glPathGlyphRangeNV" sys optional PathGlyphRangeNV 

	// GLAPI void APIENTRY WeightPathsNV (GLuint resultPath,GLsizei numPaths,const GLuint * paths,const GLfloat * weights)
	 ( Z32:_resultPath Z32:_numPaths Z32*:_paths Real*:_weights ) "libGL.so.1" "glWeightPathsNV" sys optional WeightPathsNV 

	// GLAPI void APIENTRY CopyPathNV (GLuint resultPath,GLuint srcPath)
	 ( Z32:_resultPath Z32:_srcPath ) "libGL.so.1" "glCopyPathNV" sys optional CopyPathNV 

	// GLAPI void APIENTRY InterpolatePathsNV (GLuint resultPath,GLuint pathA,GLuint pathB,GLfloat weight)
	 ( Z32:_resultPath Z32:_pathA Z32:_pathB Real:_weight ) "libGL.so.1" "glInterpolatePathsNV" sys optional InterpolatePathsNV 

	// GLAPI void APIENTRY TransformPathNV (GLuint resultPath,GLuint srcPath,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_resultPath Z32:_srcPath Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glTransformPathNV" sys optional TransformPathNV 

	// GLAPI void APIENTRY PathParameterivNV (GLuint path,GLenum pname,const GLint * value)
	 ( Z32:_path Z32:_pname Z32*:_value ) "libGL.so.1" "glPathParameterivNV" sys optional PathParameterivNV 

	// GLAPI void APIENTRY PathParameteriNV (GLuint path,GLenum pname,GLint value)
	 ( Z32:_path Z32:_pname Z32:_value ) "libGL.so.1" "glPathParameteriNV" sys optional PathParameteriNV 

	// GLAPI void APIENTRY PathParameterfvNV (GLuint path,GLenum pname,const GLfloat * value)
	 ( Z32:_path Z32:_pname Real*:_value ) "libGL.so.1" "glPathParameterfvNV" sys optional PathParameterfvNV 

	// GLAPI void APIENTRY PathParameterfNV (GLuint path,GLenum pname,GLfloat value)
	 ( Z32:_path Z32:_pname Real:_value ) "libGL.so.1" "glPathParameterfNV" sys optional PathParameterfNV 

	// GLAPI void APIENTRY PathDashArrayNV (GLuint path,GLsizei dashCount,const GLfloat * dashArray)
	 ( Z32:_path Z32:_dashCount Real*:_dashArray ) "libGL.so.1" "glPathDashArrayNV" sys optional PathDashArrayNV 

	// GLAPI void APIENTRY PathStencilFuncNV (GLenum func,GLint ref,GLuint mask)
	 ( Z32:_func Z32:_ref Z32:_mask ) "libGL.so.1" "glPathStencilFuncNV" sys optional PathStencilFuncNV 

	// GLAPI void APIENTRY PathStencilDepthOffsetNV (GLfloat factor,GLfloat units)
	 ( Real:_factor Real:_units ) "libGL.so.1" "glPathStencilDepthOffsetNV" sys optional PathStencilDepthOffsetNV 

	// GLAPI void APIENTRY StencilFillPathNV (GLuint path,GLenum fillMode,GLuint mask)
	 ( Z32:_path Z32:_fillMode Z32:_mask ) "libGL.so.1" "glStencilFillPathNV" sys optional StencilFillPathNV 

	// GLAPI void APIENTRY StencilStrokePathNV (GLuint path,GLint reference,GLuint mask)
	 ( Z32:_path Z32:_reference Z32:_mask ) "libGL.so.1" "glStencilStrokePathNV" sys optional StencilStrokePathNV 

	// GLAPI void APIENTRY StencilFillPathInstancedNV (GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLenum fillMode,GLuint mask,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_fillMode Z32:_mask Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glStencilFillPathInstancedNV" sys optional StencilFillPathInstancedNV 

	// GLAPI void APIENTRY StencilStrokePathInstancedNV (GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLint reference,GLuint mask,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_reference Z32:_mask Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glStencilStrokePathInstancedNV" sys optional StencilStrokePathInstancedNV 

	// GLAPI void APIENTRY PathCoverDepthFuncNV (GLenum func)
	 ( Z32:_func ) "libGL.so.1" "glPathCoverDepthFuncNV" sys optional PathCoverDepthFuncNV 

	// GLAPI void APIENTRY CoverFillPathNV (GLuint path,GLenum coverMode)
	 ( Z32:_path Z32:_coverMode ) "libGL.so.1" "glCoverFillPathNV" sys optional CoverFillPathNV 

	// GLAPI void APIENTRY CoverStrokePathNV (GLuint path,GLenum coverMode)
	 ( Z32:_path Z32:_coverMode ) "libGL.so.1" "glCoverStrokePathNV" sys optional CoverStrokePathNV 

	// GLAPI void APIENTRY CoverFillPathInstancedNV (GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLenum coverMode,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_coverMode Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glCoverFillPathInstancedNV" sys optional CoverFillPathInstancedNV 

	// GLAPI void APIENTRY CoverStrokePathInstancedNV (GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLenum coverMode,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_coverMode Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glCoverStrokePathInstancedNV" sys optional CoverStrokePathInstancedNV 

	// GLAPI void APIENTRY GetPathParameterivNV (GLuint path,GLenum pname,GLint * value)
	 ( Z32:_path Z32:_pname Z32*:_value ) "libGL.so.1" "glGetPathParameterivNV" sys optional GetPathParameterivNV 

	// GLAPI void APIENTRY GetPathParameterfvNV (GLuint path,GLenum pname,GLfloat * value)
	 ( Z32:_path Z32:_pname Real*:_value ) "libGL.so.1" "glGetPathParameterfvNV" sys optional GetPathParameterfvNV 

	// GLAPI void APIENTRY GetPathCommandsNV (GLuint path,GLubyte * commands)
	 ( Z32:_path String:_commands ) "libGL.so.1" "glGetPathCommandsNV" sys optional GetPathCommandsNV 

	// GLAPI void APIENTRY GetPathCoordsNV (GLuint path,GLfloat * coords)
	 ( Z32:_path Real*:_coords ) "libGL.so.1" "glGetPathCoordsNV" sys optional GetPathCoordsNV 

	// GLAPI void APIENTRY GetPathDashArrayNV (GLuint path,GLfloat * dashArray)
	 ( Z32:_path Real*:_dashArray ) "libGL.so.1" "glGetPathDashArrayNV" sys optional GetPathDashArrayNV 

	// GLAPI void APIENTRY GetPathMetricsNV (GLbitfield metricQueryMask,GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLsizei stride,GLfloat * metrics)
	 ( Z32:_metricQueryMask Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_stride Real*:_metrics ) "libGL.so.1" "glGetPathMetricsNV" sys optional GetPathMetricsNV 

	// GLAPI void APIENTRY GetPathMetricRangeNV (GLbitfield metricQueryMask,GLuint firstPathName,GLsizei numPaths,GLsizei stride,GLfloat * metrics)
	 ( Z32:_metricQueryMask Z32:_firstPathName Z32:_numPaths Z32:_stride Real*:_metrics ) "libGL.so.1" "glGetPathMetricRangeNV" sys optional GetPathMetricRangeNV 

	// GLAPI void APIENTRY GetPathSpacingNV (GLenum pathListMode,GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLfloat advanceScale,GLfloat kerningScale,GLenum transformType,GLfloat * returnedSpacing)
	 ( Z32:_pathListMode Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Real:_advanceScale Real:_kerningScale Z32:_transformType Real*:_returnedSpacing ) "libGL.so.1" "glGetPathSpacingNV" sys optional GetPathSpacingNV 

	// GLAPI GLboolean APIENTRY IsPointInFillPathNV (GLuint path,GLuint mask,GLfloat x,GLfloat y)
	 ( Z32:_path Z32:_mask Real:_x Real:_y  -> Bit ) "libGL.so.1" "glIsPointInFillPathNV" sys optional IsPointInFillPathNV 

	// GLAPI GLboolean APIENTRY IsPointInStrokePathNV (GLuint path,GLfloat x,GLfloat y)
	 ( Z32:_path Real:_x Real:_y  -> Bit ) "libGL.so.1" "glIsPointInStrokePathNV" sys optional IsPointInStrokePathNV 

	// GLAPI GLfloat APIENTRY GetPathLengthNV (GLuint path,GLsizei startSegment,GLsizei numSegments)
	 ( Z32:_path Z32:_startSegment Z32:_numSegments  -> Real ) "libGL.so.1" "glGetPathLengthNV" sys optional GetPathLengthNV 

	// GLAPI GLboolean APIENTRY PointAlongPathNV (GLuint path,GLsizei startSegment,GLsizei numSegments,GLfloat distance,GLfloat * x,GLfloat * y,GLfloat * tangentX,GLfloat * tangentY)
	 ( Z32:_path Z32:_startSegment Z32:_numSegments Real:_distance Real*:_x Real*:_y Real*:_tangentX Real*:_tangentY  -> Bit ) "libGL.so.1" "glPointAlongPathNV" sys optional PointAlongPathNV 

	// GLAPI void APIENTRY MatrixLoad3x2fNV (GLenum matrixMode,const GLfloat * m)
	 ( Z32:_matrixMode Real*:_m ) "libGL.so.1" "glMatrixLoad3x2fNV" sys optional MatrixLoad3x2fNV 

	// GLAPI void APIENTRY MatrixLoad3x3fNV (GLenum matrixMode,const GLfloat * m)
	 ( Z32:_matrixMode Real*:_m ) "libGL.so.1" "glMatrixLoad3x3fNV" sys optional MatrixLoad3x3fNV 

	// GLAPI void APIENTRY MatrixLoadTranspose3x3fNV (GLenum matrixMode,const GLfloat * m)
	 ( Z32:_matrixMode Real*:_m ) "libGL.so.1" "glMatrixLoadTranspose3x3fNV" sys optional MatrixLoadTranspose3x3fNV 

	// GLAPI void APIENTRY MatrixMult3x2fNV (GLenum matrixMode,const GLfloat * m)
	 ( Z32:_matrixMode Real*:_m ) "libGL.so.1" "glMatrixMult3x2fNV" sys optional MatrixMult3x2fNV 

	// GLAPI void APIENTRY MatrixMult3x3fNV (GLenum matrixMode,const GLfloat * m)
	 ( Z32:_matrixMode Real*:_m ) "libGL.so.1" "glMatrixMult3x3fNV" sys optional MatrixMult3x3fNV 

	// GLAPI void APIENTRY MatrixMultTranspose3x3fNV (GLenum matrixMode,const GLfloat * m)
	 ( Z32:_matrixMode Real*:_m ) "libGL.so.1" "glMatrixMultTranspose3x3fNV" sys optional MatrixMultTranspose3x3fNV 

	// GLAPI void APIENTRY StencilThenCoverFillPathNV (GLuint path,GLenum fillMode,GLuint mask,GLenum coverMode)
	 ( Z32:_path Z32:_fillMode Z32:_mask Z32:_coverMode ) "libGL.so.1" "glStencilThenCoverFillPathNV" sys optional StencilThenCoverFillPathNV 

	// GLAPI void APIENTRY StencilThenCoverStrokePathNV (GLuint path,GLint reference,GLuint mask,GLenum coverMode)
	 ( Z32:_path Z32:_reference Z32:_mask Z32:_coverMode ) "libGL.so.1" "glStencilThenCoverStrokePathNV" sys optional StencilThenCoverStrokePathNV 

	// GLAPI void APIENTRY StencilThenCoverFillPathInstancedNV (GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLenum fillMode,GLuint mask,GLenum coverMode,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_fillMode Z32:_mask Z32:_coverMode Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glStencilThenCoverFillPathInstancedNV" sys optional StencilThenCoverFillPathInstancedNV 

	// GLAPI void APIENTRY StencilThenCoverStrokePathInstancedNV (GLsizei numPaths,GLenum pathNameType,const void * paths,GLuint pathBase,GLint reference,GLuint mask,GLenum coverMode,GLenum transformType,const GLfloat * transformValues)
	 ( Z32:_numPaths Z32:_pathNameType Any*:_paths Z32:_pathBase Z32:_reference Z32:_mask Z32:_coverMode Z32:_transformType Real*:_transformValues ) "libGL.so.1" "glStencilThenCoverStrokePathInstancedNV" sys optional StencilThenCoverStrokePathInstancedNV 

	// GLAPI GLenum APIENTRY PathGlyphIndexRangeNV (GLenum fontTarget,const void * fontName,GLbitfield fontStyle,GLuint pathParameterTemplate,GLfloat emScale,GLuint * baseAndCount)
	 ( Z32:_fontTarget Any*:_fontName Z32:_fontStyle Z32:_pathParameterTemplate Real:_emScale Z32*:_baseAndCount  -> Z32 ) "libGL.so.1" "glPathGlyphIndexRangeNV" sys optional PathGlyphIndexRangeNV 

	// GLAPI GLenum APIENTRY PathGlyphIndexArrayNV (GLuint firstPathName,GLenum fontTarget,const void * fontName,GLbitfield fontStyle,GLuint firstGlyphIndex,GLsizei numGlyphs,GLuint pathParameterTemplate,GLfloat emScale)
	 ( Z32:_firstPathName Z32:_fontTarget Any*:_fontName Z32:_fontStyle Z32:_firstGlyphIndex Z32:_numGlyphs Z32:_pathParameterTemplate Real:_emScale  -> Z32 ) "libGL.so.1" "glPathGlyphIndexArrayNV" sys optional PathGlyphIndexArrayNV 

	// GLAPI GLenum APIENTRY PathMemoryGlyphIndexArrayNV (GLuint firstPathName,GLenum fontTarget,GLsizeiptr fontSize,const void * fontData,GLsizei faceIndex,GLuint firstGlyphIndex,GLsizei numGlyphs,GLuint pathParameterTemplate,GLfloat emScale)
	 ( Z32:_firstPathName Z32:_fontTarget Z32:_fontSize Any*:_fontData Z32:_faceIndex Z32:_firstGlyphIndex Z32:_numGlyphs Z32:_pathParameterTemplate Real:_emScale  -> Z32 ) "libGL.so.1" "glPathMemoryGlyphIndexArrayNV" sys optional PathMemoryGlyphIndexArrayNV 

	// GLAPI void APIENTRY ProgramPathFragmentInputGenNV (GLuint program,GLint location,GLenum genMode,GLint components,const GLfloat * coeffs)
	 ( Z32:_program Z32:_location Z32:_genMode Z32:_components Real*:_coeffs ) "libGL.so.1" "glProgramPathFragmentInputGenNV" sys optional ProgramPathFragmentInputGenNV 

	// GLAPI void APIENTRY GetProgramResourcefvNV (GLuint program,GLenum programInterface,GLuint index,GLsizei propCount,const GLenum * props,GLsizei count,GLsizei * length,GLfloat * params)
	 ( Z32:_program Z32:_programInterface Z32:_index Z32:_propCount Z32*:_props Z32:_count Z32*:_length Real*:_params ) "libGL.so.1" "glGetProgramResourcefvNV" sys optional GetProgramResourcefvNV 

	// GLAPI void APIENTRY FramebufferSampleLocationsfvNV (GLenum target,GLuint start,GLsizei count,const GLfloat * v)
	 ( Z32:_target Z32:_start Z32:_count Real*:_v ) "libGL.so.1" "glFramebufferSampleLocationsfvNV" sys optional FramebufferSampleLocationsfvNV 

	// GLAPI void APIENTRY NamedFramebufferSampleLocationsfvNV (GLuint framebuffer,GLuint start,GLsizei count,const GLfloat * v)
	 ( Z32:_framebuffer Z32:_start Z32:_count Real*:_v ) "libGL.so.1" "glNamedFramebufferSampleLocationsfvNV" sys optional NamedFramebufferSampleLocationsfvNV 

	// GLAPI void APIENTRY ResolveDepthValuesNV (void)
	 ( ) "libGL.so.1" "glResolveDepthValuesNV" sys optional ResolveDepthValuesNV 

	// GLAPI void APIENTRY ScissorExclusiveNV (GLint x,GLint y,GLsizei width,GLsizei height)
	 ( Z32:_x Z32:_y Z32:_width Z32:_height ) "libGL.so.1" "glScissorExclusiveNV" sys optional ScissorExclusiveNV 

	// GLAPI void APIENTRY ScissorExclusiveArrayvNV (GLuint first,GLsizei count,const GLint * v)
	 ( Z32:_first Z32:_count Z32*:_v ) "libGL.so.1" "glScissorExclusiveArrayvNV" sys optional ScissorExclusiveArrayvNV 

	// GLAPI void APIENTRY MakeBufferResidentNV (GLenum target,GLenum access)
	 ( Z32:_target Z32:_access ) "libGL.so.1" "glMakeBufferResidentNV" sys optional MakeBufferResidentNV 

	// GLAPI void APIENTRY MakeBufferNonResidentNV (GLenum target)
	 ( Z32:_target ) "libGL.so.1" "glMakeBufferNonResidentNV" sys optional MakeBufferNonResidentNV 

	// GLAPI GLboolean APIENTRY IsBufferResidentNV (GLenum target)
	 ( Z32:_target  -> Bit ) "libGL.so.1" "glIsBufferResidentNV" sys optional IsBufferResidentNV 

	// GLAPI void APIENTRY MakeNamedBufferResidentNV (GLuint buffer,GLenum access)
	 ( Z32:_buffer Z32:_access ) "libGL.so.1" "glMakeNamedBufferResidentNV" sys optional MakeNamedBufferResidentNV 

	// GLAPI void APIENTRY MakeNamedBufferNonResidentNV (GLuint buffer)
	 ( Z32:_buffer ) "libGL.so.1" "glMakeNamedBufferNonResidentNV" sys optional MakeNamedBufferNonResidentNV 

	// GLAPI GLboolean APIENTRY IsNamedBufferResidentNV (GLuint buffer)
	 ( Z32:_buffer  -> Bit ) "libGL.so.1" "glIsNamedBufferResidentNV" sys optional IsNamedBufferResidentNV 

	// GLAPI void APIENTRY GetBufferParameterui64vNV (GLenum target,GLenum pname,GLuint64EXT * params)
	 ( Z32:_target Z32:_pname Any&*:_params ) "libGL.so.1" "glGetBufferParameterui64vNV" sys optional GetBufferParameterui64vNV 

	// GLAPI void APIENTRY GetNamedBufferParameterui64vNV (GLuint buffer,GLenum pname,GLuint64EXT * params)
	 ( Z32:_buffer Z32:_pname Any&*:_params ) "libGL.so.1" "glGetNamedBufferParameterui64vNV" sys optional GetNamedBufferParameterui64vNV 

	// GLAPI void APIENTRY GetIntegerui64vNV (GLenum value,GLuint64EXT * result)
	 ( Z32:_value Any&*:_result ) "libGL.so.1" "glGetIntegerui64vNV" sys optional GetIntegerui64vNV 

	// GLAPI void APIENTRY Uniformui64NV (GLint location,GLuint64EXT value)
	 ( Z32:_location Any&:_value ) "libGL.so.1" "glUniformui64NV" sys optional Uniformui64NV 

	// GLAPI void APIENTRY Uniformui64vNV (GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glUniformui64vNV" sys optional Uniformui64vNV 

	// GLAPI void APIENTRY GetUniformui64vNV (GLuint program,GLint location,GLuint64EXT * params)
	 ( Z32:_program Z32:_location Any&*:_params ) "libGL.so.1" "glGetUniformui64vNV" sys optional GetUniformui64vNV 

	// GLAPI void APIENTRY ProgramUniformui64NV (GLuint program,GLint location,GLuint64EXT value)
	 ( Z32:_program Z32:_location Any&:_value ) "libGL.so.1" "glProgramUniformui64NV" sys optional ProgramUniformui64NV 

	// GLAPI void APIENTRY ProgramUniformui64vNV (GLuint program,GLint location,GLsizei count,const GLuint64EXT * value)
	 ( Z32:_program Z32:_location Z32:_count Any&*:_value ) "libGL.so.1" "glProgramUniformui64vNV" sys optional ProgramUniformui64vNV 

	// GLAPI void APIENTRY BindShadingRateImageNV (GLuint texture)
	 ( Z32:_texture ) "libGL.so.1" "glBindShadingRateImageNV" sys optional BindShadingRateImageNV 

	// GLAPI void APIENTRY GetShadingRateImagePaletteNV (GLuint viewport,GLuint entry,GLenum * rate)
	 ( Z32:_viewport Z32:_entry Z32*:_rate ) "libGL.so.1" "glGetShadingRateImagePaletteNV" sys optional GetShadingRateImagePaletteNV 

	// GLAPI void APIENTRY GetShadingRateSampleLocationivNV (GLenum rate,GLuint samples,GLuint index,GLint * location)
	 ( Z32:_rate Z32:_samples Z32:_index Z32*:_location ) "libGL.so.1" "glGetShadingRateSampleLocationivNV" sys optional GetShadingRateSampleLocationivNV 

	// GLAPI void APIENTRY ShadingRateImageBarrierNV (GLboolean synchronize)
	 ( Bit:_synchronize ) "libGL.so.1" "glShadingRateImageBarrierNV" sys optional ShadingRateImageBarrierNV 

	// GLAPI void APIENTRY ShadingRateImagePaletteNV (GLuint viewport,GLuint first,GLsizei count,const GLenum * rates)
	 ( Z32:_viewport Z32:_first Z32:_count Z32*:_rates ) "libGL.so.1" "glShadingRateImagePaletteNV" sys optional ShadingRateImagePaletteNV 

	// GLAPI void APIENTRY ShadingRateSampleOrderNV (GLenum order)
	 ( Z32:_order ) "libGL.so.1" "glShadingRateSampleOrderNV" sys optional ShadingRateSampleOrderNV 

	// GLAPI void APIENTRY ShadingRateSampleOrderCustomNV (GLenum rate,GLuint samples,const GLint * locations)
	 ( Z32:_rate Z32:_samples Z32*:_locations ) "libGL.so.1" "glShadingRateSampleOrderCustomNV" sys optional ShadingRateSampleOrderCustomNV 

	// GLAPI void APIENTRY TextureBarrierNV (void)
	 ( ) "libGL.so.1" "glTextureBarrierNV" sys optional TextureBarrierNV 

	// GLAPI void APIENTRY VertexAttribL1i64NV (GLuint index,GLint64EXT x)
	 ( Z32:_index Any&:_x ) "libGL.so.1" "glVertexAttribL1i64NV" sys optional VertexAttribL1i64NV 

	// GLAPI void APIENTRY VertexAttribL2i64NV (GLuint index,GLint64EXT x,GLint64EXT y)
	 ( Z32:_index Any&:_x Any&:_y ) "libGL.so.1" "glVertexAttribL2i64NV" sys optional VertexAttribL2i64NV 

	// GLAPI void APIENTRY VertexAttribL3i64NV (GLuint index,GLint64EXT x,GLint64EXT y,GLint64EXT z)
	 ( Z32:_index Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glVertexAttribL3i64NV" sys optional VertexAttribL3i64NV 

	// GLAPI void APIENTRY VertexAttribL4i64NV (GLuint index,GLint64EXT x,GLint64EXT y,GLint64EXT z,GLint64EXT w)
	 ( Z32:_index Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glVertexAttribL4i64NV" sys optional VertexAttribL4i64NV 

	// GLAPI void APIENTRY VertexAttribL1i64vNV (GLuint index,const GLint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL1i64vNV" sys optional VertexAttribL1i64vNV 

	// GLAPI void APIENTRY VertexAttribL2i64vNV (GLuint index,const GLint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL2i64vNV" sys optional VertexAttribL2i64vNV 

	// GLAPI void APIENTRY VertexAttribL3i64vNV (GLuint index,const GLint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL3i64vNV" sys optional VertexAttribL3i64vNV 

	// GLAPI void APIENTRY VertexAttribL4i64vNV (GLuint index,const GLint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL4i64vNV" sys optional VertexAttribL4i64vNV 

	// GLAPI void APIENTRY VertexAttribL1ui64NV (GLuint index,GLuint64EXT x)
	 ( Z32:_index Any&:_x ) "libGL.so.1" "glVertexAttribL1ui64NV" sys optional VertexAttribL1ui64NV 

	// GLAPI void APIENTRY VertexAttribL2ui64NV (GLuint index,GLuint64EXT x,GLuint64EXT y)
	 ( Z32:_index Any&:_x Any&:_y ) "libGL.so.1" "glVertexAttribL2ui64NV" sys optional VertexAttribL2ui64NV 

	// GLAPI void APIENTRY VertexAttribL3ui64NV (GLuint index,GLuint64EXT x,GLuint64EXT y,GLuint64EXT z)
	 ( Z32:_index Any&:_x Any&:_y Any&:_z ) "libGL.so.1" "glVertexAttribL3ui64NV" sys optional VertexAttribL3ui64NV 

	// GLAPI void APIENTRY VertexAttribL4ui64NV (GLuint index,GLuint64EXT x,GLuint64EXT y,GLuint64EXT z,GLuint64EXT w)
	 ( Z32:_index Any&:_x Any&:_y Any&:_z Any&:_w ) "libGL.so.1" "glVertexAttribL4ui64NV" sys optional VertexAttribL4ui64NV 

	// GLAPI void APIENTRY VertexAttribL1ui64vNV (GLuint index,const GLuint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL1ui64vNV" sys optional VertexAttribL1ui64vNV 

	// GLAPI void APIENTRY VertexAttribL2ui64vNV (GLuint index,const GLuint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL2ui64vNV" sys optional VertexAttribL2ui64vNV 

	// GLAPI void APIENTRY VertexAttribL3ui64vNV (GLuint index,const GLuint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL3ui64vNV" sys optional VertexAttribL3ui64vNV 

	// GLAPI void APIENTRY VertexAttribL4ui64vNV (GLuint index,const GLuint64EXT * v)
	 ( Z32:_index Any&*:_v ) "libGL.so.1" "glVertexAttribL4ui64vNV" sys optional VertexAttribL4ui64vNV 

	// GLAPI void APIENTRY GetVertexAttribLi64vNV (GLuint index,GLenum pname,GLint64EXT * params)
	 ( Z32:_index Z32:_pname Any&*:_params ) "libGL.so.1" "glGetVertexAttribLi64vNV" sys optional GetVertexAttribLi64vNV 

	// GLAPI void APIENTRY GetVertexAttribLui64vNV (GLuint index,GLenum pname,GLuint64EXT * params)
	 ( Z32:_index Z32:_pname Any&*:_params ) "libGL.so.1" "glGetVertexAttribLui64vNV" sys optional GetVertexAttribLui64vNV 

	// GLAPI void APIENTRY VertexAttribLFormatNV (GLuint index,GLint size,GLenum type,GLsizei stride)
	 ( Z32:_index Z32:_size Z32:_type Z32:_stride ) "libGL.so.1" "glVertexAttribLFormatNV" sys optional VertexAttribLFormatNV 

	// GLAPI void APIENTRY BufferAddressRangeNV (GLenum pname,GLuint index,GLuint64EXT address,GLsizeiptr length)
	 ( Z32:_pname Z32:_index Any&:_address Z32:_length ) "libGL.so.1" "glBufferAddressRangeNV" sys optional BufferAddressRangeNV 

	// GLAPI void APIENTRY VertexFormatNV (GLint size,GLenum type,GLsizei stride)
	 ( Z32:_size Z32:_type Z32:_stride ) "libGL.so.1" "glVertexFormatNV" sys optional VertexFormatNV 

	// GLAPI void APIENTRY NormalFormatNV (GLenum type,GLsizei stride)
	 ( Z32:_type Z32:_stride ) "libGL.so.1" "glNormalFormatNV" sys optional NormalFormatNV 

	// GLAPI void APIENTRY ColorFormatNV (GLint size,GLenum type,GLsizei stride)
	 ( Z32:_size Z32:_type Z32:_stride ) "libGL.so.1" "glColorFormatNV" sys optional ColorFormatNV 

	// GLAPI void APIENTRY IndexFormatNV (GLenum type,GLsizei stride)
	 ( Z32:_type Z32:_stride ) "libGL.so.1" "glIndexFormatNV" sys optional IndexFormatNV 

	// GLAPI void APIENTRY TexCoordFormatNV (GLint size,GLenum type,GLsizei stride)
	 ( Z32:_size Z32:_type Z32:_stride ) "libGL.so.1" "glTexCoordFormatNV" sys optional TexCoordFormatNV 

	// GLAPI void APIENTRY EdgeFlagFormatNV (GLsizei stride)
	 ( Z32:_stride ) "libGL.so.1" "glEdgeFlagFormatNV" sys optional EdgeFlagFormatNV 

	// GLAPI void APIENTRY SecondaryColorFormatNV (GLint size,GLenum type,GLsizei stride)
	 ( Z32:_size Z32:_type Z32:_stride ) "libGL.so.1" "glSecondaryColorFormatNV" sys optional SecondaryColorFormatNV 

	// GLAPI void APIENTRY FogCoordFormatNV (GLenum type,GLsizei stride)
	 ( Z32:_type Z32:_stride ) "libGL.so.1" "glFogCoordFormatNV" sys optional FogCoordFormatNV 

	// GLAPI void APIENTRY VertexAttribFormatNV (GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride)
	 ( Z32:_index Z32:_size Z32:_type Bit:_normalized Z32:_stride ) "libGL.so.1" "glVertexAttribFormatNV" sys optional VertexAttribFormatNV 

	// GLAPI void APIENTRY VertexAttribIFormatNV (GLuint index,GLint size,GLenum type,GLsizei stride)
	 ( Z32:_index Z32:_size Z32:_type Z32:_stride ) "libGL.so.1" "glVertexAttribIFormatNV" sys optional VertexAttribIFormatNV 

	// GLAPI void APIENTRY GetIntegerui64i_vNV (GLenum value,GLuint index,GLuint64EXT * result)
	 ( Z32:_value Z32:_index Any&*:_result ) "libGL.so.1" "glGetIntegerui64i_vNV" sys optional GetIntegerui64i_vNV 

	// GLAPI void APIENTRY ViewportSwizzleNV (GLuint index,GLenum swizzlex,GLenum swizzley,GLenum swizzlez,GLenum swizzlew)
	 ( Z32:_index Z32:_swizzlex Z32:_swizzley Z32:_swizzlez Z32:_swizzlew ) "libGL.so.1" "glViewportSwizzleNV" sys optional ViewportSwizzleNV 

	// GLAPI void APIENTRY FramebufferTextureMultiviewOVR (GLenum target,GLenum attachment,GLuint texture,GLint level,GLint baseViewIndex,GLsizei numViews)
	 ( Z32:_target Z32:_attachment Z32:_texture Z32:_level Z32:_baseViewIndex Z32:_numViews ) "libGL.so.1" "glFramebufferTextureMultiviewOVR" sys optional FramebufferTextureMultiviewOVR 

end


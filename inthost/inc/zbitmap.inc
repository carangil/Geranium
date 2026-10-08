section zbitmap
 type Bitmap
 Z32 var size
 Z32 var w
 Z32 var h
 Z32 var format
 Byte* var data
 [Z32]$ var data32
 end
 (Z32:w Z32:h Z32:format -> Bitmap*$) "_INTERNAL" "zbitmap_mk" sys new
 (String:filename Z32:flags -> Bitmap*$) "_INTERNAL" "zbitmap_load_tga" sys load_tga
end

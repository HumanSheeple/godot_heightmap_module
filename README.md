10/06/17
VERY BUGGY!
I got the size/opacity bars and line edit all hooked up. I have the add subtract and smooth function working but the module crashes after a couple of minutes of terrain modification. I ran it in debug mode and the mesher seems to at some point parse a null index in one of the arrays in mesh_add_surface_from_arrays 
I did a long verbose valgrind and it seems godot 3.0 has a leak somewhere I think this leak might be leaking into the terrain data somehow, not sure.

#version 130
//unsupported though, isn't it? #extension GL_EXT_gpu_shader4 : enable

// for a 1600x900 screen, this results in an image of 50x100 with each pixel representing a tile
uniform sampler2D tilemap_pixels;

// a runtime generated spritesheet of all tiles we know, each tile being 16x16 (so 48x16 if there are 3 tile types)
uniform sampler2D tile_types_super_texture;

// offset (remainder of view coordinates versus tile size). to achieve per-pixel smooth scrolling
uniform vec2 offset;

//FIXME: stop hardcoding ..
ivec2 TILE_SIZE = ivec2(16, 16);

void main()
{
    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);

    ivec2 screen_coordinates = ivec2(gl_FragCoord.x + int(offset.x), gl_FragCoord.y - int(offset.y) + TILE_SIZE.y);

    // find the pixel (RGBA) values in the tilemap pixel representation that is what we're
    // currently interested in.
    vec4 currentPixel = texelFetch(tilemap_pixels, screen_coordinates / TILE_SIZE, 0);

    ivec2 tileCoordinate;
    tileCoordinate.x = (int(currentPixel.r * 255.0) ) * TILE_SIZE.x + (screen_coordinates.x % TILE_SIZE.x);
    tileCoordinate.y = (screen_coordinates.y) % TILE_SIZE.y;

    vec4 tileColor = texelFetch(tile_types_super_texture, tileCoordinate, 0);

    gl_FragColor = tileColor;
}

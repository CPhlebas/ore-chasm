#version 130

#extension GL_ARB_fragment_coord_conventions : enable

//for a 1600x900 screen, this results in an image of 50x100 with each pixel representing a tile
uniform sampler2D tilemap_pixels;
//a runtime generated spritesheet of all tiles we know, each tile being 16x16 (so 32x32 if there are 3 tile types)
uniform sampler2D tile_types_super_texture;

uniform vec2 screen_size;

vec2 TILE_SIZE = vec2(16.0, 16.0);

//in vec4 gl_FragCoord;  // redeclaration that changes nothing is allowed
//    //============================================
//    // GL_ARB_fragment_coord_conventions, to change coord system to top-left
//    // origin vs. bottom left
//
//    // All the following are allowed redeclarations that change behavior
//    layout(origin_upper_left) in vec4 gl_FragCoord;
//    layout(pixel_center_integer) in vec4 gl_FragCoord;
//    layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;
//    //============================================
//
void main()
{

    vec2 screen_size = vec2(1600.0,900.0);

    ivec2 tilemap_pixels_size = textureSize(tilemap_pixels, 0);
    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);

//========================================================================================

    // the coordinates of the actual pixel we are setting, between 0, 0 and SCREEN_W, SCREEN_H
    vec2 screen_coordinates = gl_FragCoord.xy;

    // find the pixel (RGBA) values in the tilemap pixel representation that is what we're
    // currently interested in.
    vec4 currentPixel = texture2D(tilemap_pixels, screen_coordinates / TILE_SIZE);

    vec2 tileCoordinate;
//KINSD OF WORKS    tileCoordinate.x = currentPixel.r * TILE_SIZE.x * gl_TexCoord[0].x;
    tileCoordinate.x = currentPixel.r * TILE_SIZE.x;//(currentPixel.r * TILE_SIZE.x) + (screen_coordinates.x / TILE_SIZE.x);//+ (screen_coordinates.x/TILE_SIZE.x);
    tileCoordinate.y = screen_coordinates.y / (TILE_SIZE.y);

    vec4 tileColor = texture2D(tile_types_super_texture, tileCoordinate);

    gl_FragColor = tileColor;

//    gl_FragColor.b = 0.; //tilemap_pixel_coord.x;
//    gl_FragColor.g = 0.;
    gl_FragColor.a = 1.;
}

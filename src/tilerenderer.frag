#version 130

//#extension GL_ARB_fragment_coord_conventions : enable

//for a 1600x900 screen, this results in an image of 50x100 with each pixel representing a tile
uniform sampler2D tilemap_pixels;
//a runtime generated spritesheet of all tiles we know, each tile being 16x16 (so 32x32 if there are 3 tile types)
uniform sampler2D tile_types_super_texture;

//uniform vec2 screen_size;

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
//gl_FragCoord.y / window_height
void main()
{

    vec2 screen_size = vec2(1600.0,900.0);

//    ivec2 tilemap_pixels_size = textureSize(tilemap_pixels, 0);
    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);

//========================================================================================

    //converted, ranges from 0-1. because texture coordinates are in 0-1
    vec2 screen_coordinates = gl_FragCoord.xy / screen_size;

    // find the pixel (RGBA) values in the tilemap pixel representation that is what we're
    // currently interested in.
    vec4 currentPixel = texture2D(tilemap_pixels, screen_coordinates / TILE_SIZE);
    vec2 tileCoordinate;
//KINSD OF WORKS
//tileCoordinate.x = currentPixel.r * TILE_SIZE.x * gl_TexCoord[0].x;
//tileCoordinate.x = 1.;
//    tileCoordinate.x = currentPixel.r * 256.0 * screen_coordinates.x * TILE_SIZE.x;//(currentPixel.r * TILE_SIZE.x) ;//+ (screen_coordinates.x / (TILE_SIZE.x*2));//+ (screen_coordinates.x/TILE_SIZE.x);
 //   tileCoordinate.y = 1;//screen_coordinates.y / (TILE_SIZE.y*2);
//currentPixel.r * 256.0 * screen_coordinates.y * TILE_SIZE.x

tileCoordinate.x = currentPixel.r * 16. * 255. + screen_coordinates.x/tilemap_size.x; //* 16. + 2.;
tileCoordinate.y = currentPixel.r / tilemap_size.y;//currentPixel.r;

    vec4 tileColor = texture2D(tile_types_super_texture, tileCoordinate / tilemap_size);

    gl_FragColor = tileColor;
//    gl_FragColor = currentPixel;

    gl_FragColor.a = 1.;
}

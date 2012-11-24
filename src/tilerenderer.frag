#version 130

//for a 1600x900 screen, this results in an image of 50x100 with each pixel representing a tile
uniform sampler2D tilemap_pixels;
//a runtime generated spritesheet of all tiles we know, each tile being 16x16 (so 48x16 if there are 3 tile types)
uniform sampler2D tile_types_super_texture;

//uniform vec2 screen_size;

vec2 TILE_SIZE = vec2(16.0, 16.0);


void main()
{
    vec2 screen_size = vec2(1600.0,900.0);

    //ivec2 tilemap_pixels_size = textureSize(tilemap_pixels, 0);
    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);

//========================================================================================

    vec2 screen_coordinates = gl_FragCoord.xy / screen_size;

    // find the pixel (RGBA) values in the tilemap pixel representation that is what we're
    // currently interested in.
    vec4 currentPixel = texture2D(tilemap_pixels, screen_coordinates);
    vec2 tileCoordinate;

    tileCoordinate.x = (currentPixel.r * 255.0) + (screen_coordinates.x/16);//currentPixel.r * 255. * (screen_coordinates.x*16);
    tileCoordinate.y = 1/screen_size.y;//screen_coordinates.y;// * TILE_SIZE.y;

    //FIXME: use texelfetch instead
    vec4 tileColor = texture2D(tile_types_super_texture, tileCoordinate);

    gl_FragColor = tileColor;
}
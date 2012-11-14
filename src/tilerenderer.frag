#version 130

//for a 1600x900 screen, this results in an image of 50x100 with each pixel representing a tile
uniform sampler2D tilemap_pixels;
//a runtime generated spritesheet of all tiles we know, each tile being 16x16 (so 32x32 if there are 3 tile types)
//uniform sampler2D tile_types_super_texture;

//uniform vec2 screen_size;

const float TILE_SIZE = 16.0;

void main()
{

//FIXME:    vec2 screen_size = vec2(1600.0,900.0);

//FIXME:    ivec2 tilemap_pixels_size = textureSize(tilemap_pixels, 0);
//    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);

//========================================================================================

    // the coordinates of the actual pixel we are setting, between 0, 0 and SCREEN_W, SCREEN_H
//FIXME    vec2 screen_coordinates = gl_FragCoord.xy / screen_size;

    // find where we are in the pixel-based tile representation map (tilemap_pixels)
    // (by dividing the screen size by TILE_SIZE...)
    // xmax = 256, ymax = 256
//FIXME    vec2 tilemap_pixel_coord = (gl_TexCoord[0].xy * tilemap_pixels_size) ; /// TILE_SIZE;


    // find the pixel (RGBA) values in the tilemap pixel representation that is what we're
    // currently interested in.
    vec4 currentTile = texture2D(tilemap_pixels, vec2(0.,0.));//tilemap_pixel_coord);//screen_coordinates );

//    vec2 tileTypePosition = vec2(currentTile.r, 0.);
 //   vec4 tileData = texture2D(tile_types_super_texture, tileTypePosition);

    gl_FragColor.r = currentTile.r; //tileData;

/*
    if (0.) {
        gl_FragColor.b = 1; //tilemap_pixel_coord.y;
    }


*/
    gl_FragColor.b = 0.; //tilemap_pixel_coord.x;
    gl_FragColor.g = 1.;
    gl_FragColor.a = 1.;
}


/*
  vec2 sizeGraphics = textureSize2D(tileGraphics,0);
  vec2 sizeRatio    = sizeTilemap/sizeGraphics;
  float tilesPerLine  =sizeGraphics.x/TILE_SIZE;
  float tilesPerColumn=sizeGraphics.y/TILE_SIZE;

  // Compute the internal texel coordinates in the tile (0-31)
  vec2 internalPos=mod(gl_TexCoord[0]*sizeTilemap,TILE_SIZE);       //*sizeRatio;

  // Get the tile index
  vec2 tileIndexPos=gl_TexCoord[0].xy;
  tileIndexPos.x=tileIndexPos.x/TILE_SIZE;
  tileIndexPos.y=tileIndexPos.y/TILE_SIZE;
  vec4 tileIndex = texture2D(tilemap,tileIndexPos);

  // Find the X/Y tile coordinate from the tileIndex for the base texture
  float index=tileIndex.a*256.0;

  vec2 baseTilePos;
  baseTilePos.x=(TILE_SIZE/sizeGraphics.x)*floor(mod(index,tilesPerLine));
  baseTilePos.y=(TILE_SIZE/sizeGraphics.y)*floor(index/tilesPerLine);
  vec4 baseColor=texture2D(tileGraphics,internalPos/sizeGraphics+baseTilePos);

  // Find the X/Y tile coordinate from the tileIndex for the overlay texture
  index=tileIndex.r*256.0;
  vec2 overlayTilePos;
  overlayTilePos.x=(TILE_SIZE/sizeGraphics.x)*floor(mod(index,tilesPerLine));
  overlayTilePos.y=(TILE_SIZE/sizeGraphics.y)*floor(index/tilesPerLine);
  vec4 overlayColor=texture2D(tileOverlayGraphics,internalPos/sizeGraphics+overlayTilePos);

  // Find the values for the shadow texture
  index=tileIndex.g*256.0;
  vec2 shadowTilePos;
  shadowTilePos.x=(TILE_SIZE/sizeGraphics.x)*floor(mod(index,tilesPerLine));
  shadowTilePos.y=(TILE_SIZE/sizeGraphics.y)*floor(index/tilesPerLine);
  vec4 shadowColor=texture2D(tileShadowGraphics,internalPos/sizeGraphics+shadowTilePos);

  // Blend the base color with the overlay texture
  gl_FragColor = baseColor*(1-overlayColor.a) + overlayColor*overlayColor.a;

  if (shadowColor.a)
  {
    gl_FragColor/=2;
  }
  gl_FragColor.a=1;
*/


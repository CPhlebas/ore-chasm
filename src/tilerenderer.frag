#version 130

uniform sampler2D tile_types_super_texture;
// the texture that we send this shader, which is a pixel-based representation of the current renderable tiles
uniform sampler2D texture;

const float TILE_SIZE = 16.0;

void main()
{
  // Get the dimensions of the tiles texture
//  ivec2 tileTypesSize  = textureSize(tileTypesSuperTexture, 0);

  //  float factor = 1.0 / (16.0 + 0.001);
//        vec2 pos = floor(gl_TexCoord[0].xy * factor + 0.5) / factor;
 //       gl_FragColor = texture2D(tileTypesSuperTexture, pos) * gl_Color;

//    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);
//    gl_TexCoord[0];

    ivec2 texture_size = textureSize(texture, 0);
    ivec2 tilemap_size = textureSize(tile_types_super_texture, 0);

//    vec2 screen_coordinates = gl_FragCoord.xy / uniform_target_size;
//    vec2 middle_position = vec2(0.5, 0.5);

//    float layer_difference = uniform_layer_z - uniform_camera_z;
//    vec2 perspective_modifier = (screen_coordinates - middle_position) * layer_difference;
 //   screen_coordinates -= perspective_modifier / uniform_target_size * 2;

    vec2 screen_coordinates = gl_FragCoord.xy;
//    vec4 data = texture2D(tile_types_super_texture, screen_coordinates * TILE_SIZE);
//    vec4 renderableTiles = texture2D(texture, texture_size);

    // the tile that we are actually rendering
    vec4 renderableTile = texture2D


//    vec2 texel_internal = mod(screen_coordinates * vec2(1, -1), TILE_SIZE) / texture_size;
//    vec2 texture_start = data.rg * texture_size / TILE_SIZE;
//    gl_FragColor = vec4(renderableTiles.rg + data.rg, 0, 1);//texture2D(uniform_texture, texture_start + texel_internal);
















//
//    gl_FragColor.r = 0;
//    gl_FragColor.b = 255;
//    gl_FragColor.g = 255;
//    gl_FragColor.a = 255;
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


// r3d_cubemap.inc -- R3D Cubemap Module.

// ========================================
// CONSTANTS
// ========================================

// Note: In Pascal, we'll create a function to initialize the sky structure
// instead of using a macro

// ========================================
// ENUM TYPES
// ========================================

type
  {*
   * @brief Supported cubemap source layouts.
   *
   * Used when converting an image into a cubemap. AUTO_DETECT tries to guess
   * the layout based on image dimensions.
   *}
  TR3D_CubemapLayout = (
    R3D_CUBEMAP_LAYOUT_AUTO_DETECT = 0,         ///< Automatically detect layout type
    R3D_CUBEMAP_LAYOUT_LINE_VERTICAL,           ///< Layout is defined by a vertical line with faces
    R3D_CUBEMAP_LAYOUT_LINE_HORIZONTAL,         ///< Layout is defined by a horizontal line with faces
    R3D_CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     ///< Layout is defined by a 3x4 cross with cubemap faces
    R3D_CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,     ///< Layout is defined by a 4x3 cross with cubemap faces
    R3D_CUBEMAP_LAYOUT_PANORAMA                 ///< Layout is defined by an equirectangular panorama
  );

// ========================================
// STRUCT TYPES
// ========================================

type
  {*
   * @brief GPU cubemap texture.
   *
   * Holds the OpenGL texture handle and its base resolution (per face).
   *}
  TR3D_Cubemap = record
    texture: cuint32;
    fbo: cuint32;
    size: Integer;
  end;
  PR3D_Cubemap = ^TR3D_Cubemap;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads a cubemap from an image file.
 *
 * The layout parameter tells how faces are arranged inside the source image.
 *
 * @param fileName Path to the image file containing cubemap faces
 * @param layout Arrangement of faces in the source image
 * @return Loaded cubemap texture
 *}
function R3D_LoadCubemap(fileName: PChar; layout: TR3D_CubemapLayout): TR3D_Cubemap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadCubemap';

{*
 * @brief Builds a cubemap from an existing Image.
 *
 * Same behavior as R3D_LoadCubemap(), but without loading from disk.
 *
 * @param image Source image containing cubemap faces
 * @param layout Arrangement of faces in the source image
 * @return Generated cubemap texture
 *}
function R3D_LoadCubemapFromImage(image: TImage; layout: TR3D_CubemapLayout): TR3D_Cubemap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadCubemapFromImage';

{*
 * @brief Releases GPU resources associated with a cubemap.
 *
 * @param cubemap Cubemap to unload
 *}
procedure R3D_UnloadCubemap(cubemap: TR3D_Cubemap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadCubemap';











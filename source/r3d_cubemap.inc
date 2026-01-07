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
    size: cint;
  end;
  PR3D_Cubemap = ^TR3D_Cubemap;

  {*
   * @brief Parameters for procedural sky generation.
   *
   * Curves control gradient falloff (lower = sharper transition at horizon).
   *}
  TR3D_CubemapSky = record
    skyTopColor: TColor;          // Sky color at zenith
    skyHorizonColor: TColor;      // Sky color at horizon
    skyHorizonCurve: Single;      // Gradient curve exponent (0.01 - 1.0, typical: 0.15)
    skyEnergy: Single;            // Sky brightness multiplier

    groundBottomColor: TColor;    // Ground color at nadir
    groundHorizonColor: TColor;   // Ground color at horizon
    groundHorizonCurve: Single;   // Gradient curve exponent (typical: 0.02)
    groundEnergy: Single;         // Ground brightness multiplier

    sunDirection: TVector3;       // Direction from which light comes (can take not normalized)
    sunColor: TColor;             // Sun disk color
    sunSize: Single;              // Sun angular size in radians (real sun: ~0.0087 rad = 0.5°)
    sunCurve: Single;             // Sun edge softness exponent (typical: 0.15)
    sunEnergy: Single;            // Sun brightness multiplier
  end;
  PR3D_CubemapSky = ^TR3D_CubemapSky;

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
 * @brief Generates a procedural sky cubemap.
 *
 * Creates a GPU cubemap with procedural gradient sky and sun rendering.
 * The cubemap is ready for use as environment map or IBL source.
 *
 * @param size Resolution of each cubemap face
 * @param params Sky generation parameters
 * @return Generated procedural sky cubemap
 *}
function R3D_GenCubemapSky(size: cint; params: TR3D_CubemapSky): TR3D_Cubemap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenCubemapSky';

{*
 * @brief Releases GPU resources associated with a cubemap.
 *
 * @param cubemap Cubemap to unload
 *}
procedure R3D_UnloadCubemap(cubemap: TR3D_Cubemap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadCubemap';

{*
 * @brief Updates an existing procedural sky cubemap.
 *
 * Re-renders the cubemap with new parameters. Faster than unload + generate
 * when animating sky conditions (time of day, weather, etc.).
 *
 * @param cubemap Pointer to the cubemap to update
 * @param params New sky generation parameters
 *}
procedure R3D_UpdateCubemapSky(cubemap: PR3D_Cubemap; params: TR3D_CubemapSky); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateCubemapSky';

{*
 * @brief Helper function to initialize sky parameters with default values.
 *
 * Returns sky parameters initialized with the same values as R3D_CUBEMAP_SKY_BASE macro.
 *}










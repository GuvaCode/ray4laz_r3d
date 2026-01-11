// r3d_ambient_map.inc -- R3D Ambient Map Module.


// ========================================
// FLAG TYPES
// ========================================

type
  {*
   * @brief Bit-flags controlling what components are generated.
   *
   * - R3D_AMBIENT_ILLUMINATION -> generate diffuse irradiance
   * - R3D_AMBIENT_REFLECTION   -> generate specular prefiltered map
   *}
  TR3D_AmbientFlag = cuint32;

const
  R3D_AMBIENT_ILLUMINATION    = (1 shl 0);
  R3D_AMBIENT_REFLECTION      = (1 shl 1);

// ========================================
// STRUCT TYPES
// ========================================

type
  {*
   * @brief Global environment lighting data.
   *
   * An ambient map is built from a cubemap (like a skybox)
   * and preprocessed into two specialized textures:
   *
   *  - irradiance:
   *      Low-frequency lighting used for diffuse IBL.
   *      Captures soft ambient light from all directions.
   *
   *  - prefilter:
   *      Mipmapped environment used for specular reflections.
   *      Higher mip levels simulate rougher surfaces.
   *
   * Both textures are derived from the same source cubemap,
   * but serve different shading purposes.
   *}
  TR3D_AmbientMap = record
    flags: TR3D_AmbientFlag;  ///< Components generated for this map
    irradiance: cuint32;     ///< Diffuse IBL cubemap (may be 0 if not generated)
    prefilter: cuint32;      ///< Specular prefiltered cubemap (may be 0 if not generated)
  end;
  PR3D_AmbientMap = ^TR3D_AmbientMap;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads a ambient map from an image file.
 *
 * The layout parameter tells how faces are arranged inside the source image.
 *}
function R3D_LoadAmbientMap(const fileName: PChar; layout: TR3D_CubemapLayout; flag: TR3D_AmbientFlag): TR3D_AmbientMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAmbientMap';

{*
 * @brief Builds a ambient map from an existing Image.
 *
 * Same behavior as R3D_LoadAmbientMap(), but without loading from disk.
 *}
function R3D_LoadAmbientMapFromImage(image: TImage; layout: TR3D_CubemapLayout; flag: TR3D_AmbientFlag): TR3D_AmbientMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAmbientMapFromImage';

{*
 * @brief Generates an ambient map from a cubemap.
 *
 * The source cubemap should usually be an HDR sky/environment.
 *
 * Depending on the provided flags, this function:
 *  - convolves the cubemap into diffuse irradiance
 *  - builds a mipmapped prefiltered cubemap for reflections
 *
 * @param cubemap Source cubemap (environment / sky).
 * @param flags   Which components to generate (irradiance, reflection, or both).
 * @return A fully initialized ambient map.
 *}
function R3D_GenAmbientMap(cubemap: TR3D_Cubemap; flags: TR3D_AmbientFlag): TR3D_AmbientMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenAmbientMap';

{*
 * @brief Frees the textures used by an ambient map.
 *
 * After this call, the ambient map is no longer valid.
 *
 * @param ambientMap Ambient map to unload
 *}
procedure R3D_UnloadAmbientMap(ambientMap: TR3D_AmbientMap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAmbientMap';

{*
 * @brief Rebuilds an existing ambient map from a new cubemap.
 *
 * Use this when the environment changes dynamically (time of day,
 * weather, interior/exterior transitions, etc).
 *
 * Only the components enabled in `ambientMap.flags` are regenerated.
 *
 * @param ambientMap Existing ambient map to update.
 * @param cubemap    New cubemap source.
 *}
procedure R3D_UpdateAmbientMap(ambientMap: TR3D_AmbientMap; cubemap: TR3D_Cubemap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateAmbientMap';



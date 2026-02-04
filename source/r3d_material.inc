// R3D Material Module.


// ========================================
// ENUMS TYPES
// ========================================

{*
 * @brief Transparency modes.
 *
 * This enumeration defines how a material handles transparency during rendering.
 * It controls whether transparency is disabled, rendered using a depth pre-pass,
 * or rendered with standard alpha blending.
 *}
type
  TR3D_TransparencyMode = (
    R3D_TRANSPARENCY_DISABLED,      ///< No transparency, supports alpha cutoff.
    R3D_TRANSPARENCY_PREPASS,       ///< Supports transparency with shadows. Writes shadows for alpha > 0.1 and depth for alpha > 0.99.
    R3D_TRANSPARENCY_ALPHA          ///< Standard transparency without shadows or depth writes.
  );

{*
 * @brief Billboard modes.
 *
 * This enumeration defines how a 3D object aligns itself relative to the camera.
 * It provides options to disable billboarding or to enable specific modes of alignment.
 *}
type
  TR3D_BillboardMode = (
    R3D_BILLBOARD_DISABLED,         ///< Billboarding is disabled; the object retains its original orientation.
    R3D_BILLBOARD_FRONT,            ///< Full billboarding; the object fully faces the camera, rotating on all axes.
    R3D_BILLBOARD_Y_AXIS            ///< Y-axis constrained billboarding; the object rotates only around the Y-axis,
                                    ///< keeping its "up" orientation fixed. This is suitable for upright objects like characters or signs.
  );

{*
 * @brief Blend modes.
 *
 * Defines common blending modes used in 3D rendering to combine source and destination colors.
 * @note The blend mode is applied only if you are in forward rendering mode or auto-detect mode.
 *}
type
  TR3D_BlendMode = (
    R3D_BLEND_MIX,                  ///< Default mode: the result will be opaque or alpha blended depending on the transparency mode.
    R3D_BLEND_ADDITIVE,             ///< Additive blending: source color is added to the destination, making bright effects.
    R3D_BLEND_MULTIPLY,             ///< Multiply blending: source color is multiplied with the destination, darkening the image.
    R3D_BLEND_PREMULTIPLIED_ALPHA   ///< Premultiplied alpha blending: source color is blended with the destination assuming the source color is already multiplied by its alpha.
  );

{*
 * @brief Depth comparison modes.
 *
 * Defines how fragments are tested against the depth buffer during rendering.
 * @note The depth mode affects both forward and deferred rendering passes.
 *}
type
  TR3D_DepthMode = (
    R3D_DEPTH_LESS = 0,     ///< Passes if depth < depth buffer (default)
    R3D_DEPTH_LEQUAL,       ///< Passes if depth <= depth buffer
    R3D_DEPTH_EQUAL,        ///< Passes if depth == depth buffer
    R3D_DEPTH_GREATER,      ///< Passes if depth > depth buffer
    R3D_DEPTH_GEQUAL,       ///< Passes if depth >= depth buffer
    R3D_DEPTH_NOTEQUAL,     ///< Passes if depth != depth buffer
    R3D_DEPTH_ALWAYS,       ///< Always passes
    R3D_DEPTH_NEVER         ///< Never passes
  );

{*
 * @brief Face culling modes.
 *
 * Specifies which faces of a geometry are discarded during rendering based on their winding order.
 *}
type
  TR3D_CullMode = (
    R3D_CULL_NONE,          ///< No culling; all faces are rendered.
    R3D_CULL_BACK,          ///< Cull back-facing polygons (faces with clockwise winding order).
    R3D_CULL_FRONT          ///< Cull front-facing polygons (faces with counter-clockwise winding order).
  );

// ========================================
// STRUCTS TYPES
// ========================================

type
  {*
   * @brief Albedo (base color) map.
   *
   * Provides the base color texture and a color multiplier.
   *}
  TR3D_AlbedoMap = record
    texture: TTexture2D;  ///< Base color texture (default: WHITE)
    color: TColor;        ///< Color multiplier (default: WHITE)
  end;

  {*
   * @brief Emission map.
   *
   * Provides emission texture, color, and energy multiplier.
   *}
  TR3D_EmissionMap = record
    texture: TTexture2D;  ///< Emission texture (default: WHITE)
    color: TColor;        ///< Emission color (default: WHITE)
    energy: Single;       ///< Emission strength (default: 0.0f)
  end;

  {*
   * @brief Normal map.
   *
   * Provides normal map texture and scale factor.
   *}
  TR3D_NormalMap = record
    texture: TTexture2D;  ///< Normal map texture (default: Front Facing)
    scale: Single;        ///< Normal scale (default: 1.0f)
  end;

  {*
   * @brief Combined Occlusion-Roughness-Metalness (ORM) map.
   *
   * Provides texture and individual multipliers for occlusion, roughness, and metalness.
   *}
  TR3D_OrmMap = record
    texture: TTexture2D;  ///< ORM texture (default: WHITE)
    occlusion: Single;    ///< Occlusion multiplier (default: 1.0f)
    roughness: Single;    ///< Roughness multiplier (default: 1.0f)
    metalness: Single;    ///< Metalness multiplier (default: 0.0f)
  end;

  {*
   * @brief Material definition.
   *
   * Combines multiple texture maps and rendering parameters for shading.
   *}
  TR3D_Material = record
    albedo: TR3D_AlbedoMap;                   ///< Albedo map
    emission: TR3D_EmissionMap;               ///< Emission map
    normal: TR3D_NormalMap;                   ///< Normal map
    orm: TR3D_OrmMap;                         ///< Occlusion-Roughness-Metalness map

    uvOffset: TVector2;                       ///< UV offset (default: {0.0f, 0.0f})
    uvScale: TVector2;                        ///< UV scale (default: {1.0f, 1.0f})
    alphaCutoff: Single;                      ///< Alpha cutoff threshold (default: 0.01f)

    transparencyMode: TR3D_TransparencyMode;  ///< Transparency mode (default: DISABLED)
    billboardMode: TR3D_BillboardMode;        ///< Billboard mode (default: DISABLED)
    blendMode: TR3D_BlendMode;                ///< Blend mode (default: MIX)
    depthMode: TR3D_DepthMode;                ///< Depth mode (default: LESS)
    cullMode: TR3D_CullMode;                  ///< Face culling mode (default: BACK)

    unlit: Boolean;                           ///< If true, material does not participate in lighting (default: false)

    shader: PR3D_SurfaceShader;               ///< Custom shader applied to the material (default: NULL)
  end;
  PR3D_Material = ^TR3D_Material;





// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Get the default material configuration.
 *
 * Returns `R3D_MATERIAL_BASE` by default,
 * or the material defined via `R3D_SetDefaultMaterial()`.
 *
 * @return Default material structure with standard properties.
 *}
function R3D_GetDefaultMaterial: TR3D_Material; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDefaultMaterial';

{*
 * @brief Set the default material configuration.
 *
 * Allows you to override the default material.
 * The default material will be used as the basis for loading 3D models.
 *
 * @param material Default material to define.
 *}
procedure R3D_SetDefaultMaterial(material: TR3D_Material); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetDefaultMaterial';

{*
 * @brief Unload a material and its associated textures.
 *
 * Frees all memory associated with a material, including its textures.
 * This function will unload all textures that are not default textures.
 *
 * @warning Only call this function if you are certain that the textures
 * are not shared with other materials or objects, as this will permanently
 * free the texture data.
 *
 * @param material Material structure to be unloaded.
 *}
procedure R3D_UnloadMaterial(material: TR3D_Material); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadMaterial';

{*
 * @brief Load an albedo (base color) map from file.
 *
 * Loads an image, uploads it as an sRGB texture (if enabled),
 * and applies the provided tint color.
 *
 * @param fileName Path to the texture file.
 * @param color Multiplicative tint applied in the shader.
 * @return Albedo map structure. Returns an empty map on failure.
 *}
function R3D_LoadAlbedoMap(const fileName: PAnsiChar; color: TColor): TR3D_AlbedoMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAlbedoMap';

{*
 * @brief Load an albedo (base color) map from memory.
 *
 * Same behavior as R3D_LoadAlbedoMap(), but reads from memory instead of disk.
 *
 * @param fileType Image format hint (e.g. "png", "jpg").
 * @param fileData Pointer to image data.
 * @param dataSize Size of image data in bytes.
 * @param color Multiplicative tint applied in the shader.
 * @return Albedo map structure. Returns an empty map on failure.
 *}
function R3D_LoadAlbedoMapFromMemory(fileType: PAnsiChar; fileData: Pointer;
  dataSize: Integer; color: TColor): TR3D_AlbedoMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadAlbedoMapFromMemory';

{*
 * @brief Unload an albedo map texture.
 *
 * Frees the underlying texture unless it is a default texture.
 *
 * @param map Albedo map to unload.
 *}
procedure R3D_UnloadAlbedoMap(map: TR3D_AlbedoMap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadAlbedoMap';

{*
 * @brief Load an emission map from file.
 *
 * Loads an emissive texture (sRGB if enabled) and sets color + energy.
 *
 * @param fileName Path to the texture file.
 * @param color Emission color.
 * @param energy Emission intensity multiplier.
 * @return Emission map. Returns an empty map on failure.
 *}
function R3D_LoadEmissionMap(fileName: PAnsiChar; color: TColor; energy: Single): TR3D_EmissionMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadEmissionMap';

{*
 * @brief Load an emission map from memory.
 *
 * Same behavior as R3D_LoadEmissionMap(), but reads from memory.
 *
 * @param fileType Image format hint.
 * @param fileData Pointer to image data.
 * @param dataSize Size of image data in bytes.
 * @param color Emission color.
 * @param energy Emission intensity multiplier.
 * @return Emission map. Returns an empty map on failure.
 *}
function R3D_LoadEmissionMapFromMemory(fileType: PAnsiChar; fileData: Pointer;
  dataSize: Integer; color: TColor; energy: Single): TR3D_EmissionMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadEmissionMapFromMemory';

{*
 * @brief Unload an emission map texture.
 *
 * Frees the texture unless it is a default texture.
 *
 * @param map Emission map to unload.
 *}
procedure R3D_UnloadEmissionMap(map: TR3D_EmissionMap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadEmissionMap';

{*
 * @brief Load a normal map from file.
 *
 * Uploads the texture in linear space and stores the normal scale factor.
 *
 * @param fileName Path to the texture file.
 * @param scale Normal intensity multiplier.
 * @return Normal map. Returns an empty map on failure.
 *}
function R3D_LoadNormalMap(fileName: PAnsiChar; scale: Single): TR3D_NormalMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadNormalMap';

{*
 * @brief Load a normal map from memory.
 *
 * Same behavior as R3D_LoadNormalMap(), but reads from memory.
 *
 * @param fileType Image format hint.
 * @param fileData Pointer to image data.
 * @param dataSize Size of image data in bytes.
 * @param scale Normal intensity multiplier.
 * @return Normal map. Returns an empty map on failure.
 *}
function R3D_LoadNormalMapFromMemory(fileType: PAnsiChar; fileData: Pointer;
  dataSize: Integer; scale: Single): TR3D_NormalMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadNormalMapFromMemory';

{*
 * @brief Unload a normal map texture.
 *
 * Frees the texture unless it is a default texture.
 *
 * @param map Normal map to unload.
 *}
procedure R3D_UnloadNormalMap(map: TR3D_NormalMap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadNormalMap';

{*
 * @brief Load a combined ORM (Occlusion-Roughness-Metalness) map from file.
 *
 * Uploads the texture in linear space and applies the provided multipliers.
 *
 * @param fileName Path to the ORM texture.
 * @param occlusion Occlusion multiplier.
 * @param roughness Roughness multiplier.
 * @param metalness Metalness multiplier.
 * @return ORM map. Returns an empty map on failure.
 *}
function R3D_LoadOrmMap(fileName: PAnsiChar; occlusion, roughness, metalness: Single): TR3D_OrmMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadOrmMap';

{*
 * @brief Load a combined ORM (Occlusion-Roughness-Metalness) map from memory.
 *
 * Same behavior as R3D_LoadOrmMap(), but reads from memory.
 *
 * @param fileType Image format hint.
 * @param fileData Pointer to image data.
 * @param dataSize Size of image data in bytes.
 * @param occlusion Occlusion multiplier.
 * @param roughness Roughness multiplier.
 * @param metalness Metalness multiplier.
 * @return ORM map. Returns an empty map on failure.
 *}
function R3D_LoadOrmMapFromMemory(fileType: PAnsiChar; fileData: Pointer; dataSize: Integer;
  occlusion, roughness, metalness: Single): TR3D_OrmMap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadOrmMapFromMemory';

{*
 * @brief Unload an ORM map texture.
 *
 * Frees the texture unless it is a default texture.
 *
 * @param map ORM map to unload.
 *}
procedure R3D_UnloadOrmMap(map: TR3D_OrmMap); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadOrmMap';

// ========================================
// HELPER FUNCTION IMPLEMENTATION
// ========================================

{function R3D_MATERIAL_BASE: TR3D_Material; inline;
begin
  Result.albedo.texture := Default(TTexture2D);
  Result.albedo.color := WHITE;

  Result.emission.texture := Default(TTexture2D);
  Result.emission.color := WHITE;
  Result.emission.energy := 0.0;

  Result.normal.texture := Default(TTexture2D);
  Result.normal.scale := 1.0;

  Result.orm.texture := Default(TTexture2D);
  Result.orm.occlusion := 1.0;
  Result.orm.roughness := 1.0;
  Result.orm.metalness := 0.0;

  Result.uvOffset := Vector2Create(0.0, 0.0);
  Result.uvScale := Vector2Create(1.0, 1.0);
  Result.alphaCutoff := 0.01;

  Result.transparencyMode := R3D_TRANSPARENCY_DISABLED;
  Result.billboardMode := R3D_BILLBOARD_DISABLED;
  Result.blendMode := R3D_BLEND_MIX;
  Result.depthMode := R3D_DEPTH_LESS;
  Result.cullMode := R3D_CULL_BACK;

  Result.unlit := False;
  Result.shader := nil;
end;
}
function R3D_MATERIAL_BASE: TR3D_Material; inline;

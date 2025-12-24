// R3D Material Module.

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
 * @brief Face culling modes.
 *
 * Specifies which faces of a geometry are discarded during rendering based on their winding order.
 *}
type
  TR3D_CullMode = (
    R3D_CULL_NONE,              ///< No culling; all faces are rendered.
    R3D_CULL_BACK,              ///< Cull back-facing polygons (faces with clockwise winding order).
    R3D_CULL_FRONT              ///< Cull front-facing polygons (faces with counter-clockwise winding order).
  );

// ========================================
// Material Structure Types
// ========================================

type
  {*
   * @brief Albedo map structure containing texture and color multiplier.
   *}
  TR3D_MapAlbedo = record
    texture: TTexture2D;      ///< Albedo (base color) texture.
    color: TColor;            ///< Albedo color multiplier.
  end;

  {*
   * @brief Emission map structure containing texture, color, and energy multiplier.
   *}
  TR3D_MapEmission = record
    texture: TTexture2D;      ///< Emission texture.
    color: TColor;            ///< Emission color.
    energy: Single;           ///< Emission energy multiplier.
  end;

  {*
   * @brief Normal map structure containing texture and scale factor.
   *}
  TR3D_MapNormal = record
    texture: TTexture2D;      ///< Normal map texture.
    scale: Single;            ///< Normal scale.
  end;

  {*
   * @brief Combined Occlusion-Roughness-Metalness map structure.
   *}
  TR3D_MapORM = record
    texture: TTexture2D;      ///< Combined Occlusion-Roughness-Metalness texture.
    occlusion: Single;        ///< Occlusion multiplier.
    roughness: Single;        ///< Roughness multiplier.
    metalness: Single;        ///< Metalness multiplier.
  end;

  {*
   * @brief Represents a material with textures, parameters, and rendering modes.
   *
   * Combines multiple texture maps and settings used during shading.
   *}
  TR3D_Material = record
    albedo: TR3D_MapAlbedo;           ///< Albedo (base color) map.
    emission: TR3D_MapEmission;       ///< Emission map.
    normal: TR3D_MapNormal;           ///< Normal map.
    orm: TR3D_MapORM;                 ///< Combined Occlusion-Roughness-Metalness map.

    transparencyMode: TR3D_TransparencyMode;  ///< Transparency mode applied to the object.
    billboardMode: TR3D_BillboardMode;        ///< Billboard mode applied to the object.
    blendMode: TR3D_BlendMode;                ///< Blend mode used for rendering.
    cullMode: TR3D_CullMode;                  ///< Face culling mode used for rendering.

    uvOffset: TVector2;                       ///< UV offset applied to the texture coordinates.
                                              ///< For models, this can be set manually.
                                              ///< For sprites, this value is overridden automatically.

    uvScale: TVector2;                        ///< UV scale factor applied to the texture coordinates.
                                              ///< For models, this can be set manually.
                                              ///< For sprites, this value is overridden automatically.

    alphaCutoff: Single;                      ///< Alpha threshold below which fragments are discarded during opaque rendering.
  end;
  PR3D_Material = ^TR3D_Material;

// ========================================
// Public API Functions
// ========================================

{*
 * @brief Get the default material configuration.
 *
 * Returns a default material with standard properties and default textures.
 * This material can be used as a fallback or starting point for custom materials.
 *
 * @return Default material structure with standard properties.
 *}
function R3D_GetDefaultMaterial: TR3D_Material; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDefaultMaterial';

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
 * @param material Pointer to the material structure to be unloaded.
 *}
procedure R3D_UnloadMaterial(material: PR3D_Material); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadMaterial';

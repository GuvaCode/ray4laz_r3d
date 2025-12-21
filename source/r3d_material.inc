
{*
 * @brief Blend modes for rendering.
 *
 * Defines common blending modes used in 3D rendering to combine source and destination colors.
 * @note The blend mode is applied only if you are in forward rendering mode or auto-detect mode.
 *}
type
  TR3D_BlendMode = (
    R3D_BLEND_OPAQUE,               ///< No blending, the source color fully replaces the destination color.
    R3D_BLEND_ALPHA,                ///< Alpha blending: source color is blended with the destination based on alpha value.
    R3D_BLEND_ADDITIVE,             ///< Additive blending: source color is added to the destination, making bright effects.
    R3D_BLEND_MULTIPLY,             ///< Multiply blending: source color is multiplied with the destination, darkening the image.
    R3D_BLEND_PREMULTIPLIED_ALPHA   ///< Premultiplied alpha blending: source color is blended with the destination assuming the source color is already multiplied by its alpha.
  );

{*
 * @brief Defines the depth mode used to render the mesh.
 *}
type
  TR3D_DepthMode = (
    R3D_DEPTH_READ_WRITE,       ///< Enable depth testing and writing to the depth buffer.
    R3D_DEPTH_READ_ONLY,        ///< Enable depth testing but disable writing to the depth buffer.
    R3D_DEPTH_DISABLED          ///< Disable depth testing and writing to the depth buffer.
  );

{*
 * @brief Face culling modes for a mesh.
 *
 * Specifies which faces of a geometry are discarded during rendering based on their winding order.
 *}
type
  TR3D_CullMode = (
    R3D_CULL_NONE,   ///< No culling; all faces are rendered.
    R3D_CULL_BACK,   ///< Cull back-facing polygons (faces with clockwise winding order).
    R3D_CULL_FRONT   ///< Cull front-facing polygons (faces with counter-clockwise winding order).
  );

{*
 * @brief Defines billboard modes for 3D objects.
 *
 * This enumeration defines how a 3D object aligns itself relative to the camera.
 * It provides options to disable billboarding or to enable specific modes of alignment.
 *}
type
  TR3D_BillboardMode = (
    R3D_BILLBOARD_DISABLED,     ///< Billboarding is disabled; the object retains its original orientation.
    R3D_BILLBOARD_FRONT,        ///< Full billboarding; the object fully faces the camera, rotating on all axes.
    R3D_BILLBOARD_Y_AXIS        ///< Y-axis constrained billboarding; the object rotates only around the Y-axis,
                                 ///< keeping its "up" orientation fixed. This is suitable for upright objects like characters or signs.
  );

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Albedo map structure.
 *}
type
  TR3D_MapAlbedo = record
    texture: TTexture2D;      ///< Albedo (base color) texture.
    color: TColor;            ///< Albedo color multiplier.
  end;

{*
 * @brief Emission map structure.
 *}
type
  TR3D_MapEmission = record
    texture: TTexture2D;      ///< Emission texture.
    color: TColorB;            ///< Emission color.
    energy: Single;           ///< Emission energy multiplier.
  end;

{*
 * @brief Normal map structure.
 *}
type
  TR3D_MapNormal = record
    texture: TTexture2D;      ///< Normal map texture.
    scale: Single;            ///< Normal scale.
  end;

{*
 * @brief ORM (Occlusion-Roughness-Metalness) map structure.
 *}
type
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
type
  PR3D_Material = ^TR3D_Material;
  TR3D_Material = record
    albedo: TR3D_MapAlbedo;                 ///< Albedo (base color) map.
    emission: TR3D_MapEmission;             ///< Emission map.
    normal: TR3D_MapNormal;                 ///< Normal map.
    orm: TR3D_MapORM;                       ///< ORM (Occlusion-Roughness-Metalness) map.

    blendMode: TR3D_BlendMode;              ///< Blend mode used for rendering the material.
    depthMode: TR3D_DepthMode;              ///< Depth mode used for rendering the material. Not taken into account for decals.
    cullMode: TR3D_CullMode;                ///< Face culling mode used for the material.

    billboardMode: TR3D_BillboardMode;      ///< Billboard mode applied to the object.

    uvOffset: TVector2;                     ///< UV offset applied to the texture coordinates.
                                            ///< For models, this can be set manually.
                                            ///< For sprites, this value is overridden automatically.

    uvScale: TVector2;                      ///< UV scale factor applied to the texture coordinates.
                                            ///< For models, this can be set manually.
                                            ///< For sprites, this value is overridden automatically.

    alphaCutoff: Single;                    ///< Alpha threshold below which fragments are discarded.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Get the default material configuration.
 *
 * Returns a default material with standard properties and default textures.
 * This material can be used as a fallback or starting point for custom materials.
 *
 * @return Default material structure with standard properties.
 *}
function R3D_GetDefaultMaterial: TR3D_Material; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetDefaultMaterial';

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
procedure R3D_UnloadMaterial(const material: PR3D_Material); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadMaterial';

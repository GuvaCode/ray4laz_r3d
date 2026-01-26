// R3D Decal Module.

{*
* @brief Represents a decal and its properties.
*
* This structure defines a decal that can be projected onto geometry that has already been rendered.
*
* @note Decals are drawn using deferred screen space rendering and do not interact with any
* forward rendered or non-opaque objects.
*}
type
  PR3D_Decal = ^TR3D_Decal;
  TR3D_Decal = record
    albedo: TR3D_AlbedoMap;       ///< Albedo map (if the texture is undefined, implicitly treat `applyColor` as false, with alpha = 1.0)
    emission: TR3D_EmissionMap;   ///< Emission map
    normal: TR3D_NormalMap;       ///< Normal map
    orm: TR3D_OrmMap;             ///< Occlusion-Roughness-Metalness map
    uvOffset: TVector2;           ///< UV offset (default: {0.0f, 0.0f})
    uvScale: TVector2;            ///< UV scale (default: {1.0f, 1.0f})
    alphaCutoff: Single;          ///< Alpha cutoff threshold (default: 0.01f)
    normalThreshold: Single;      ///< Maximum angle against the surface normal to draw decal. 0.0f disables threshold. (default: 0.0f)
    fadeWidth: Single;            ///< The width of fading along the normal threshold (default: 0.0f)
    applyColor: Boolean;          ///< Indicates that the albedo color will not be rendered, only the alpha component of the albedo will be used as a mask. (default: true)
  end;


{*
 * @brief Unload all map textures assigned to a R3D_Decal.
 *
 * Frees all underlying textures in a R3D_Decal that are not a default texture.
 *
* @param decal to unload maps from.
*}
procedure R3D_UnloadDecalMaps(decal: TR3D_Decal); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadDecalMaps';


function R3D_DECAL_BASE: TR3D_Decal; inline;



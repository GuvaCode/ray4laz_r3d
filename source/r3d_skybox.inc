// R3D SkyBox Module.
{*
 * @brief Structure representing a skybox and its related textures for lighting.
 *
 * This structure contains textures used for rendering a skybox, as well as
 * precomputed lighting textures used for image-based lighting (IBL).
 *}
type
PR3D_Skybox = ^TR3D_Skybox;
TR3D_Skybox = record
  cubemap:    TTextureCubemap; // The skybox cubemap texture for the background and reflections.
  irradiance: TTexture2D;      // The irradiance cubemap for diffuse ambient lighting.
  prefilter:  TTexture2D;      // The prefiltered cubemap for specular reflections with mipmaps.
end;

// ----------------------------------------
// SKYBOX: Skybox Loading Functions
// ----------------------------------------

{*
 * @brief Loads a skybox from a texture file.
 *
 * This function loads a skybox cubemap from a texture file using a specified cubemap layout.
 * The layout defines how the six faces of the cubemap are arranged within the texture.
 *
 * @param filePath The path to the texture file.
 * @param layout The cubemap layout format.
 * @return The loaded skybox object.
 *}
function R3D_LoadSkybox(const filePath: PChar; layout: TCubemapLayout): TR3D_Skybox; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkybox';

{*
 * @brief Loads a skybox from an image in memory.
 *
 * This function loads a skybox cubemap from an image already loaded in memory,
 * using a specified cubemap layout to map the six faces.
 *
 * @param image The source image in memory.
 * @param layout The cubemap layout format.
 * @return The loaded skybox object.
 *}
function R3D_LoadSkyboxFromMemory(image: TImage; layout: TCubemapLayout): TR3D_Skybox; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkyboxFromMemory';

{*
 * @brief Loads a skybox from a panorama texture file.
 *
 * This function loads a skybox from a panorama (equirectangular) texture file,
 * and converts it into a cubemap with the specified resolution.
 *
 * @param filePath The path to the panorama texture file.
 * @param size The resolution of the generated cubemap (e.g., 512, 1024).
 * @return The loaded skybox object.
 *}
function R3D_LoadSkyboxPanorama(const filePath: Pchar; size: Integer): TR3D_Skybox; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkyboxPanorama';

{*
 * @brief Loads a skybox from a panorama image in memory.
 *
 * This function loads a skybox from a panorama (equirectangular) image already loaded in memory,
 * and converts it into a cubemap with the specified resolution.
 *
 * @param image The panorama image in memory.
 * @param size The resolution of the generated cubemap (e.g., 512, 1024).
 * @return The loaded skybox object.
 *}
function R3D_LoadSkyboxPanoramaFromMemory(image: TImage; size: Integer): TR3D_Skybox; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkyboxPanoramaFromMemory';

{*
 * @brief Unloads a skybox and frees its resources.
 *
 * This function removes a previously loaded skybox from memory.
 * It should be called when the skybox is no longer needed to prevent memory leaks.
 *
 * @param sky The skybox to unload.
 *}
procedure R3D_UnloadSkybox(sky: TR3D_Skybox); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadSkybox';

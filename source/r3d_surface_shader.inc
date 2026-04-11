// R3D Surface Shader Module.

// ========================================
// OPAQUE TYPES
// ========================================

type
  {*
   * @brief Surface shader opaque type.
   *
   * Opaque handle to a surface shader object.
   * Use the provided API functions to create, configure, and destroy shaders.
   *}
  TR3D_SurfaceShader = record end;
  PR3D_SurfaceShader = ^TR3D_SurfaceShader;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads a surface shader from a file.
 *
 * The shader must define at least one entry point: `void vertex()` or `void fragment()`.
 * It can define either one or both.
 *
 * @param filePath Path to the shader source file.
 * @return Pointer to the loaded surface shader, or nil on failure.
 *}
function R3D_LoadSurfaceShader(const filePath: PAnsiChar): PR3D_SurfaceShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSurfaceShader';

{*
 * @brief Loads a surface shader from a source code string in memory.
 *
 * The shader must define at least one entry point: `void vertex()` or `void fragment()`.
 * It can define either one or both.
 *
 * @param code Null-terminated shader source code.
 * @return Pointer to the loaded surface shader, or nil on failure.
 *}
function R3D_LoadSurfaceShaderFromMemory(const code: PAnsiChar): PR3D_SurfaceShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSurfaceShaderFromMemory';

{*
 * @brief Creates an alias of an existing surface shader.
 *
 * The alias shares the same compiled program as the original but holds its own
 * independent uniform and sampler state, allowing the same shader to be used
 * multiple times within a single frame with different values.
 *
 * Uniform and sampler state is copied from the original at the moment this
 * function is called, not from the shader source defaults. Any values set
 * on the original after compilation but before this call will be reflected
 * in the alias; values set afterward will not.
 *
 * @note The alias does not own the program. Always unload all aliases before
 *       unloading the original, or the alias program references become dangling.
 *
 * @param shader The original surface shader to alias.
 * @return Pointer to the alias, or nil on failure.
 *}
function R3D_LoadSurfaceShaderAlias(shader: PR3D_SurfaceShader): PR3D_SurfaceShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSurfaceShaderAlias';

{*
 * @brief Unloads and destroys a surface shader.
 *
 * If the shader owns its program shaders (i.e. it was created with
 * R3D_LoadSurfaceShader or R3D_LoadSurfaceShaderFromMemory), they are deleted.
 * Aliases created from this shader via R3D_LoadSurfaceShaderAlias must be
 * unloaded beforehand, as they share the same programs and will be left
 * with dangling references.
 *
 * @param shader Surface shader to unload.
 *}
procedure R3D_UnloadSurfaceShader(shader: PR3D_SurfaceShader); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadSurfaceShader';

{*
 * @brief Sets a uniform value for the current frame.
 *
 * Once a uniform is set, it remains valid for all frames.
 * If a uniform is set multiple times during the same frame,
 * the last value defined before R3D_End() is used.
 *
 * Supported types:
 * bool, int, float,
 * ivec2, ivec3, ivec4,
 * vec2, vec3, vec4,
 * mat2, mat3, mat4
 *
 * @warning Boolean values are read as 4 bytes.
 *
 * @param shader Target surface shader.
 *               May be NULL. In that case, the call is ignored
 *               and a warning is logged.
 * @param name   Name of the uniform. Must not be NULL.
 * @param value  Pointer to the uniform value. Must not be NULL.
 *}
procedure R3D_SetSurfaceShaderUniform(shader: PR3D_SurfaceShader; const name: PAnsiChar; const value: Pointer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSurfaceShaderUniform';

{*
 * @brief Sets a texture sampler for the current frame.
 *
 * Once a sampler is set, it remains valid for all frames.
 * If a sampler is set multiple times during the same frame,
 * the last value defined before R3D_End() is used.
 *
 * Supported samplers:
 * sampler1D, sampler2D, sampler3D, samplerCube
 *
 * @param shader  Target surface shader.
 *                May be NULL. In that case, the call is ignored
 *                and a warning is logged.
 * @param name    Name of the sampler uniform. Must not be NULL.
 * @param texture Texture to bind to the sampler.
 *}
procedure R3D_SetSurfaceShaderSampler(shader: PR3D_SurfaceShader; const name: PAnsiChar; texture: TTexture); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSurfaceShaderSampler';

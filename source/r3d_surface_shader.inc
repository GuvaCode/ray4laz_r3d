//* r3d_surface_shader.inc -- R3D Surface Shader Module.
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
 * @brief Unloads and destroys a surface shader.
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
 * @param name Name of the uniform.
 * @param value Pointer to the uniform value.
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
 * @param shader Target surface shader.
 * @param name Name of the sampler uniform.
 * @param texture Texture to bind to the sampler.
 *}
procedure R3D_SetSurfaceShaderSampler(shader: PR3D_SurfaceShader; const name: PAnsiChar; texture: TTexture); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSurfaceShaderSampler';



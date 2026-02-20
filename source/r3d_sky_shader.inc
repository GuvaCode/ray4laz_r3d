// R3D Sky Shader Module


// ========================================
// OPAQUE TYPES
// ========================================

type
  PR3D_SkyShader = ^TR3D_SkyShader;
  TR3D_SkyShader = record
    // Поля структуры не указаны в оригинале,
    // это opaque type (непрозрачный тип)
  end;
// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Loads a sky shader from a file.
 *
 * The shader must define a single entry point:
 * `void fragment()`. Any other entry point, such as `vertex()`,
 * or any varyings will be ignored.
 *
 * @param filePath Path to the shader source file.
 * @return Pointer to the loaded sky shader, or NULL on failure.
 *}
function R3D_LoadSkyShader(const filePath: PAnsiChar): PR3D_SkyShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkyShader';
{*
 * @brief Loads a sky shader from a source code string in memory.
 *
 * The shader must define a single entry point:
 * `void fragment()`. Any other entry point, such as `vertex()`,
 * or any varyings will be ignored.
 *
 * @param code Null-terminated shader source code.
 * @return Pointer to the loaded sky shader, or NULL on failure.
 *}
function R3D_LoadSkyShaderFromMemory(const code: PAnsiChar): PR3D_SkyShader; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadSkyShaderFromMemory';
{*
 * @brief Unloads and destroys a sky shader.
 *
 * @param shader Sky shader to unload.
 *}
procedure R3D_UnloadSkyShader(shader: PR3D_SkyShader); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadSkyShader';
{*
 * @brief Sets a uniform value for all subsequent sky generations.
 *
 * Supported types:
 * bool, int, float,
 * ivec2, ivec3, ivec4,
 * vec2, vec3, vec4,
 * mat2, mat3, mat4
 *
 * @warning Boolean values are read as 4 bytes.
 *
 * @param shader Target sky shader.
 *               May be NULL. In that case, the call is ignored
 *               and a warning is logged.
 * @param name   Name of the uniform. Must not be NULL.
 * @param value  Pointer to the uniform value. Must not be NULL.
 *}
procedure R3D_SetSkyShaderUniform(shader: PR3D_SkyShader; const name: PAnsiChar; const value: Pointer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSkyShaderUniform';
{*
 * @brief Sets a uniform value for all subsequent sky generations.
 *
 * Supported samplers:
 * sampler1D, sampler2D, sampler3D, samplerCube
 *
 * @param shader  Target sky shader.
 *                May be NULL. In that case, the call is ignored
 *                and a warning is logged.
 * @param name    Name of the sampler uniform. Must not be NULL.
 * @param texture Texture to bind to the sampler.
 *}
procedure R3D_SetSkyShaderSampler(shader: PR3D_SkyShader; const name: PAnsiChar; texture: TTexture); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SetSkyShaderSampler';

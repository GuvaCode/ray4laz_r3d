// R3D Sky Module

// ========================================
// STRUCT TYPES
// ========================================

{*
 * @brief Parameters for procedural sky generation.
 *
 * Curves control gradient falloff (lower = sharper transition at horizon).
 *}
type
  TR3D_ProceduralSky = record
    skyTopColor: TColorB;           // Sky color at zenith
    skyHorizonColor: TColorB;       // Sky color at horizon
    skyHorizonCurve: Single;        // Gradient curve exponent (0.01 - 1.0, typical: 0.15)
    skyEnergy: Single;              // Sky brightness multiplier

    groundBottomColor: TColorB;     // Ground color at nadir
    groundHorizonColor: TColorB;    // Ground color at horizon
    groundHorizonCurve: Single;     // Gradient curve exponent (typical: 0.02)
    groundEnergy: Single;           // Ground brightness multiplier

    sunDirection: TVector3;         // Direction from which light comes (can take not normalized)
    sunColor: TColorB;              // Sun disk color
    sunSize: Single;                // Sun angular size in radians (real sun: ~0.0087 rad = 0.5°)
    sunCurve: Single;               // Sun edge softness exponent (typical: 0.15)
    sunEnergy: Single;              // Sun brightness multiplier
  end;
  PR3D_ProceduralSky = ^TR3D_ProceduralSky;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Generates a procedural sky cubemap.
 *
 * Creates a GPU cubemap with procedural gradient sky and sun rendering.
 * The cubemap is ready for use as environment map or IBL source.
 *}
function R3D_GenProceduralSky(size: Integer; params: TR3D_ProceduralSky): TR3D_Cubemap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenProceduralSky';
{*
 * @brief Generates a custom sky cubemap.
 *
 * Creates a GPU cubemap rendered using the provided sky shader.
 * The cubemap is ready for use as environment map or IBL source.
 *}
function R3D_GenCustomSky(size: Integer; shader: PR3D_SkyShader): TR3D_Cubemap; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenCustomSky';
{*
 * @brief Updates an existing procedural sky cubemap.
 *
 * Re-renders the cubemap with new parameters. Faster than unload + generate
 * when animating sky conditions (time of day, weather, etc.).
 *}
procedure R3D_UpdateProceduralSky(cubemap: PR3D_Cubemap; params: TR3D_ProceduralSky); cdecl
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateProceduralSky';
{*
 * @brief Updates an existing custom sky cubemap.
 *
 * Re-renders the cubemap using the provided sky shader. Faster than unload + generate
 * when animating sky conditions or updating shader uniforms (time, clouds, stars, etc.).
 *}
procedure R3D_UpdateCustomSky(cubemap: PR3D_Cubemap; shader: PR3D_SkyShader); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateCustomSky';


function R3D_PROCEDURAL_SKY_BASE: TR3D_ProceduralSky; inline;

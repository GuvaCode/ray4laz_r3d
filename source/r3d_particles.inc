{*
 * @struct R3D_Particle
 * @brief Represents a particle in a 3D particle system, with properties
 *        such as position, velocity, rotation, and color modulation.
 *}
type

PR3D_Particle = ^TR3D_Particle;
TR3D_Particle = record
  lifetime:            Single;   // Duration of the particle's existence in seconds.

  transform:           TMatrix;  // The particle's current transformation matrix in 3D space.

  position:            TVector3; // The current position of the particle in 3D space.
  rotation:            TVector3; // The current rotation of the particle in 3D space (Euler angles).
  scale:               TVector3; // The current scale of the particle in 3D space.
  color:               TColor;   // The current color of the particle, representing its color modulation.

  velocity:            TVector3; // The current velocity of the particle in 3D space.
  angularVelocity:     TVector3; // The current angular velocity of the particle in radians (Euler angles).

  baseScale:           TVector3; // The initial scale of the particle in 3D space.
  baseVelocity:        TVector3; // The initial velocity of the particle in 3D space.
  baseAngularVelocity: TVector3; // The initial angular velocity of the particle in radians (Euler angles).
  baseOpacity:         Byte;     // The initial opacity of the particle, ranging from 0 (fully transparent) to 255 (fully opaque).
end;

PR3D_ParticleSystem = ^TR3D_ParticleSystem;
TR3D_ParticleSystem = record
  particles:                   PR3D_Particle; // Pointer to the array of particles in the system.
  capacity:                    Integer;  // The maximum number of particles the system can manage.
  count:                       Integer;  // The current number of active particles in the system.

  position:                    TVector3; // The initial position of the particle system. Default: (0, 0, 0).
  gravity:                     TVector3; // The gravity applied to the particles. Default: (0, -9.81, 0)

  initialScale:                TVector3; // The initial scale of the particles. Default: (1, 1, 1).
  scaleVariance:               Single;   // The variance in particle scale. Default: 0.0f.

  initialRotation:             TVector3; // The initial rotation of the particles in Euler angles (degrees). Default: (0, 0, 0).
  rotationVariance:            TVector3; // The variance in particle rotation in Euler angles (degrees). Default: (0, 0, 0).

  initialColor:                TColor;   // The initial color of the particles. Default: WHITE.
  colorVariance:               TColor;   // The variance in particle color. Default: BLANK.

  initialVelocity:             TVector3; // The initial velocity of the particles. Default: (0, 0, 0).
  velocityVariance:            TVector3; // The variance in particle velocity. Default: (0, 0, 0).

  initialAngularVelocity:      TVector3; // The initial angular velocity of the particles in Euler angles (degrees). Default: (0, 0, 0).
  angularVelocityVariance:     TVector3; // The variance in angular velocity. Default: (0, 0, 0).

  lifetime:                    Single;   // The lifetime of the particles in seconds. Default: 1.0.
  lifetimeVariance:            Single;   // The variance in lifetime in seconds. Default: 0.0.

  emissionTimer:               Single;   // Use to control automatic emission, should not be modified manually.
  emissionRate:                Single;   // The rate of particle emission in particles per second. Default: 10.0
  spreadAngle:                 Single;   // The angle of propagation of the particles in a cone (degrees). Default: 0.0

  scaleOverLifetime:           PR3D_InterpolationCurve; // Curve controlling the scale evolution of the particles over their lifetime. Default: NULL.
  speedOverLifetime:           PR3D_InterpolationCurve; // Curve controlling the speed evolution of the particles over their lifetime. Default: NULL.
  opacityOverLifetime:         PR3D_InterpolationCurve; // Curve controlling the opacity evolution of the particles over their lifetime. Default: NULL.
  angularVelocityOverLifetime: PR3D_InterpolationCurve; // Curve controlling the angular velocity evolution of the particles over their lifetime. Default: NULL.

  aabb:                        TBoundingBox; // For frustum culling. Defaults to a large AABB; compute manually via `R3D_CalculateParticleSystemBoundingBox` after setup.

  autoEmission:                Boolean; // Indicates whether particle emission is automatic when calling `R3D_UpdateParticleSystem`.
                                        // If false, emission is manual using `R3D_EmitParticle`. Default: true.
end;

{*
 * @brief Loads a particle emitter system for the CPU.
 *
 * This function initializes a particle emitter system on the CPU with a specified maximum
 * number of particles. It prepares the necessary data structures and allocates memory
 * for managing the particles.
 *
 * @param maxParticles The maximum number of particles the system can handle at once.
 *                     This value determines the memory allocation and performance constraints.
 * @return A newly initialized `R3D_ParticleSystem` structure.
 *         The caller is responsible for properly managing and freeing the system when no longer needed.
 *}
function R3D_LoadParticleSystem(maxParticles: Integer): TR3D_ParticleSystem; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadParticleSystem';

{*
 * @brief Unloads the particle emitter system and frees allocated memory.
 *
 * This function deallocates the memory used by the particle emitter system and clears the associated resources.
 * It should be called when the particle system is no longer needed to prevent memory leaks.
 *
 * @param system A pointer to the `R3D_ParticleSystem` to be unloaded.
 *}
procedure R3D_UnloadParticleSystem(system: PR3D_ParticleSystem); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadParticleSystem';

{*
 * @brief Emits a particle in the particle system.
 *
 * This function triggers the emission of a new particle in the particle system. It handles the logic of adding a new
 * particle to the system and initializing its properties based on the current state of the system.
 *
 * @param system A pointer to the `R3D_ParticleSystemCPU` where the particle will be emitted.
 * @return `true` if the particle was successfully emitted, `false` if the system is at full capacity and cannot emit more particles.
 *}
function R3D_EmitParticle(system: PR3D_ParticleSystem): Boolean; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EmitParticle';

{*
 * @brief Updates the particle emitter system by advancing particle positions.
 *
 * This function updates the positions and properties of particles in the system based on the elapsed time. It handles
 * simulation of particle movement, gravity, and other physics-based calculations.
 *
 * @param system A pointer to the `R3D_ParticleSystem` to be updated.
 * @param deltaTime The time elapsed since the last update (in seconds).
 *}
procedure R3D_UpdateParticleSystem(system: PR3D_ParticleSystem; deltaTime: Single); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateParticleSystem';

{*
 * @brief Computes and updates the AABB (Axis-Aligned Bounding Box) of a particle system.
 *
 * This function simulates the particle system to estimate the region of space it occupies.
 * It considers particle positions at mid-life and end-of-life to approximate the AABB,
 * which is then stored in the system's `aabb` field. This is useful for enabling frustum culling,
 * especially when the bounds are not known beforehand.
 *
 * @param system Pointer to the `R3D_ParticleSystem` to update.
 *}
procedure R3D_CalculateParticleSystemBoundingBox(system: PR3D_ParticleSystem); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CalculateParticleSystemBoundingBox';
















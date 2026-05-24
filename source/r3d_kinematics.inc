// r3d_kinematics.inc -- R3D Kinematics Module.

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Collision information from a sweep test
 *}
type
  PR3D_SweepCollision = ^TR3D_SweepCollision;
  TR3D_SweepCollision = record
    hit: Boolean;           ///< Whether a collision occurred
    time: Single;           ///< Time of impact [0-1], fraction along velocity vector
    point: TVector3;        ///< World space collision point
    normal: TVector3;       ///< Surface normal at collision point
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Remove the normal component from a velocity vector (project onto a plane)
 * @param velocity Incoming velocity
 * @param normal Surface normal (must be normalized)
 * @return Velocity with the component along normal removed
 *}
function R3D_ClipVelocity(velocity, normal: TVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ClipVelocity';

{*
 * @brief Reflect a velocity vector off a surface
 * @param velocity Incoming velocity
 * @param normal Surface normal (must be normalized)
 * @param bounciness Coefficient of restitution (0=no bounce, 1=perfect bounce)
 * @return Reflected velocity scaled by bounciness
 *}
function R3D_ReflectVelocity(velocity, normal: TVector3; bounciness: Single): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ReflectVelocity';

{*
 * @brief Resolve a velocity vector against a sweep collision, sliding along the hit surface
 * @param velocity Desired movement vector
 * @param collision Sweep collision result to resolve against
 * @param outNormal Optional: receives the collision normal if a hit occurred
 * @return Resolved velocity (safe movement + clipped remainder)
 *}
function R3D_SlideVelocity(velocity: TVector3; collision: TR3D_SweepCollision;
  outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideVelocity';

{*
 * @brief Slide sphere along bounding box surface, resolving collisions
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Desired movement vector
 * @param box Obstacle bounding box
 * @param outNormal Optional: receives collision normal if collision occurred
 * @return Actual movement applied (may be reduced/redirected by collision)
 *}
function R3D_SlideSphereBoundingBox(center: TVector3; radius: Single;
  velocity: TVector3; box: TR3D_BoundingBox; outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideSphereBoundingBox';

{*
 * @brief Slide sphere along mesh surface, resolving collisions
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Desired movement vector
 * @param mesh Mesh data to collide against
 * @param transform Mesh world transform
 * @param outNormal Optional: receives collision normal if collision occurred
 * @return Actual movement applied (may be reduced/redirected by collision)
 *}
function R3D_SlideSphereMesh(center: TVector3; radius: Single; velocity: TVector3;
  mesh: TR3D_MeshData; transform: TMatrix; outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideSphereMesh';

{*
 * @brief Slide capsule along bounding box surface, resolving collisions
 * @param capsule Capsule shape
 * @param velocity Desired movement vector
 * @param box Obstacle bounding box
 * @param outNormal Optional: receives collision normal if collision occurred
 * @return Actual movement applied (may be reduced/redirected by collision)
 *}
function R3D_SlideCapsuleBoundingBox(capsule: TR3D_Capsule; velocity: TVector3;
  box: TR3D_BoundingBox; outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideCapsuleBoundingBox';

{*
 * @brief Slide capsule along mesh surface, resolving collisions
 * @param capsule Capsule shape
 * @param velocity Desired movement vector
 * @param mesh Mesh data to collide against
 * @param transform Mesh world transform
 * @param outNormal Optional: receives collision normal if collision occurred
 * @return Actual movement applied (may be reduced/redirected by collision)
 *}
function R3D_SlideCapsuleMesh(capsule: TR3D_Capsule; velocity: TVector3;
  mesh: TR3D_MeshData; transform: TMatrix; outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideCapsuleMesh';

{*
 * @brief Push sphere out of bounding box if penetrating
 * @param center Sphere center (modified in place if penetrating)
 * @param radius Sphere radius
 * @param box Obstacle box
 * @param outPenetration Optional: receives penetration depth
 * @return true if depenetration occurred
 *}
function R3D_DepenetrateSphereBoundingBox(var center: TVector3; radius: Single;
  box: TR3D_BoundingBox; outPenetration: PSingle): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DepenetrateSphereBoundingBox';

{*
 * @brief Push capsule out of bounding box if penetrating
 * @param capsule Capsule shape (modified in place if penetrating)
 * @param box Obstacle box
 * @param outPenetration Optional: receives penetration depth
 * @return true if depenetration occurred
 *}
function R3D_DepenetrateCapsuleBoundingBox(var capsule: TR3D_Capsule;
  box: TR3D_BoundingBox; outPenetration: PSingle): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DepenetrateCapsuleBoundingBox';

{*
 * @brief Check if a sphere is supported by a bounding box in a given direction
 * @param center Sphere center
 * @param radius Sphere radius
 * @param direction Ray direction to probe (must be normalized)
 * @param distance Maximum probe distance beyond the sphere surface
 * @param box Bounding box to test against
 * @param outHit Optional: receives raycast hit info
 * @return true if a surface is within reach in the given direction
 *}
function R3D_CheckSphereSupportBoundingBox(center: TVector3; radius: Single;
  direction: TVector3; distance: Single; box: TR3D_BoundingBox;
  outHit: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckSphereSupportBoundingBox';

{*
 * @brief Check if a sphere is supported by mesh geometry in a given direction
 * @param center Sphere center
 * @param radius Sphere radius
 * @param direction Ray direction to probe (must be normalized)
 * @param distance Maximum probe distance beyond the sphere surface
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @param outHit Optional: receives raycast hit info
 * @return true if a surface is within reach in the given direction
 *}
function R3D_CheckSphereSupportMesh(center: TVector3; radius: Single;
  direction: TVector3; distance: Single; mesh: TR3D_MeshData;
  transform: TMatrix; outHit: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckSphereSupportMesh';

{*
 * @brief Check if a capsule is supported by a bounding box in a given direction
 * @param capsule Capsule shape
 * @param direction Ray direction to probe (must be normalized)
 * @param distance Maximum probe distance beyond the capsule surface
 * @param box Bounding box to test against
 * @param outHit Optional: receives raycast hit info
 * @return true if a surface is within reach in the given direction
 *}
function R3D_CheckCapsuleSupportBoundingBox(capsule: TR3D_Capsule;
  direction: TVector3; distance: Single; box: TR3D_BoundingBox;
  outHit: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCapsuleSupportBoundingBox';

{*
 * @brief Check if a capsule is supported by mesh geometry in a given direction
 * @param capsule Capsule shape
 * @param direction Ray direction to probe (must be normalized)
 * @param distance Maximum probe distance beyond the capsule surface
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @param outHit Optional: receives raycast hit info
 * @return true if a surface is within reach in the given direction
 *}
function R3D_CheckCapsuleSupportMesh(capsule: TR3D_Capsule; direction: TVector3;
  distance: Single; mesh: TR3D_MeshData; transform: TMatrix;
  outHit: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCapsuleSupportMesh';

{*
 * @brief Sweep sphere against single point
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Movement vector (direction and magnitude)
 * @param point Point to test against
 * @return Sweep collision info (hit, time, point, normal)
 *}
function R3D_SweepSpherePoint(center: TVector3; radius: Single; velocity: TVector3;
  point: TVector3): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSpherePoint';

{*
 * @brief Sweep sphere against line segment
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Movement vector (direction and magnitude)
 * @param a Segment start point
 * @param b Segment end point
 * @return Sweep collision info (hit, time, point, normal)
 *}
function R3D_SweepSphereSegment(center: TVector3; radius: Single; velocity: TVector3;
  a, b: TVector3): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSphereSegment';

{*
 * @brief Sweep sphere against triangle plane (no edge/vertex clipping)
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Movement vector (direction and magnitude)
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @return Sweep collision info (hit, time, point, normal)
 *}
function R3D_SweepSphereTrianglePlane(center: TVector3; radius: Single;
  velocity: TVector3; a, b, c: TVector3): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSphereTrianglePlane';

{*
 * @brief Sweep sphere against triangle with edge/vertex handling
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Movement vector (direction and magnitude)
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @return Sweep collision info (hit, time, point, normal)
 *}
function R3D_SweepSphereTriangle(center: TVector3; radius: Single; velocity: TVector3;
  a, b, c: TVector3): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSphereTriangle';

{*
 * @brief Sweep sphere along velocity vector
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Movement vector (direction and magnitude)
 * @param box Obstacle bounding box
 * @return Sweep collision info (hit, distance, point, normal)
 *}
function R3D_SweepSphereBoundingBox(center: TVector3; radius: Single;
  velocity: TVector3; box: TR3D_BoundingBox): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSphereBoundingBox';

{*
 * @brief Sweep sphere along velocity vector against mesh geometry
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Movement vector (direction and magnitude)
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @return Sweep collision info (hit, time, point, normal)
 *}
function R3D_SweepSphereMesh(center: TVector3; radius: Single; velocity: TVector3;
  mesh: TR3D_MeshData; transform: TMatrix): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSphereMesh';

{*
 * @brief Sweep capsule along velocity vector
 * @param capsule Capsule shape to sweep
 * @param velocity Movement vector (direction and magnitude)
 * @param box Obstacle bounding box
 * @return Sweep collision info (hit, distance, point, normal)
 *}
function R3D_SweepCapsuleBoundingBox(capsule: TR3D_Capsule; velocity: TVector3;
  box: TR3D_BoundingBox): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepCapsuleBoundingBox';

{*
 * @brief Sweep capsule along velocity vector against mesh geometry
 * @param capsule Capsule shape to sweep
 * @param velocity Movement vector (direction and magnitude)
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @return Sweep collision info (hit, time, point, normal)
 *}
function R3D_SweepCapsuleMesh(capsule: TR3D_Capsule; velocity: TVector3;
  mesh: TR3D_MeshData; transform: TMatrix): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepCapsuleMesh';

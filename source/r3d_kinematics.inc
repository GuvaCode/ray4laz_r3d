// r3d_kinematics.inc -- R3D Kinematics Module.

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Capsule shape defined by two endpoints and radius
 *}
type
  PR3D_Capsule = ^TR3D_Capsule;
  TR3D_Capsule = record
    start: TVector3;     //< Start point of capsule axis
    end_: TVector3;      //< End point of capsule axis
    radius: Single;      //< Capsule radius
  end;

{*
 * @brief Penetration information from an overlap test
 *}
  PR3D_Penetration = ^TR3D_Penetration;
  TR3D_Penetration = record
    collides: Boolean;      ///< Whether shapes are overlapping
    depth: Single;        ///< Penetration depth
    normal: TVector3;     ///< Collision normal (direction to resolve penetration)
    mtv: TVector3;        ///< Minimum Translation Vector (normal * depth)
  end;

{*
 * @brief Collision information from a sweep test
 *}
  PR3D_SweepCollision = ^TR3D_SweepCollision;
  TR3D_SweepCollision = record
    hit: Boolean;           ///< Whether a collision occurred
    time: Single;         ///< Time of impact [0-1], fraction along velocity vector
    point: TVector3;      ///< World space collision point
    normal: TVector3;     ///< Surface normal at collision point
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Check if capsule intersects with box
 * @param capsule Capsule shape
 * @param box Bounding box
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsuleBox(capsule: TR3D_Capsule; box: TBoundingBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsuleBox';

{*
 * @brief Check if capsule intersects with sphere
 * @param capsule Capsule shape
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsuleSphere(capsule: TR3D_Capsule; center: TVector3; radius: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsuleSphere';

{*
 * @brief Check if two capsules intersect
 * @param a First capsule
 * @param b Second capsule
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsules(a, b: TR3D_Capsule): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsules';

{*
 * @brief Check if capsule intersects with mesh
 * @param capsule Capsule shape
 * @param mesh Mesh data
 * @param transform Mesh transform
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsuleMesh(capsule: TR3D_Capsule; mesh: TR3D_MeshData; transform: TMatrix): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsuleMesh';

{*
 * @brief Check penetration between capsule and box
 * @param capsule Capsule shape
 * @param box Bounding box
 * @return Penetration information.
 *}
function R3D_CheckPenetrationCapsuleBox(capsule: TR3D_Capsule; box: TBoundingBox): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationCapsuleBox';

{*
 * @brief Check penetration between capsule and sphere
 * @param capsule Capsule shape
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information.
 *}
function R3D_CheckPenetrationCapsuleSphere(capsule: TR3D_Capsule; center: TVector3; radius: Single): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationCapsuleSphere';

{*
 * @brief Check penetration between two capsules
 * @param a First capsule
 * @param b Second capsule
 * @return Penetration information.
 *}
function R3D_CheckPenetrationCapsules(a, b: TR3D_Capsule): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationCapsules';

{*
 * @brief Calculate slide velocity along surface
 * @param velocity Original velocity
 * @param normal Surface normal (must be normalized)
 * @return Velocity sliding along surface (perpendicular component removed)
 *}
function R3D_SlideVelocity(velocity, normal: TVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideVelocity';

{*
 * @brief Calculate bounce velocity after collision
 * @param velocity Incoming velocity
 * @param normal Surface normal (must be normalized)
 * @param bounciness Coefficient of restitution (0=no bounce, 1=perfect bounce)
 * @return Reflected velocity
 *}
function R3D_BounceVelocity(velocity, normal: TVector3; bounciness: Single): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_BounceVelocity';

{*
 * @brief Slide sphere along box surface, resolving collisions
 * @param center Sphere center position
 * @param radius Sphere radius
 * @param velocity Desired movement vector
 * @param box Obstacle bounding box
 * @param outNormal Optional: receives collision normal if collision occurred
 * @return Actual movement applied (may be reduced/redirected by collision)
 *}
function R3D_SlideSphereBox(center: TVector3; radius: Single; velocity: TVector3;
  box: TBoundingBox; outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideSphereBox';

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
 * @brief Slide capsule along box surface, resolving collisions
 * @param capsule Capsule shape
 * @param velocity Desired movement vector
 * @param box Obstacle bounding box
 * @param outNormal Optional: receives collision normal if collision occurred
 * @return Actual movement applied (may be reduced/redirected by collision)
 *}
function R3D_SlideCapsuleBox(capsule: TR3D_Capsule; velocity: TVector3;
  box: TBoundingBox; outNormal: PVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SlideCapsuleBox';

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
 * @brief Push sphere out of box if penetrating
 * @param center Sphere center (modified in place if penetrating)
 * @param radius Sphere radius
 * @param box Obstacle box
 * @param outPenetration Optional: receives penetration depth
 * @return true if depenetration occurred
 *}
function R3D_DepenetrateSphereBox(var center: TVector3; radius: Single;
  box: TBoundingBox; outPenetration: PSingle): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DepenetrateSphereBox';

{*
 * @brief Push capsule out of box if penetrating
 * @param capsule Capsule shape (modified in place if penetrating)
 * @param box Obstacle box
 * @param outPenetration Optional: receives penetration depth
 * @return true if depenetration occurred
 *}
function R3D_DepenetrateCapsuleBox(var capsule: TR3D_Capsule; box: TBoundingBox;
  outPenetration: PSingle): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DepenetrateCapsuleBox';

{*
 * @brief Cast a ray against mesh geometry
 * @param ray Ray to cast
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastMesh(ray: TRay; mesh: TR3D_MeshData; transform: TMatrix): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastMesh';

{*
 * @brief Cast a ray against a model (tests all meshes)
 * @param ray Ray to cast
 * @param model Model to test against (must have valid meshData)
 * @param transform Model world transform
 * @return Ray collision info for closest hit (hit=false if no meshData)
 *}
function R3D_RaycastModel(ray: TRay; model: TR3D_Model; transform: TMatrix): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastModel';

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
function R3D_SweepSphereTrianglePlane(center: TVector3; radius: Single; velocity: TVector3;
  a, b, c: TVector3): TR3D_SweepCollision; cdecl;
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
function R3D_SweepSphereBox(center: TVector3; radius: Single; velocity: TVector3;
  box: TBoundingBox): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepSphereBox';

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
function R3D_SweepCapsuleBox(capsule: TR3D_Capsule; velocity: TVector3;
  box: TBoundingBox): TR3D_SweepCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_SweepCapsuleBox';

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

{*
 * @brief Check if sphere is grounded against a box
 * @param center Sphere center
 * @param radius Sphere radius
 * @param checkDistance How far below to check
 * @param ground Ground box to test against
 * @param outGround Optional: receives raycast hit info
 * @return true if grounded within checkDistance
 *}
function R3D_IsSphereGroundedBox(center: TVector3; radius, checkDistance: Single;
  ground: TBoundingBox; outGround: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsSphereGroundedBox';

{*
 * @brief Check if sphere is grounded against mesh geometry
 * @param center Sphere center
 * @param radius Sphere radius
 * @param checkDistance How far below to check
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @param outGround Optional: receives raycast hit info
 * @return true if grounded within checkDistance
 *}
function R3D_IsSphereGroundedMesh(center: TVector3; radius, checkDistance: Single;
  mesh: TR3D_MeshData; transform: TMatrix; outGround: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsSphereGroundedMesh';

{*
 * @brief Check if capsule is grounded against a box
 * @param capsule Character capsule
 * @param checkDistance How far below to check (e.g., 0.1)
 * @param ground Ground box to test against
 * @param outGround Optional: receives raycast hit info
 * @return true if grounded within checkDistance
 *}
function R3D_IsCapsuleGroundedBox(capsule: TR3D_Capsule; checkDistance: Single;
  ground: TBoundingBox; outGround: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsCapsuleGroundedBox';

{*
 * @brief Check if capsule is grounded against mesh geometry
 * @param capsule Character capsule
 * @param checkDistance How far below to check
 * @param mesh Mesh data to test against
 * @param transform Mesh world transform
 * @param outGround Optional: receives raycast hit info
 * @return true if grounded within checkDistance
 *}
function R3D_IsCapsuleGroundedMesh(capsule: TR3D_Capsule; checkDistance: Single;
  mesh: TR3D_MeshData; transform: TMatrix; outGround: PRayCollision): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsCapsuleGroundedMesh';

{*
 * @brief Find closest point on line segment to given point
 * @param point Query point
 * @param start Segment start
 * @param end Segment end
 * @return Closest point on segment [start, end]
 *}
function R3D_ClosestPointOnSegment(point, start, end_: TVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ClosestPointOnSegment';

{*
 * @brief Find closest point on triangle to given point
 * @param p Query point
 * @param a Triangle vertex A
 * @param b Triangle vertex B
 * @param c Triangle vertex C
 * @return Closest point on triangle surface
 *}
function R3D_ClosestPointOnTriangle(p, a, b, c: TVector3): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ClosestPointOnTriangle';

{*
 * @brief Find closest point on box surface to given point
 * @param point Query point
 * @param box Bounding box
 * @return Closest point on/in box (clamped to box bounds)
 *}
function R3D_ClosestPointOnBox(point: TVector3; box: TBoundingBox): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ClosestPointOnBox';

// r3d_shape.inc -- R3D Shape Module.

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Alias for raylib's BoundingBox (min/max corner points).
 *}
type
  TR3D_BoundingBox = TBoundingBox;

{*
 * @brief Oriented bounding box (OBB).
 *
 * Defined by a center point, three orthogonal axes, and half-extents along each axis.
 *}
  PR3D_OrientedBox = ^TR3D_OrientedBox;
  TR3D_OrientedBox = record
    center: TVector3;
    axisX: TVector3;
    axisY: TVector3;
    axisZ: TVector3;
    halfExtents: TVector3;
  end;

{*
 * @brief Capsule shape defined by two endpoints and radius
 *}
  PR3D_Capsule = ^TR3D_Capsule;
  TR3D_Capsule = record
    start: TVector3;      ///< Start point of capsule axis
    end_: TVector3;       ///< End point of capsule axis
    radius: Single;       ///< Capsule radius
  end;

{*
 * @brief Penetration information from an overlap test
 *}
  PR3D_Penetration = ^TR3D_Penetration;
  TR3D_Penetration = record
    collides: Boolean;    ///< Whether shapes are overlapping
    depth: Single;        ///< Penetration depth
    normal: TVector3;     ///< Collision normal (direction to resolve penetration)
    mtv: TVector3;        ///< Minimum Translation Vector (normal * depth)
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Compute an axis-aligned bounding box from a center and half-extents.
 *}
function R3D_GetBoundingBox(center, halfExtents: TVector3): TR3D_BoundingBox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetBoundingBox';

{*
 * @brief Compute an oriented bounding box from an AABB and transform.
 *}
function R3D_GetOrientedBox(aabb: TR3D_BoundingBox; transform: TMatrix): TR3D_OrientedBox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetOrientedBox';

{*
 * @brief Check if two axis-aligned bounding boxes intersect
 * @param box1 First bounding box
 * @param box2 Second bounding box
 * @return true if collision detected
 *}
function R3D_CheckCollisionBoundingBoxes(box1, box2: TR3D_BoundingBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionBoundingBoxes';

{*
 * @brief Check if an axis-aligned bounding box intersects a sphere
 * @param box Bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 *}
function R3D_CheckCollisionBoundingBoxSphere(box: TR3D_BoundingBox;
  center: TVector3; radius: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionBoundingBoxSphere';

{*
 * @brief Check if two oriented bounding boxes intersect
 * @param box1 First oriented box
 * @param box2 Second oriented box
 * @return true if collision detected
 *}
function R3D_CheckCollisionOrientedBoxes(box1, box2: TR3D_OrientedBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionOrientedBoxes';

{*
 * @brief Check if an oriented bounding box intersects a sphere
 * @param box Oriented bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 *}
function R3D_CheckCollisionOrientedBoxSphere(box: TR3D_OrientedBox;
  center: TVector3; radius: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionOrientedBoxSphere';

{*
 * @brief Check if two spheres intersect
 * @param center1 First sphere center
 * @param radius1 First sphere radius
 * @param center2 Second sphere center
 * @param radius2 Second sphere radius
 * @return true if collision detected
 *}
function R3D_CheckCollisionSpheres(center1: TVector3; radius1: Single;
  center2: TVector3; radius2: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionSpheres';

{*
 * @brief Check if capsule intersects with bounding box
 * @param capsule Capsule shape
 * @param box Bounding box
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsuleBoundingBox(capsule: TR3D_Capsule;
  box: TR3D_BoundingBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsuleBoundingBox';

{*
 * @brief Check if capsule intersects with oriented box
 * @param capsule Capsule shape
 * @param box Oriented box
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsuleOrientedBox(capsule: TR3D_Capsule;
  box: TR3D_OrientedBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsuleOrientedBox';

{*
 * @brief Check if capsule intersects with sphere
 * @param capsule Capsule shape
 * @param center Sphere center
 * @param radius Sphere radius
 * @return true if collision detected
 *}
function R3D_CheckCollisionCapsuleSphere(capsule: TR3D_Capsule;
  center: TVector3; radius: Single): Boolean; cdecl;
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
function R3D_CheckCollisionCapsuleMesh(capsule: TR3D_Capsule; mesh: TR3D_MeshData;
  transform: TMatrix): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckCollisionCapsuleMesh';

{*
 * @brief Check penetration between two axis-aligned bounding boxes
 * @param box1 First bounding box
 * @param box2 Second bounding box
 * @return Penetration information
 *}
function R3D_CheckPenetrationBoundingBoxes(box1, box2: TR3D_BoundingBox): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationBoundingBoxes';

{*
 * @brief Check penetration between an axis-aligned bounding box and a sphere
 * @param box Bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information
 *}
function R3D_CheckPenetrationBoundingBoxSphere(box: TR3D_BoundingBox;
  center: TVector3; radius: Single): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationBoundingBoxSphere';

{*
 * @brief Check penetration between two oriented bounding boxes
 * @param box1 First oriented box
 * @param box2 Second oriented box
 * @return Penetration information
 *}
function R3D_CheckPenetrationOrientedBoxes(box1, box2: TR3D_OrientedBox): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationOrientedBoxes';

{*
 * @brief Check penetration between an oriented bounding box and a sphere
 * @param box Oriented bounding box
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information
 *}
function R3D_CheckPenetrationOrientedBoxSphere(box: TR3D_OrientedBox;
  center: TVector3; radius: Single): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationOrientedBoxSphere';

{*
 * @brief Check penetration between two spheres
 * @param center1 First sphere center
 * @param radius1 First sphere radius
 * @param center2 Second sphere center
 * @param radius2 Second sphere radius
 * @return Penetration information
 *}
function R3D_CheckPenetrationSpheres(center1: TVector3; radius1: Single;
  center2: TVector3; radius2: Single): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationSpheres';

{*
 * @brief Check penetration between a capsule and a bounding box
 * @param capsule Capsule shape
 * @param box Bounding box
 * @return Penetration information
 *}
function R3D_CheckPenetrationCapsuleBoundingBox(capsule: TR3D_Capsule;
  box: TR3D_BoundingBox): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationCapsuleBoundingBox';

{*
 * @brief Check penetration between a capsule and an oriented bounding box
 * @param capsule Capsule shape
 * @param box Oriented bounding box
 * @return Penetration information
 *}
function R3D_CheckPenetrationCapsuleOrientedBox(capsule: TR3D_Capsule;
  box: TR3D_OrientedBox): TR3D_Penetration; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CheckPenetrationCapsuleOrientedBox';

{*
 * @brief Check penetration between capsule and sphere
 * @param capsule Capsule shape
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Penetration information.
 *}
function R3D_CheckPenetrationCapsuleSphere(capsule: TR3D_Capsule;
  center: TVector3; radius: Single): TR3D_Penetration; cdecl;
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
 * @brief Cast a ray against a triangle
 * @param ray Ray to cast
 * @param p1 First triangle vertex
 * @param p2 Second triangle vertex
 * @param p3 Third triangle vertex
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastTriangle(ray: TRay; p1, p2, p3: TVector3): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastTriangle';

{*
 * @brief Cast a ray against a quad
 * @param ray Ray to cast
 * @param p1 First quad vertex
 * @param p2 Second quad vertex
 * @param p3 Third quad vertex
 * @param p4 Fourth quad vertex
 * @note The quad must be strictly planar and non-self-intersecting
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastQuad(ray: TRay; p1, p2, p3, p4: TVector3): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastQuad';

{*
 * @brief Cast a ray against an axis-aligned bounding box
 * @param ray Ray to cast
 * @param box Bounding box to test against
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastBoundingBox(ray: TRay; box: TR3D_BoundingBox): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastBoundingBox';

{*
 * @brief Cast a ray against an oriented bounding box
 * @param ray Ray to cast
 * @param box Oriented bounding box to test against
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastOrientedBox(ray: TRay; box: TR3D_OrientedBox): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastOrientedBox';

{*
 * @brief Cast a ray against a sphere
 * @param ray Ray to cast
 * @param center Sphere center
 * @param radius Sphere radius
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastSphere(ray: TRay; center: TVector3; radius: Single): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastSphere';

{*
 * @brief Cast a ray against a capsule
 * @param ray Ray to cast
 * @param capsule Capsule shape to test against
 * @return Ray collision info (hit, distance, point, normal)
 *}
function R3D_RaycastCapsule(ray: TRay; capsule: TR3D_Capsule): TRayCollision; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RaycastCapsule';

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
function R3D_ClosestPointOnBox(point: TVector3; box: TR3D_BoundingBox): TVector3; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ClosestPointOnBox';

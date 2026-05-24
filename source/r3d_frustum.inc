// r3d_frustum.inc -- R3D Frustum Module.

// ========================================
// ENUM TYPES
// ========================================

{*
 * @brief Frustum plane indices.
 *}
type
  R3D_FrustumPlane = (
    R3D_PLANE_BACK,
    R3D_PLANE_FRONT,
    R3D_PLANE_BOTTOM,
    R3D_PLANE_TOP,
    R3D_PLANE_RIGHT,
    R3D_PLANE_LEFT
  );
const
  R3D_PLANE_COUNT = 6;

// ========================================
// STRUCT TYPES
// ========================================

{*
 * @brief View frustum defined by its clipping planes.
 *
 * Planes are stored as Vector4 (xyz = normal, w = distance).
 *}
type
  PR3D_Frustum = ^TR3D_Frustum;
  TR3D_Frustum = record
    planes: array[0..R3D_PLANE_COUNT-1] of TVector4;  //< Clipping planes
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Get the current frustum.
 *
 * Returns the frustum computed during the last call to R3D_Begin().
 *}
function R3D_GetFrustum: TR3D_Frustum; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GetFrustum';

{*
 * @brief Compute a frustum from a view-projection matrix.
 *}
function R3D_ComputeFrustum(viewProj: TMatrix): TR3D_Frustum; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeFrustum';

{*
 * @brief Compute the axis-aligned bounding box of a frustum.
 *
 * @param invViewProj Inverse view-projection matrix.
 *}
function R3D_ComputeFrustumBoundingBox(invViewProj: TMatrix): TBoundingBox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeFrustumBoundingBox';

{*
 * @brief Compute the eight corner points of a frustum.
 *
 * @param invViewProj Inverse view-projection matrix.
 * @param corners Output array of 8 points (must be allocated by caller).
 *}
procedure R3D_ComputeFrustumCorners(invViewProj: TMatrix; var corners: array of TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ComputeFrustumCorners';

{*
 * @brief Check if a point is inside the frustum.
 *
 * @param frustum Frustum to test against. Must not be NULL.
 *}
function R3D_FrustumContainsPoint(const frustum: PR3D_Frustum; position: TVector3): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_FrustumContainsPoint';

{*
 * @brief Check if any point from a set is inside the frustum.
 *
 * @param frustum Frustum to test against. Must not be NULL.
 *}
function R3D_FrustumContainsAnyPoint(const frustum: PR3D_Frustum; const positions: PVector3; count: Integer): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_FrustumContainsAnyPoint';

{*
 * @brief Check if a sphere intersects the frustum.
 *
 * @param frustum Frustum to test against. Must not be NULL.
 *}
function R3D_FrustumIntersectsSphere(const frustum: PR3D_Frustum; position: TVector3; radius: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_FrustumIntersectsSphere';

{*
 * @brief Check if a bounding box intersects the frustum.
 *
 * @param frustum Frustum to test against. Must not be NULL.
 *}
function R3D_FrustumIntersectsBoundingBox(const frustum: PR3D_Frustum; aabb: TBoundingBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_FrustumIntersectsBoundingBox';

{*
 * @brief Check if an oriented box intersects the frustum.
 *
 * @param frustum Frustum to test against. Must not be NULL.
 *}
function R3D_FrustumIntersectsOrientedBox(const frustum: PR3D_Frustum; obb: TR3D_OrientedBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_FrustumIntersectsOrientedBox';

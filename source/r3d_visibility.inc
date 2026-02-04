//* r3d_visibility.inc -- R3D Visibility Module.
// R3D Visibility Module.

{*
 * @defgroup Visibility
 * @brief Defines screen visibility query functions.
 * @note Do not use these functions to cull objects yourself; r3d handles this correctly internally.
 *}

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Checks if a point is inside the view frustum.
 *
 * Tests whether a 3D point lies within the camera's frustum by checking against all six planes.
 * You can call this only after `R3D_Begin`, which updates the internal frustum state.
 *
 * @param position The 3D point to test.
 * @return `true` if inside the frustum, `false` otherwise.
 *}
function R3D_IsPointVisible(position: TVector3): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsPointVisible';

{*
 * @brief Checks if a sphere is inside the view frustum.
 *
 * Tests whether a sphere intersects the camera's frustum using plane-sphere tests.
 * You can call this only after `R3D_Begin`, which updates the internal frustum state.
 *
 * @param position The center of the sphere.
 * @param radius The sphere's radius (must be positive).
 * @return `true` if at least partially inside the frustum, `false` otherwise.
 *}
function R3D_IsSphereVisible(position: TVector3; radius: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsSphereVisible';

{*
 * @brief Checks if an AABB is inside the view frustum.
 *
 * Determines whether an axis-aligned bounding box intersects the frustum.
 * You can call this only after `R3D_Begin`, which updates the internal frustum state.
 *
 * @param aabb The bounding box to test.
 * @return `true` if at least partially inside the frustum, `false` otherwise.
 *}
function R3D_IsBoundingBoxVisible(aabb: TBoundingBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsBoundingBoxVisible';

{*
 * @brief Checks if an OBB is inside the view frustum.
 *
 * Tests an oriented bounding box (transformed AABB) for frustum intersection.
 * You can call this only after `R3D_Begin`, which updates the internal frustum state.
 *
 * @param aabb Local-space bounding box.
 * @param transform World-space transform matrix.
 * @return `true` if the transformed box intersects the frustum, `false` otherwise.
 *}
function R3D_IsOrientedBoxVisible(aabb: TBoundingBox; transform: TMatrix): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsOrientedBoxVisible';

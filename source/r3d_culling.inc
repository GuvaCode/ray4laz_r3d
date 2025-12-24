// R3D Culling Module.

{*
 * @brief Checks if a point is inside the view frustum.
 *
 * Tests whether a 3D point lies within the camera's frustum by checking against all six planes.
 * Call this only between `R3D_Begin` and `R3D_End`.
 *
 * Useful when automatic frustum culling is disabled and you're using a custom spatial structure
 * (e.g., octree, BVH, etc.).
 *
 * @param position The 3D point to test.
 * @return `true` if inside the frustum, `false` otherwise.
 *
 * @note This performs an exact plane-point test. Slower than bounding box tests.
 * @warning Frustum culling may incorrectly discard objects casting visible shadows.
 * @todo Improve shadow-aware culling in future versions.
 *
 * @see R3D_IsPointInFrustumBoundingBox()
 *}
function R3D_IsPointInFrustum(position: TVector3): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsPointInFrustum';

{*
 * @brief Checks if a sphere is inside the view frustum.
 *
 * Tests whether a sphere intersects the camera's frustum using plane-sphere tests.
 * Call this only between `R3D_Begin` and `R3D_End`.
 *
 * Useful when managing visibility manually.
 *
 * @param position The center of the sphere.
 * @param radius The sphere's radius (must be positive).
 * @return `true` if at least partially inside the frustum, `false` otherwise.
 *
 * @note More accurate but slower than bounding box approximations.
 * @warning May cause visual issues with shadow casters being culled too early.
 * @todo Add support for shadow-aware visibility.
 *
 * @see R3D_IsSphereInFrustumBoundingBox()
 *}
function R3D_IsSphereInFrustum(position: TVector3; radius: Single): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsSphereInFrustum';

{*
 * @brief Checks if an AABB is inside the view frustum.
 *
 * Determines whether an axis-aligned bounding box intersects the frustum.
 * Call between `R3D_Begin` and `R3D_End`.
 *
 * For use in custom culling strategies or spatial partitioning systems.
 *
 * @param aabb The bounding box to test.
 * @return `true` if at least partially inside the frustum, `false` otherwise.
 *
 * @note Exact but more costly than AABB pre-tests.
 * @warning May prematurely cull objects casting visible shadows.
 * @todo Add support for light-aware visibility tests.
 *
 * @see R3D_IsAABBInFrustumBoundingBox()
 *}
function R3D_IsAABBInFrustum(aabb: TBoundingBox): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsAABBInFrustum';

{*
 * @brief Checks if an OBB is inside the view frustum.
 *
 * Tests an oriented bounding box (transformed AABB) for frustum intersection.
 * Must be called between `R3D_Begin` and `R3D_End`.
 *
 * Use this for objects with transformations when doing manual culling.
 *
 * @param aabb Local-space bounding box.
 * @param transform World-space transform matrix.
 * @return `true` if the transformed box intersects the frustum, `false` otherwise.
 *
 * @note More expensive than AABB checks due to matrix operations.
 * @warning May incorrectly cull shadow casters.
 * @todo Consider shadow-aware culling improvements.
 *
 * @see R3D_IsAABBInFrustum()
 *}
function R3D_IsOBBInFrustum(aabb: TBoundingBox; transform: TMatrix): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsOBBInFrustum';


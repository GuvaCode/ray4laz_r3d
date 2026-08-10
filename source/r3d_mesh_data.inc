// R3D Mesh Data Module.


// ========================================
// ENUMS TYPES
// ========================================

{*
 * @brief Defines the geometric primitive type.
 *}
type
  TR3D_PrimitiveType = (
    R3D_PRIMITIVE_POINTS,           ///< Each vertex represents a single point.
    R3D_PRIMITIVE_LINES,            ///< Each pair of vertices forms an independent line segment.
    R3D_PRIMITIVE_LINE_STRIP,       ///< Connected series of line segments sharing vertices.
    R3D_PRIMITIVE_LINE_LOOP,        ///< Closed loop of connected line segments.
    R3D_PRIMITIVE_TRIANGLES,        ///< Each set of three vertices forms an independent triangle.
    R3D_PRIMITIVE_TRIANGLE_STRIP,   ///< Connected strip of triangles sharing vertices.
    R3D_PRIMITIVE_TRIANGLE_FAN      ///< Fan of triangles sharing the first vertex.
  );

// ========================================
// STRUCTS TYPES
// ========================================
{*
 * @brief Represents a mesh stored in CPU memory.
 *
 * R3D_MeshData is the CPU-side container of a mesh. It stores vertex and index data,
 * and provides utility functions to generate, transform, and process geometry before
 * uploading it to the GPU as an R3D_Mesh.
 *
 * Think of it as a toolbox for procedural or dynamic mesh generation on the CPU.
 *}
type
  PR3D_MeshData = ^TR3D_MeshData;
  TR3D_MeshData = record
    vertices: PR3D_Vertex;       ///< Pointer to vertex data in CPU memory.
    indices: PUInt32;            ///< Pointer to index data in CPU memory.
    vertexCapacity: Integer;     ///< Capacity of vertices.
    indexCapacity: Integer;      ///< Capacity of indices.
    vertexCount: Integer;        ///< Number of vertices.
    indexCount: Integer;         ///< Number of indices.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Allocates a mesh data container with the given capacity.
 *
 * This function allocates CPU-side buffers for vertices and indices, but does NOT
 * initialize the mesh with any actual data. The returned R3D_MeshData has:
 *
 * - vertexCapacity and indexCapacity set to the requested sizes
 * - vertexCount and indexCount set to 0
 *
 * You must manually set vertexCount/indexCount after filling the buffers,
 * or use helper functions like R3D_AppendMeshData() to populate the mesh.
 *
 * All allocated memory is zero-initialized.
 *
 * @param vertexCount Number of vertices to allocate (capacity). Must be > 0.
 * @param indexCount Number of indices to allocate (capacity). May be 0.
 *                   If 0, no index buffer is allocated.
 *
 * @return A new R3D_MeshData with allocated buffers and zero element counts.
 *}
function R3D_LoadMeshData(vertexCount, indexCount: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadMeshData';

{*
 * @brief Releases memory used by a mesh data container.
 * @param meshData R3D_MeshData to destroy.
 *}
procedure R3D_UnloadMeshData(meshData: TR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadMeshData';

{*
 * @brief Check if mesh data is valid.
 *
 * Returns true if the mesh data contains at least one vertex buffer
 * with a positive number of vertices.
 *
 * @param meshData Mesh data to check.
 * @return true if valid, false otherwise.
 *}
function R3D_IsMeshDataValid(meshData: TR3D_MeshData): Boolean; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsMeshDataValid';

{*
 * @brief Generate a quad mesh with specified dimensions, resolution, and orientation.
 *
 * Creates a flat rectangular quad mesh with customizable facing direction.
 * The mesh can be subdivided for higher resolution or displacement mapping.
 * The quad is centered at the origin and oriented according to the frontDir parameter,
 * which defines both the face direction and the surface normal.
 *
 * @param width Width of the quad along its local X axis.
 * @param length Length of the quad along its local Z axis.
 * @param resX Number of subdivisions along the width.
 * @param resZ Number of subdivisions along the length.
 * @param frontDir Direction vector defining the quad's front face and normal.
 *                 This vector will be normalized internally.
 *
 * @return Generated quad mesh structure with proper normals, tangents, and UVs.
 *}
function R3D_GenMeshDataQuad(width, length: Single; resX, resZ: Integer;
  frontDir: TVector3): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataQuad';

{*
 * @brief Generate a plane mesh with specified dimensions and resolution.
 *
 * Creates a flat plane mesh in the XZ plane, centered at the origin.
 * The mesh can be subdivided for higher resolution or displacement mapping.
 *
 * @param width Width of the plane along the X axis.
 * @param length Length of the plane along the Z axis.
 * @param resX Number of subdivisions along the X axis.
 * @param resZ Number of subdivisions along the Z axis.
 *
 * @return Generated plane mesh structure.
 *}
function R3D_GenMeshDataPlane(width, length: Single; resX, resZ: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataPlane';

{*
 * @brief Generate a polygon mesh with specified number of sides.
 *
 * Creates a regular polygon mesh centered at the origin in the XY plane.
 * The polygon is generated with vertices evenly distributed around a circle.
 *
 * @param sides Number of sides for the polygon (minimum 3).
 * @param radius Radius of the circumscribed circle.
 * @param frontDir Direction vector defining the polygon's front face and normal.
 *                 This vector will be normalized internally.
 *
 * @return Generated polygon mesh structure.
 *}
function R3D_GenMeshDataPoly(sides: Integer; radius: Single; frontDir: TVector3): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataPoly';

{*
 * @brief Generate a cube mesh with specified dimensions.
 *
 * Creates a cube mesh centered at the origin with the specified width, height, and length.
 * Each face consists of two triangles with proper normals and texture coordinates.
 *
 * @param width Width of the cube along the X axis.
 * @param height Height of the cube along the Y axis.
 * @param length Length of the cube along the Z axis.
 *
 * @return Generated cube mesh structure.
 *}
function R3D_GenMeshDataCube(width, height, length: Single): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCube';

{*
 * @brief Generate a subdivided cube mesh with specified dimensions.
 *
 * Extension of R3D_GenMeshDataCube() allowing per-axis subdivision.
 * Each face can be tessellated along the X, Y, and Z axes according
 * to the provided resolutions.
 *
 * @param width Width of the cube along the X axis.
 * @param height Height of the cube along the Y axis.
 * @param length Length of the cube along the Z axis.
 * @param resX Number of subdivisions along the X axis.
 * @param resY Number of subdivisions along the Y axis.
 * @param resZ Number of subdivisions along the Z axis.
 *
 * @return Generated cube mesh structure.
 *}
function R3D_GenMeshDataCubeEx(width, height, length: Single; resX, resY, resZ: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCubeEx';

{*
 * @brief Generate a slope mesh by cutting a cube with a plane.
 *
 * Creates a slope mesh by slicing a cube with a plane that passes through the origin.
 * The plane is defined by its normal vector, and the portion of the cube on the side
 * opposite to the normal direction is kept. This allows creating ramps, wedges, and
 * angled surfaces with arbitrary orientations.
 *
 * @param width Width of the base cube along the X axis.
 * @param height Height of the base cube along the Y axis.
 * @param length Length of the base cube along the Z axis.
 * @param slopeNormal Normal vector of the cutting plane. The mesh keeps the portion
 *                    of the cube in the direction opposite to this normal.
 *                    Example: (-1, 0, 0) creates a ramp rising towards +X.
 *                             (0, 1, 0) creates a wedge with the slope facing up.
 *                             (-1.0, 1.0, 0) creates a 45° diagonal slope.
 *
 * @return Generated slope mesh structure.
 *
 * @note The normal vector will be automatically normalized internally.
 * @note The cutting plane always passes through the center of the cube (origin).
 *}
function R3D_GenMeshDataSlope(width, height, length: Single; slopeNormal: TVector3): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataSlope';

{*
 * @brief Generate a sphere mesh with specified parameters.
 *
 * Creates a UV sphere mesh centered at the origin using latitude-longitude subdivision.
 * Higher ring and slice counts produce smoother spheres but with more vertices.
 *
 * @param radius Radius of the sphere.
 * @param rings Number of horizontal rings (latitude divisions).
 * @param slices Number of vertical slices (longitude divisions).
 *
 * @return Generated sphere mesh structure.
 *}
function R3D_GenMeshDataSphere(radius: Single; rings, slices: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataSphere';

{*
 * @brief Generate a hemisphere mesh with specified parameters.
 *
 * Creates a half-sphere mesh (dome) centered at the origin, extending upward in the Y axis.
 * Uses the same UV sphere generation technique as R3D_GenMeshSphere but only the upper half.
 *
 * @param radius Radius of the hemisphere.
 * @param rings Number of horizontal rings (latitude divisions).
 * @param slices Number of vertical slices (longitude divisions).
 *
 * @return Generated hemisphere mesh structure.
 *}
function R3D_GenMeshDataHemiSphere(radius: Single; rings, slices: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataHemiSphere';

{*
 * @brief Generates a cylinder mesh centered at the origin along the Y axis.
 *
 * Both caps are included. For a cone or truncated cone, use R3D_GenMeshDataCylinderEx.
 *
 * @param radius Radius of the cylinder. Must be > 0.
 * @param height Total height along the Y axis. Must be > 0.
 * @param slices Radial subdivisions around the circumference. Must be >= 3.
 *
 * @return The generated mesh data, or an empty mesh on invalid input.
 * @see R3D_GenMeshDataCylinderEx
 *}
function R3D_GenMeshDataCylinder(radius, height: Single; slices: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCylinder';

{*
 * @brief Generates a cylinder, cone, or truncated cone mesh centered at the origin along the Y axis.
 *
 * The bottom cap sits at Y = -height/2 and the top cap at Y = +height/2.
 * Setting one radius to 0 produces a cone; caps can be toggled independently.
 *
 * @param bottomRadius Radius of the bottom end. Must be >= 0. Cannot both be 0.
 * @param topRadius Radius of the top end. Must be >= 0. Cannot both be 0.
 * @param height Total height along the Y axis. Must be > 0.
 * @param slices Radial subdivisions around the circumference. Must be >= 3.
 * @param stacks Vertical subdivisions along the height. Must be >= 1.
 *               Higher values reduce faceting, especially on cones.
 * @param bottomCap Whether to generate the bottom cap.
 * @param topCap Whether to generate the top cap.
 *
 * @return The generated mesh data, or an empty mesh on invalid input.
 *}
function R3D_GenMeshDataCylinderEx(bottomRadius, topRadius, height: Single; slices, stacks: Integer;
  bottomCap, topCap: Boolean): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCylinderEx';

{*
 * @brief Generate a capsule mesh with specified parameters.
 *
 * Creates a capsule mesh centered at the origin, extending along the Y axis.
 * The capsule consists of a cylindrical body with hemispherical caps on both ends.
 * The total height of the capsule is height + 2 * radius.
 *
 * @param radius Radius of the capsule (both cylindrical body and hemispherical caps).
 * @param height Height of the cylindrical portion along the Y axis.
 * @param rings Total number of latitudinal subdivisions for both hemispheres combined.
 * @param slices Number of radial subdivisions around the shape.
 *
 * @return Generated mesh structure.
 *}
function R3D_GenMeshDataCapsule(radius, height: Single; rings, slices: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCapsule';

{*
 * @brief Generate a torus mesh with specified parameters.
 *
 * Creates a torus (donut shape) mesh centered at the origin in the XZ plane.
 * The torus is defined by a major radius (distance from center to tube center)
 * and a minor radius (tube thickness).
 *
 * @param radius Major radius of the torus (distance from center to tube center).
 * @param size Minor radius of the torus (tube thickness/radius).
 * @param radSeg Number of segments around the major radius.
 * @param sides Number of sides around the tube cross-section.
 *
 * @return Generated torus mesh structure.
 *}
function R3D_GenMeshDataTorus(radius, size: Single; radSeg, sides: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataTorus';

{*
 * @brief Generate a trefoil knot mesh with specified parameters.
 *
 * Creates a trefoil knot mesh, which is a mathematical knot shape.
 * Similar to a torus but with a twisted, knotted topology.
 *
 * @param radius Major radius of the knot.
 * @param size Minor radius (tube thickness) of the knot.
 * @param radSeg Number of segments around the major radius.
 * @param sides Number of sides around the tube cross-section.
 *
 * @return Generated trefoil knot mesh structure.
 *}
function R3D_GenMeshDataKnot(radius, size: Single; radSeg, sides: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataKnot';

{*
 * @brief Generate a terrain mesh from a heightmap image.
 *
 * Creates a terrain mesh by interpreting the brightness values of a heightmap image
 * as height values. The resulting mesh represents a 3D terrain surface.
 *
 * @param heightmap Image containing height data (grayscale values represent elevation).
 * @param size 3D vector defining the terrain dimensions (width, max height, depth).
 *
 * @return Generated heightmap terrain mesh structure.
 *}
function R3D_GenMeshDataHeightmap(heightmap: TImage; size: TVector3): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataHeightmap';

{*
 * @brief Generate a voxel-style mesh from a cubicmap image.
 *
 * Creates a mesh composed of cubes based on a cubicmap image, where each pixel
 * represents the presence or absence of a cube at that position. Useful for
 * creating voxel-based or block-based geometry.
 *
 * @param cubicmap Image where pixel values determine cube placement.
 * @param cubeSize 3D vector defining the size of each individual cube.
 *
 * @return Generated cubicmap mesh structure.
 *}
function R3D_GenMeshDataCubicmap(cubicmap: TImage; cubeSize: TVector3): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCubicmap';

{*
 * @brief Reserves memory for the specified number of vertices and indices.
 * @param meshData Target mesh data container.
 * @param vertexCount Number of vertices to reserve space for.
 * @param indexCount Number of indices to reserve space for.
 *}
procedure R3D_ReserveMeshData(meshData: PR3D_MeshData; vertexCount, indexCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ReserveMeshData';

{*
 * @brief Shrinks allocated memory to fit the current vertex and index counts.
 * @param meshData Target mesh data container to shrink.
 *}
procedure R3D_ShrinkMeshData(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ShrinkMeshData';

{*
 * @brief Clears all vertices and indices without releasing allocated memory.
 * @param meshData Target mesh data container to reset.
 *}
procedure R3D_ResetMeshData(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ResetMeshData';

{*
 * @brief Creates a deep copy of an existing mesh data container.
 * @param meshData Source mesh data to duplicate.
 * @return A new R3D_MeshData containing a copy of the source data.
 *}
function R3D_CopyMeshData(meshData: TR3D_MeshData): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CopyMeshData';

{*
 * @brief Merges two mesh data containers into a single one.
 * @param a First mesh data.
 * @param b Second mesh data.
 * @return A new R3D_MeshData containing the merged geometry.
 *}
function R3D_MergeMeshData(a, b: TR3D_MeshData): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_MergeMeshData';

{*
 * @brief Appends vertices and indices to the mesh data container.
 * @param meshData Target mesh data container.
 * @param vertices Array of vertices to append.
 * @param vertexCount Number of vertices to append.
 * @param indices Array of indices to append.
 * @param indexCount Number of indices to append.
 *}
procedure R3D_AppendMeshData(meshData: PR3D_MeshData; vertices: PR3D_Vertex;
  vertexCount: Integer; indices: PUInt32; indexCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_AppendMeshData';

{*
 * @brief Applies a transformation matrix to all vertices in the mesh data.
 * @param meshData Target mesh data container.
 * @param transform Transformation matrix to apply.
 *}
procedure R3D_TransformMeshData(meshData: PR3D_MeshData; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_TransformMeshData';

{*
 * @brief Translates all vertices by a given offset.
 * @param meshData Mesh data to modify.
 * @param translation Offset to apply to all vertex positions.
 *}
procedure R3D_TranslateMeshData(meshData: PR3D_MeshData; translation: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_TranslateMeshData';

{*
 * @brief Rotates all vertices using a quaternion.
 * @param meshData Mesh data to modify.
 * @param rotation Quaternion representing the rotation.
 *}
procedure R3D_RotateMeshData(meshData: PR3D_MeshData; rotation: TQuaternion); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_RotateMeshData';

{*
 * @brief Scales all vertices by given factors.
 * @param meshData Mesh data to modify.
 * @param scale Scaling factors for each axis.
 *}
procedure R3D_ScaleMeshData(meshData: PR3D_MeshData; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_ScaleMeshData';

{*
 * @brief Generates planar UV coordinates.
 * @param meshData Mesh data to modify.
 * @param uvScale Scaling factors for UV coordinates.
 * @param axis Axis along which to project the planar mapping.
 *}
procedure R3D_GenMeshDataUVsPlanar(meshData: PR3D_MeshData; uvScale: TVector2; axis: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataUVsPlanar';

{*
 * @brief Generates spherical UV coordinates.
 * @param meshData Mesh data to modify.
 *}
procedure R3D_GenMeshDataUVsSpherical(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataUVsSpherical';

{*
 * @brief Generates cylindrical UV coordinates.
 * @param meshData Mesh data to modify.
 *}
procedure R3D_GenMeshDataUVsCylindrical(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataUVsCylindrical';

{*
 * @brief Computes vertex normals from triangle geometry.
 * @param meshData Mesh data to modify.
 * @param primitiveType Primitive type of the mesh. Points and line primitives are not
 *                      supported and will default to a front-facing normal (0, 0, 1).
 * @warning Not thread-safe: uses the shared global scratch stack
 *}
procedure R3D_GenMeshDataNormals(meshData: PR3D_MeshData; primitiveType: TR3D_PrimitiveType); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataNormals';

{*
 * @brief Computes vertex tangents based on existing normals and UVs.
 * @param meshData Mesh data to modify.
 * @param primitiveType Primitive type of the mesh. Points and line primitives are not
 *                      supported and will default to a front-facing tangent (1, 0, 0, 1).
 * @warning Not thread-safe: uses the shared global scratch stack
 *}
procedure R3D_GenMeshDataTangents(meshData: PR3D_MeshData; primitiveType: TR3D_PrimitiveType); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataTangents';

{*
 * @brief Calculates the axis-aligned bounding box of the mesh.
 * @param meshData Mesh data to analyze.
 * @return The computed bounding box.
 *}
function R3D_CalculateMeshDataBoundingBox(meshData: TR3D_MeshData): TBoundingBox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CalculateMeshDataBoundingBox';


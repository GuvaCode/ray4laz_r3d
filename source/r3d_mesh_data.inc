// R3D Mesh Data Module.

{*
 * @brief Represents a vertex and all its attributes for a mesh.
 *}
type
  PR3D_Vertex = ^TR3D_Vertex;
  TR3D_Vertex = record
    position: TVector3;           ///< The 3D position of the vertex in object space.
    texcoord: TVector2;           ///< The 2D texture coordinates (UV) for mapping textures.
    normal: TVector3;             ///< The normal vector used for lighting calculations.
    color: TColor;                ///< Vertex color, in RGBA32.
    tangent: TVector4;            ///< The tangent vector, used in normal mapping (often with a handedness in w).
    boneIds: array[0..3] of Integer; ///< Indices of up to 4 bones that influence this vertex (for skinning).
    weights: array[0..3] of Single;  ///< Corresponding bone weights (should sum to 1.0). Defines the influence of each bone.
  end;

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
    vertices: PR3D_Vertex;    ///< Pointer to vertex data in CPU memory.
    indices: PLongWord;       ///< Pointer to index data in CPU memory.
    vertexCount: Integer;     ///< Number of vertices.
    indexCount: Integer;      ///< Number of indices.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Creates an empty mesh data container.
 *
 * Allocates memory for vertex and index buffers. All allocated buffers
 * are zero-initialized.
 *
 * @param vertexCount Number of vertices to allocate. Must be non-zero.
 * @param indexCount Number of indices to allocate. May be zero.
 *                   If zero, no index buffer is allocated.
 *
 * @return A new R3D_MeshData instance with allocated memory.
 *}
function R3D_CreateMeshData(vertexCount, indexCount: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CreateMeshData';

{*
 * @brief Releases memory used by a mesh data container.
 * @param meshData Pointer to the R3D_MeshData to destroy.
 *}
procedure R3D_UnloadMeshData(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadMeshData';

{*
 * @brief Check if mesh data is valid.
 *
 * Returns true if the mesh data contains at least one vertex buffer
 * with a positive number of vertices.
 *
 * @param meshData Pointer to the mesh data to check.
 * @return true if valid, false otherwise.
 *}
function R3D_IsMeshDataValid(const meshData: PR3D_MeshData): Boolean; cdecl;
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
 *
 * @return Generated polygon mesh structure.
 *}
function R3D_GenMeshDataPoly(sides: Integer; radius: Single): TR3D_MeshData; cdecl;
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
 * @brief Generate a cylinder mesh with specified parameters.
 *
 * Creates a cylinder mesh centered at the origin, extending along the Y axis.
 * The cylinder includes both top and bottom caps and smooth side surfaces.
 *
 * @param radius Radius of the cylinder base.
 * @param height Height of the cylinder along the Y axis.
 * @param slices Number of radial subdivisions around the cylinder.
 *
 * @return Generated cylinder mesh structure.
 *}
function R3D_GenMeshDataCylinder(radius, height: Single; slices: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCylinder';

{*
 * @brief Generate a cone mesh with specified parameters.
 *
 * Creates a cone mesh with its base centered at the origin and apex pointing upward along the Y axis.
 * The cone includes a circular base and smooth tapered sides.
 *
 * @param radius Radius of the cone base.
 * @param height Height of the cone along the Y axis.
 * @param slices Number of radial subdivisions around the cone base.
 *
 * @return Generated cone mesh structure.
 *}
function R3D_GenMeshDataCone(radius, height: Single; slices: Integer): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataCone';

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
 * @brief Creates a deep copy of an existing mesh data container.
 * @param meshData Source mesh data to duplicate.
 * @return A new R3D_MeshData containing a copy of the source data.
 *}
function R3D_DuplicateMeshData(const meshData: PR3D_MeshData): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DuplicateMeshData';

{*
 * @brief Merges two mesh data containers into a single one.
 * @param a First mesh data.
 * @param b Second mesh data.
 * @return A new R3D_MeshData containing the merged geometry.
 *}
function R3D_MergeMeshData(const a, b: PR3D_MeshData): TR3D_MeshData; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_MergeMeshData';

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
 *}
procedure R3D_GenMeshDataNormals(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataNormals';

{*
 * @brief Computes vertex tangents based on existing normals and UVs.
 * @param meshData Mesh data to modify.
 *}
procedure R3D_GenMeshDataTangents(meshData: PR3D_MeshData); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshDataTangents';

{*
 * @brief Calculates the axis-aligned bounding box of the mesh.
 * @param meshData Mesh data to analyze.
 * @return The computed bounding box.
 *}
function R3D_CalculateMeshDataBoundingBox(const meshData: PR3D_MeshData): TBoundingBox; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_CalculateMeshDataBoundingBox';

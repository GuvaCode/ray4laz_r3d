// R3D Mesh Module.

{*
 * @brief Hint on how a mesh will be used.
 *}
type
  TR3D_MeshUsage = (
    R3D_STATIC_MESH,            ///< Will never be updated.
    R3D_DYNAMIC_MESH,           ///< Will be updated occasionally.
    R3D_STREAMED_MESH           ///< Will be update on each frame.
  );

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

{*
 * @brief Shadow casting modes for objects.
 *
 * Controls how an object interacts with the shadow mapping system.
 * These modes determine whether the object contributes to shadows,
 * and if so, whether it is also rendered in the main pass.
 *}
type
  TR3D_ShadowCastMode = (
    R3D_SHADOW_CAST_ON_AUTO,            ///< The object casts shadows; the faces used are determined by the material's culling mode.
    R3D_SHADOW_CAST_ON_DOUBLE_SIDED,    ///< The object casts shadows with both front and back faces, ignoring face culling.
    R3D_SHADOW_CAST_ON_FRONT_SIDE,      ///< The object casts shadows with only front faces, culling back faces.
    R3D_SHADOW_CAST_ON_BACK_SIDE,       ///< The object casts shadows with only back faces, culling front faces.
    R3D_SHADOW_CAST_ONLY_AUTO,          ///< The object only casts shadows; the faces used are determined by the material's culling mode.
    R3D_SHADOW_CAST_ONLY_DOUBLE_SIDED,  ///< The object only casts shadows with both front and back faces, ignoring face culling.
    R3D_SHADOW_CAST_ONLY_FRONT_SIDE,    ///< The object only casts shadows with only front faces, culling back faces.
    R3D_SHADOW_CAST_ONLY_BACK_SIDE,     ///< The object only casts shadows with only back faces, culling front faces.
    R3D_SHADOW_CAST_DISABLED            ///< The object does not cast shadows at all.
  );

// ========================================
// STRUCTS TYPES
// ========================================

{*
 * @brief Represents a 3D mesh.
 *
 * Stores vertex and index data, shadow casting settings, bounding box, and layer information.
 * Can represent a static or skinned mesh.
 *}
type
  PR3D_Mesh = ^TR3D_Mesh;
  TR3D_Mesh = record
    vao, vbo, ebo: UInt32;                ///< OpenGL objects handles.
    vertexCount, indexCount: Integer;     ///< Number of vertices and indices currently in use.
    allocVertexCount, allocIndexCount: Integer; ///< Number of vertices and indices allocated in GPU buffers.
    shadowCastMode: TR3D_ShadowCastMode;  ///< Shadow casting mode for the mesh.
    primitiveType: TR3D_PrimitiveType;    ///< Type of primitive that constitutes the vertices.
    usage: TR3D_MeshUsage;                ///< Hint about the usage of the mesh, retained in case of update if there is a reallocation.
    layerMask: TR3D_Layer;                ///< Bitfield indicating the rendering layer(s) of this mesh.
    aabb: TBoundingBox;                   ///< Axis-Aligned Bounding Box in local space.
  end;


{*
 * @brief Creates a 3D mesh from CPU-side mesh data.
 * @param type Primitive type used to interpret vertex data.
 * @param data R3D_MeshData containing vertices and indices (cannot be NULL).
 * @param aabb Optional pointer to a bounding box. If NULL, it will be computed automatically.
 * @param usage Hint on how the mesh will be used.
 * @return Created R3D_Mesh.
 * @note The function copies all vertex and index data into GPU buffers.
 *}
function R3D_LoadMesh(&type: TR3D_PrimitiveType; data: TR3D_MeshData;
  const aabb: PBoundingBox; usage: TR3D_MeshUsage): TR3D_Mesh; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadMesh';


{*
 * @brief Destroys a 3D mesh and frees its resources.
 * @param mesh R3D_Mesh to destroy.
 *}
procedure R3D_UnloadMesh(mesh: TR3D_Mesh); cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadMesh';

{*
 * @brief Check if a mesh is valid for rendering.
 *
 * Returns true if the mesh has a valid VAO and VBO.
 *
 * @param mesh The mesh to check.
 * @return true if valid, false otherwise.
 *}
function R3D_IsMeshValid(const mesh: TR3D_Mesh): Boolean; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_IsMeshValid';

{*
 * @brief Generate a quad mesh with orientation.
 * @param width Width along local X axis.
 * @param length Length along local Z axis.
 * @param resX Subdivisions along width.
 * @param resZ Subdivisions along length.
 * @param frontDir Direction vector for the quad's front face.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataQuad
 *}
function R3D_GenMeshQuad(width, length: Single; resX, resZ: Integer;
  frontDir: TVector3): TR3D_Mesh; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshQuad';

{*
 * @brief Generate a plane mesh.
 * @param width Width along X axis.
 * @param length Length along Z axis.
 * @param resX Subdivisions along X axis.
 * @param resZ Subdivisions along Z axis.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataPlane
 *}
function R3D_GenMeshPlane(width, length: Single; resX, resZ: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshPlane';

{*
 * @brief Generate a polygon mesh.
 * @param sides Number of sides (min 3).
 * @param radius Radius of the polygon.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataPoly
 *}
function R3D_GenMeshPoly(sides: Integer; radius: Single): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshPoly';

{*
 * @brief Generate a cube mesh.
 * @param width Width along X axis.
 * @param height Height along Y axis.
 * @param length Length along Z axis.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataCube
 *}
function R3D_GenMeshCube(width, height, length: Single): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshCube';

{*
 * @brief Generate a sphere mesh.
 * @param radius Sphere radius.
 * @param rings Number of latitude divisions.
 * @param slices Number of longitude divisions.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataSphere
 *}
function R3D_GenMeshSphere(radius: Single; rings, slices: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshSphere';

{*
 * @brief Generate a hemisphere mesh.
 * @param radius Hemisphere radius.
 * @param rings Number of latitude divisions.
 * @param slices Number of longitude divisions.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataHemiSphere
 *}
function R3D_GenMeshHemiSphere(radius: Single; rings, slices: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshHemiSphere';

{*
 * @brief Generate a cylinder mesh.
 * @param radius Cylinder base radius.
 * @param height Height along Y axis.
 * @param slices Radial subdivisions.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataCylinder
 *}
function R3D_GenMeshCylinder(radius, height: Single; slices: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshCylinder';

{*
 * @brief Generate a cone mesh.
 * @param radius Cone base radius.
 * @param height Height along Y axis.
 * @param slices Radial subdivisions.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataCone
 *}
function R3D_GenMeshCone(radius, height: Single; slices: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshCone';

{*
 * @brief Generate a torus mesh.
 * @param radius Major radius (center to tube).
 * @param size Minor radius (tube thickness).
 * @param radSeg Segments around major radius.
 * @param sides Sides around tube cross-section.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataTorus
 *}
function R3D_GenMeshTorus(radius, size: Single; radSeg, sides: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshTorus';

{*
 * @brief Generate a trefoil knot mesh.
 * @param radius Major radius.
 * @param size Minor radius.
 * @param radSeg Segments around major radius.
 * @param sides Sides around tube cross-section.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataKnot
 *}
function R3D_GenMeshKnot(radius, size: Single; radSeg, sides: Integer): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshKnot';

{*
 * @brief Generate a heightmap terrain mesh.
 * @param heightmap Heightmap image.
 * @param size 3D dimensions of terrain.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataHeightmap
 *}
function R3D_GenMeshHeightmap(heightmap: TImage; size: TVector3): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshHeightmap';

{*
 * @brief Generate a cubicmap voxel mesh.
 * @param cubicmap Cubicmap image.
 * @param cubeSize Size of each cube.
 * @return Mesh ready for rendering.
 * @see R3D_GenMeshDataCubicmap
 *}
function R3D_GenMeshCubicmap(cubicmap: TImage; cubeSize: TVector3): TR3D_Mesh;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_GenMeshCubicmap';

{*
 * @brief Upload a mesh data on the GPU.
 *
 * This function uploads a mesh's vertex and optional index data to the GPU.
 *
 * If `aabb` is provided, it will be used as the mesh's bounding box; if null,
 * the bounding box is automatically recalculated from the vertex data.
 *
 * @param mesh Pointer to the mesh structure to update.
 * @param data Mesh data (vertices and indices) to upload.
 * @param aabb Optional bounding box; if null, it is recalculated automatically.
 * @return Returns true if the update is successful, false otherwise.
 *}
function R3D_UpdateMesh(mesh: PR3D_Mesh; data: TR3D_MeshData;
  const aabb: PBoundingBox): Boolean; cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UpdateMesh';





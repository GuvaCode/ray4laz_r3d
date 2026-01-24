// R3D Model Module.

{*
 * @brief Represents a complete 3D model with meshes and materials.
 *
 * Contains multiple meshes and their associated materials, along with animation and bounding information.
 *}
type
  PR3D_Model = ^TR3D_Model;
  TR3D_Model = record
    meshes: PR3D_Mesh;               ///< Array of meshes composing the model.
    meshData: PR3D_MeshData;         ///< Array of meshes data in RAM (optional, can be NULL).
    materials: PR3D_Material;        ///< Array of materials used by the model.
    meshMaterials: PInteger;         ///< Array of material indices, one per mesh.

    meshCount: Integer;              ///< Number of meshes.
    materialCount: Integer;          ///< Number of materials.

    aabb: TBoundingBox;              ///< Axis-Aligned Bounding Box encompassing the whole model.
    skeleton: TR3D_Skeleton;         ///< Skeleton hierarchy and bind pose used for skinning (NULL if non-skinned).
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Load a 3D model from a file.
 *
 * Loads a 3D model from the specified file path. Supports various 3D file formats
 * and automatically parses meshes, materials, and texture references.
 *
 * @param filePath Path to the 3D model file to load.
 *
 * @return Loaded model structure containing meshes and materials.
 *}
function R3D_LoadModel(const filePath: PAnsiChar): TR3D_Model; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadModel';

{*
 * @brief Load a 3D model from a file with import flags.
 *
 * Extended version of R3D_LoadModel() allowing control over the import
 * process through additional flags.
 *
 * @param filePath Path to the 3D model file to load.
 * @param flags Importer behavior flags.
 *
 * @return Loaded model structure containing meshes and materials.
 *}
function R3D_LoadModelEx(const filePath: PAnsiChar; flags: R3D_ImportFlags): TR3D_Model; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadModelEx';

{*
 * @brief Load a 3D model from memory buffer.
 *
 * Loads a 3D model from a memory buffer containing the file data.
 * Useful for loading models from embedded resources or network streams.
 *
 * @param data Pointer to the memory buffer containing the model data.
 * @param size Size of the data buffer in bytes.
 * @param hint Hint on the model format (can be NULL).
 *
 * @return Loaded model structure containing meshes and materials.
 *
 * @note External dependencies (e.g., textures or linked resources) are not supported.
 *       The model data must be fully self-contained. Use embedded formats like .glb to ensure compatibility.
 *}
function R3D_LoadModelFromMemory(const data: Pointer; size: LongWord;
  const hint: PAnsiChar): TR3D_Model; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadModelFromMemory';

{*
 * @brief Load a 3D model from a memory buffer with import flags.
 *
 * Extended version of R3D_LoadModelFromMemory() allowing control over
 * the import process through additional flags.
 *
 * @param data Pointer to the memory buffer containing the model data.
 * @param size Size of the data buffer in bytes.
 * @param hint Hint on the model format (can be NULL).
 * @param flags Importer behavior flags.
 *
 * @return Loaded model structure containing meshes and materials.
 *
 * @note External dependencies (e.g., textures or linked resources) are not supported.
 *       The model data must be fully self-contained.
 *}
function R3D_LoadModelFromMemoryEx(const data: Pointer; size: LongWord;
  const hint: PAnsiChar; flags: R3D_ImportFlags): TR3D_Model; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadModelFromMemoryEx';

{*
 * @brief Load a 3D model from an existing importer.
 *
 * Creates a model from a previously loaded importer instance.
 * This avoids re-importing the source file.
 *
 * @param importer Importer instance to extract the model from.
 *
 * @return Loaded model structure containing meshes and materials.
 *}
function R3D_LoadModelFromImporter(const importer: PR3D_Importer): TR3D_Model; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadModelFromImporter';

{*
 * @brief Unload a model and optionally its materials.
 *
 * Frees all memory associated with a model, including its meshes.
 * Materials can be optionally unloaded as well.
 *
 * @param model The model to be unloaded.
 * @param unloadMaterials If true, also unloads all materials associated with the model.
 * Set to false if textures are still being used elsewhere to avoid freeing shared resources.
 *}
procedure R3D_UnloadModel(model: TR3D_Model; unloadMaterials: Boolean); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadModel';

// r3d_importer.inc -- R3D Importer Module.


// ========================================
// ENUMS TYPES
// ========================================

type
  {*
   * Flags controlling importer behavior.
   * These flags define how the importer processes the source asset.
   *}
  R3D_ImportFlags = cuint32;

const
  {*
   * Keep a CPU-side copy of mesh data.
   * When enabled, raw mesh data is preserved in RAM after model import.
   *}
  R3D_IMPORT_MESH_DATA    = 1 shl 0;

  {*
   * Enable high-quality import processing.
   * When enabled, the importer uses a higher-quality post-processing
   * (e.g. smooth normals, mesh optimization, data validation).
   * This mode is intended for editor usage and offline processing.
   * When disabled, a faster import preset is used, suitable for runtime.
   *}
  R3D_IMPORT_QUALITY      = 1 shl 1;


// ========================================
// STRUCTS TYPES
// ========================================

type
  {*
   * Opaque importer handle.
   * Represents a loaded asset file that can be used to extract multiple
   * resources (models, skeletons, animations) without re-importing the file.
   *}
  PR3D_Importer = ^TR3D_Importer;
  TR3D_Importer = record
    { Internal structure - opaque }
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Load an importer from a file.
 *
 * Creates an importer instance from the specified file path.
 * The file is parsed once and can be reused to extract multiple
 * resources such as models and animations.
 *
 * @param filePath Path to the asset file.
 * @param flags Importer behavior flags.
 *
 * @return Pointer to a new importer instance, or NULL on failure.
 *}
function R3D_LoadImporter(const filePath: PChar; flags: R3D_ImportFlags): PR3D_Importer;
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadImporter';


{*
 * @brief Load an importer from a memory buffer.
 *
 * Creates an importer instance from in-memory asset data.
 * This is useful for embedded assets or streamed content.
 *
 * @param data Pointer to the asset data.
 * @param size Size of the data buffer in bytes.
 * @param hint Optional file format hint (may be NULL).
 * @param flags Importer behavior flags.
 *
 * @return Pointer to a new importer instance, or NULL on failure.
 *}
function R3D_LoadImporterFromMemory(const data: Pointer; size: cuint; const hint: PChar; flags: R3D_ImportFlags): PR3D_Importer;
   cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_LoadImporterFromMemory';

{*
 * @brief Destroy an importer instance.
 *
 * Frees all resources associated with the importer.
 * Any models or animations extracted from it remain valid.
 *
 * @param importer Importer instance to destroy.
 *}
procedure R3D_UnloadImporter(importer: PR3D_Importer);
  cdecl; external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_UnloadImporter';



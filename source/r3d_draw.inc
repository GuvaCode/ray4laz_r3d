// R3D Draw Module.

{*
 * @brief Begins a rendering session for a 3D camera.
 *
 * This function starts a rendering session, preparing the engine to handle subsequent
 * draw calls using the provided camera settings. Rendering output will be directed
 * to the default screen framebuffer.
 *
 * @param camera The camera to use for rendering the scene.
 *}
procedure R3D_Begin(camera: TCamera3D); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Begin';

{*
 * @brief Begins a rendering session for a 3D camera with an optional custom render target.
 *
 * This function starts a rendering session, preparing the engine to handle subsequent
 * draw calls using the provided camera settings. If a render target is provided, rendering
 * output will be directed to it. If the target is `NULL`, rendering will be performed
 * directly to the screen framebuffer (same behavior as R3D_Begin).
 *
 * @param camera The camera to use for rendering the scene.
 * @param target Optional pointer to a RenderTexture to render into. Can be NULL to render
 *               directly to the screen.
 *}
procedure R3D_BeginEx(camera: TCamera3D; const target: PRenderTexture); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_BeginEx';

{*
 * @brief Ends the current rendering session.
 *
 * This function signals the end of a rendering session, at which point the engine
 * will process all necessary render passes and output the final result to the main
 * or custom framebuffer.
 *}
procedure R3D_End; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_End';

{*
 * @brief Draws a mesh with a specified material and transformation.
 *
 * This function renders a mesh with the provided material and transformation matrix.
 *
 * @param mesh A pointer to the mesh to render. Cannot be NULL.
 * @param material A pointer to the material to apply to the mesh. Can be NULL, default material will be used.
 * @param transform The transformation matrix to apply to the mesh.
 *}
procedure R3D_DrawMesh(const mesh: PR3D_Mesh; const material: PR3D_Material;
  transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMesh';

{*
 * @brief Draws a mesh with instancing support.
 *
 * This function renders a mesh multiple times with different transformation matrices
 * for each instance.
 *
 * @param mesh A pointer to the mesh to render. Cannot be NULL.
 * @param material A pointer to the material to apply to the mesh. Can be NULL, default material will be used.
 * @param instanceTransforms Array of transformation matrices for each instance. Cannot be NULL.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawMeshInstanced(const mesh: PR3D_Mesh; const material: PR3D_Material;
  const instanceTransforms: PMatrix; instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstanced';

{*
 * @brief Draws a mesh with instancing support and different colors per instance.
 *
 * This function renders a mesh multiple times with different transformation matrices
 * and different colors for each instance.
 *
 * @param mesh A pointer to the mesh to render. Cannot be NULL.
 * @param material A pointer to the material to apply to the mesh. Can be NULL, default material will be used.
 * @param instanceTransforms Array of transformation matrices for each instance. Cannot be NULL.
 * @param instanceColors Array of colors for each instance. Can be NULL if no per-instance colors are needed.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawMeshInstancedEx(const mesh: PR3D_Mesh; const material: PR3D_Material;
  const instanceTransforms: PMatrix; const instanceColors: PColor; instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstancedEx';

{*
 * @brief Draws a mesh with instancing support, a global transformation, and different colors per instance.
 *
 * This function renders a mesh multiple times using instancing, with a global transformation
 * applied to all instances, and individual transformation matrices and colors for each instance.
 * Each instance can have its own position, rotation, scale, and color while sharing the same mesh
 * and material.
 *
 * @param mesh A pointer to the mesh to render. Cannot be NULL.
 * @param material A pointer to the material to apply to the mesh. Can be NULL, default material will be used.
 * @param globalAabb Optional bounding box encompassing all instances, in local space. Used for frustum culling.
 *                   Can be NULL to disable culling. Will be transformed by the global matrix if necessary.
 * @param globalTransform The global transformation matrix applied to all instances.
 * @param instanceTransforms Pointer to an array of transformation matrices for each instance, allowing unique transformations. Cannot be NULL.
 * @param transformsStride The stride (in bytes) между последовательными матрицами преобразования в массиве.
 *                         Set to 0 if the matrices are tightly packed (stride equals sizeof(Matrix)).
 *                         If matrices are embedded in a struct, set to the size of the struct or the actual byte offset between elements.
 * @param instanceColors Pointer to an array of colors for each instance, allowing unique colors. Can be NULL if no per-instance colors are needed.
 * @param colorsStride The stride (in bytes) между последовательными цветами в массиве.
 *                     Set to 0 if the colors are tightly packed (stride equals sizeof(Color)).
 *                     If colors are embedded in a struct, set to the size of the struct or the actual byte offset between elements.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawMeshInstancedPro(const mesh: PR3D_Mesh; const material: PR3D_Material;
  const globalAabb: PBoundingBox; globalTransform: TMatrix;
  const instanceTransforms: PMatrix; transformsStride: Integer;
  const instanceColors: PColor; colorsStride: Integer;
  instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstancedPro';

{*
 * @brief Draws a model at a specified position and scale.
 *
 * This function renders a model at the given position with the specified scale factor.
 *
 * @param model A pointer to the model to render.
 * @param position The position to place the model at.
 * @param scale The scale factor to apply to the model.
 *}
procedure R3D_DrawModel(const model: PR3D_Model; position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModel';

{*
 * @brief Draws a model with advanced transformation options.
 *
 * This function renders a model with a specified position, rotation axis, rotation
 * angle, and scale. It provides more control over how the model is transformed before
 * rendering.
 *
 * @param model A pointer to the model to render.
 * @param position The position to place the model at.
 * @param rotationAxis The axis of rotation for the model.
 * @param rotationAngle The angle to rotate the model.
 * @param scale The scale factor to apply to the model.
 *}
procedure R3D_DrawModelEx(const model: PR3D_Model; position, rotationAxis: TVector3;
  rotationAngle: Single; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelEx';

{*
 * @brief Draws a model using a transformation matrix.
 *
 * This function renders a model using a custom transformation matrix, allowing full control
 * over the model's position, rotation, scale, and skew. It is intended for advanced rendering
 * scenarios where a single matrix defines the complete transformation.
 *
 * @param model A pointer to the model to render.
 * @param transform A transformation matrix that defines how to position, rotate, and scale the model.
 *}
procedure R3D_DrawModelPro(const model: PR3D_Model; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelPro';

{*
 * @brief Draws a model with instancing support.
 *
 * This function renders a model multiple times with different transformation matrices
 * for each instance.
 *
 * @param model A pointer to the model to render. Cannot be NULL.
 * @param instanceTransforms Array of transformation matrices for each instance. Cannot be NULL.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawModelInstanced(const model: PR3D_Model;
  const instanceTransforms: PMatrix; instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstanced';

{*
 * @brief Draws a model with instancing support and different colors per instance.
 *
 * This function renders a model multiple times with different transformation matrices
 * and different colors for each instance.
 *
 * @param model A pointer to the model to render. Cannot be NULL.
 * @param instanceTransforms Array of transformation matrices for each instance. Cannot be NULL.
 * @param instanceColors Array of colors for each instance. Can be NULL if no per-instance colors are needed.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawModelInstancedEx(const model: PR3D_Model;
  const instanceTransforms: PMatrix; const instanceColors: PColor;
  instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstancedEx';

{*
 * @brief Draws a model with instancing support, a global transformation, and different colors per instance.
 *
 * This function renders a model multiple times using instancing, with a global transformation
 * applied to all instances, and individual transformation matrices and colors for each instance.
 * Each instance can have its own position, rotation, scale, and color while sharing the same model.
 *
 * @param model A pointer to the model to render. Cannot be NULL.
 * @param globalAabb Optional bounding box encompassing all instances, in local space. Used for frustum culling.
 *                   Can be NULL to disable culling. Will be transformed by the global matrix if necessary.
 * @param globalTransform The global transformation matrix applied to all instances.
 * @param instanceTransforms Pointer to an array of transformation matrices for each instance, allowing unique transformations. Cannot be NULL.
 * @param transformsStride The stride (in bytes) между последовательными матрицами преобразования в массиве.
 *                         Set to 0 if the matrices are tightly packed (stride equals sizeof(Matrix)).
 *                         If matrices are embedded in a struct, set to the size of the struct or the actual byte offset between elements.
 * @param instanceColors Pointer to an array of colors for each instance, allowing unique colors. Can be NULL if no per-instance colors are needed.
 * @param colorsStride The stride (in bytes) между последовательными цветами в массиве.
 *                     Set to 0 if the colors are tightly packed (stride equals sizeof(Color)).
 *                     If colors are embedded in a struct, set to the size of the struct or the actual byte offset between elements.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawModelInstancedPro(const model: PR3D_Model;
  const globalAabb: PBoundingBox; globalTransform: TMatrix;
  const instanceTransforms: PMatrix; transformsStride: Integer;
  const instanceColors: PColor; colorsStride: Integer;
  instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstancedPro';

{*
 * @brief Draws a decal using a transformation matrix.
 *
 * This function renders a decal in 3D space at the given position.
 *
 * @param decal A pointer to the decal to render.
 * @param transform A transformation matrix that defines how to position, rotate, and scale the decal.
 *}
procedure R3D_DrawDecal(const decal: PR3D_Decal; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecal';

{*
 * @brief Draws a decal with instancing support.
 *
 * This function renders a decal multiple times with different transformation matrices
 * for each instance.
 *
 * @param decal A pointer to the decal to render. Cannot be NULL.
 * @param instanceTransforms Array of transformation matrices for each instance. Cannot be NULL.
 * @param instanceCount The number of instances to render. Must be greater than 0.
 *}
procedure R3D_DrawDecalInstanced(const decal: PR3D_Decal;
  const instanceTransforms: PMatrix; instanceCount: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalInstanced';

{*
 * @brief Renders the current state of a CPU-based particle system.
 *
 * This function draws the particles of a CPU-simulated particle system
 * in their current state. It does not modify the simulation or update
 * particle properties such as position, velocity, or lifetime.
 *
 * @param system A pointer to the `R3D_ParticleSystem` to be rendered.
 *               The particle system must be properly initialized and updated
 *               to the desired state before calling this function.
 * @param mesh A pointer to the mesh used to represent each particle. Cannot be NULL.
 * @param material A pointer to the material applied to the particle mesh. Can be NULL, default material will be used.
 *}
procedure R3D_DrawParticleSystem(const system: PR3D_ParticleSystem;
  const mesh: PR3D_Mesh; const material: PR3D_Material); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawParticleSystem';

{*
 * @brief Renders the current state of a CPU-based particle system with a global transformation.
 *
 * This function is similar to `R3D_DrawParticleSystem`, but it applies an additional
 * global transformation to all particles. This is useful for rendering particle effects
 * in a transformed space (e.g., attached to a moving object).
 *
 * @param system A pointer to the `R3D_ParticleSystem` to be rendered.
 *               The particle system must be properly initialized and updated
 *               to the desired state before calling this function.
 * @param mesh A pointer to the mesh used to represent each particle. Cannot be NULL.
 * @param material A pointer to the material applied to the particle mesh. Can be NULL, default material will be used.
 * @param transform A transformation matrix applied to all particles.
 *}
procedure R3D_DrawParticleSystemEx(const system: PR3D_ParticleSystem;
  const mesh: PR3D_Mesh; const material: PR3D_Material; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawParticleSystemEx';

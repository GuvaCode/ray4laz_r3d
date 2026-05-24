// R3D Draw Module.
// Complete translation of r3d_draw.h to Pascal

{*
 * @brief Describes an R3D rendering view.
 *
 * A view defines the camera and output area used for a single rendering
 * session. It combines an R3D camera, an optional render target and an optional
 * viewport inside that target.
 *
 * If `target` is zero-initialized, rendering is directed to the default
 * framebuffer. If `viewport.width` or `viewport.height` is less than or equal
 * to zero, the full target size is used.
 *}
type
  PR3D_View = ^TR3D_View;
  TR3D_View = record
    camera: TR3D_Camera;      ///< Camera used for this view.
    target: TRenderTexture;   ///< Render target. Zero-initialized means screen/backbuffer.
    viewport: TRectangle;     ///< Viewport inside the target. If width or height <= 0, the full target is used.
  end;

// ========================================
// PUBLIC API
// ========================================

{*
 * @brief Begins a rendering session using the given raylib camera.
 *
 * Rendering output is directed to the default framebuffer.
 *
 * The given Camera3D is converted internally to an R3D_Camera. Since raylib
 * cameras do not store near/far clipping planes, the converted camera uses the
 * current rlgl culling distances for those values.
 *
 * @param camera Camera used to render the scene.
 *}
procedure R3D_Begin(camera: TCamera3D); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Begin';

{*
 * @brief Begins a rendering session using an R3D camera.
 *
 * Rendering output is directed to the default framebuffer.
 *
 * This entry point provides access to R3D-specific camera features such as
 * layer masks, custom near/far clipping planes and quaternion-based orientation.
 *
 * @param camera Camera used to render the scene.
 *}
procedure R3D_BeginEx(camera: TR3D_Camera); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_BeginEx';

{*
 * @brief Begins a rendering session using a complete R3D view descriptor.
 *
 * This is the advanced entry point. It allows the caller to specify the camera,
 * render target and viewport used for the rendering session.
 *
 * The view camera is used as-is, including its near/far clipping planes and
 * layer mask. If the camera was created from a raylib Camera3D using
 * `R3D_CameraFromRL()`, its near/far planes come from the current rlgl culling
 * distances because raylib cameras do not store those values directly.
 *
 * Use this function for render-to-texture workflows, custom viewports,
 * multipass rendering, editor views, minimaps, probes or any case where the
 * default framebuffer is not enough.
 *
 * @param view View descriptor used to render the scene.
 *}
procedure R3D_BeginPro(view: TR3D_View); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_BeginPro';

{*
 * @brief Ends the current rendering session.
 *
 * This function is the one that actually performs the full
 * rendering of the described scene. It carries out culling,
 * sorting, shadow rendering, scene rendering, and screen /
 * post-processing effects.
 *}
procedure R3D_End; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_End';

{*
 * @brief Begins a clustered draw pass.
 *
 * All draw calls submitted in this pass are first tested against the
 * cluster AABB. If the cluster fails the scene/shadow frustum test,
 * none of the contained objects are tested or drawn.
 *
 * @param aabb Bounding box used as the cluster-level frustum test.
 *}
procedure R3D_BeginCluster(aabb: TBoundingBox); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_BeginCluster';

{*
 * @brief Ends the current clustered draw pass.
 *
 * Stops submitting draw calls to the active cluster.
 *}
procedure R3D_EndCluster; cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_EndCluster';

{*
 * @brief Queues a mesh draw command with position and uniform scale.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawMesh(mesh: TR3D_Mesh; material: TR3D_Material;
  position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMesh';

{*
 * @brief Queues a mesh draw command with position, rotation and non-uniform scale.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawMeshEx(mesh: TR3D_Mesh; material: TR3D_Material;
  position: TVector3; rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshEx';

{*
 * @brief Queues a mesh draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawMeshPro(mesh: TR3D_Mesh; material: TR3D_Material;
  transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshPro';

{*
 * @brief Queues an instanced mesh draw command.
 *
 * Draws multiple instances using the provided instance buffer.
 * Does nothing if the number of instances is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawMeshInstanced(mesh: TR3D_Mesh; material: TR3D_Material;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstanced';

{*
 * @brief Queues an instanced mesh draw command with an instance range.
 *
 * Draws 'count' instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawMeshInstancedEx(mesh: TR3D_Mesh; material: TR3D_Material;
  instances: TR3D_InstanceBuffer; offset, count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstancedEx';

{*
 * @brief Queues an instanced mesh draw command with an instance range and an additional transform.
 *
 * Draws 'count' instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 * The transform is applied to all instances.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawMeshInstancedPro(mesh: TR3D_Mesh; material: TR3D_Material;
  instances: TR3D_InstanceBuffer; offset, count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstancedPro';

{*
 * @brief Queues a model draw command with position and uniform scale.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawModel(model: TR3D_Model; position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModel';

{*
 * @brief Queues a model draw command with position, rotation and non-uniform scale.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawModelEx(model: TR3D_Model; position: TVector3;
  rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelEx';

{*
 * @brief Queues a model draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawModelPro(model: TR3D_Model; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelPro';

{*
 * @brief Queues an instanced model draw command.
 *
 * Draws multiple instances using the provided instance buffer.
 * Does nothing if the number of instances is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawModelInstanced(model: TR3D_Model;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstanced';

{*
 * @brief Queues an instanced model draw command with an instance range.
 *
 * Draws 'count' instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawModelInstancedEx(model: TR3D_Model;
  instances: TR3D_InstanceBuffer; offset, count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstancedEx';

{*
 * @brief Queues an instanced model draw command with an instance range and an additional transform.
 *
 * Draws 'count' instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 * The transform is applied to all instances.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawModelInstancedPro(model: TR3D_Model;
  instances: TR3D_InstanceBuffer; offset, count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstancedPro';

{*
 * @brief Queues an animated model draw command.
 *
 * Uses the provided animation player to compute the pose.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawAnimatedModel(model: TR3D_Model; player: TR3D_AnimationPlayer;
  position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModel';

{*
 * @brief Queues an animated model draw command with position, rotation and non-uniform scale.
 *
 * Uses the provided animation player to compute the pose.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawAnimatedModelEx(model: TR3D_Model; player: TR3D_AnimationPlayer;
  position: TVector3; rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelEx';

{*
 * @brief Queues an animated model draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawAnimatedModelPro(model: TR3D_Model; player: TR3D_AnimationPlayer;
  transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelPro';

{*
 * @brief Queues an instanced animated model draw command.
 *
 * Draws multiple animated instances using the provided instance buffer.
 * Does nothing if the number of instances is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawAnimatedModelInstanced(model: TR3D_Model; player: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelInstanced';

{*
 * @brief Queues an instanced animated model draw command with an instance range.
 *
 * Draws 'count' animated instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawAnimatedModelInstancedEx(model: TR3D_Model; player: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer; offset, count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelInstancedEx';

{*
 * @brief Queues an instanced animated model draw command with an instance range and an additional transform.
 *
 * Draws 'count' animated instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 * The transform is applied to all instances.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawAnimatedModelInstancedPro(model: TR3D_Model; player: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer; offset, count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelInstancedPro';

{*
 * @brief Queues a decal draw command with position and uniform scale.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawDecal(decal: TR3D_Decal; position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecal';

{*
 * @brief Queues a decal draw command with position, rotation and non-uniform scale.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawDecalEx(decal: TR3D_Decal; position: TVector3;
  rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalEx';

{*
 * @brief Queues a decal draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawDecalPro(decal: TR3D_Decal; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalPro';

{*
 * @brief Queues an instanced decal draw command.
 *
 * Draws multiple instances using the provided instance buffer.
 * Does nothing if the number of instances is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawDecalInstanced(decal: TR3D_Decal;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalInstanced';

{*
 * @brief Queues an instanced decal draw command with an instance range.
 *
 * Draws 'count' instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawDecalInstancedEx(decal: TR3D_Decal;
  instances: TR3D_InstanceBuffer; offset, count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalInstancedEx';

{*
 * @brief Queues an instanced decal draw command with an instance range and an additional transform.
 *
 * Draws 'count' instances starting at 'offset' in the instance buffer.
 * Both 'offset' and 'count' are clamped to stay within [0, instances.capacity]:
 *   - offset is clamped to [0, capacity]
 *   - count is clamped to [0, capacity - offset]
 * Does nothing if the resulting count is <= 0.
 * The transform is applied to all instances.
 *
 * The command is executed during R3D_End().
 *}
procedure R3D_DrawDecalInstancedPro(decal: TR3D_Decal;
  instances: TR3D_InstanceBuffer; offset, count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalInstancedPro';

// R3D Draw Module.

{*
 * @brief Begins a rendering session using the given camera.
 *
 * Rendering output is directed to the default framebuffer.
 *
 * @param camera Camera used to render the scene.
 *}
procedure R3D_Begin(camera: TCamera3D); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_Begin';

{*
 * @brief Begins a rendering session with a custom render target.
 *
 * If the render target is invalid (ID = 0), rendering goes to the screen.
 *
 * @param target Render texture to render into.
 * @param camera Camera used to render the scene.
 *}
procedure R3D_BeginEx(target: TRenderTexture; camera: TCamera3D); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_BeginEx';

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
 *
 * @param mesh Mesh to render.
 * @param material Material to apply to the mesh.
 * @param position Position of the mesh.
 * @param scale Uniform scale factor.
 *}
procedure R3D_DrawMesh(mesh: TR3D_Mesh; material: TR3D_Material;
  position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMesh';

{*
 * @brief Queues a mesh draw command with position, rotation and non-uniform scale.
 *
 * The command is executed during R3D_End().
 *
 * @param mesh Mesh to render.
 * @param material Material to apply to the mesh.
 * @param position Position of the mesh.
 * @param rotation Rotation quaternion.
 * @param scale Non-uniform scale vector.
 *}
procedure R3D_DrawMeshEx(mesh: TR3D_Mesh; material: TR3D_Material;
  position: TVector3; rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshEx';

{*
 * @brief Queues a mesh draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *
 * @param mesh Mesh to render.
 * @param material Material to apply to the mesh.
 * @param transform Full transformation matrix.
 *}
procedure R3D_DrawMeshPro(mesh: TR3D_Mesh; material: TR3D_Material;
  transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshPro';

{*
 * @brief Queues an instanced mesh draw command.
 *
 * Draws multiple instances using the provided instance buffer.
 * The command is executed during R3D_End().
 *
 * @param mesh Mesh to render.
 * @param material Material to apply to the mesh.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 *}
procedure R3D_DrawMeshInstanced(mesh: TR3D_Mesh; material: TR3D_Material;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstanced';

{*
 * @brief Queues an instanced mesh draw command with an additional transform.
 *
 * The transform is applied to all instances.
 * The command is executed during R3D_End().
 *
 * @param mesh Mesh to render.
 * @param material Material to apply to the mesh.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 * @param transform Additional transformation matrix applied to all instances.
 *}
procedure R3D_DrawMeshInstancedEx(mesh: TR3D_Mesh; material: TR3D_Material;
  instances: TR3D_InstanceBuffer; count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawMeshInstancedEx';

{*
 * @brief Queues a model draw command with position and uniform scale.
 *
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param position Position of the model.
 * @param scale Uniform scale factor.
 *}
procedure R3D_DrawModel(model: TR3D_Model; position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModel';

{*
 * @brief Queues a model draw command with position, rotation and non-uniform scale.
 *
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param position Position of the model.
 * @param rotation Rotation quaternion.
 * @param scale Non-uniform scale vector.
 *}
procedure R3D_DrawModelEx(model: TR3D_Model; position: TVector3;
  rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelEx';

{*
 * @brief Queues a model draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param transform Full transformation matrix.
 *}
procedure R3D_DrawModelPro(model: TR3D_Model; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelPro';

{*
 * @brief Queues an instanced model draw command.
 *
 * Draws multiple instances using the provided instance buffer.
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 *}
procedure R3D_DrawModelInstanced(model: TR3D_Model;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstanced';

{*
 * @brief Queues an instanced model draw command with an additional transform.
 *
 * The transform is applied to all instances.
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 * @param transform Additional transformation matrix applied to all instances.
 *}
procedure R3D_DrawModelInstancedEx(model: TR3D_Model;
  instances: TR3D_InstanceBuffer; count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawModelInstancedEx';

{*
 * @brief Queues an animated model draw command.
 *
 * Uses the provided animation player to compute the pose.
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param player Animation player controlling the model's pose.
 * @param position Position of the model.
 * @param scale Uniform scale factor.
 *}
procedure R3D_DrawAnimatedModel(model: TR3D_Model; player: TR3D_AnimationPlayer;
  position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModel';

{*
 * @brief Queues an animated model draw command with position, rotation and non-uniform scale.
 *
 * Uses the provided animation player to compute the pose.
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param player Animation player controlling the model's pose.
 * @param position Position of the model.
 * @param rotation Rotation quaternion.
 * @param scale Non-uniform scale vector.
 *}
procedure R3D_DrawAnimatedModelEx(model: TR3D_Model; player: TR3D_AnimationPlayer;
  position: TVector3; rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelEx';

{*
 * @brief Queues an animated model draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param player Animation player controlling the model's pose.
 * @param transform Full transformation matrix.
 *}
procedure R3D_DrawAnimatedModelPro(model: TR3D_Model; player: TR3D_AnimationPlayer;
  transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelPro';

{*
 * @brief Queues an instanced animated model draw command.
 *
 * Draws multiple animated instances using the provided instance buffer.
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param player Animation player controlling the model's pose.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 *}
procedure R3D_DrawAnimatedModelInstanced(model: TR3D_Model; player: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelInstanced';

{*
 * @brief Queues an instanced animated model draw command with an additional transform.
 *
 * The transform is applied to all instances.
 * The command is executed during R3D_End().
 *
 * @param model Model to render.
 * @param player Animation player controlling the model's pose.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 * @param transform Additional transformation matrix applied to all instances.
 *}
procedure R3D_DrawAnimatedModelInstancedEx(model: TR3D_Model; player: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer; count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawAnimatedModelInstancedEx';

{*
 * @brief Queues a decal draw command with position and uniform scale.
 *
 * The command is executed during R3D_End().
 *
 * @param decal Decal to render.
 * @param position Position of the decal.
 * @param scale Uniform scale factor.
 *}
procedure R3D_DrawDecal(decal: TR3D_Decal; position: TVector3; scale: Single); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecal';

{*
 * @brief Queues a decal draw command with position, rotation and non-uniform scale.
 *
 * The command is executed during R3D_End().
 *
 * @param decal Decal to render.
 * @param position Position of the decal.
 * @param rotation Rotation quaternion.
 * @param scale Non-uniform scale vector.
 *}
procedure R3D_DrawDecalEx(decal: TR3D_Decal; position: TVector3;
  rotation: TQuaternion; scale: TVector3); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalEx';

{*
 * @brief Queues a decal draw command using a full transform matrix.
 *
 * The command is executed during R3D_End().
 *
 * @param decal Decal to render.
 * @param transform Full transformation matrix.
 *}
procedure R3D_DrawDecalPro(decal: TR3D_Decal; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalPro';

{*
 * @brief Queues an instanced decal draw command.
 *
 * Draws multiple instances using the provided instance buffer.
 * The command is executed during R3D_End().
 *
 * @param decal Decal to render.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 *}
procedure R3D_DrawDecalInstanced(decal: TR3D_Decal;
  instances: TR3D_InstanceBuffer; count: Integer); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalInstanced';

{*
 * @brief Queues an instanced decal draw command with an additional transform.
 *
 * The transform is applied to all instances.
 * The command is executed during R3D_End().
 *
 * @param decal Decal to render.
 * @param instances Instance buffer containing instance data.
 * @param count Number of instances to draw.
 * @param transform Additional transformation matrix applied to all instances.
 *}
procedure R3D_DrawDecalInstancedEx(decal: TR3D_Decal;
  instances: TR3D_InstanceBuffer; count: Integer; transform: TMatrix); cdecl;
  external {$IFNDEF RAY_STATIC}r3dName{$ENDIF} name 'R3D_DrawDecalInstancedEx';


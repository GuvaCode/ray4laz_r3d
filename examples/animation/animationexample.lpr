program AnimationExample;

{$mode objfpc}{$H+}

uses
  SysUtils,
  raylib,
  r3d,
  raymath;

const
    RESOURCES_PATH = 'resources/';

var
  // Window and camera
  screenWidth, screenHeight: Integer;
  camera: TCamera3D;

  // R3D resources
  cubemap: TR3D_Cubemap;
  ambientMap: TR3D_AmbientMap;
  plane: TR3D_Mesh;
  model: TR3D_Model;
  modelAnims: TR3D_AnimationLib;
  modelPlayer: TR3D_AnimationPlayer;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  light: TR3D_Light;

  // Loop variables
  delta: Single;
  i, x, z: Integer;
begin
  // Initialize window
  screenWidth := 800;
  screenHeight := 450;
  InitWindow(screenWidth, screenHeight, '[r3d] - Animation example');
  SetTargetFPS(60);

  // Initialize R3D with FXAA
  R3D_Init(GetScreenWidth(), GetScreenHeight());
  R3D_SetAntiAliasingMode(R3D_ANTI_ALIASING_MODE_FXAA);

  // Setup environment sky
  cubemap := R3D_LoadCubemap(RESOURCES_PATH + 'panorama/indoor.hdr', R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  R3D_ENVIRONMENT_SET('background.skyBlur', 0.3);
  R3D_ENVIRONMENT_SET('background.energy', 0.6);
  R3D_ENVIRONMENT_SET('background.sky', cubemap);

  // Setup environment ambient
  ambientMap := R3D_GenAmbientMap(cubemap, R3D_AMBIENT_ILLUMINATION);
  R3D_ENVIRONMENT_SET('ambient.map', ambientMap);
  R3D_ENVIRONMENT_SET('ambient.energy', 0.25);

  // Setup tonemapping
  R3D_ENVIRONMENT_SET('tonemap.mode', R3D_TONEMAP_FILMIC);
  R3D_ENVIRONMENT_SET('tonemap.exposure', 0.75);

  // Generate a ground plane and load the animated model
  plane := R3D_GenMeshPlane(10, 10, 1, 1);
  model := R3D_LoadModel(RESOURCES_PATH + 'models/CesiumMan.glb');

  // Load animations
  modelAnims := R3D_LoadAnimationLib(RESOURCES_PATH + 'models/CesiumMan.glb');
  modelPlayer := R3D_LoadAnimationPlayer(model.skeleton, modelAnims);

  // Setup animation playing
  R3D_SetAnimationLoop(@modelPlayer, 0, True);
  R3D_PlayAnimation(@modelPlayer, 0);

  // Create model instances
  instances := R3D_LoadInstanceBuffer(4, R3D_INSTANCE_POSITION);

  // Map instances to fill positions
  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION, False);

  // Fill positions in a 2x2 grid
  for z := 0 to 1 do
    for x := 0 to 1 do
    begin
      i := z * 2 + x;
      positions[i] := Vector3Create(
        x - 0.5,
        0.0,
        z - 0.5
      );
    end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Setup lights with shadows
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1.0, -1.0, -1.0));
  R3D_EnableLight(light);
  R3D_SetLightRange(light, 10.0);
  R3D_EnableShadow(light);

  // Setup camera
  camera.position := Vector3Create(0, 1.5, 3.0);
  camera.target := Vector3Create(0, 0.75, 0.0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    UpdateCamera(@camera, CAMERA_ORBITAL);
    R3D_UpdateAnimationPlayer(@modelPlayer, delta);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        // Draw ground plane
        R3D_DrawMesh(plane, R3D_MATERIAL_BASE, Vector3Zero(), 1.0);

        // Draw single animated model
        R3D_DrawAnimatedModel(model, modelPlayer, Vector3Zero(), 1.25);

        // Draw instanced animated models
        R3D_DrawAnimatedModelInstanced(model, modelPlayer, instances, 4);
      R3D_End();

      DrawFPS(10, 10);
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAnimationPlayer(modelPlayer);
  R3D_UnloadAnimationLib(modelAnims);
  R3D_UnloadModel(model, True);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

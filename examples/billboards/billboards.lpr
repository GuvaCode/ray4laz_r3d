program BillboardsExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  ScreenWidth, ScreenHeight: Integer;
  meshGround, meshBillboard: TR3D_Mesh;
  matGround, matBillboard: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  positionsPtr, scalesPtr: PVector3;
  light: TR3D_Light;
  camera: TCamera3D;
  i: Integer;
  scaleFactor: Single;
  bgColor, ambColor: TColor;
begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Billboards example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);
  R3D_SetTextureFilter(TEXTURE_FILTER_POINT);

  // Set background/ambient color
  bgColor := ColorCreate(102, 191, 255, 255);
  ambColor := ColorCreate(10, 19, 25, 255);
  //R3D_ENVIRONMENT_SET(background.color, bgColor);
  R3D_GetEnvironment^.background.color := bgColor;

  //R3D_ENVIRONMENT_SET(ambient.color, ambColor);
  R3D_GetEnvironment^.ambient.color := ambColor;
  //R3D_ENVIRONMENT_SET(tonemap.mode, R3D_TONEMAP_FILMIC);
  R3D_GetEnvironment^.tonemap.mode := R3D_TONEMAP_FILMIC;

  // Create ground mesh and material
  meshGround := R3D_GenMeshPlane(200, 200, 1, 1);
  matGround := R3D_GetDefaultMaterial();
  matGround.albedo.color := GREEN;

  // Create billboard mesh and material
  meshBillboard := R3D_GenMeshQuad(1.0, 1.0, 1, 1, Vector3Create(0.0, 0.0, 1.0));
  meshBillboard.shadowCastMode := R3D_SHADOW_CAST_ON_DOUBLE_SIDED;

  matBillboard := R3D_GetDefaultMaterial();
  matBillboard.albedo := R3D_LoadAlbedoMap(PAnsiChar(RESOURCES_PATH + 'tree.png'), WHITE);
  matBillboard.billboardMode := R3D_BILLBOARD_Y_AXIS;

  // Create transforms for instanced billboards
  instances := R3D_LoadInstanceBuffer(64, R3D_INSTANCE_POSITION or R3D_INSTANCE_SCALE);

  // Получаем указатели на данные инстансов
  positionsPtr := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_POSITION));
  scalesPtr := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_SCALE));

  Randomize;
  for i := 0 to 63 do
  begin
    scaleFactor := GetRandomValue(25, 50) / 10.0;
    scalesPtr[i] := Vector3Create(scaleFactor, scaleFactor, 1.0);
    positionsPtr[i] := Vector3Create(
      GetRandomValue(-100, 100),
      scaleFactor * 0.5,
      GetRandomValue(-100, 100)
    );
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION or R3D_INSTANCE_SCALE);

  // Setup directional light with shadows
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1, -1, -1));
  R3D_SetShadowDepthBias(light, 0.01);
  R3D_EnableShadow(light, 4096);
  R3D_SetLightActive(light, True);
  R3D_SetLightRange(light, 32.0);

  // Setup camera
  camera.position := Vector3Create(0, 5, 0);
  camera.target := Vector3Create(0, 5, -1);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawMesh(meshGround, matGround, Vector3Zero(), 1.0);
        R3D_DrawMeshInstanced(meshBillboard, matBillboard, instances, 64);
      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMaterial(matBillboard);
  R3D_UnloadMesh(meshBillboard);
  R3D_UnloadMesh(meshGround);
  R3D_Close();

  CloseWindow();
end.

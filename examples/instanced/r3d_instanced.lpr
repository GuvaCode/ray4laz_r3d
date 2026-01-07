program InstancedRenderingExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  INSTANCE_COUNT = 1000;

var
  ScreenWidth, ScreenHeight: Integer;
  mesh: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  rotations: PQuaternion;
  scales: PVector3;
  colors: PColor;
  light: TR3D_Light;
  camera: TCamera3D;
  i: Integer;
  ambientColor: TColor;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Instanced rendering example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Set ambient light
  ambientColor := DARKGRAY;
 // R3D_ENVIRONMENT_SET(ambient.color, ambientColor);
  R3D_GetEnvironment^.ambient.color := ambientColor;
  // Create cube mesh and default material
  mesh := R3D_GenMeshCube(1, 1, 1);
  material := R3D_GetDefaultMaterial();

  // Generate random transforms and colors for instances
  instances := R3D_LoadInstanceBuffer(INSTANCE_COUNT,
    R3D_INSTANCE_POSITION or R3D_INSTANCE_ROTATION or
    R3D_INSTANCE_SCALE or R3D_INSTANCE_COLOR);

  positions := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_POSITION));
  rotations := PQuaternion(R3D_MapInstances(instances, R3D_INSTANCE_ROTATION));
  scales := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_SCALE));
  colors := PColor(R3D_MapInstances(instances, R3D_INSTANCE_COLOR));

  Randomize;
  for i := 0 to INSTANCE_COUNT - 1 do
  begin
    positions[i] := Vector3Create(
      GetRandomValue(-50000, 50000) / 1000.0,
      GetRandomValue(-50000, 50000) / 1000.0,
      GetRandomValue(-50000, 50000) / 1000.0
    );

    rotations[i] := QuaternionFromEuler(
      GetRandomValue(-314000, 314000) / 100000.0,
      GetRandomValue(-314000, 314000) / 100000.0,
      GetRandomValue(-314000, 314000) / 100000.0
    );

    scales[i] := Vector3Create(
      GetRandomValue(100, 2000) / 1000.0,
      GetRandomValue(100, 2000) / 1000.0,
      GetRandomValue(100, 2000) / 1000.0
    );

    colors[i] := ColorFromHSV(
      GetRandomValue(0, 360000) / 1000.0,
      1.0,
      1.0
    );
  end;

  R3D_UnmapInstances(instances,
    R3D_INSTANCE_POSITION or R3D_INSTANCE_ROTATION or
    R3D_INSTANCE_SCALE or R3D_INSTANCE_COLOR);

  // Setup directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, -1, 0));
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0, 2, 2);
  camera.target := Vector3Create(0, 0, 0);
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
        R3D_DrawMeshInstanced(mesh, material, instances, INSTANCE_COUNT);
      R3D_End();

      DrawFPS(10, 10);
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMaterial(material);
  R3D_UnloadMesh(mesh);
  R3D_Close();

  CloseWindow();
end.

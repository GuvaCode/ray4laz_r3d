program DirectionalLightExample;

uses
  SysUtils, raylib, r3d, raymath;

const
  X_INSTANCES = 50;
  Y_INSTANCES = 50;
  INSTANCE_COUNT = X_INSTANCES * Y_INSTANCES;

var
  ScreenWidth, ScreenHeight: Integer;
  plane, sphere: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  spacing, offsetX, offsetZ: Single;
  x, y, idx: Integer;
  camera: TCamera3D;
  ambientColor: TColor;
  light: TR3D_Light;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Directional light example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create meshes and material
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  sphere := R3D_GenMeshSphere(0.35, 24, 16);
  material := R3D_GetDefaultMaterial();

  // Create transforms for instanced spheres
  instances := R3D_LoadInstanceBuffer(INSTANCE_COUNT, R3D_INSTANCE_POSITION);
  positions := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_POSITION));

  spacing := 1.5;
  offsetX := (X_INSTANCES * spacing) / 2.0;
  offsetZ := (Y_INSTANCES * spacing) / 2.0;

  idx := 0;
  for x := 0 to X_INSTANCES - 1 do
  begin
    for y := 0 to Y_INSTANCES - 1 do
    begin
      positions[idx] := Vector3Create(
        x * spacing - offsetX,
        0,
        y * spacing - offsetZ
      );
      Inc(idx);
    end;
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Setup environment
  ambientColor := ColorCreate(10, 10, 10, 255);
  //R3D_ENVIRONMENT_SET(ambient.color, ambientColor);
  R3D_GetEnvironment^.ambient.color := ambientColor;
  // Create directional light with shadows
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, -1, -1));
  R3D_SetLightActive(light, True);
  R3D_SetLightRange(light, 16.0);
  R3D_EnableShadow(light);
  R3D_SetShadowDepthBias(light, 0.01);
  R3D_SetShadowSoftness(light, 2.0);

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
        R3D_DrawMesh(plane, material, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawMeshInstanced(sphere, material, instances, INSTANCE_COUNT);
      R3D_End();

      DrawFPS(10, 10);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMaterial(material);
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

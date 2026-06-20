program InstancedRenderingExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  INSTANCE_COUNT = 1000;

type
  TPackedRotation = record
    x, y, z, w: Int16;
  end;

  TPackedScale = record
    x, y, z: UInt16;
  end;

var
  ScreenWidth, ScreenHeight: Integer;
  mesh: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  layout: TR3D_InstanceLayout;
  positions: PVector3;
  rotations: ^TPackedRotation;
  scales: ^TPackedScale;
  colors: PColor;
  light: TR3D_Light;
  camera: TCamera3D;
  i: Integer;
  rotation: TQuaternion;
  scale: TVector3;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Instanced rendering example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Set ambient light
  R3D_GetEnvironment^.ambient.color := DARKGRAY;

  // Create cube mesh and default material
  mesh := R3D_GenMeshCube(1, 1, 1);
  material := R3D_GetDefaultMaterial();

  // Setup instance layout with custom formats
  layout.formats[0] := R3D_INSTANCE_FORMAT_FLOAT32;    // position
  layout.formats[1] := R3D_INSTANCE_FORMAT_SNORM16;    // rotation quaternion
  layout.formats[2] := R3D_INSTANCE_FORMAT_FLOAT16;    // scale
  layout.formats[3] := R3D_INSTANCE_FORMAT_UNORM8;     // color
  layout.formats[4] := R3D_INSTANCE_FORMAT_FLOAT32;    // custom (unused)
  layout.flags := R3D_INSTANCE_POSITION or
                  R3D_INSTANCE_ROTATION or
                  R3D_INSTANCE_SCALE or
                  R3D_INSTANCE_COLOR;

  instances := R3D_LoadInstanceBufferEx(INSTANCE_COUNT, layout);

  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION, false);
  rotations := R3D_MapInstances(instances, R3D_INSTANCE_ROTATION, false);
  scales := R3D_MapInstances(instances, R3D_INSTANCE_SCALE, false);
  colors := R3D_MapInstances(instances, R3D_INSTANCE_COLOR, false);

  Randomize;
  for i := 0 to INSTANCE_COUNT - 1 do
  begin
    positions[i] := Vector3Create(
      GetRandomValue(-50000, 50000) / 1000.0,
      GetRandomValue(-50000, 50000) / 1000.0,
      GetRandomValue(-50000, 50000) / 1000.0
    );

    rotation := QuaternionFromEuler(
      GetRandomValue(-314000, 314000) / 100000.0,
      GetRandomValue(-314000, 314000) / 100000.0,
      GetRandomValue(-314000, 314000) / 100000.0
    );

    rotations[i].x := R3D_PackSnorm16(rotation.x);
    rotations[i].y := R3D_PackSnorm16(rotation.y);
    rotations[i].z := R3D_PackSnorm16(rotation.z);
    rotations[i].w := R3D_PackSnorm16(rotation.w);

    scale := Vector3Create(
      GetRandomValue(100, 2000) / 1000.0,
      GetRandomValue(100, 2000) / 1000.0,
      GetRandomValue(100, 2000) / 1000.0
    );

    scales[i].x := R3D_PackFloat16(scale.x);
    scales[i].y := R3D_PackFloat16(scale.y);
    scales[i].z := R3D_PackFloat16(scale.z);

    colors[i] := ColorFromHSV(
      GetRandomValue(0, 360000) / 1000.0,
      1.0,
      1.0
    );
  end;

  R3D_UnmapInstances(instances,
    R3D_INSTANCE_POSITION or
    R3D_INSTANCE_ROTATION or
    R3D_INSTANCE_SCALE or
    R3D_INSTANCE_COLOR);

  // Setup directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, -1, 0));
  R3D_EnableLight(light);

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
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMaterial(material);
  R3D_UnloadMesh(mesh);
  R3D_Close();

  CloseWindow();
end.

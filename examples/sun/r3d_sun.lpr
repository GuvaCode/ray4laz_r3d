program SunExample;

uses
  Math, SysUtils,
  raylib,
  r3d,
  raymath;

const
  X_INSTANCES = 50;
  Y_INSTANCES = 50;
  INSTANCE_COUNT = X_INSTANCES * Y_INSTANCES;

var
  plane, sphere: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  spacing, offsetX, offsetZ: Single;
  x, y, idx: Integer;
  skybox: TR3D_Cubemap;
  ambientMap: TR3D_AmbientMap;
  light: TR3D_Light;
  camera: TCamera3D;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Sun example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());
  R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA);

  // Create meshes and material
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  sphere := R3D_GenMeshSphere(0.35, 16, 32);
  material := R3D_GetDefaultMaterial();

  // Create transforms for instanced spheres
  instances := R3D_LoadInstanceBuffer(INSTANCE_COUNT, R3D_INSTANCE_POSITION);
  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION);

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
  skybox := R3D_GenCubemapSky(1024, R3D_CubemapSkyBase);
  R3D_GetEnvironment()^.background.sky := skybox;

  ambientMap := R3D_GenAmbientMap(skybox, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  R3D_GetEnvironment()^.ambient.map := ambientMap;

  // Create directional light with shadows
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(-1, -1, -1));
  R3D_SetLightActive(light, True);
  R3D_SetLightRange(light, 16.0);
  R3D_SetShadowSoftness(light, 2.0);
  R3D_SetShadowDepthBias(light, 0.01);
  R3D_EnableShadow(light);

  // Setup camera
  camera.position := Vector3Create(0, 1, 0);
  camera.target := Vector3Create(1, 1.25, 1);
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

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMaterial(material);
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_UnloadAmbientMap(ambientMap);
  R3D_UnloadCubemap(skybox);
  R3D_Close();

  CloseWindow();
end.

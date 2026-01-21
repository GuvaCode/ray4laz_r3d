program TransparencyExample;

{$mode objfpc}{$H+}

uses
  raylib,
  r3d,
  raymath;

var
  mesh: TR3D_Mesh;
  cube, plane, sphere: TR3D_Model;
  camera: TCamera3D;
  light: TR3D_Light;
begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Transparency example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create cube model
  mesh := R3D_GenMeshCube(1, 1, 1);
  cube := R3D_LoadModelFromMesh(mesh);
  cube.materials[0].transparencyMode := R3D_TRANSPARENCY_ALPHA;
  cube.materials[0].albedo.color := ColorCreate(150, 150, 255, 100);
  cube.materials[0].orm.occlusion := 1.0;
  cube.materials[0].orm.roughness := 0.2;
  cube.materials[0].orm.metalness := 0.2;

  // Create plane model
  mesh := R3D_GenMeshPlane(1000, 1000, 1, 1);
  plane := R3D_LoadModelFromMesh(mesh);
  plane.materials[0].orm.occlusion := 1.0;
  plane.materials[0].orm.roughness := 1.0;
  plane.materials[0].orm.metalness := 0.0;

  // Create sphere model
  mesh := R3D_GenMeshSphere(0.5, 64, 64);
  sphere := R3D_LoadModelFromMesh(mesh);
  sphere.materials[0].orm.occlusion := 1.0;
  sphere.materials[0].orm.roughness := 0.25;
  sphere.materials[0].orm.metalness := 0.75;

  // Setup camera
  camera.position := Vector3Create(0, 2, 2);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Setup lighting
  R3D_GetEnvironment()^.ambient.color := ColorCreate(10, 10, 10, 255);
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_SetLightActive(light, true);
  R3D_EnableShadow(light);

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawModel(plane, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawModel(sphere, Vector3Zero(), 1.0);
        R3D_DrawModel(cube, Vector3Zero(), 1.0);
      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadModel(sphere, false);
  R3D_UnloadModel(plane, false);
  R3D_UnloadModel(cube, false);
  R3D_Close();

  CloseWindow();
end.

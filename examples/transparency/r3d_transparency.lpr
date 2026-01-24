program TransparencyExample;

{$mode objfpc}{$H+}

uses
  raylib,
  r3d,
  raymath;

var
  cubeMesh, planeMesh, sphereMesh: TR3D_Mesh;
  matCube, matPlane, matSphere: TR3D_Material;
  camera: TCamera3D;
  light: TR3D_Light;

begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Transparency example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create cube mesh and material
  cubeMesh := R3D_GenMeshCube(1.0, 1.0, 1.0);
  matCube :=  R3D_MATERIAL_BASE;//R3D_GetBaseMaterial;
  matCube.transparencyMode := R3D_TRANSPARENCY_ALPHA;
  matCube.albedo.color := ColorCreate(150, 150, 255, 100);
  matCube.orm.occlusion := 1.0;
  matCube.orm.roughness := 0.2;
  matCube.orm.metalness := 0.2;

  // Create plane mesh and material
  planeMesh := R3D_GenMeshPlane(100.0, 100.0, 1, 1);
  matPlane := R3D_MATERIAL_BASE;// R3D_GetBaseMaterial;
  matPlane.orm.occlusion := 1.0;
  matPlane.orm.roughness := 1.0;
  matPlane.orm.metalness := 0.0;

  // Create sphere mesh and material
  sphereMesh := R3D_GenMeshSphere(0.5, 64, 64);
  matSphere := R3D_MATERIAL_BASE;
  matSphere.orm.occlusion := 1.0;
  matSphere.orm.roughness := 0.25;
  matSphere.orm.metalness := 0.75;

   // Setup environment
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(10, 10, 10, 255));

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_EnableShadow(light);
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0, 2, 2);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;


  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);
        R3D_DrawMesh(planeMesh, matPlane, Vector3Create(0.0, -0.5, 0.0), 1.0);
        R3D_DrawMesh(sphereMesh, matSphere, Vector3Zero(), 1.0);
        R3D_DrawMesh(cubeMesh, matCube, Vector3Zero(), 1.0);
      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(sphereMesh);
  R3D_UnloadMesh(planeMesh);
  R3D_UnloadMesh(cubeMesh);
  R3D_Close();

  CloseWindow();
end.

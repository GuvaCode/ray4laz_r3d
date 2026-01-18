program ProbeExample;

{$mode objfpc}{$H+}

uses
  Classes, SysUtils, Math,
  raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 450;

var
  cubemap: TR3D_Cubemap;
  ambientMap: TR3D_AmbientMap;
  plane, sphere: TR3D_Mesh;
  material: TR3D_Material;
  light: TR3D_Light;
  probe: TR3D_Probe;
  camera: TCamera3D;
  i: Integer;

begin
  // Initialize window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Probe example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Setup environment sky
  cubemap := R3D_LoadCubemap(RESOURCES_PATH + 'panorama/indoor.hdr', R3D_CUBEMAP_LAYOUT_AUTO_DETECT);

  // Use R3D_GetEnvironment()^ instead of R3D_ENVIRONMENT_SET
  R3D_GetEnvironment()^.background.skyBlur := 0.3;
  R3D_GetEnvironment()^.background.energy := 0.6;
  R3D_GetEnvironment()^.background.sky := cubemap;

  // Setup environment ambient
  ambientMap := R3D_GenAmbientMap(cubemap, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  R3D_GetEnvironment()^.ambient.map := ambientMap;
  R3D_GetEnvironment()^.ambient.energy := 0.25;

  // Setup tonemapping
  R3D_GetEnvironment()^.tonemap.mode := R3D_TONEMAP_FILMIC;

  // Create meshes
  plane := R3D_GenMeshPlane(30, 30, 1, 1);
  sphere := R3D_GenMeshSphere(0.5, 64, 64);
  material := R3D_GetDefaultMaterial();

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0.0, 10.0, 5.0), Vector3Create(0.0, 0.0, 0.0));
  R3D_SetLightActive(light, True);
  R3D_EnableShadow(light);

  // Create probe
  probe := R3D_CreateProbe(R3D_PROBE_ILLUMINATION or R3D_PROBE_REFLECTION);
  R3D_SetProbePosition(probe, Vector3Create(0.0, 1.0, 0.0));
  R3D_SetProbeShadows(probe, True);
  R3D_SetProbeFalloff(probe, 0.5);
  R3D_SetProbeActive(probe, True);

  // Setup camera
  camera.position := Vector3Create(0.0, 3.0, 6.0);
  camera.target := Vector3Create(0.0, 0.5, 0.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);

        material.orm.roughness := 0.5;
        material.orm.metalness := 0.0;
        R3D_DrawMesh(plane, material, Vector3Zero(), 1.0);

        for i := -1 to 1 do
        begin
          material.orm.roughness := Abs(i) * 0.4;
          material.orm.metalness := 1.0 - Abs(i);
          R3D_DrawMesh(sphere, material, Vector3Create(i * 3.0, 1.0, 0.0), 2.0);
        end;

      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAmbientMap(ambientMap);
  R3D_UnloadCubemap(cubemap);
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

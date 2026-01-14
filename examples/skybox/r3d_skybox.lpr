program SkyboxExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  ScreenWidth, ScreenHeight: Integer;
  sphere: TR3D_Mesh;
  skyProcedural, skyPanorama: TR3D_Cubemap;
  ambientProcedural, ambientPanorama: TR3D_AmbientMap;
  camera: TCamera3D;
  x, y, i: Integer;
 // material: TR3D_Material;
    Materials: array[0..48] of TR3D_Material; // 7x7 grid
begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Skybox example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Create sphere mesh
  sphere := R3D_GenMeshSphere(0.5, 32, 64);

  // Load and generate skyboxes
  skyProcedural := R3D_GenCubemapSky(512, R3D_CubemapSkyBase);
  skyPanorama := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'panorama/sky.hdr'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);

  // Generate ambient maps
  ambientProcedural := R3D_GenAmbientMap(skyProcedural, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  ambientPanorama := R3D_GenAmbientMap(skyPanorama, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);

  // Set default sky/ambient maps
  R3D_GetEnvironment^.background.sky := skyPanorama;
  R3D_GetEnvironment^.ambient.map := ambientPanorama;

  // Setup camera
  camera.position := Vector3Create(0, 0, 10);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Create materials grid (7x7)
    for x := 0 to 6 do
    begin
      for y := 0 to 6 do
      begin
        i := y * 7 + x;
        Materials[i] := R3D_GetDefaultMaterial();
        Materials[i].orm.metalness := x / 7;
        Materials[i].orm.roughness := y / 7;
        Materials[i].albedo.color := ColorFromHSV((x / 7) * 360, 1, 1);
      end;
    end;

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
      begin
        if R3D_GetEnvironment^.background.sky.texture = skyPanorama.texture then
        begin
          R3D_GetEnvironment^.background.sky := skyProcedural;
          R3D_GetEnvironment^.ambient.map := ambientProcedural;
        end
        else
        begin
          R3D_GetEnvironment^.background.sky := skyPanorama;
          R3D_GetEnvironment^.ambient.map := ambientPanorama;
        end;
      end;

      // Draw sphere grid
      R3D_Begin(camera);

           //

            // Draw 7x7 grid of spheres
            for x := 0 to 6 do
            begin
              for y := 0 to 6 do
              begin
                R3D_DrawMesh(Sphere, Materials[y * 7 + x], Vector3Create(x - 3, y - 3, 0.0), 1);
              end;
            end;

            {
            material := R3D_GetBaseMaterial;// R3D_MATERIAL_BASE;
            material.orm.roughness := Remap(y, 0.0, 8.0, 0.0, 1.0);
            material.orm.metalness := Remap(x, 0.0, 8.0, 0.0, 1.0);
            R3D_DrawMesh(sphere, material,
              Vector3Create((x - 4) * 1.25, (y - 4) * 1.25, 0.0), 1.0);
            }

      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAmbientMap(ambientProcedural);
  R3D_UnloadAmbientMap(ambientPanorama);
  R3D_UnloadCubemap(skyProcedural);
  R3D_UnloadCubemap(skyPanorama);
  R3D_UnloadMesh(sphere);
  R3D_Close();

  CloseWindow();
end.

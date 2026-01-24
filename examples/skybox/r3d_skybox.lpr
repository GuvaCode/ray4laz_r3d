program SkyboxExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  sphere: TR3D_Mesh;
  skyProcedural, skyPanorama: TR3D_Cubemap;
  ambientProcedural, ambientPanorama: TR3D_AmbientMap;
  camera: TCamera3D;
  skyParams: TR3D_CubemapSky;
  x, y: Integer;
  material: TR3D_Material;
begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Skybox example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create sphere mesh
  sphere := R3D_GenMeshSphere(0.5, 32, 64);

  // Define procedural skybox parameters
  skyParams := R3D_CUBEMAP_SKY_BASE;
  skyParams.groundEnergy := 2.0;
  skyParams.skyEnergy := 2.0;
  skyParams.sunEnergy := 2.0;

  // Load and generate skyboxes
  skyProcedural := R3D_GenCubemapSky(512, skyParams);
  skyPanorama := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'panorama/sky.hdr'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);

  // Generate ambient maps
  ambientProcedural := R3D_GenAmbientMap(skyProcedural, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  ambientPanorama := R3D_GenAmbientMap(skyPanorama, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);

  // Set default sky/ambient maps
  R3D_ENVIRONMENT_SET('background.sky', skyPanorama);
  R3D_ENVIRONMENT_SET('ambient.map', ambientPanorama);

  // Set tonemapping
  R3D_ENVIRONMENT_SET('tonemap.mode', R3D_TONEMAP_AGX);

  // Setup camera
  camera.position := Vector3Create(0.0, 0.0, 10.0);
  camera.target := Vector3Create(0.0, 0.0, 0.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Capture mouse
  DisableCursor();

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_FREE);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      if IsMouseButtonPressed(MOUSE_LEFT_BUTTON) then
      begin
        if R3D_GetEnvironment^.background.sky.texture = skyPanorama.texture then
        begin
          R3D_ENVIRONMENT_SET('background.sky', skyProcedural);
          R3D_ENVIRONMENT_SET('ambient.map', ambientProcedural);
        end
        else
        begin
          R3D_ENVIRONMENT_SET('background.sky', skyPanorama);
          R3D_ENVIRONMENT_SET('ambient.map', ambientPanorama);
        end;
      end;

      // Draw sphere grid
      R3D_Begin(camera);
        for x := 0 to 8 do
        begin
          for y := 0 to 8 do
          begin
            material := R3D_MATERIAL_BASE;
            material.orm.roughness := Remap(y, 0.0, 8.0, 0.0, 1.0);
            material.orm.metalness := Remap(x, 0.0, 8.0, 0.0, 1.0);
            R3D_DrawMesh(sphere, material,
              Vector3Create((x - 4) * 1.25, (y - 4) * 1.25, 0.0), 1.0);
          end;
        end;
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

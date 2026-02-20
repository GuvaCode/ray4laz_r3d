program SkyboxExample;

{$mode objfpc}{$H+}

uses
  SysUtils, Math,
  raylib,
  r3d,
  raymath;

const
    RESOURCES_PATH = 'resources/';

var
  // Window and camera
  screenWidth, screenHeight: Integer;
  camera: TCamera3D;

  // R3D resources
  sphere: TR3D_Mesh;
  skyParams: TR3D_ProceduralSky;
  shader: PR3D_SkyShader;
  skyPanorama, skyProcedural, skyCustom: TR3D_Cubemap;
  ambientPanorama, ambientProcedural, ambientCustom: TR3D_AmbientMap;

  // Sky and ambient arrays
  backgrounds: array[0..2] of TR3D_EnvBackground;
  ambients: array[0..2] of TR3D_EnvAmbient;
  currentSky: Integer;

  // Loop variables
  i, x, y: Integer;
  material: TR3D_Material;
  colorVal: TVector3;
  cells: array[0..1] of Integer;
  lineWidth: array[0..0] of Single;
begin
  // Initialize window
  screenWidth := 800;
  screenHeight := 450;
  InitWindow(screenWidth, screenHeight, '[r3d] - Skybox example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create sphere mesh
  sphere := R3D_GenMeshSphere(0.5, 32, 64);

  // Define procedural skybox parameters
  skyParams := R3D_PROCEDURAL_SKY_BASE;
  skyParams.groundEnergy := 2.0;
  skyParams.skyEnergy := 2.0;
  skyParams.sunEnergy := 2.0;

  // Load a custom sky shader
  shader := Default(PR3D_SkyShader);
  shader := R3D_LoadSkyShader(PAnsiChar(RESOURCES_PATH + 'shaders/sky.glsl'));


  // Set shader uniforms
  colorVal := Vector3Create(0.0, 0.5, 0.0);
  R3D_SetSkyShaderUniform(shader, PAnsiChar('u_color'), @colorVal);
     //R3D_SetSkyShaderUniform(shader, 'u_color', @colorVal);
  cells[0] := 10;
  cells[1] := 10;
  R3D_SetSkyShaderUniform(shader, 'u_cells', @cells);

  lineWidth[0] := 1.0;
  R3D_SetSkyShaderUniform(shader, 'u_line_px', @lineWidth);

  // Load and generate skyboxes
  skyPanorama := R3D_LoadCubemap(PAnsiChar(RESOURCES_PATH + 'panorama/sky.hdr'), R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
  skyProcedural := R3D_GenProceduralSky(1024, skyParams);
  skyCustom := R3D_GenCustomSky(512, shader);

  // Generate ambient maps
  ambientPanorama := R3D_GenAmbientMap(skyPanorama, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  ambientProcedural := R3D_GenAmbientMap(skyProcedural, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);
  ambientCustom := R3D_GenAmbientMap(skyCustom, R3D_AMBIENT_ILLUMINATION or R3D_AMBIENT_REFLECTION);

  // Store skies/ambients
  currentSky := 0;

  for i := 0 to 2 do
  begin
    backgrounds[i].energy := 1.0;
    ambients[i].energy := 1.0;
  end;

  backgrounds[0].sky := skyPanorama;
  backgrounds[1].sky := skyProcedural;
  backgrounds[2].sky := skyCustom;

  ambients[0].map := ambientPanorama;
  ambients[1].map := ambientProcedural;
  ambients[2].map := ambientCustom;

  // Set default sky/ambient maps
  R3D_ENVIRONMENT_SET('background.sky', skyPanorama);
  R3D_ENVIRONMENT_SET('ambient.map', ambientPanorama);

  // Set tonemapping
  R3D_ENVIRONMENT_SET('tonemap.mode', R3D_TONEMAP_AGX);

  // Setup camera
  camera.position := Vector3Create(0, 0, 10);
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

      // Change sky with mouse buttons
      if IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) then
        currentSky := currentSky + 1;
      if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
        currentSky := currentSky - 1;

      currentSky := (currentSky + 3) mod 3;

      // Apply current sky and ambient
      R3D_ENVIRONMENT_SET('background', backgrounds[currentSky]);
      R3D_ENVIRONMENT_SET('ambient', ambients[currentSky]);

      // Draw sphere grid
      R3D_Begin(camera);
        for x := 0 to 8 do
        begin
          for y := 0 to 8 do
          begin
            material := R3D_MATERIAL_BASE;
            material.orm.roughness := Remap(y, 0, 8, 0.0, 1.0);
            material.orm.metalness := Remap(x, 0, 8, 0.0, 1.0);

            R3D_DrawMesh(sphere, material,
              Vector3Create(
                (x - 4) * 1.25,
                (y - 4) * 1.25,
                0.0
              ),
              1.0
            );
          end;
        end;
      R3D_End();

      // Draw UI
      DrawText('Left/Right click to change skybox', 10, 10, 20, WHITE);

      case currentSky of
        0: DrawText('Current: Panorama Sky', 10, 40, 20, GREEN);
        1: DrawText('Current: Procedural Sky', 10, 40, 20, GREEN);
        2: DrawText('Current: Custom Shader Sky', 10, 40, 20, GREEN);
      end;

      DrawFPS(10, 70);
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadAmbientMap(ambientProcedural);
  R3D_UnloadAmbientMap(ambientPanorama);
  R3D_UnloadAmbientMap(ambientCustom);

  R3D_UnloadCubemap(skyProcedural);
  R3D_UnloadCubemap(skyPanorama);
  R3D_UnloadCubemap(skyCustom);

  R3D_UnloadMesh(sphere);
  R3D_Close();

  CloseWindow();
end.

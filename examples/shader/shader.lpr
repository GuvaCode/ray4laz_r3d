program ShaderExample;

{$mode objfpc}{$H+}

uses
  SysUtils, Classes, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 450;

var
  plane, torus: TR3D_Mesh;
  material: TR3D_Material;
  texture: TTexture;
  image: TImage;
  shader: PR3D_ScreenShader;
  light: TR3D_Light;
  camera: TCamera3D;
  time: Single;

begin
  // Initialize window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Shader example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Setup environment
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(10, 10, 10, 255));
  R3D_ENVIRONMENT_SET('bloom.mode', R3D_BLOOM_ADDITIVE);

  // Create meshes
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  torus := R3D_GenMeshTorus(0.5, 0.1, 32, 16);

  // Create material
  material := R3D_GetDefaultMaterial();
  material.shader := R3D_LoadSurfaceShader(PAnsiChar(RESOURCES_PATH + 'shaders/material.glsl'));

  // Generate a texture for custom sampler
  image := GenImageChecked(512, 512, 16, 32, WHITE, BLACK);
  texture := LoadTextureFromImage(image);
  UnloadImage(image);

  // Set custom sampler
  R3D_SetSurfaceShaderSampler(material.shader, 'u_texture', texture);

  // Load a screen shader
  shader := R3D_LoadScreenShader(PAnsiChar(RESOURCES_PATH + 'shaders/screen.glsl'));
      R3D_SetScreenShaderChain(R3D_SCREEN_SHADER_STAGE_OUTPUT, @shader, 1);

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_SetLightTarget(light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_EnableShadow(light);
  R3D_EnableLight(light);

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

      time := 2.0 * GetTime();
      R3D_SetScreenShaderUniform(shader, 'u_time', @time);
      R3D_SetSurfaceShaderUniform(material.shader, 'u_time', @time);

      R3D_Begin(camera);
        R3D_DrawMesh(plane, R3D_MATERIAL_BASE, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawMesh(torus, material, Vector3Zero(), 1.0);
      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadSurfaceShader(material.shader);
  R3D_UnloadScreenShader(shader);
  R3D_UnloadMesh(torus);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

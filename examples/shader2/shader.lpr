program ShaderExample;

{$mode objfpc}{$H+}

uses
  SysUtils, Classes, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 450;

var
  portalMesh: TR3D_Mesh;
  portalMaterial: TR3D_Material;
  portalShader: PR3D_SurfaceShader;
  light: TR3D_Light;
  camera: TCamera3D;
  time: Single;
  resolution: array[0..2] of Single;
  shaderTime: Single;

begin
  // Initialize window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Portal Effect Example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Setup environment for better portal visibility with proper bloom settings
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(5, 5, 10, 255));

  // Настройка bloom для красивого свечения портала
  R3D_ENVIRONMENT_SET('bloom.mode', R3D_BLOOM_ADDITIVE);
  //R3D_ENVIRONMENT_SET('bloom.intensity', 1.5);
  //R3D_ENVIRONMENT_SET('bloom.threshold', 0.1);

  // Create meshes
  // Правильное создание цилиндра/диска: R3D_GenMeshCylinder(bottomRadius, topRadius, height, slices)
  portalMesh := R3D_GenMeshCylinder(2.0, 2.0, 0.1, 64); // Радиус 2.0 и сверху и снизу, высота 0.1

  // Загружаем шейдер портала
  portalShader := R3D_LoadSurfaceShader(PAnsiChar(RESOURCES_PATH + 'shaders/portal_surface3.glsl'));

  if portalShader = nil then
  begin
    TraceLog(LOG_ERROR, 'Failed to load portal shader!');
    CloseWindow();
    Exit;
  end;

  TraceLog(LOG_INFO, 'Portal shader loaded successfully');

  // Create material for portal
  portalMaterial := R3D_GetDefaultMaterial();
  portalMaterial.shader := portalShader;

  // Initialize resolution uniform
  resolution[0] := SCREEN_WIDTH;
  resolution[1] := SCREEN_HEIGHT;
  resolution[2] := 1.0;

  // Set initial uniform values
  R3D_SetSurfaceShaderUniform(portalShader, 'iResolution', @resolution);

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(5, 10, 5), Vector3Create(0, 1, 0));
  R3D_EnableShadow(light);
  R3D_SetLightActive(light, True);

  // Setup camera - статичная камера
  camera.position := Vector3Create(4, 3, 4);
  camera.target := Vector3Create(0, 1, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    // Камера не вращается
    // UpdateCamera(@camera, CAMERA_ORBITAL); - закомментировано

    BeginDrawing();
      ClearBackground(BLACK);

      time := GetTime();
      shaderTime := time;

      // Update shader uniforms
      R3D_SetSurfaceShaderUniform(portalShader, 'iTime', @shaderTime);

      R3D_Begin(camera);
        // Рисуем портал
        // Вариант 1: Вертикальный портал (как дверь/зеркало)
        R3D_DrawMesh(portalMesh, portalMaterial,
                     Vector3Create(0, 1.0, 0),  // Позиция
                     1.0                        // Масштаб
                   );   // Поворот: 90° по X для вертикальной ориентации

        // Вариант 2: Горизонтальный портал (на полу) - раскомментируйте если нужно
        // R3D_DrawMesh(portalMesh, portalMaterial, Vector3Create(0, 0, 0), 1.0);

        // Добавим маленькую сферу для ориентации в пространстве
        // R3D_DrawMesh(R3D_GenMeshSphere(0.2, 16, 16), R3D_GetDefaultMaterial(),
        //              Vector3Create(2, 0.5, 2), 1.0);
      R3D_End();

      // Draw UI
      DrawFPS(10, 10);
      DrawText('PORTAL EFFECT - VERTICAL', 10, 30, 20, RAYWHITE);
      DrawText(TextFormat('Time: %.2f', time), 10, 50, 20, LIGHTGRAY);
      DrawText('Camera: STATIC', 10, 70, 20, GREEN);

    EndDrawing();
  end;

  // Cleanup
  if portalShader <> nil then
    R3D_UnloadSurfaceShader(portalShader);

  R3D_UnloadMesh(portalMesh);
  R3D_Close();

  CloseWindow();
end.

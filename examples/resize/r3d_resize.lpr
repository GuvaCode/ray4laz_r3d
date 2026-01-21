program ResizeExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

var
  ScreenWidth, ScreenHeight: Integer;
  sphere: TR3D_Mesh;
  materials: array[0..4] of TR3D_Material;
  light: TR3D_Light;
  camera: TCamera3D;
  i: Integer;
  aspect: TR3D_AspectMode;
  upscale: TR3D_UpscaleMode;

function GetAspectModeName(mode: TR3D_AspectMode): string;
begin
  case mode of
    R3D_ASPECT_EXPAND: Result := 'EXPAND';
    R3D_ASPECT_KEEP: Result := 'KEEP';
  else
    Result := 'UNKNOWN';
  end;
end;

function GetUpscaleModeName(mode: TR3D_UpscaleMode): string;
begin
  case mode of
    R3D_UPSCALE_NEAREST: Result := 'NEAREST';
    R3D_UPSCALE_LINEAR: Result := 'LINEAR';
    R3D_UPSCALE_BICUBIC: Result := 'BICUBIC';
    R3D_UPSCALE_LANCZOS: Result := 'LANCZOS';
  else
    Result := 'UNKNOWN';
  end;
end;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Resize example');
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create sphere mesh and materials
  sphere := R3D_GenMeshSphere(0.5, 64, 64);
  for i := 0 to 4 do
  begin
    materials[i] := R3D_GetDefaultMaterial();
    materials[i].albedo.color := ColorFromHSV(i / 5.0 * 330, 1.0, 1.0);
  end;

  // Setup directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, 0, -1));
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0, 2, 2);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  // Current blit state
  aspect := R3D_ASPECT_EXPAND;
  upscale := R3D_UPSCALE_NEAREST;

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    // Toggle aspect keep
    if IsKeyPressed(KEY_R) then
    begin
      aspect := TR3D_AspectMode((Integer(aspect) + 1) mod 2);
      R3D_SetAspectMode(aspect);
    end;

    // Toggle upscale filtering
    if IsKeyPressed(KEY_F) then
    begin
      upscale := TR3D_UpscaleMode((Integer(upscale) + 1) mod 4);
      R3D_SetUpscaleMode(upscale);
    end;

    BeginDrawing();
      ClearBackground(BLACK);

      // Draw spheres
      R3D_Begin(camera);
        for i := 0 to 4 do
        begin
          R3D_DrawMesh(sphere, materials[i], Vector3Create(i - 2, 0, 0), 1.0);
        end;
      R3D_End();

      // Draw info
      DrawText(PAnsiChar('Resize mode: ' + GetAspectModeName(aspect)), 10, 10, 20, RAYWHITE);
      DrawText(PAnsiChar('Filter mode: ' + GetUpscaleModeName(upscale)), 10, 40, 20, RAYWHITE);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(sphere);
  R3D_Close();

  CloseWindow();
end.

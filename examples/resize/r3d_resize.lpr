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
  keep, linear: Boolean;
  keepText, filterText: string;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Resize example');
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

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

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    // Toggle aspect keep
    if IsKeyPressed(KEY_R) then
    begin
      if R3D_HasState(R3D_FLAG_ASPECT_KEEP) then
        R3D_ClearState(R3D_FLAG_ASPECT_KEEP)
      else
        R3D_SetState(R3D_FLAG_ASPECT_KEEP);
    end;

    // Toggle linear filtering
    if IsKeyPressed(KEY_F) then
    begin
      if R3D_HasState(R3D_FLAG_BLIT_LINEAR) then
        R3D_ClearState(R3D_FLAG_BLIT_LINEAR)
      else
        R3D_SetState(R3D_FLAG_BLIT_LINEAR);
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
      keep := R3D_HasState(R3D_FLAG_ASPECT_KEEP);
      linear := R3D_HasState(R3D_FLAG_BLIT_LINEAR);

      if keep then
        keepText := 'KEEP'
      else
        keepText := 'EXPAND';

      if linear then
        filterText := 'LINEAR'
      else
        filterText := 'NEAREST';

      DrawText(PAnsiChar('Resize mode: ' + keepText), 10, 10, 20, RAYWHITE);
      DrawText(PAnsiChar('Filter mode: ' + filterText), 10, 40, 20, RAYWHITE);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(sphere);
  R3D_Close();

  CloseWindow();
end.

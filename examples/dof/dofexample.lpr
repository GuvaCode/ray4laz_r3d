program DoFExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  X_INSTANCES = 10;
  Y_INSTANCES = 10;
  INSTANCE_COUNT = X_INSTANCES * Y_INSTANCES;

var
  ScreenWidth, ScreenHeight: Integer;
  light: TR3D_Light;
  meshSphere: TR3D_Mesh;
  matDefault: TR3D_Material;
  spacing, offsetX, offsetZ: Single;
  idx, x, y: Integer;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  colors: PColor;
  camDefault: TCamera3D;
  delta, focusPoint, focusScale, maxBlur, mouseWheel: Single;
  rotation: TMatrix;
  mousePos: TVector2;
  dofText, fpsText: string;
  bgColor, textColor: TColor;
  view: TVector3;
begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - DoF example');
  SetTargetFPS(60);

  // Initialize R3D with FXAA
  R3D_Init(GetScreenWidth(), GetScreenHeight());
  R3D_SetAntiAliasing(R3D_ANTI_ALIASING_FXAA);
  // Configure depth of field and background
  bgColor := ColorCreate(0, 0, 0, 255);
 // R3D_ENVIRONMENT_SET(background.color, bgColor);
  R3D_GetEnvironment^.background.color := bgColor;
 // R3D_ENVIRONMENT_SET(dof.mode, R3D_DOF_ENABLED);
  R3D_GetEnvironment^.dof.mode:=R3D_DOF_ENABLED;
 // R3D_ENVIRONMENT_SET(dof.focusPoint, 2.0);
  R3D_GetEnvironment^.dof.focusPoint:=2.0;
 // R3D_ENVIRONMENT_SET(dof.focusScale, 3.0);
  R3D_GetEnvironment^.dof.focusScale:=3.0;
//  R3D_ENVIRONMENT_SET(dof.maxBlurSize, 20.0);
  R3D_GetEnvironment^.dof.maxBlurSize:=20.0;
//  R3D_ENVIRONMENT_SET(dof.debugMode, False);
  R3D_GetEnvironment^.dof.debugMode:=False;
  // Create directional light
  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, -1, 0));
  R3D_SetLightActive(light, True);

  // Create sphere mesh and default material
  meshSphere := R3D_GenMeshSphere(0.2, 64, 64);
  matDefault := R3D_GetDefaultMaterial();

  // Generate instance matrices and colors
  spacing := 0.5;
  offsetX := (X_INSTANCES * spacing) / 2.0;
  offsetZ := (Y_INSTANCES * spacing) / 2.0;
  idx := 0;

  instances := R3D_LoadInstanceBuffer(INSTANCE_COUNT, R3D_INSTANCE_POSITION or R3D_INSTANCE_COLOR);
  positions := PVector3(R3D_MapInstances(instances, R3D_INSTANCE_POSITION));
  colors := PColor(R3D_MapInstances(instances, R3D_INSTANCE_COLOR));

  Randomize;
  for x := 0 to X_INSTANCES - 1 do
  begin
    for y := 0 to Y_INSTANCES - 1 do
    begin
      positions[idx] := Vector3Create(
        x * spacing - offsetX,
        0,
        y * spacing - offsetZ
      );
      colors[idx] := ColorCreate(
        Random(256),
        Random(256),
        Random(256),
        255
      );
      Inc(idx);
    end;
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION or R3D_INSTANCE_COLOR);

  // Setup camera
  camDefault.position := Vector3Create(0, 2, 2);
  camDefault.target := Vector3Create(0, 0, 0);
  camDefault.up := Vector3Create(0, 1, 0);
  camDefault.fovy := 60;
  camDefault.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    // Rotate camera
    rotation := MatrixRotate(camDefault.up, 0.1 * delta);
    view := Vector3Subtract(camDefault.position, camDefault.target);
    view := Vector3Transform(view, rotation);
    camDefault.position := Vector3Add(camDefault.target, view);

    // Adjust DoF based on mouse
    mousePos := GetMousePosition();
    focusPoint := 0.5 + (5.0 - (mousePos.y / ScreenHeight) * 5.0);
    focusScale := 0.5 + (5.0 - (mousePos.x / ScreenWidth) * 5.0);

    //R3D_ENVIRONMENT_SET(dof.focusPoint, focusPoint);
    R3D_GetEnvironment^.dof.focusPoint:=focusPoint;
    //R3D_ENVIRONMENT_SET(dof.focusScale, focusScale);
   R3D_GetEnvironment^.dof.focusScale:=focusScale;
    mouseWheel := GetMouseWheelMove();
    if mouseWheel <> 0.0 then
    begin
      //maxBlur := R3D_ENVIRONMENT_GET(dof.maxBlurSize);
      maxBlur := R3D_GetEnvironment^.dof.maxBlurSize;
      //R3D_ENVIRONMENT_SET(dof.maxBlurSize, maxBlur + mouseWheel * 0.1);
      R3D_GetEnvironment^.dof.maxBlurSize:= maxBlur + mouseWheel * 0.1;
    end;

    if IsKeyPressed(KEY_F1) then
    begin
     // R3D_ENVIRONMENT_SET(dof.debugMode, not R3D_ENVIRONMENT_GET(dof.debugMode));
      R3D_GetEnvironment^.dof.debugMode:= not R3D_GetEnvironment^.dof.debugMode;
    end;

    BeginDrawing();
      ClearBackground(BLACK);

      // Render scene
      R3D_Begin(camDefault);
        R3D_DrawMeshInstanced(meshSphere, matDefault, instances, INSTANCE_COUNT);
      R3D_End();

      // Display DoF values
      dofText := Format('Focus Point: %.2f'#10 +
                       'Focus Scale: %.2f'#10 +
                       'Max Blur Size: %.2f'#10 +
                       'Debug Mode: %d',
                       [R3D_GetEnvironment^.dof.focusPoint,
                        R3D_GetEnvironment^.dof.focusScale,
                        R3D_GetEnvironment^.dof.maxBlurSize,
                        Integer(R3D_GetEnvironment^.dof.debugMode)]);

      textColor := ColorCreate(255, 255, 255, 127);
      DrawText(PAnsiChar(dofText), 10, 30, 20, textColor);

      // Display instructions
      DrawText('F1: Toggle Debug Mode'#10 +
               'Scroll: Adjust Max Blur Size'#10 +
               'Mouse Left/Right: Shallow/Deep DoF'#10 +
               'Mouse Up/Down: Adjust Focus Point Depth',
               300, 10, 20, textColor);

      // Display FPS
      fpsText := Format('FPS: %d', [GetFPS()]);
      DrawText(PAnsiChar(fpsText), 10, 10, 20, textColor);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMesh(meshSphere);
  R3D_Close();

  CloseWindow();
end.

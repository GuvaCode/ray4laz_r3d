program DoFExample;

uses
  SysUtils, Math,
  raylib, r3d, raymath, rcamera;

const
  X_INSTANCES = 10;
  Y_INSTANCES = 10;
  INSTANCE_COUNT = X_INSTANCES * Y_INSTANCES;

var
  meshSphere: TR3D_Mesh;
  matDefault: TR3D_Material;
  camDefault: TCamera3D;
  instances: array[0..INSTANCE_COUNT-1] of TMatrix;
  instanceColors: array[0..INSTANCE_COUNT-1] of TColor;
  light: TR3D_Light;

function Init: PChar;
var
  x, y, idx: Integer;
  spacing, offsetX, offsetZ: Single;
begin
  R3D_Init(GetScreenWidth, GetScreenHeight, R3D_FLAG_FXAA);
  SetTargetFPS(60);

  { --- Enable and configure DOF --- }

  R3D_GetEnvironment^.background.color := BLACK;

  R3D_GetEnvironment^.dof.mode := R3D_DOF_ENABLED;
  R3D_GetEnvironment^.dof.focusPoint := 2.0;
  R3D_GetEnvironment^.dof.focusScale := 3.0;
  R3D_GetEnvironment^.dof.maxBlurSize := 20.0;
  R3D_GetEnvironment^.dof.debugMode := false;

  { --- Setup scene lighting --- }

  light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(light, Vector3Create(0, -1, 0));
  R3D_SetLightActive(light, true);

  { --- Load sphere mesh and material --- }

  meshSphere := R3D_GenMeshSphere(0.2, 64, 64);
  matDefault := R3D_GetDefaultMaterial();

  { --- Generate instances --- }

  spacing := 0.5;
  offsetX := (X_INSTANCES * spacing) / 2;
  offsetZ := (Y_INSTANCES * spacing) / 2;
  idx := 0;

  for x := 0 to X_INSTANCES - 1 do
  begin
    for y := 0 to Y_INSTANCES - 1 do
    begin
      instances[idx] := MatrixTranslate(x * spacing - offsetX, 0, y * spacing - offsetZ);
      instanceColors[idx] := ColorCreate(
        Random(256),
        Random(256),
        Random(256),
        255
      );
      Inc(idx);
    end;
  end;

  { --- Configure the camera and ready to go! --- }

  camDefault := Default(TCamera3D);
  camDefault.position := Vector3Create(0, 2, 2);
  camDefault.target := Vector3Create(0, 0, 0);
  camDefault.up := Vector3Create(0, 1, 0);
  camDefault.fovy := 60;
  camDefault.projection := ord(CAMERA_PERSPECTIVE);

  Randomize;
  Result := '[r3d] - DoF example';
end;

procedure Update(delta: Single);
var
  rotation: TMatrix;
  view: TVector3;
  mousePosition: TVector2;
  mouseWheel: Single;
  focusPoint, focusScale, maxBlurSize: Single;
  env: PR3D_Environment;
begin
  env := R3D_GetEnvironment;

  { --- Rotate camera --- }

  rotation := MatrixRotate(GetCameraUp(@camDefault), 0.1 * delta);
  view := Vector3Subtract(camDefault.position, camDefault.target);
  view := Vector3Transform(view, rotation);
  camDefault.position := Vector3Add(camDefault.target, view);

  { --- Adjust DoF based on mouse position --- }

  mousePosition := GetMousePosition;
  mouseWheel := GetMouseWheelMove;

  focusPoint := 0.5 + (5.0 - (mousePosition.y / GetScreenHeight) * 5.0);
  env^.dof.focusPoint := focusPoint;

  focusScale := 0.5 + (5.0 - (mousePosition.x / GetScreenWidth) * 5.0);
  env^.dof.focusScale := focusScale;

  if mouseWheel <> 0.0 then
  begin
    maxBlurSize := env^.dof.maxBlurSize;
    maxBlurSize := maxBlurSize + mouseWheel * 0.1;
    env^.dof.maxBlurSize := maxBlurSize;
  end;

  if IsKeyPressed(KEY_F1) then
  begin
    env^.dof.debugMode := not env^.dof.debugMode;
  end;
end;

procedure Draw;
var
  dofText, fpsText: string;
  env: PR3D_Environment;
begin
  env := R3D_GetEnvironment;

  { --- Ensure Clear Background --- }

  ClearBackground(BLACK);

  { --- Render R3D scene --- }

  R3D_Begin(camDefault);
    R3D_DrawMeshInstancedEx(@meshSphere, @matDefault,
      @instances[0], @instanceColors[0], INSTANCE_COUNT);
  R3D_End();

  { --- Draw DoF values --- }

  dofText := Format(
    'Focus Point: %.2f'#10 +
    'Focus Scale: %.2f'#10 +
    'Max Blur Size: %.2f'#10 +
    'Debug Mode: %d',
    [env^.dof.focusPoint, env^.dof.focusScale,
     env^.dof.maxBlurSize, Integer(env^.dof.debugMode)]
  );

  DrawText(PChar(dofText), 10, 30, 20, WHITE);

  { --- Print instructions --- }

  DrawText('F1: Toggle Debug Mode'#10 +
           'Scroll: Adjust Max Blur Size'#10 +
           'Mouse Left/Right: Shallow/Deep DoF'#10 +
           'Mouse Up/Down: Adjust Focus Point Depth',
           300, 10, 20, WHITE);

  { --- Draw FPS --- }

  fpsText := Format('FPS: %d', [GetFPS]);
  DrawText(PChar(fpsText), 10, 10, 20, WHITE);
end;

procedure Close;
begin
  R3D_UnloadMesh(@meshSphere);
  R3D_Close;
end;

{ Main program }
var
  screenWidth, screenHeight: Integer;
begin
  screenWidth := 800;
  screenHeight := 600;

  InitWindow(screenWidth, screenHeight, 'R3D Depth of Field Example');

  Init();

  while not WindowShouldClose() do
  begin
    Update(GetFrameTime());

    BeginDrawing();
      Draw();
    EndDrawing();
  end;

  Close();

  CloseWindow();
end.

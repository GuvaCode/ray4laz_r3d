program BillboardsExample;

uses
  SysUtils, Math,
  raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';

var
  camera: TCamera3D;

  meshGround: TR3D_Mesh;
  matGround: TR3D_Material;

  meshBillboard: TR3D_Mesh;
  matBillboard: TR3D_Material;
  transformsBillboards: array[0..63] of TMatrix;

  light: TR3D_Light;

function Init: PChar;
var
  i: Integer;
  scaleFactor: Single;
  scaleMat, translateMat: TMatrix;
begin
  { --- Initialize R3D with its internal resolution --- }

  R3D_Init(GetScreenWidth, GetScreenHeight, 0);
  SetTargetFPS(60);

  { --- Set the background color --- }

  R3D_GetEnvironment^.background.color := SKYBLUE;

  { --- Setup the ground mesh / material --- }

  meshGround := R3D_GenMeshPlane(200, 200, 1, 1);
  matGround := R3D_GetDefaultMaterial();
  matGround.albedo.color := GREEN;

  { --- Setup the billboard mesh / material --- }

  meshBillboard := R3D_GenMeshQuad(1.0, 1.0, 1, 1,
    Vector3Create(0.0, 0.0, 1.0));
  meshBillboard.shadowCastMode := R3D_SHADOW_CAST_ON_DOUBLE_SIDED;

  matBillboard := R3D_GetDefaultMaterial();
  matBillboard.billboardMode := R3D_BILLBOARD_Y_AXIS;
  matBillboard.albedo.texture := LoadTexture(PChar(RESOURCES_PATH + 'tree.png'));

  { --- Create multiple transforms for instanced billboards --- }

  for i := 0 to High(transformsBillboards) do
  begin
    scaleFactor := GetRandomValue(25, 50) / 10.0;
    scaleMat := MatrixScale(scaleFactor, scaleFactor, 1.0);
    translateMat := MatrixTranslate(
      GetRandomValue(-100, 100),
      scaleFactor * 0.5,
      GetRandomValue(-100, 100)
    );
    transformsBillboards[i] := MatrixMultiply(scaleMat, translateMat);
  end;

  { --- Setup the scene lighting --- }

  light := R3D_CreateLight(R3D_LIGHT_DIR);

  R3D_SetLightDirection(light, Vector3Create(-1, -1, -1));
  R3D_SetShadowDepthBias(light, 0.01);
  R3D_EnableShadow(light, 4096);
  R3D_SetLightActive(light, true);
  R3D_SetLightRange(light, 32.0);

  { --- Setup the camera --- }

  camera := Default(TCamera3D);
  camera.position := Vector3Create(0, 5, 0);
  camera.target := Vector3Create(0, 5, -1);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;

  { --- Capture the mouse and let's go! --- }

  DisableCursor();

  Result := '[r3d] - Billboards example';
end;

procedure Update(delta: Single);
begin
  UpdateCamera(@camera, CAMERA_FREE);
end;

procedure Draw;
begin
  R3D_Begin(camera);
    R3D_DrawMesh(@meshGround, @matGround, MatrixIdentity());
    R3D_DrawMeshInstanced(@meshBillboard, @matBillboard,
      @transformsBillboards[0], Length(transformsBillboards));
  R3D_End();
end;

procedure Close;
begin
  R3D_UnloadMaterial(@matBillboard);
  R3D_UnloadMesh(@meshBillboard);
  R3D_UnloadMesh(@meshGround);
  R3D_Close();
end;

{ Main program }
var
  screenWidth, screenHeight: Integer;
begin
  screenWidth := 800;
  screenHeight := 600;

  InitWindow(screenWidth, screenHeight, 'R3D Billboards Example');

  Randomize;
  Init();

  while not WindowShouldClose() do
  begin
    Update(GetFrameTime());

    BeginDrawing();
      ClearBackground(BLACK);
      Draw();
    EndDrawing();
  end;

  Close();

  CloseWindow();
end.

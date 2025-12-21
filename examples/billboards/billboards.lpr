program BillboardsExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 600;

var
  // === Resources ===
  Camera: TCamera3D;
  MeshGround: TR3D_Mesh;
  MatGround: TR3D_Material;
  MeshBillboard: TR3D_Mesh;
  MatBillboard: TR3D_Material;
  TransformsBillboards: array[0..63] of TMatrix;

procedure InitExample;
var
  Light: TR3D_Light;
  I: Integer;
  ScaleFactor: Single;
  ScaleMat, TranslateMat: TMatrix;
begin
  // --- Initialize R3D with its internal resolution ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SetTargetFPS(60);

  // --- Set the background color ---
  R3D_SetBackgroundColor(SKYBLUE);

  // --- Setup the ground mesh / material ---
  MeshGround := R3D_GenMeshPlane(200, 200, 1, 1);
  MatGround := R3D_GetDefaultMaterial();
  MatGround.albedo.color := GREEN;

  // --- Setup the billboard mesh / material ---
  MeshBillboard := R3D_GenMeshQuad(1.0, 1.0, 1, 1, Vector3Create(0.0, 0.0, 1.0));
  MeshBillboard.shadowCastMode := R3D_SHADOW_CAST_ON_DOUBLE_SIDED;

  MatBillboard := R3D_GetDefaultMaterial();
  MatBillboard.blendMode := R3D_BLEND_ALPHA;
  MatBillboard.billboardMode := R3D_BILLBOARD_Y_AXIS;
  MatBillboard.albedo.texture := LoadTexture(RESOURCES_PATH + 'tree.png');

  // --- Create multiple transforms for instanced billboards ---
  for I := 0 to High(TransformsBillboards) do
  begin
    ScaleFactor := GetRandomValue(25, 50) / 10.0;
    ScaleMat := MatrixScale(ScaleFactor, ScaleFactor, 1.0);
    TranslateMat := MatrixTranslate(
      GetRandomValue(-100, 100),
      ScaleFactor * 0.5,
      GetRandomValue(-100, 100)
    );
    TransformsBillboards[I] := MatrixMultiply(ScaleMat, TranslateMat);
  end;

  // --- Setup the scene lighting ---
  R3D_SetSceneBounds(BoundingBoxCreate(
    Vector3Create(-100, -10, -100),
    Vector3Create(100, 10, 100)
  ));

  Light := R3D_CreateLight(R3D_LIGHT_DIR);
  R3D_SetLightDirection(Light, Vector3Create(-1, -1, -1));
  R3D_EnableShadow(Light, 4096);
  R3D_SetLightActive(Light, True);

  // --- Setup the camera ---
  Camera.position := Vector3Create(0, 5, 0);
  Camera.target := Vector3Create(0, 5, -1);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;
  Camera.projection := CAMERA_PERSPECTIVE;

  // --- Capture the mouse and let's go! ---
  DisableCursor();
end;

procedure UpdateExample(Delta: Single);
begin
  UpdateCamera(@Camera, CAMERA_FREE);
end;

procedure DrawExample;
begin
  R3D_Begin(Camera);
    R3D_DrawMesh(@MeshGround, @MatGround, MatrixIdentity());
    R3D_DrawMeshInstanced(
      @MeshBillboard,
      @MatBillboard,
      @TransformsBillboards[0],
      Length(TransformsBillboards)
    );
  R3D_End();
end;

procedure CloseExample;
begin
  R3D_UnloadMaterial(@MatBillboard);
  R3D_UnloadMesh(@MeshBillboard);
  R3D_UnloadMesh(@MeshGround);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Billboards example');

  // Инициализация примера
  InitExample;

  // Главный цикл
  while not WindowShouldClose() do
  begin
    UpdateExample(GetFrameTime());

    BeginDrawing();
      ClearBackground(SKYBLUE);
      DrawExample;
    EndDrawing();
  end;

  // Очистка ресурсов
  CloseExample;
  CloseWindow();
end.

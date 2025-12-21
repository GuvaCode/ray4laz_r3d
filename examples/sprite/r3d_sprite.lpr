program SpriteExample;

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
  MeshSprite: TR3D_Mesh;
  MatSprite: TR3D_Material;

  // === Bird Data ===
  BirdDirX: Single = 1.0;
  BirdPos: TVector3 = (x: 0.0; y: 0.5; z: 0.0);

// === Sprite Helper ===

procedure GetTexCoordScaleOffset(out UvScale, UvOffset: TVector2;
  XFrameCount, YFrameCount: Integer; CurrentFrame: Single);
var
  FrameIndex, FrameX, FrameY: Integer;
begin
  UvScale.x := 1.0 / XFrameCount;
  UvScale.y := 1.0 / YFrameCount;

  FrameIndex := Round(CurrentFrame) mod (XFrameCount * YFrameCount);
  FrameX := FrameIndex mod XFrameCount;
  FrameY := FrameIndex div XFrameCount;

  UvOffset.x := FrameX * UvScale.x;
  UvOffset.y := FrameY * UvScale.y;
end;

// === Examples ===

procedure InitExample;
var
  Light: TR3D_Light;
begin
  // --- Initialize R3D with screen resolution ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SetTargetFPS(60);

  // --- Setup the ground mesh / material ---
  MeshGround := R3D_GenMeshPlane(200, 200, 1, 1);
  MatGround := R3D_GetDefaultMaterial();
  MatGround.albedo.color := GREEN;

  // --- Setup the sprite mesh / material ---
  MeshSprite := R3D_GenMeshQuad(1.0, 1.0, 1, 1, Vector3Create(0.0, 0.0, 1.0));
  MeshSprite.shadowCastMode := R3D_SHADOW_CAST_ON_DOUBLE_SIDED;

  MatSprite := R3D_GetDefaultMaterial();
  MatSprite.blendMode := R3D_BLEND_ALPHA;
  MatSprite.billboardMode := R3D_BILLBOARD_Y_AXIS;
  MatSprite.albedo.texture := LoadTexture(RESOURCES_PATH + 'spritesheet.png');

  // --- Setup a spotlight in the scene ---
  Light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(Light, Vector3Create(0, 10, 10), Vector3Create(0, 0, 0));
  R3D_SetLightRange(Light, 64.0);
  R3D_EnableShadow(Light, 1024);
  R3D_SetLightActive(Light, True);

  // --- Setup the camera ---
  Camera.position := Vector3Create(0, 2, 5);
  Camera.target := Vector3Create(0, 0, 0);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;
  Camera.projection := CAMERA_PERSPECTIVE;
end;

procedure UpdateExample(Delta: Single);
var
  BirdPosPrev: TVector3;
  CurrentFrame: Single;
  XFrameCount: Integer;
begin
  // --- Update bird ---
  BirdPosPrev := BirdPos;

  BirdPos.x := 2.0 * Sin(GetTime());
  BirdPos.y := 1.0 + Cos(GetTime() * 4.0) * 0.5;

  if BirdPos.x - BirdPosPrev.x >= 0.0 then
    BirdDirX := 1.0
  else
    BirdDirX := -1.0;

  // --- Update sprite ---
  CurrentFrame := 10.0 * GetTime();

  // Умножаем на знак направления X для инверсии uvScale.x
  if BirdDirX >= 0 then
    XFrameCount := 4
  else
    XFrameCount := -4;  // Отрицательное значение инвертирует спрайт по горизонтали

  GetTexCoordScaleOffset(
    MatSprite.uvScale,
    MatSprite.uvOffset,
    XFrameCount,
    1,
    CurrentFrame
  );
end;

procedure DrawExample;
begin
  R3D_Begin(Camera);
    R3D_DrawMesh(@MeshGround, @MatGround, MatrixTranslate(0, -0.5, 0));
    R3D_DrawMesh(@MeshSprite, @MatSprite, MatrixTranslate(BirdPos.x, BirdPos.y, 0.0));
  R3D_End();
end;

procedure CloseExample;
begin
  R3D_UnloadMaterial(@MatSprite);
  R3D_UnloadMesh(@MeshSprite);
  R3D_UnloadMesh(@MeshGround);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Sprite example');

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

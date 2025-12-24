program DecalExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib, r3d, raymath;

const
  MAXDECALS = 256;
  RESOURCES_PATH = 'resources/';
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 600;

var
  // === Resources ===
  MeshPlane: TR3D_Mesh;
  MaterialWalls: TR3D_Material;
  Texture: TTexture2D;
  Decal: TR3D_Decal;
  Camera: TCamera3D;
  Light: TR3D_Light;

  // === Data ===
  RoomSize: Single = 32.0;
  MatRoom: array[0..5] of TMatrix;

  DecalScale: TVector3 = (x: 3.0; y: 3.0; z: 3.0);

  DecalTransforms: array[0..MAXDECALS-1] of TMatrix;
  TargetDecalTransform: TMatrix;/// = IDENTITY_MATRIX;
  DecalCount: Integer = 0;
  DecalIndex: Integer = 0;
  TargetPosition: TVector3;

// === Helper Functions ===

function RayCubeIntersection(Ray: TRay; CubePosition, CubeSize: TVector3;
  out IntersectionPoint, Normal: TVector3): Boolean;
var
  HalfSize, MinPos, MaxPos: TVector3;
  tMin, tMax, tYMin, tYMax, tZMin, tZMax, Temp: Single;
begin
  Result := False;

  HalfSize.x := CubeSize.x / 2;
  HalfSize.y := CubeSize.y / 2;
  HalfSize.z := CubeSize.z / 2;

  MinPos.x := CubePosition.x - HalfSize.x;
  MinPos.y := CubePosition.y - HalfSize.y;
  MinPos.z := CubePosition.z - HalfSize.z;

  MaxPos.x := CubePosition.x + HalfSize.x;
  MaxPos.y := CubePosition.y + HalfSize.y;
  MaxPos.z := CubePosition.z + HalfSize.z;

  tMin := (MinPos.x - Ray.position.x) / Ray.direction.x;
  tMax := (MaxPos.x - Ray.position.x) / Ray.direction.x;

  if tMin > tMax then
  begin
    Temp := tMin;
    tMin := tMax;
    tMax := Temp;
  end;

  tYMin := (MinPos.y - Ray.position.y) / Ray.direction.y;
  tYMax := (MaxPos.y - Ray.position.y) / Ray.direction.y;

  if tYMin > tYMax then
  begin
    Temp := tYMin;
    tYMin := tYMax;
    tYMax := Temp;
  end;

  if (tMin > tYMax) or (tYMin > tMax) then Exit(False);

  if tYMin > tMin then tMin := tYMin;
  if tYMax < tMax then tMax := tYMax;

  tZMin := (MinPos.z - Ray.position.z) / Ray.direction.z;
  tZMax := (MaxPos.z - Ray.position.z) / Ray.direction.z;

  if tZMin > tZMax then
  begin
    Temp := tZMin;
    tZMin := tZMax;
    tZMax := Temp;
  end;

  if (tMin > tZMax) or (tZMin > tMax) then Exit(False);

  if tZMin > tMin then tMin := tZMin;
  if tZMax < tMax then tMax := tZMax;

  if tMin < 0 then
  begin
    tMin := tMax;
    if tMin < 0 then Exit(False);
  end;

  IntersectionPoint.x := Ray.position.x + Ray.direction.x * tMin;
  IntersectionPoint.y := Ray.position.y + Ray.direction.y * tMin;
  IntersectionPoint.z := Ray.position.z + Ray.direction.z * tMin;

  // Определение нормали пересечения
  Normal := Vector3Zero();

  if Abs(IntersectionPoint.x - MinPos.x) < 0.001 then
    Normal.x := -1.0  // Левая грань
  else if Abs(IntersectionPoint.x - MaxPos.x) < 0.001 then
    Normal.x := 1.0;  // Правая грань

  if Abs(IntersectionPoint.y - MinPos.y) < 0.001 then
    Normal.y := -1.0  // Нижняя грань
  else if Abs(IntersectionPoint.y - MaxPos.y) < 0.001 then
    Normal.y := 1.0;  // Верхняя грань

  if Abs(IntersectionPoint.z - MinPos.z) < 0.001 then
    Normal.z := -1.0  // Ближняя грань
  else if Abs(IntersectionPoint.z - MaxPos.z) < 0.001 then
    Normal.z := 1.0;  // Дальняя грань

  Result := True;
end;

procedure DrawTransformedCube(Transform: TMatrix; Color: TColor);
var
  Vertices: array[0..7] of TVector3 = (
    (x: -0.5; y: -0.5; z: -0.5),
    (x:  0.5; y: -0.5; z: -0.5),
    (x:  0.5; y:  0.5; z: -0.5),
    (x: -0.5; y:  0.5; z: -0.5),
    (x: -0.5; y: -0.5; z:  0.5),
    (x:  0.5; y: -0.5; z:  0.5),
    (x:  0.5; y:  0.5; z:  0.5),
    (x: -0.5; y:  0.5; z:  0.5)
  );
  TransformedVertices: array[0..7] of TVector3;
  I: Integer;
begin
  // Трансформация вершин
  for I := 0 to 7 do
  begin
    TransformedVertices[I] := Vector3Transform(Vertices[I], Transform);
  end;

  // Отрисовка ребер куба
  for I := 0 to 3 do
  begin
    // Нижняя грань
    DrawLine3D(TransformedVertices[I], TransformedVertices[(I + 1) mod 4], Color);
    // Верхняя грань
    DrawLine3D(TransformedVertices[I + 4], TransformedVertices[(I + 1) mod 4 + 4], Color);
    // Боковые ребра
    DrawLine3D(TransformedVertices[I], TransformedVertices[I + 4], Color);
  end;
end;

// === Example ===

procedure InitExample;
begin
  // --- Initialize R3D with its internal resolution ---
  R3D_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  SetTargetFPS(60);
    // Инициализация матрицы
  TargetDecalTransform := MatrixIdentity();
  // --- Load textures ---
  Texture := LoadTexture(RESOURCES_PATH + 'decal.png');

  // --- Create materials ---
  MaterialWalls := R3D_GetDefaultMaterial();
  MaterialWalls.albedo.color := DARKGRAY;

  Decal.material := R3D_GetDefaultMaterial();
  Decal.material.albedo.texture := Texture;


  // --- Create a plane along with the transformation matrices to place them to represent a room ---
  MeshPlane := R3D_GenMeshPlane(RoomSize, RoomSize, 1, 1);

  MatRoom[0] := MatrixMultiply(MatrixRotateZ(90.0 * DEG2RAD), MatrixTranslate(RoomSize / 2.0, 0.0, 0.0));
  MatRoom[1] := MatrixMultiply(MatrixRotateZ(-90.0 * DEG2RAD), MatrixTranslate(-RoomSize / 2.0, 0.0, 0.0));
  MatRoom[2] := MatrixMultiply(MatrixRotateX(90.0 * DEG2RAD), MatrixTranslate(0.0, 0.0, -RoomSize / 2.0));
  MatRoom[3] := MatrixMultiply(MatrixRotateX(-90.0 * DEG2RAD), MatrixTranslate(0.0, 0.0, RoomSize / 2.0));
  MatRoom[4] := MatrixMultiply(MatrixRotateX(180.0 * DEG2RAD), MatrixTranslate(0.0, RoomSize / 2.0, 0.0));
  MatRoom[5] := MatrixTranslate(0.0, -RoomSize / 2.0, 0.0);

  // --- Setup the scene lighting ---
  Light := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightEnergy(Light, 2.0);
  R3D_SetLightActive(Light, True);

  // --- Setup the camera ---
  Camera.position := Vector3Create(0.0, 0.0, 0.0);
  Camera.target := Vector3Create(RoomSize / 2.0, 0.0, 0.0);
  Camera.up := Vector3Create(0.0, 1.0, 0.0);
  Camera.fovy := 70;
  Camera.projection := CAMERA_PERSPECTIVE;

  //DisableCursor();
end;

procedure UpdateExample(Delta: Single);
var
  HitRay: TRay;
  HitPoint, HitNormal: TVector3;
  Translation, Scaling, Rotation: TMatrix;
begin
  UpdateCamera(@Camera, CAMERA_FREE);

  // --- Find intersection point of camera target on cube ---
  HitRay.position := Camera.position;
  HitRay.direction := Vector3Normalize(Vector3Subtract(Camera.target, Camera.position));

  if RayCubeIntersection(HitRay, Vector3Zero(),
    Vector3Create(RoomSize, RoomSize, RoomSize), HitPoint, HitNormal) then
  begin
    TargetPosition := HitPoint;
  end;

  // --- Create transformation matrix at intersection point ---
  Translation := MatrixTranslate(TargetPosition.x, TargetPosition.y, TargetPosition.z);
  Scaling := MatrixScale(DecalScale.x, DecalScale.y, DecalScale.z);

  // Определение вращения на основе нормали
  if Abs(HitNormal.x + 1.0) < 0.001 then
    Rotation := MatrixRotateXYZ(Vector3Create(-90.0 * DEG2RAD, 180.0 * DEG2RAD, 90.0 * DEG2RAD))
  else if Abs(HitNormal.x - 1.0) < 0.001 then
    Rotation := MatrixRotateXYZ(Vector3Create(-90.0 * DEG2RAD, 180.0 * DEG2RAD, -90.0 * DEG2RAD))
  else if Abs(HitNormal.y + 1.0) < 0.001 then
    Rotation := MatrixRotateY(180.0 * DEG2RAD)
  else if Abs(HitNormal.y - 1.0) < 0.001 then
    Rotation := MatrixRotateZ(180.0 * DEG2RAD)
  else if Abs(HitNormal.z + 1.0) < 0.001 then
    Rotation := MatrixRotateX(90.0 * DEG2RAD)
  else if Abs(HitNormal.z - 1.0) < 0.001 then
    Rotation := MatrixRotateXYZ(Vector3Create(-90.0 * DEG2RAD, 180.0 * DEG2RAD, 0))
  else
    Rotation := MatrixIdentity();

  TargetDecalTransform := MatrixMultiply(MatrixMultiply(Scaling, Rotation), Translation);

  // --- Input ---
  if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
  begin
    DecalTransforms[DecalIndex] := TargetDecalTransform;
    Inc(DecalIndex);
    if DecalIndex >= MAXDECALS then DecalIndex := 0;
    if DecalCount < MAXDECALS then Inc(DecalCount);
  end;
end;

procedure DrawExample;
var
  I: Integer;
begin
  R3D_Begin(Camera);

  // --- Draw the faces of our "room" ---
  for I := 0 to 5 do
  begin
    R3D_DrawMesh(@MeshPlane, @MaterialWalls, MatRoom[I]);
  end;

  // --- Draw applied decals ---
  if DecalCount > 0 then
  begin
    R3D_DrawDecalInstanced(@Decal, @DecalTransforms[0], DecalCount);
  end;

  // --- Draw targeting decal ---
  R3D_DrawDecal(@Decal, TargetDecalTransform);

  R3D_End();

  // --- Show decal projection box ---
  BeginMode3D(Camera);
    DrawTransformedCube(TargetDecalTransform, WHITE);
  EndMode3D();

  DrawText('LEFT CLICK TO APPLY DECAL', 10, 10, 20, LIME);
end;

procedure CloseExample;
begin
  R3D_UnloadMesh(@MeshPlane);
  UnloadTexture(Texture);
  R3D_UnloadMaterial(@MaterialWalls);
  R3D_UnloadMaterial(@Decal.material);
  R3D_Close();
end;

begin
  // Инициализация окна
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Decal example');

  // Инициализация примера
  InitExample;

  // Главный цикл
  while not WindowShouldClose() do
  begin
    UpdateExample(GetFrameTime());

    BeginDrawing();
      ClearBackground(BLACK);
      DrawExample;
    EndDrawing();
  end;

  // Очистка ресурсов
  CloseExample;
  CloseWindow();
end.

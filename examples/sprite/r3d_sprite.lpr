program SpriteExample;

{$mode objfpc}{$H+}

uses
  Math, SysUtils,
  raylib,
  r3d,
  raymath;

const
  RESOURCES_PATH = './';

procedure GetTexCoordScaleOffset(var uvScale, uvOffset: TVector2; xFrameCount, yFrameCount: Integer; currentFrame: Single);
var frameIndex,  frameX, frameY: Integer;
begin
  uvScale.x := 1.0 / xFrameCount;
  uvScale.y := 1.0 / yFrameCount;

  // Смещение целочисленного деления в положительную сторону
  if currentFrame < 0 then
    currentFrame := currentFrame - 0.5
  else
    currentFrame := currentFrame + 0.5;

  // Приведение к целому с округлением
  frameIndex := Trunc(currentFrame) mod (xFrameCount * yFrameCount);
  if frameIndex < 0 then
    frameIndex := frameIndex + (xFrameCount * yFrameCount);

  frameX := frameIndex mod xFrameCount;
  if frameX < 0 then
    frameX := frameX + xFrameCount;

   frameY := frameIndex div xFrameCount;
  if frameY < 0 then
    frameY := 0;

  uvOffset.x := frameX * uvScale.x;
  uvOffset.y := frameY * uvScale.y;
end;

var
  meshGround: TR3D_Mesh;
  matGround: TR3D_Material;
  meshSprite: TR3D_Mesh;
  matSprite: TR3D_Material;
  light: TR3D_Light;
  camera: TCamera3D;
  birdPos, birdPrev: TVector3;
  birdDirX: Single;
  currentFrame: Single;
begin
  // Initialize window
  InitWindow(800, 450, '[r3d] - Sprite example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);
  R3D_SetTextureFilter(TEXTURE_FILTER_POINT);

  // Set background/ambient color
 // R3D_ENVIRONMENT_SET(background.color, ColorCreate(102, 191, 255, 255));
 // R3D_ENVIRONMENT_SET(ambient.color, ColorCreate(10, 19, 25, 255));
 // R3D_ENVIRONMENT_SET(tonemap.mode, R3D_TONEMAP_FILMIC);

  // Create ground mesh and material
  meshGround := R3D_GenMeshPlane(200, 200, 1, 1);
  matGround := R3D_GetDefaultMaterial();
  matGround.albedo.color := GREEN;

  // Create sprite mesh and material
  meshSprite := R3D_GenMeshQuad(1.0, 1.0, 1, 1, Vector3Create(0, 0, 1));
  meshSprite.shadowCastMode := R3D_SHADOW_CAST_ON_DOUBLE_SIDED;

  matSprite := R3D_GetDefaultMaterial();
  matSprite.albedo := R3D_LoadAlbedoMap(PChar('resources/spritesheet.png'), WHITE);
  matSprite.billboardMode := R3D_BILLBOARD_Y_AXIS;

  // Setup spotlight
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0, 10, 10), Vector3Create(0, 0, 0));
  R3D_SetLightRange(light, 64.0);
  R3D_EnableShadow(light, 1024);
  R3D_SetLightActive(light, true);

  // Setup camera
  camera.position := Vector3Create(0, 2, 5);
  camera.target := Vector3Create(0, 0.5, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 45;
  camera.projection := CAMERA_PERSPECTIVE;

  // Bird data
  birdPos := Vector3Create(0, 0.5, 0);
  birdDirX := 1.0;

  // Main loop
  while not WindowShouldClose() do
  begin
    // Update bird position
    birdPrev := birdPos;
    birdPos.x := 2.0 * Sin(GetTime());
    birdPos.y := 1.0 + Cos(GetTime() * 4.0) * 0.5;

    if birdPos.x - birdPrev.x >= 0.0 then
      birdDirX := 1.0
    else
      birdDirX := -1.0;

    // Update sprite UVs
    // We multiply by the sign of the X direction to invert the uvScale.x
    currentFrame := 10.0 * GetTime();
    GetTexCoordScaleOffset(matSprite.uvScale, matSprite.uvOffset,
      Trunc(4 * birdDirX), 1, currentFrame);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw scene
      R3D_Begin(camera);
        R3D_DrawMesh(meshGround, matGround, Vector3Create(0, -0.5, 0), 1.0);
        R3D_DrawMesh(meshSprite, matSprite, Vector3Create(birdPos.x, birdPos.y, 0), 1.0);
      R3D_End();

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMaterial(matSprite);
  R3D_UnloadMesh(meshSprite);
  R3D_UnloadMesh(meshGround);
  R3D_Close();

  CloseWindow();
end.

program ManyLightsExample;

{$mode objfpc}{$H+}

uses
  Classes, SysUtils, Math,
  raylib, r3d, raymath;

const
  NUM_LIGHTS = 128;
  GRID_SIZE = 100;
  SCREEN_WIDTH = 800;
  SCREEN_HEIGHT = 450;


function RandF(minVal, maxVal: Single): Single;
begin
  Result := minVal + (maxVal - minVal) * Random;
end;

var
  plane, cube: TR3D_Mesh;
  material: TR3D_Material;
  instances: TR3D_InstanceBuffer;
  positions: PVector3;
  lights: array[0..NUM_LIGHTS-1] of TR3D_Light;
  camera: TCamera3D;
  x, z, i: Integer;

begin
  // Initialize random seed
  Randomize;

  // Initialize window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, '[r3d] - Many lights example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Set ambient light
  // Use R3D_GetEnvironment()^ instead of R3D_ENVIRONMENT_SET
  R3D_GetEnvironment()^.background.color := BLACK;
  R3D_GetEnvironment()^.ambient.color := ColorCreate(10, 10, 10, 255);

  // Create plane and cube meshes
  plane := R3D_GenMeshPlane(100, 100, 1, 1);
  cube := R3D_GenMeshCube(0.5, 0.5, 0.5);
  material := R3D_GetDefaultMaterial();

  // Allocate transforms for all spheres
  instances := R3D_LoadInstanceBuffer(GRID_SIZE * GRID_SIZE, R3D_INSTANCE_POSITION);
  positions := R3D_MapInstances(instances, R3D_INSTANCE_POSITION);

  z := -50;
  while z < 50 do
  begin
    x := -50;
    while x < 50 do
    begin
      positions[(z + 50) * GRID_SIZE + (x + 50)].x := x + 0.5;
      positions[(z + 50) * GRID_SIZE + (x + 50)].y := 0.0;
      positions[(z + 50) * GRID_SIZE + (x + 50)].z := z + 0.5;
      Inc(x);
    end;
    Inc(z);
  end;

  R3D_UnmapInstances(instances, R3D_INSTANCE_POSITION);

  // Create lights
  for i := 0 to NUM_LIGHTS - 1 do
  begin
    lights[i] := R3D_CreateLight(R3D_LIGHT_OMNI);
    R3D_SetLightPosition(lights[i], Vector3Create(RandF(-50.0, 50.0), RandF(1.0, 5.0), RandF(-50.0, 50.0)));
    R3D_SetLightColor(lights[i], ColorFromHSV(RandF(0.0, 360.0), 1.0, 1.0));
    R3D_SetLightRange(lights[i], RandF(8.0, 16.0));
    R3D_SetLightActive(lights[i], True);
  end;

  // Setup camera
  camera.position := Vector3Create(0.0, 10.0, 10.0);
  camera.target := Vector3Create(0.0, 0.0, 0.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 60.0;
  camera.projection := CAMERA_PERSPECTIVE;

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    BeginDrawing();
      ClearBackground(RAYWHITE);

      // Draw scene
      R3D_Begin(camera);
        R3D_DrawMesh(plane, material, Vector3Create(0.0, -0.25, 0.0), 1.0);
        R3D_DrawMeshInstanced(cube, material, instances, GRID_SIZE * GRID_SIZE);
      R3D_End();

      // Optionally show lights shapes
      if IsKeyDown(KEY_F) then
      begin
        BeginMode3D(camera);
        for i := 0 to NUM_LIGHTS - 1 do
        begin
          R3D_DrawLightShape(lights[i]);
        end;
        EndMode3D();
      end;

      DrawFPS(10, 10);
      DrawText('Press ''F'' to show the lights', 10, GetScreenHeight() - 34, 24, BLACK);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadInstanceBuffer(instances);
  R3D_UnloadMesh(cube);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

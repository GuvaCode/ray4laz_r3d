program stencil;

{$mode objfpc}{$H+}

uses
  SysUtils, Math, raylib, r3d, raymath;

var
  plane, box, sphere: TR3D_Mesh;
  matGround, matWall, matXraySolid, matXrayGhost, matOutlineSolid, matOutlineRing: TR3D_Material;
  light: TR3D_Light;
  camera: TCamera3D;
begin
  InitWindow(800, 450, '[r3d] - Stencil Effects');
  SetTargetFPS(60);

  R3D_Init(GetScreenWidth(), GetScreenHeight());
  R3D_SetAntiAliasingMode(R3D_ANTI_ALIASING_MODE_SMAA);

  // Create the meshes used in the scene
  plane := R3D_GenMeshPlane(20, 20, 1, 1);
  box := R3D_GenMeshCube(1.5, 2.0, 0.3);
  sphere := R3D_GenMeshSphere(0.5, 32, 32);

  matGround := R3D_GetDefaultMaterial();
  matGround.albedo.color := ColorCreate(160, 160, 160, 255);

  matWall := R3D_GetDefaultMaterial();
  matWall.albedo.color := ColorCreate(120, 80, 60, 255);

  // Main X-Ray sphere material
  // The first pass draws the sphere normally and marks its visible pixels
  // in the stencil buffer with the value 0x01
  matXraySolid := R3D_GetDefaultMaterial();
  matXraySolid.albedo.color := ColorCreate(80, 140, 220, 255);
  matXraySolid.stencil.mode := R3D_COMPARE_ALWAYS;
  matXraySolid.stencil.ref := $01;
  matXraySolid.stencil.opPass := R3D_STENCIL_REPLACE;

  // Ghost X-Ray sphere material
  // The second pass ignores depth so the sphere can be drawn through the wall,
  // but only where the first pass did not already mark the stencil buffer
  matXrayGhost := R3D_GetDefaultMaterial();
  matXrayGhost.albedo.color := ColorCreate(80, 140, 220, 60);
  matXrayGhost.depth.mode := R3D_COMPARE_ALWAYS;
  matXrayGhost.stencil.mode := R3D_COMPARE_NOTEQUAL;
  matXrayGhost.stencil.ref := $01;
  matXrayGhost.transparencyMode := R3D_TRANSPARENCY_ALPHA;
  matXrayGhost.unlit := true;

  // Main outline sphere material
  // The first pass draws the red sphere and marks its silhouette
  // in the stencil buffer with the value 0x02
  matOutlineSolid := R3D_GetDefaultMaterial();
  matOutlineSolid.albedo.color := ColorCreate(220, 100, 80, 255);
  matOutlineSolid.stencil.mode := R3D_COMPARE_ALWAYS;
  matOutlineSolid.stencil.ref := $02;
  matOutlineSolid.stencil.opPass := R3D_STENCIL_REPLACE;

  // Outline material
  // The second pass draws the same sphere slightly larger, only on pixels
  // outside the silhouette already marked by the first pass
  matOutlineRing := R3D_GetDefaultMaterial();
  matOutlineRing.albedo.color := ColorCreate(255, 220, 0, 255);
  matOutlineRing.stencil.mode := R3D_COMPARE_NOTEQUAL;
  matOutlineRing.stencil.ref := $02;
  matOutlineRing.cullMode := R3D_CULL_FRONT;
  matOutlineRing.unlit := true;

  // Configure lighting, shadows, and ambient color
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(10, 10, 15, 255));

  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_SetLightTarget(light, Vector3Create(4, 8, 5), Vector3Zero);
  R3D_SetShadowSoftness(light, 8.0);
  R3D_EnableLight(light);
  R3D_EnableShadow(light);

 camera.position :=  Vector3Create(0,3,5);
 camera.target := Vector3Create(0, 0, 0);
 camera.up := Vector3Create(0, 1, 0);
 camera.fovy := 55;

  // Main loop
  while not WindowShouldClose() do
  begin
    UpdateCamera(@camera, CAMERA_ORBITAL);

    BeginDrawing();
        ClearBackground(BLACK);

        R3D_Begin(camera);
            // Base scene geometry
            R3D_DrawMesh(plane, matGround, Vector3Create(  0.0, -0.5,  0.0 ), 1.0);
            R3D_DrawMesh(box,   matWall,   Vector3Create(  0.0,  0.5,  0.0 ), 1.0);

            // X-Ray sphere: visible solid pass, then transparent pass through the wall
            R3D_DrawMesh(sphere, matXraySolid, Vector3Create( 0.0, 0.5, -1.5 ), 1.0);
            R3D_DrawMesh(sphere, matXrayGhost, Vector3Create( 0.0, 0.5, -1.5 ), 1.0);

            // Outline sphere: normal object pass, then slightly enlarged outline pass
            R3D_DrawMesh(sphere, matOutlineSolid, Vector3Create( 2.2, 0.2, 0.8 ), 1.00);
            R3D_DrawMesh(sphere, matOutlineRing,  Vector3Create( 2.2, 0.2, 0.8 ), 1.08);
        R3D_End();
    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(box);
  R3D_UnloadMesh(plane);
  R3D_Close();

  CloseWindow();
end.

program r3d_transparency;
{$mode objfpc}{$H+}

uses
  cthreads,
  Classes, SysUtils, CustApp, raylib, r3d, raymath;

var
  Cube: TR3D_Model;
  Plane: TR3D_Model;
  Sphere: TR3D_Model;
  Camera: TCamera3D;

function Init: PChar;
var
  Mesh: TR3D_Mesh;
  Light: TR3D_Light;
  CubeColor: TColor;
begin
  R3D_Init(GetScreenWidth, GetScreenHeight, 0);
  SetTargetFPS(60);

  // --- Load cube model ---
  Mesh := R3D_GenMeshCube(1, 1, 1);
  Cube := R3D_LoadModelFromMesh(@Mesh);

  CubeColor := ColorCreate(100, 100, 255, 100);
  cube.materials[0].transparencyMode := R3D_TRANSPARENCY_ALPHA;
  Cube.materials[0].albedo.color := CubeColor;
  Cube.materials[0].orm.occlusion := 1.0;
  Cube.materials[0].orm.roughness := 0.2;
  Cube.materials[0].orm.metalness := 0.2;


  // --- Load plane model ---
  Mesh := R3D_GenMeshPlane(1000, 1000, 1, 1);
  Plane := R3D_LoadModelFromMesh(@Mesh);

  Plane.materials[0].orm.occlusion := 1.0;
  Plane.materials[0].orm.roughness := 1.0;
  Plane.materials[0].orm.metalness := 0.0;

  // --- Load sphere model ---
  Mesh := R3D_GenMeshSphere(0.5, 64, 64);
  Sphere := R3D_LoadModelFromMesh(@Mesh);

  Sphere.materials[0].orm.occlusion := 1.0;
  Sphere.materials[0].orm.roughness := 0.25;
  Sphere.materials[0].orm.metalness := 0.75;

  // --- Configure the camera ---
  Camera.position := Vector3Create(0, 2, 2);
  Camera.target := Vector3Create(0, 0, 0);
  Camera.up := Vector3Create(0, 1, 0);
  Camera.fovy := 60;

  // Setup lighting
  R3D_GetEnvironment^.ambient.color := ColorCreate(10, 10, 10, 255);
  Light := R3D_CreateLight(R3D_LIGHT_SPOT);

  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Create(0,0,0));
  R3D_SetLightActive(light, true);
  R3D_EnableShadow(light, 4096);


  Result := '[r3d] - Transparency example';
end;

procedure Update({%H-}delta: Single);
begin
  UpdateCamera(@Camera, CAMERA_ORBITAL);
end;

procedure Draw;
begin
  R3D_Begin(Camera);
    // Draw ground plane slightly below origin
    R3D_DrawModel(@Plane, Vector3Create(0, -0.5, 0), 1.0);

    // Draw metallic sphere at origin
    R3D_DrawModel(@Sphere, Vector3Create(0, 0, 0), 1.0);

    // Draw transparent cube at origin
    R3D_DrawModel(@Cube, Vector3Create(0, 0, 0), 1.0);
  R3D_End();
end;

procedure Close;
begin
  R3D_UnloadModel(@Plane, False);
  R3D_UnloadModel(@Sphere, False);
  R3D_UnloadModel(@Cube, False);
  R3D_Close();
end;

begin
  InitWindow(800, 600, 'Transparency Example');
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

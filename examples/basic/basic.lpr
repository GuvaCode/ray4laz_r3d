program basic;

{$mode objfpc}{$H+}

uses
  cthreads,
  Classes, SysUtils, CustApp, raylib, r3d, raymath;

type
  { TRayApplication }
  TRayApplication = class(TCustomApplication)
  protected
    procedure DoRun; override;
  private
    plane, sphere: TR3D_Mesh;
    material: TR3D_Material;
    light: TR3D_Light;
    camera: TCamera3D;
    procedure Init;
    procedure Update;
    procedure Draw;
    procedure Close;
  public
    constructor Create(TheOwner: TComponent); override;
    destructor Destroy; override;
  end;

const
  AppTitle = '[r3d] - Basic example';

{ TRayApplication }

constructor TRayApplication.Create(TheOwner: TComponent);
begin
  inherited Create(TheOwner);

  // Initialize window
  InitWindow(800, 450, AppTitle);
  SetTargetFPS(60);

  // Initialize
  Init;
end;

procedure TRayApplication.DoRun;
begin
  while not WindowShouldClose() do
  begin
    Update;

    BeginDrawing();
      Draw;
    EndDrawing();
  end;

  // Stop program loop
  Terminate;
end;

procedure TRayApplication.Init;
begin
  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create meshes
  plane := R3D_GenMeshPlane(1000, 1000, 1, 1);
  sphere := R3D_GenMeshSphere(0.5, 64, 64);
  material := R3D_GetDefaultMaterial();

  // Setup environment
  R3D_ENVIRONMENT_SET('ambient.color', ColorCreate(10, 10, 10, 255));

  // Create light
  light := R3D_CreateLight(R3D_LIGHT_SPOT);
  R3D_LightLookAt(light, Vector3Create(0, 10, 5), Vector3Create(0, 0, 0));
  R3D_EnableShadow(light);
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0, 2, 2);
  camera.target := Vector3Create(0, 0, 0);
  camera.up := Vector3Create(0, 1, 0);
  camera.fovy := 60;
  camera.projection := CAMERA_PERSPECTIVE;
end;

procedure TRayApplication.Update;
begin
  UpdateCamera(@camera, CAMERA_ORBITAL);
end;

procedure TRayApplication.Draw;
begin
  ClearBackground(RAYWHITE);

  R3D_Begin(camera);
    R3D_DrawMesh(plane, material, Vector3Create(0, -0.5, 0), 1.0);
    R3D_DrawMesh(sphere, material, Vector3Zero(), 1.0);
  R3D_End();

  // Draw FPS
  DrawFPS(10, 10);
end;

procedure TRayApplication.Close;
begin
  // Cleanup
  R3D_UnloadMesh(sphere);
  R3D_UnloadMesh(plane);
  R3D_Close();
end;

destructor TRayApplication.Destroy;
begin
  Close;
  CloseWindow(); // Close window and OpenGL context

  inherited Destroy;
end;

var
  Application: TRayApplication;
begin
  Application := TRayApplication.Create(nil);
  Application.Title := AppTitle;
  Application.Run;
  Application.Free;
end.


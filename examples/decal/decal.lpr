program DecalExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  MAXDECALS = 32;
  DEG2RAD = PI / 180.0;

type
  PSurface = ^TSurface;
  TSurface = record
    position: TVector3;
    normal: TVector3;
    rotation: TMatrix;
    translation: TMatrix;
  end;

var
  ScreenWidth, ScreenHeight: Integer;
  materialRoom: TR3D_Material;
  decal: TR3D_Decal;
  roomSize: Single;
  meshPlane: TR3D_Mesh;
  surfaces: array[0..5] of TSurface;
  light: TR3D_Light;
  camera: TCamera3D;
  decalScale: TVector3;
  targetDecalTransform, transform: TMatrix;
  instances: TR3D_InstanceBuffer;
  decalCount, decalIndex, i: Integer;
  hitPoint: TVector3;
  delta: Single;
  hitRay: TRay;
  decalRotation: TQuaternion;
  bgColor: TColor;

function MatrixTransform(position: TVector3; rotation: TQuaternion; scale: TVector3): TMatrix;
var
  xx, yy, zz, xy, xz, yz, wx, wy, wz: Single;
begin
  xx := rotation.x * rotation.x;
  yy := rotation.y * rotation.y;
  zz := rotation.z * rotation.z;
  xy := rotation.x * rotation.y;
  xz := rotation.x * rotation.z;
  yz := rotation.y * rotation.z;
  wx := rotation.w * rotation.x;
  wy := rotation.w * rotation.y;
  wz := rotation.w * rotation.z;

  Result := MatrixIdentity();
  Result.m0 := scale.x * (1.0 - 2.0 * (yy + zz));
  Result.m1 := scale.y * 2.0 * (xy - wz);
  Result.m2 := scale.z * 2.0 * (xz + wy);
  Result.m3 := position.x;

  Result.m4 := scale.x * 2.0 * (xy + wz);
  Result.m5 := scale.y * (1.0 - 2.0 * (xx + zz));
  Result.m6 := scale.z * 2.0 * (yz - wx);
  Result.m7 := position.y;

  Result.m8 := scale.x * 2.0 * (xz - wy);
  Result.m9 := scale.y * 2.0 * (yz + wx);
  Result.m10 := scale.z * (1.0 - 2.0 * (xx + yy));
  Result.m11 := position.z;

  Result.m12 := 0.0;
  Result.m13 := 0.0;
  Result.m14 := 0.0;
  Result.m15 := 1.0;
end;

function RayIntersectsSurface(ray: TRay; surface: PSurface; size: Single;
  var intersectionOut: TVector3): Boolean;
var
  d, t: Single;
  intersectionPoint, topLeft, bottomRight: TVector3;
  surfaceNormal, surfacePosition: TVector3;
begin
  surfaceNormal := surface^.normal;
  surfacePosition := surface^.position;

  d := -Vector3DotProduct(surfaceNormal, surfacePosition);
  t := -(Vector3DotProduct(surfaceNormal, ray.position) + d) /
        Vector3DotProduct(surfaceNormal, ray.direction);

  if t > 0 then
  begin
    // Calculate the intersection point
    intersectionPoint := Vector3Add(ray.position, Vector3Scale(ray.direction, t));

    topLeft := Vector3Subtract(surfacePosition,
      Vector3Create(size / 2.0, 0.0, size / 2.0));
    bottomRight := Vector3Add(surfacePosition,
      Vector3Create(size / 2.0, 0.0, size / 2.0));

    // Check if the intersection point is within the bounds of the surface
    if (intersectionPoint.x >= topLeft.x) and (intersectionPoint.x <= bottomRight.x) and
       (intersectionPoint.z >= topLeft.z) and (intersectionPoint.z <= bottomRight.z) then
    begin
      // Offset the intersection point
      intersectionOut := intersectionPoint;
      Result := True;
      Exit;
    end;
  end;

  Result := False;
end;

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Decal example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight());

  // Create decal
  decal := R3D_DECAL_BASE;
  decal.albedo := R3D_LoadAlbedoMap(PAnsiChar(RESOURCES_PATH + 'images/decal.png'), WHITE);
  decal.normal := R3D_LoadNormalMap(PAnsiChar(RESOURCES_PATH + 'images/decal_normal.png'), 1.0);
  decal.normalThreshold := 89.0;

  // Create room mesh, material and data
  roomSize := 25.0;
  meshPlane := R3D_GenMeshPlane(roomSize, roomSize, 1, 1);

  materialRoom := R3D_GetDefaultMaterial();
  materialRoom.albedo.color := GRAY;

  // Setup surfaces (walls, floor, ceiling)
  surfaces[0].position := Vector3Create(0.0, roomSize / 2, 0.0);
  surfaces[0].normal := Vector3Create(0.0, -1.0, 0.0);
  surfaces[0].rotation := MatrixRotateX(180.0 * DEG2RAD);
  surfaces[0].translation := MatrixTranslate(0.0, roomSize / 2, 0.0);

  surfaces[1].position := Vector3Create(0.0, -roomSize / 2, 0.0);
  surfaces[1].normal := Vector3Create(0.0, 1.0, 0.0);
  surfaces[1].rotation := MatrixIdentity();
  surfaces[1].translation := MatrixTranslate(0.0, -roomSize / 2, 0.0);

  surfaces[2].position := Vector3Create(roomSize / 2, 0.0, 0.0);
  surfaces[2].normal := Vector3Create(-1.0, 0.0, 0.0);
  surfaces[2].rotation := MatrixRotateZ(90.0 * DEG2RAD);
  surfaces[2].translation := MatrixTranslate(roomSize / 2, 0.0, 0.0);

  surfaces[3].position := Vector3Create(-roomSize / 2, 0.0, 0.0);
  surfaces[3].normal := Vector3Create(1.0, 0.0, 0.0);
  surfaces[3].rotation := MatrixRotateZ(-90.0 * DEG2RAD);
  surfaces[3].translation := MatrixTranslate(-roomSize / 2, 0.0, 0.0);

  surfaces[4].position := Vector3Create(0.0, 0.0, roomSize / 2);
  surfaces[4].normal := Vector3Create(0.0, 0.0, -1.0);
  surfaces[4].rotation := MatrixRotateX(-90.0 * DEG2RAD);
  surfaces[4].translation := MatrixTranslate(0.0, 0.0, roomSize / 2);

  surfaces[5].position := Vector3Create(0.0, 0.0, -roomSize / 2);
  surfaces[5].normal := Vector3Create(0.0, 0.0, 1.0);
  surfaces[5].rotation := MatrixRotateX(90.0 * DEG2RAD);
  surfaces[5].translation := MatrixTranslate(0.0, 0.0, -roomSize / 2);

  // Setup light
  light := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightPosition(light, Vector3Create(roomSize * 0.3, roomSize * 0.3, roomSize * 0.3));
  R3D_SetLightEnergy(light, 2.0);
  R3D_SetLightActive(light, True);
  R3D_ENVIRONMENT_SET('ambient.color', DARKGRAY);

  // Setup camera
  camera.position := Vector3Create(0.0, 0.0, 0.0);
  camera.target := Vector3Create(0.0, 0.0, 1.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 70;
  camera.projection := CAMERA_PERSPECTIVE;

  DisableCursor();

  // Decal state
  decalScale := Vector3Create(5.0, 5.0, 5.0);
  targetDecalTransform := MatrixIdentity();
  instances := R3D_LoadInstanceBuffer(MAXDECALS,
    R3D_INSTANCE_POSITION or R3D_INSTANCE_ROTATION or R3D_INSTANCE_SCALE);
  decalCount := 0;
  decalIndex := 0;
  hitPoint := Vector3Zero();

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    UpdateCamera(@camera, CAMERA_FREE);

    // Compute ray from camera to target
    hitRay.position := camera.position;
    hitRay.direction := Vector3Normalize(Vector3Subtract(camera.target, camera.position));

    // Check ray intersection
    hitPoint := Vector3Zero();
    decalRotation := QuaternionIdentity();

    for i := 0 to 5 do
    begin
      if RayIntersectsSurface(hitRay, @surfaces[i], roomSize, hitPoint) then
      begin
        decalRotation := QuaternionFromMatrix(surfaces[i].rotation);
        Break;
      end;
    end;

    // Apply decal on mouse click
    if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
    begin
      R3D_UploadInstances(instances, R3D_INSTANCE_POSITION, decalIndex, 1, @hitPoint);
      R3D_UploadInstances(instances, R3D_INSTANCE_ROTATION, decalIndex, 1, @decalRotation);
      R3D_UploadInstances(instances, R3D_INSTANCE_SCALE, decalIndex, 1, @decalScale);
      decalIndex := (decalIndex + 1) mod MAXDECALS;
      if decalCount < MAXDECALS then Inc(decalCount);
    end;

    // Draw scene
    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);

        for i := 0 to 5 do
        begin
          transform := MatrixMultiply(surfaces[i].rotation, surfaces[i].translation);
          R3D_DrawMeshPro(meshPlane, materialRoom, transform);
        end;

        if decalCount > 0 then
        begin
          R3D_DrawDecalInstanced(decal, instances, decalCount);
        end;

        R3D_DrawDecal(decal, MatrixTransform(hitPoint, decalRotation, decalScale));

      R3D_End();

      BeginMode3D(camera);
        DrawCubeWires(hitPoint, decalScale.x, decalScale.y, decalScale.z, WHITE);
      EndMode3D();

      DrawText('LEFT CLICK TO APPLY DECAL', 10, 10, 20, LIME);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMesh(meshPlane);
  R3D_UnloadDecalMaps(decal);
  R3D_Close();

  CloseWindow();
end.

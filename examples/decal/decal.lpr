program DecalExample;

uses
  SysUtils, Math, raylib, r3d, raymath;

const
  RESOURCES_PATH = 'resources/';
  MAXDECALS = 256;
  DEG2RAD = PI / 180.0;

var
  ScreenWidth, ScreenHeight: Integer;
  materialWalls, decalMaterial: TR3D_Material;
  decal: TR3D_Decal;
  roomSize: Single;
  meshPlane: TR3D_Mesh;
  matRoom: array[0..5] of TMatrix;
  light: TR3D_Light;
  camera: TCamera3D;
  decalScale: TVector3;
  targetDecalTransform: TMatrix;
  instances: TR3D_InstanceBuffer;
  decalCount, decalIndex, i: Integer;
  targetPosition: TVector3;
  delta: Single;
  hitRay: TRay;
  hitPoint, hitNormal: TVector3;
  decalRotation: TQuaternion;
  bgColor: TColor;

function RayCubeIntersection(ray: TRay; cubePosition, cubeSize: TVector3;
  var intersectionPoint, normal: TVector3): Boolean;
var
  halfSize, min, max: TVector3;
  tMin, tMax, tYMin, tYMax, tZMin, tZMax, temp: Single;
begin
  halfSize := Vector3Create(cubeSize.x / 2, cubeSize.y / 2, cubeSize.z / 2);

  min := Vector3Create(cubePosition.x - halfSize.x, cubePosition.y - halfSize.y, cubePosition.z - halfSize.z);
  max := Vector3Create(cubePosition.x + halfSize.x, cubePosition.y + halfSize.y, cubePosition.z + halfSize.z);

  tMin := (min.x - ray.position.x) / ray.direction.x;
  tMax := (max.x - ray.position.x) / ray.direction.x;

  if tMin > tMax then
  begin
    temp := tMin;
    tMin := tMax;
    tMax := temp;
  end;

  tYMin := (min.y - ray.position.y) / ray.direction.y;
  tYMax := (max.y - ray.position.y) / ray.direction.y;

  if tYMin > tYMax then
  begin
    temp := tYMin;
    tYMin := tYMax;
    tYMax := temp;
  end;

  if (tMin > tYMax) or (tYMin > tMax) then
  begin
    Result := False;
    Exit;
  end;

  if tYMin > tMin then tMin := tYMin;
  if tYMax < tMax then tMax := tYMax;

  tZMin := (min.z - ray.position.z) / ray.direction.z;
  tZMax := (max.z - ray.position.z) / ray.direction.z;

  if tZMin > tZMax then
  begin
    temp := tZMin;
    tZMin := tZMax;
    tZMax := temp;
  end;

  if (tMin > tZMax) or (tZMin > tMax) then
  begin
    Result := False;
    Exit;
  end;

  if tZMin > tMin then tMin := tZMin;
  if tZMax < tMax then tMax := tZMax;

  if tMin < 0 then
  begin
    tMin := tMax;
    if tMin < 0 then
    begin
      Result := False;
      Exit;
    end;
  end;

  intersectionPoint := Vector3Create(
    ray.position.x + ray.direction.x * tMin,
    ray.position.y + ray.direction.y * tMin,
    ray.position.z + ray.direction.z * tMin
  );

  normal := Vector3Zero();

  if Abs(intersectionPoint.x - min.x) < 0.001 then
    normal.x := -1.0  // Left face
  else if Abs(intersectionPoint.x - max.x) < 0.001 then
    normal.x := 1.0;  // Right face

  if Abs(intersectionPoint.y - min.y) < 0.001 then
    normal.y := -1.0  // Bottom face
  else if Abs(intersectionPoint.y - max.y) < 0.001 then
    normal.y := 1.0;  // Top face

  if Abs(intersectionPoint.z - min.z) < 0.001 then
    normal.z := -1.0  // Near face
  else if Abs(intersectionPoint.z - max.z) < 0.001 then
    normal.z := 1.0;  // Far face

  Result := True;
end;

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

begin
  // Initialize window
  ScreenWidth := 800;
  ScreenHeight := 450;
  InitWindow(ScreenWidth, ScreenHeight, '[r3d] - Decal example');
  SetTargetFPS(60);

  // Initialize R3D
  R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

  // Create wall material
  materialWalls := R3D_GetDefaultMaterial();
  materialWalls.albedo.color := GRAY;

  // Create decal material
  decalMaterial := R3D_GetDefaultMaterial();
  decalMaterial.albedo := R3D_LoadAlbedoMap(PAnsiChar(RESOURCES_PATH + 'decal.png'), WHITE);

  decal.material := decalMaterial;

  // Create room mesh and transforms
  roomSize := 32.0;
  meshPlane := R3D_GenMeshPlane(roomSize, roomSize, 1, 1);

  matRoom[0] := MatrixMultiply(MatrixRotateZ(90.0 * DEG2RAD), MatrixTranslate(roomSize / 2.0, 0.0, 0.0));
  matRoom[1] := MatrixMultiply(MatrixRotateZ(-90.0 * DEG2RAD), MatrixTranslate(-roomSize / 2.0, 0.0, 0.0));
  matRoom[2] := MatrixMultiply(MatrixRotateX(90.0 * DEG2RAD), MatrixTranslate(0.0, 0.0, -roomSize / 2.0));
  matRoom[3] := MatrixMultiply(MatrixRotateX(-90.0 * DEG2RAD), MatrixTranslate(0.0, 0.0, roomSize / 2.0));
  matRoom[4] := MatrixMultiply(MatrixRotateX(180.0 * DEG2RAD), MatrixTranslate(0.0, roomSize / 2.0, 0.0));
  matRoom[5] := MatrixTranslate(0.0, -roomSize / 2.0, 0.0);

  // Setup light
  light := R3D_CreateLight(R3D_LIGHT_OMNI);
  R3D_SetLightEnergy(light, 2.0);
  R3D_SetLightActive(light, True);

  // Setup camera
  camera.position := Vector3Create(0.0, 0.0, 0.0);
  camera.target := Vector3Create(1.0, 0.0, 0.0);
  camera.up := Vector3Create(0.0, 1.0, 0.0);
  camera.fovy := 70;
  camera.projection := CAMERA_PERSPECTIVE;

  DisableCursor();

  // Decal state
  decalScale := Vector3Create(3.0, 3.0, 3.0);
  targetDecalTransform := MatrixIdentity();
  instances := R3D_LoadInstanceBuffer(MAXDECALS, R3D_INSTANCE_POSITION or R3D_INSTANCE_ROTATION or R3D_INSTANCE_SCALE);
  decalCount := 0;
  decalIndex := 0;
  targetPosition := Vector3Zero();

  // Main loop
  while not WindowShouldClose() do
  begin
    delta := GetFrameTime();

    UpdateCamera(@camera, CAMERA_FREE);

    // Compute ray from camera to target
    hitRay.position := camera.position;
    hitRay.direction := Vector3Normalize(Vector3Subtract(camera.target, camera.position));

    hitPoint := Vector3Zero();
    hitNormal := Vector3Zero();
    if RayCubeIntersection(hitRay, Vector3Zero(), Vector3Create(roomSize, roomSize, roomSize), hitPoint, hitNormal) then
    begin
      targetPosition := hitPoint;
    end;

    // Compute decal rotation
    decalRotation := QuaternionIdentity();
    if hitNormal.x = -1.0 then
      decalRotation := QuaternionFromMatrix(MatrixRotateXYZ(Vector3Create(-90.0 * DEG2RAD, 180.0 * DEG2RAD, 90.0 * DEG2RAD)))
    else if hitNormal.x = 1.0 then
      decalRotation := QuaternionFromMatrix(MatrixRotateXYZ(Vector3Create(-90.0 * DEG2RAD, 180.0 * DEG2RAD, -90.0 * DEG2RAD)))
    else if hitNormal.y = -1.0 then
      decalRotation := QuaternionFromMatrix(MatrixRotateY(180.0 * DEG2RAD))
    else if hitNormal.y = 1.0 then
      decalRotation := QuaternionFromMatrix(MatrixRotateZ(180.0 * DEG2RAD))
    else if hitNormal.z = -1.0 then
      decalRotation := QuaternionFromMatrix(MatrixRotateX(90.0 * DEG2RAD))
    else if hitNormal.z = 1.0 then
      decalRotation := QuaternionFromMatrix(MatrixRotateXYZ(Vector3Create(-90.0 * DEG2RAD, 180.0 * DEG2RAD, 0)));

    // Apply decal on mouse click
    if IsMouseButtonPressed(MOUSE_BUTTON_LEFT) then
    begin
      R3D_UploadInstances(instances, R3D_INSTANCE_POSITION, decalIndex, 1, @targetPosition);
      R3D_UploadInstances(instances, R3D_INSTANCE_ROTATION, decalIndex, 1, @decalRotation);
      R3D_UploadInstances(instances, R3D_INSTANCE_SCALE, decalIndex, 1, @decalScale);
      decalIndex := (decalIndex + 1) mod MAXDECALS;
      if decalCount < MAXDECALS then Inc(decalCount);
    end;

    // Draw scene
    BeginDrawing();
      ClearBackground(RAYWHITE);

      R3D_Begin(camera);

        for  i := 0 to 5 do
        begin
          R3D_DrawMeshPro(meshPlane, materialWalls, matRoom[i]);
        end;

        if decalCount > 0 then
        begin
          R3D_DrawDecalInstanced(decal, instances, decalCount);
        end;

        R3D_DrawDecal(decal, MatrixTransform(targetPosition, decalRotation, decalScale));

      R3D_End();

      BeginMode3D(camera);
        DrawCubeWires(targetPosition, decalScale.x, decalScale.y, decalScale.z, WHITE);
      EndMode3D();

      DrawText('LEFT CLICK TO APPLY DECAL', 10, 10, 20, LIME);

    EndDrawing();
  end;

  // Cleanup
  R3D_UnloadMaterial(decalMaterial);
  R3D_UnloadMesh(meshPlane);
  R3D_Close();

  CloseWindow();
end.

// // R3D Cubemap Helpers Module

function R3D_CUBEMAP_SKY_BASE: TR3D_CubemapSky; inline;
begin
  Result.skyTopColor := ColorCreate(98, 116, 140, 255);
  Result.skyHorizonColor := ColorCreate(165, 167, 171, 255);
  Result.skyHorizonCurve := 0.15;
  Result.skyEnergy := 1.0;

  Result.groundBottomColor := ColorCreate(51, 43, 34, 255);
  Result.groundHorizonColor := ColorCreate(165, 167, 171, 255);
  Result.groundHorizonCurve := 0.02;
  Result.groundEnergy := 1.0;

  Result.sunDirection := Vector3Create(-1.0, -1.0, -1.0);
  Result.sunColor := WHITE;
  Result.sunSize := 1.5 * DEG2RAD;
  Result.sunCurve := 0.15;
  Result.sunEnergy := 1.0;
end;

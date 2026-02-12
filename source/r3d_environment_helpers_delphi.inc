// ========================================
// HELPER FUNCTIONS
// ========================================

{*
 * @brief Default environment configuration.
 *
 * Initializes an R3D_Environment structure with sensible default values for all
 * rendering parameters. Use this as a starting point for custom configurations.
 *}
function R3D_ENVIRONMENT_BASE: TR3D_Environment;
begin
  Result.background.color := GRAY;
  Result.background.energy := 1.0;
  Result.background.skyBlur := 0.0;
  Result.background.sky := Default(TR3D_Cubemap); // Пустая кубическая карта
  Result.background.rotation.x := 0.0;
  Result.background.rotation.y := 0.0;
  Result.background.rotation.z := 0.0;
  Result.background.rotation.w := 1.0;

  Result.ambient.color := BLACK;
  Result.ambient.energy := 1.0;
  Result.ambient.map := Default(TR3D_AmbientMap); // Пустая карта окружения

  Result.ssao.sampleCount := 16;
  Result.ssao.intensity := 0.5;
  Result.ssao.power := 1.5;
  Result.ssao.radius := 0.5;
  Result.ssao.bias := 0.02;
  Result.ssao.enabled := false;

  Result.ssil.sampleCount := 4;
  Result.ssil.sliceCount := 4;
  Result.ssil.sampleRadius := 2.0;
  Result.ssil.hitThickness := 0.5;
  Result.ssil.aoPower := 1.0;
  Result.ssil.energy := 1.0;
  Result.ssil.bounce := 0.5;
  Result.ssil.convergence := 0.5;
  Result.ssil.enabled := false;

  Result.ssr.maxRaySteps := 32;
  Result.ssr.binarySteps := 4;
  Result.ssr.stepSize := 0.125;
  Result.ssr.thickness := 0.2;
  Result.ssr.maxDistance := 4.0;
  Result.ssr.edgeFade := 0.25;
  Result.ssr.enabled := false;

  Result.bloom.mode := R3D_BLOOM_DISABLED;
  Result.bloom.levels := 0.5;
  Result.bloom.intensity := 0.05;
  Result.bloom.threshold := 0.0;
  Result.bloom.softThreshold := 0.5;
  Result.bloom.filterRadius := 1.0;

  Result.fog.mode := R3D_FOG_DISABLED;
  Result.fog.color.r := 255;
  Result.fog.color.g := 255;
  Result.fog.color.b := 255;
  Result.fog.color.a := 255;
  Result.fog.start := 1.0;
  Result.fog.end_ := 50.0;
  Result.fog.density := 0.05;
  Result.fog.skyAffect := 0.5;

  Result.dof.mode := R3D_DOF_DISABLED;
  Result.dof.focusPoint := 10.0;
  Result.dof.focusScale := 1.0;
  Result.dof.maxBlurSize := 20.0;
  //Result.dof.debugMode := false;

  Result.tonemap.mode := R3D_TONEMAP_LINEAR;
  Result.tonemap.exposure := 1.0;
  Result.tonemap.white := 1.0;

  Result.color.brightness := 1.0;
  Result.color.contrast := 1.0;
  Result.color.saturation := 1.0;
end;


procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Single);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Integer);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Boolean);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TColor);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Cubemap);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_AmbientMap);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TQuaternion);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Bloom);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Fog);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_DoF);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Tonemap);
begin
  raise Exception.Create('Use R3D_GetEnvironment() to set Value');
end;

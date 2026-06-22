// R3D Environment Helpers Module

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
  Result.ssao.intensity := 1.0;
  Result.ssao.power := 1.0;
  Result.ssao.maxRadius := 0.2;
  Result.ssao.radius := 1.0;
  Result.ssao.bias := 0.03;
  Result.ssao.enabled := false;

  Result.ssil.sampleCount := 16;
  Result.ssil.giIntensity := 1.0;
  Result.ssil.aoIntensity := 1.0;
  Result.ssil.aoPower := 1.0;
  Result.ssil.maxRadius := 0.2;
  Result.ssil.radius := 4.0;
  Result.ssil.bias := 0.03;
  Result.ssil.enabled := false;

  Result.ssgi.sliceCount := 4;
  Result.ssgi.edgeFade := 0.1;
  Result.ssgi.distanceFalloff := 1.0;
  Result.ssgi.normalRejection:= 0.0;
  Result.ssgi.intensity := 1.0;
  Result.ssgi.denoiseSteps :=  4;
  Result.ssgi.enabled := False;

  Result.ssr.maxRaySteps := 32;
  Result.ssr.binarySteps := 4;
  Result.ssr.stepSize := 0.125;
  Result.ssr.thickness := 0.2;
  Result.ssr.maxDistance := 4.0;
  Result.ssr.edgeFade := 0.25;
  Result.ssr.enabled := false;

  Result.fog.mode := R3D_FOG_DISABLED;
  Result.fog.color.r := 255;
  Result.fog.color.g := 255;
  Result.fog.color.b := 255;
  Result.fog.color.a := 255;
  Result.fog.start := 1.0;
  Result.fog.end_ := 50.0;
  Result.fog.density := 0.05;
  Result.fog.skyAffect := 0.5;

  Result.volumetricFog.scatteringDensity := 0.01;
  Result.volumetricFog.absortionDensity := 0.03;
  Result.volumetricFog.scatteringColor := WHITE;
  Result.volumetricFog.anisotropy := 0.5;
  Result.volumetricFog.emissionColor := WHITE;
  Result.volumetricFog.emissionEnergy := 0.0;
  Result.volumetricFog.skyAffect := 0.5;
  Result.volumetricFog.length := 50.0;
  Result.volumetricFog.stepSize := 1.0;
  Result.volumetricFog.enabled := false;

  Result.dof.mode := R3D_DOF_DISABLED;
  Result.dof.focusPoint := 10.0;
  Result.dof.focusScale := 1.0;
  Result.dof.nearScale := 1.0;
  Result.dof.maxBlurSize := 20.0;

  Result.bloom.mode := R3D_BLOOM_DISABLED;
  Result.bloom.levels := 0.5;
  Result.bloom.intensity := 0.05;
  Result.bloom.threshold := 0.0;
  Result.bloom.softThreshold := 0.5;
  Result.bloom.filterRadius := 1.0;

  Result.autoExposure.minEV := -1.0;
  Result.autoExposure.maxEV := 1.0;
  Result.autoExposure.exposureCompensation := 0.0;
  Result.autoExposure.adaptationToBright := 0.5;
  Result.autoExposure.adaptationToDark := 1.0;

  Result.tonemap.mode := R3D_TONEMAP_LINEAR;
  Result.tonemap.exposure := 1.0;
  Result.tonemap.white := 1.0;

  Result.color.brightness := 1.0;
  Result.color.contrast := 1.0;
  Result.color.saturation := 1.0;

end;

// ========================================
// HELPER FUNCTIONS IMPLEMENTATION
// ========================================

// Реализация для Single (числа с плавающей точкой)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Single);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.energy': Env^.background.energy := Value;
    'background.skyblur': Env^.background.skyBlur := Value;

    // Ambient
    'ambient.energy': Env^.ambient.energy := Value;

    // SSAO
    'ssao.intensity': Env^.ssao.intensity := Value;
    'ssao.power': Env^.ssao.power := Value;
    'ssao.maxRadius': Env^.ssao.maxRadius := Value;
    'ssao.radius': Env^.ssao.radius := Value;
    'ssao.bias': Env^.ssao.bias := Value;

    // SSIL
    'ssil.giIntensity': Env^.ssil.giIntensity := Value;
    'ssil.aoIntensity': Env^.ssil.aoIntensity := Value;
    'ssil.aoPower': Env^.ssil.aoPower := Value;
    'ssil.maxRadius': Env^.ssil.maxRadius := Value;
    'ssil.radius': Env^.ssil.radius := Value;
    'ssil.bias': Env^.ssil.bias := Value;

    // SSGI
    'ssgi.edgeFade': Env^.ssgi.edgeFade := Value;
    'ssgi.distanceFalloff': Env^.ssgi.distanceFalloff := Value;
    'ssgi.normalRejection': Env^.ssgi.normalRejection := Value;
    'ssgi.intensity': Env^.ssgi.intensity := Value;

    // SSR
    'ssr.stepsize': Env^.ssr.stepSize := Value;
    'ssr.thickness': Env^.ssr.thickness := Value;
    'ssr.maxdistance': Env^.ssr.maxDistance := Value;
    'ssr.edgefade': Env^.ssr.edgeFade := Value;

    // Fog
    'fog.start': Env^.fog.start := Value;
    'fog.end': Env^.fog.end_ := Value;
    'fog.density': Env^.fog.density := Value;
    'fog.skyaffect': Env^.fog.skyAffect := Value;

    // VolumetricFog
    'volumetricFog.scatteringDensity': Env^.volumetricFog.scatteringDensity := Value;
    'volumetricFog.absortionDensity': Env^.volumetricFog.absortionDensity := Value;
    'volumetricFog.anisotropy': Env^.volumetricFog.anisotropy := Value;
    'volumetricFog.emissionEnergy': Env^.volumetricFog.emissionEnergy := Value;
    'volumetricFog.skyAffect': Env^.volumetricFog.skyAffect := Value;
    'volumetricFog.length': Env^.volumetricFog.length := Value;
    'volumetricFog.stepSize': Env^.volumetricFog.stepSize := Value;

    // Depth of Field
    'dof.focuspoint': Env^.dof.focusPoint := Value;
    'dof.focusscale': Env^.dof.focusScale := Value;
    'dof.nearScale': Env^.dof.nearScale := Value;
    'dof.maxblursize': Env^.dof.maxBlurSize := Value;

    // Bloom
    'bloom.levels': Env^.bloom.levels := Value;
    'bloom.intensity': Env^.bloom.intensity := Value;
    'bloom.threshold': Env^.bloom.threshold := Value;
    'bloom.softthreshold': Env^.bloom.softThreshold := Value;
    'bloom.filterradius': Env^.bloom.filterRadius := Value;

    //autoExposure
    'autoExposure.minEV': Env^.autoExposure.minEV := Value;
    'autoExposure.maxEV': Env^.autoExposure.maxEV := Value;
    'autoExposure.exposureCompensation': Env^.autoExposure.exposureCompensation := Value;
    'autoExposure.adaptationToBright': Env^.autoExposure.adaptationToBright := Value;
    'autoExposure.adaptationToDark': Env^.autoExposure.adaptationToDark := Value;

    // Tonemap
    'tonemap.exposure': Env^.tonemap.exposure := Value;
    'tonemap.white': Env^.tonemap.white := Value;

    // Color Grading
    'color.brightness': Env^.color.brightness := Value;
    'color.contrast': Env^.color.contrast := Value;
    'color.saturation': Env^.color.saturation := Value;

  else
    raise Exception.CreateFmt('Unknown Single field or wrong type: %s', [Path]);
  end;
end;

// Реализация для Integer
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Integer);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // SSAO
    'ssao.samplecount': Env^.ssao.sampleCount := Value;

    // SSIL
    'ssil.samplecount': Env^.ssil.sampleCount := Value;

    // SSR
    'ssr.maxraysteps': Env^.ssr.maxRaySteps := Value;
    'ssr.binarysteps': Env^.ssr.binarySteps := Value;
    // SSGI
    'sampleCount': Env^.ssgi.sliceCount := Value;
    'denoiseSteps': Env^.ssgi.denoiseSteps := Value;

  else
    raise Exception.CreateFmt('Unknown Integer field or wrong type: %s', [Path]);
  end;
end;

// Реализация для Boolean
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: Boolean);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // SSAO
    'ssao.enabled': Env^.ssao.enabled := Value;

    // SSIL
    'ssil.enabled': Env^.ssil.enabled := Value;

    // SSR
    'ssr.enabled': Env^.ssr.enabled := Value;

    // SSGI
    'ssgi.enabled': Env^.ssgi.enabled := Value;

    // AutoExposure
    'autoExposure.enabled': Env^.autoExposure.enabled := Value;

   // VolumetricFog

   'volumetricFog.enabled': Env^.volumetricFog.enabled := Value;

  else
    raise Exception.CreateFmt('Unknown Boolean field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TColor
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TColor);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.color': Env^.background.color := Value;

    // Ambient
    'ambient.color': Env^.ambient.color := Value;

    // Fog
    'fog.color': Env^.fog.color := Value;

    // VolumetricFog
    'volumetricFog.scatteringColor' :Env^.volumetricFog.scatteringColor := Value;
    'volumetricFog.emissionColor' :Env^.volumetricFog.emissionColor := Value;
  else
    raise Exception.CreateFmt('Unknown TColor field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Cubemap
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Cubemap);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.sky': Env^.background.sky := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Cubemap field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_AmbientMap
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_AmbientMap);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Ambient
    'ambient.map': Env^.ambient.map := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_AmbientMap field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TQuaternion
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TQuaternion);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Background
    'background.rotation': Env^.background.rotation := Value;

  else
    raise Exception.CreateFmt('Unknown TQuaternion field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Bloom (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Bloom);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Bloom
    'bloom.mode': Env^.bloom.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Bloom field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Fog (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Fog);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Fog
    'fog.mode': Env^.fog.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Fog field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_DoF (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_DoF);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Depth of Field
    'dof.mode': Env^.dof.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_DoF field or wrong type: %s', [Path]);
  end;
end;

// Реализация для TR3D_Tonemap (перечисление)
procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_Tonemap);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // Tonemap
    'tonemap.mode': Env^.tonemap.mode := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Tonemap field or wrong type: %s', [Path]);
  end;
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_EnvBackground);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // background
    'background': Env^.background := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Tonemap field or wrong type: %s', [Path]);
  end;
end;

procedure R3D_ENVIRONMENT_SET(const Path: string; Value: TR3D_EnvAmbient);
var
  Env: PR3D_Environment;
begin
  Env := R3D_GetEnvironment();
  if not Assigned(Env) then Exit;

  case LowerCase(Path) of
    // ambient    //TR3D_EnvAmbient
    'ambient': Env^.ambient := Value;

  else
    raise Exception.CreateFmt('Unknown TR3D_Tonemap field or wrong type: %s', [Path]);
  end;
end;



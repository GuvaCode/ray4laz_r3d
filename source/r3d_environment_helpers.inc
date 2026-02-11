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
 // Result.dof.debugMode := false;

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
    'ssao.radius': Env^.ssao.radius := Value;
    'ssao.bias': Env^.ssao.bias := Value;

    // SSIL
    'ssil.sampleradius': Env^.ssil.sampleRadius := Value;
    'ssil.hitthickness': Env^.ssil.hitThickness := Value;
    'ssil.aopower': Env^.ssil.aoPower := Value;
    'ssil.energy': Env^.ssil.energy := Value;
    'ssil.convergence': Env^.ssil.convergence := Value;
    'ssil.bounce': Env^.ssil.bounce := Value;

    // SSR - ИСПРАВЛЕННЫЕ имена полей
    'ssr.stepsize': Env^.ssr.stepSize := Value;
    'ssr.thickness': Env^.ssr.thickness := Value;
    'ssr.maxdistance': Env^.ssr.maxDistance := Value;
    'ssr.edgefade': Env^.ssr.edgeFade := Value;

    // Bloom
    'bloom.levels': Env^.bloom.levels := Value;
    'bloom.intensity': Env^.bloom.intensity := Value;
    'bloom.threshold': Env^.bloom.threshold := Value;
    'bloom.softthreshold': Env^.bloom.softThreshold := Value;
    'bloom.filterradius': Env^.bloom.filterRadius := Value;

    // Fog
    'fog.start': Env^.fog.start := Value;
    'fog.end': Env^.fog.end_ := Value;
    'fog.density': Env^.fog.density := Value;
    'fog.skyaffect': Env^.fog.skyAffect := Value;

    // Depth of Field
    'dof.focuspoint': Env^.dof.focusPoint := Value;
    'dof.focusscale': Env^.dof.focusScale := Value;
    'dof.maxblursize': Env^.dof.maxBlurSize := Value;

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
    'ssil.slicecount': Env^.ssil.sliceCount := Value;

    // SSR - ИСПРАВЛЕННЫЕ имена полей
    'ssr.maxraysteps': Env^.ssr.maxRaySteps := Value;
    'ssr.binarysteps': Env^.ssr.binarySteps := Value;

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

    // Depth of Field
   // 'dof.debugmode': Env^.dof.debugMode := Value;

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

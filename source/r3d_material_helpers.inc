// R3D Material Helpers Module

{*
 * @brief Default material configuration.
 *
 * Initializes an R3D_Material structure with sensible default values for all
 * rendering parameters. Use this as a starting point for custom configurations.
 *}
function R3D_MATERIAL_BASE: TR3D_Material; inline;
begin
  // Инициализация текстурных карт
  Result.albedo.texture := Default(TTexture2D);
  Result.albedo.color := WHITE;

  Result.emission.texture := Default(TTexture2D);
  Result.emission.color := WHITE;
  Result.emission.energy := 0.0;

  Result.normal.texture := Default(TTexture2D);
  Result.normal.scale := 1.0;

  Result.orm.texture := Default(TTexture2D);
  Result.orm.occlusion := 1.0;
  Result.orm.roughness := 1.0;
  Result.orm.metalness := 0.0;

  // UV параметры
  Result.uvOffset := Vector2Create(0.0, 0.0);
  Result.uvScale := Vector2Create(1.0, 1.0);
  Result.alphaCutoff := 0.01;

  // Состояние глубины
  Result.depth.mode := R3D_COMPARE_LESS;
  Result.depth.offsetFactor := 0.0;
  Result.depth.offsetUnits := 0.0;
  Result.depth.rangeNear := 0.0;
  Result.depth.rangeFar := 1.0;

  // Состояние трафарета
  Result.stencil.mode := R3D_COMPARE_ALWAYS;
  Result.stencil.ref := 0;
  Result.stencil.mask := $FF;
  Result.stencil.opFail := R3D_STENCIL_KEEP;
  Result.stencil.opZFail := R3D_STENCIL_KEEP;
  Result.stencil.opPass := R3D_STENCIL_REPLACE;

  // Режимы рендеринга
  Result.transparencyMode := R3D_TRANSPARENCY_DISABLED;
  Result.billboardMode := R3D_BILLBOARD_DISABLED;
  Result.blendMode := R3D_BLEND_MIX;
  Result.cullMode := R3D_CULL_BACK;

  // Дополнительные параметры
  Result.unlit := False;
  Result.shader := nil;
end;

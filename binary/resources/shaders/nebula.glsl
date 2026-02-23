uniform vec2 u_resolution;
uniform float u_seed;  // Seed для рандомизации (0-100)

// Цвета туманностей
uniform vec3 uNebulaColor1;
uniform vec3 uNebulaColor2;
uniform vec3 uNebulaColor3;
uniform vec3 uNebulaColor4;

// Интенсивность туманностей
uniform float uNebulaIntensity1;
uniform float uNebulaIntensity2;
uniform float uNebulaIntensity3;
uniform float uNebulaIntensity4;

// Параметры для вариативности
uniform float uNoiseScale;      // Масштаб шума

// Общая яркость
uniform float uBrightness;

const float pi = 3.1415927;

// ----------------------------------------------------------------------------
// Noise functions
// ----------------------------------------------------------------------------

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float hash(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

float noise(float x) {
    float i = floor(x);
    float f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    return mix(hash(vec2(i, 0.0)), hash(vec2(i + 1.0, 0.0)), f);
}

float noise(vec2 x) {
    vec2 i = floor(x);
    vec2 f = fract(x);
    
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

// ----------------------------------------------------------------------------
// 4D Perlin noise
// ----------------------------------------------------------------------------

vec4 mod289(vec4 x) {
    return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
    return mod289(((x * 34.0) + 1.0) * x);
}

vec4 taylorInvSqrt(vec4 r) {
    return 1.79284291400159 - 0.85373472095314 * r;
}

vec4 fade(vec4 t) {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float cnoise(vec4 P) {
    vec4 Pi0 = floor(P);
    vec4 Pi1 = Pi0 + 1.0;
    Pi0 = mod289(Pi0);
    Pi1 = mod289(Pi1);
    vec4 Pf0 = fract(P);
    vec4 Pf1 = Pf0 - 1.0;
    vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
    vec4 iy = vec4(Pi0.yy, Pi1.yy);
    vec4 iz0 = vec4(Pi0.zzzz);
    vec4 iz1 = vec4(Pi1.zzzz);
    vec4 iw0 = vec4(Pi0.wwww);
    vec4 iw1 = vec4(Pi1.wwww);

    vec4 ixy = permute(permute(ix) + iy);
    vec4 ixy0 = permute(ixy + iz0);
    vec4 ixy1 = permute(ixy + iz1);
    vec4 ixy00 = permute(ixy0 + iw0);
    vec4 ixy01 = permute(ixy0 + iw1);
    vec4 ixy10 = permute(ixy1 + iw0);
    vec4 ixy11 = permute(ixy1 + iw1);

    vec4 gx00 = ixy00 * (1.0 / 7.0);
    vec4 gy00 = floor(gx00) * (1.0 / 7.0);
    vec4 gz00 = floor(gy00) * (1.0 / 6.0);
    gx00 = fract(gx00) - 0.5;
    gy00 = fract(gy00) - 0.5;
    gz00 = fract(gz00) - 0.5;
    vec4 gw00 = vec4(0.75) - abs(gx00) - abs(gy00) - abs(gz00);
    vec4 sw00 = step(gw00, vec4(0.0));
    gx00 -= sw00 * (step(0.0, gx00) - 0.5);
    gy00 -= sw00 * (step(0.0, gy00) - 0.5);

    vec4 gx01 = ixy01 * (1.0 / 7.0);
    vec4 gy01 = floor(gx01) * (1.0 / 7.0);
    vec4 gz01 = floor(gy01) * (1.0 / 6.0);
    gx01 = fract(gx01) - 0.5;
    gy01 = fract(gy01) - 0.5;
    gz01 = fract(gz01) - 0.5;
    vec4 gw01 = vec4(0.75) - abs(gx01) - abs(gy01) - abs(gz01);
    vec4 sw01 = step(gw01, vec4(0.0));
    gx01 -= sw01 * (step(0.0, gx01) - 0.5);
    gy01 -= sw01 * (step(0.0, gy01) - 0.5);

    vec4 gx10 = ixy10 * (1.0 / 7.0);
    vec4 gy10 = floor(gx10) * (1.0 / 7.0);
    vec4 gz10 = floor(gy10) * (1.0 / 6.0);
    gx10 = fract(gx10) - 0.5;
    gy10 = fract(gy10) - 0.5;
    gz10 = fract(gz10) - 0.5;
    vec4 gw10 = vec4(0.75) - abs(gx10) - abs(gy10) - abs(gz10);
    vec4 sw10 = step(gw10, vec4(0.0));
    gx10 -= sw10 * (step(0.0, gx10) - 0.5);
    gy10 -= sw10 * (step(0.0, gy10) - 0.5);

    vec4 gx11 = ixy11 * (1.0 / 7.0);
    vec4 gy11 = floor(gx11) * (1.0 / 7.0);
    vec4 gz11 = floor(gy11) * (1.0 / 6.0);
    gx11 = fract(gx11) - 0.5;
    gy11 = fract(gy11) - 0.5;
    gz11 = fract(gz11) - 0.5;
    vec4 gw11 = vec4(0.75) - abs(gx11) - abs(gy11) - abs(gz11);
    vec4 sw11 = step(gw11, vec4(0.0));
    gx11 -= sw11 * (step(0.0, gx11) - 0.5);
    gy11 -= sw11 * (step(0.0, gy11) - 0.5);

    vec4 g0000 = vec4(gx00.x,gy00.x,gz00.x,gw00.x);
    vec4 g1000 = vec4(gx00.y,gy00.y,gz00.y,gw00.y);
    vec4 g0100 = vec4(gx00.z,gy00.z,gz00.z,gw00.z);
    vec4 g1100 = vec4(gx00.w,gy00.w,gz00.w,gw00.w);
    vec4 g0010 = vec4(gx10.x,gy10.x,gz10.x,gw10.x);
    vec4 g1010 = vec4(gx10.y,gy10.y,gz10.y,gw10.y);
    vec4 g0110 = vec4(gx10.z,gy10.z,gz10.z,gw10.z);
    vec4 g1110 = vec4(gx10.w,gy10.w,gz10.w,gw10.w);
    vec4 g0001 = vec4(gx01.x,gy01.x,gz01.x,gw01.x);
    vec4 g1001 = vec4(gx01.y,gy01.y,gz01.y,gw01.y);
    vec4 g0101 = vec4(gx01.z,gy01.z,gz01.z,gw01.z);
    vec4 g1101 = vec4(gx01.w,gy01.w,gz01.w,gw01.w);
    vec4 g0011 = vec4(gx11.x,gy11.x,gz11.x,gw11.x);
    vec4 g1011 = vec4(gx11.y,gy11.y,gz11.y,gw11.y);
    vec4 g0111 = vec4(gx11.z,gy11.z,gz11.z,gw11.z);
    vec4 g1111 = vec4(gx11.w,gy11.w,gz11.w,gw11.w);

    vec4 norm00 = taylorInvSqrt(vec4(dot(g0000, g0000), dot(g0100, g0100), dot(g1000, g1000), dot(g1100, g1100)));
    g0000 *= norm00.x;
    g0100 *= norm00.y;
    g1000 *= norm00.z;
    g1100 *= norm00.w;

    vec4 norm01 = taylorInvSqrt(vec4(dot(g0001, g0001), dot(g0101, g0101), dot(g1001, g1001), dot(g1101, g1101)));
    g0001 *= norm01.x;
    g0101 *= norm01.y;
    g1001 *= norm01.z;
    g1101 *= norm01.w;

    vec4 norm10 = taylorInvSqrt(vec4(dot(g0010, g0010), dot(g0110, g0110), dot(g1010, g1010), dot(g1110, g1110)));
    g0010 *= norm10.x;
    g0110 *= norm10.y;
    g1010 *= norm10.z;
    g1110 *= norm10.w;

    vec4 norm11 = taylorInvSqrt(vec4(dot(g0011, g0011), dot(g0111, g0111), dot(g1011, g1011), dot(g1111, g1111)));
    g0011 *= norm11.x;
    g0111 *= norm11.y;
    g1011 *= norm11.z;
    g1111 *= norm11.w;

    float n0000 = dot(g0000, Pf0);
    float n1000 = dot(g1000, vec4(Pf1.x, Pf0.yzw));
    float n0100 = dot(g0100, vec4(Pf0.x, Pf1.y, Pf0.zw));
    float n1100 = dot(g1100, vec4(Pf1.xy, Pf0.zw));
    float n0010 = dot(g0010, vec4(Pf0.xy, Pf1.z, Pf0.w));
    float n1010 = dot(g1010, vec4(Pf1.x, Pf0.y, Pf1.z, Pf0.w));
    float n0110 = dot(g0110, vec4(Pf0.x, Pf1.yz, Pf0.w));
    float n1110 = dot(g1110, vec4(Pf1.xyz, Pf0.w));
    float n0001 = dot(g0001, vec4(Pf0.xyz, Pf1.w));
    float n1001 = dot(g1001, vec4(Pf1.x, Pf0.yz, Pf1.w));
    float n0101 = dot(g0101, vec4(Pf0.x, Pf1.y, Pf0.z, Pf1.w));
    float n1101 = dot(g1101, vec4(Pf1.xy, Pf0.z, Pf1.w));
    float n0011 = dot(g0011, vec4(Pf0.xy, Pf1.zw));
    float n1011 = dot(g1011, vec4(Pf1.x, Pf0.y, Pf1.zw));
    float n0111 = dot(g0111, vec4(Pf0.x, Pf1.yzw));
    float n1111 = dot(g1111, Pf1);

    vec4 fade_xyzw = fade(Pf0);
    vec4 n_0w = mix(vec4(n0000, n1000, n0100, n1100), vec4(n0001, n1001, n0101, n1101), fade_xyzw.w);
    vec4 n_1w = mix(vec4(n0010, n1010, n0110, n1110), vec4(n0011, n1011, n0111, n1111), fade_xyzw.w);
    vec4 n_zw = mix(n_0w, n_1w, fade_xyzw.z);
    vec2 n_yzw = mix(n_zw.xy, n_zw.zw, fade_xyzw.y);
    float n_xyzw = mix(n_yzw.x, n_yzw.y, fade_xyzw.x);
    return 2.2 * n_xyzw;
}

float nebulaNoise(vec3 p, float seed) {
    const int steps = 6;
    float scale = pow(2.0, float(steps));
    vec3 displace = vec3(0.0);
    
    // Добавляем seed для вариативности
    vec3 seedOffset = vec3(seed * 100.0, seed * 200.0, seed * 300.0);
    
    for (int i = 0; i < steps; i++) {
        displace = vec3(
            cnoise(vec4(p.xyz * scale + displace + seedOffset, 0.0)) * 0.5 + 0.5,
            cnoise(vec4(p.yzx * scale + displace + seedOffset, 0.0)) * 0.5 + 0.5,
            cnoise(vec4(p.zxy * scale + displace + seedOffset, 0.0)) * 0.5 + 0.5
        );
        scale *= 0.5;
    }
    
    return cnoise(vec4(p * scale + displace + seedOffset, 0.0)) * 0.5 + 0.5;
}

// ----------------------------------------------------------------------------
// Звезды с вариативностью 
// ----------------------------------------------------------------------------

float stars(vec3 dir, float seed) {
    float starIntensity = 0.0;
    
    // Мелкие звезды - фон
    for (int i = 0; i < 4; i++) {
        float scale = 200.0;
        vec3 pos = dir * scale;
        vec3 cell = floor(pos);
        vec3 gridPos = fract(pos);
        
        float r1 = hash(cell.xy + vec2(12.34 + seed, 56.78 + seed));
        float r2 = hash(cell.yz + vec2(34.56 + seed, 78.91 + seed));
        float r3 = hash(cell.zx + vec2(56.78 + seed, 91.23 + seed));
        
        float starProb = r1 * r2 * r3;
        
        if (starProb > 0.996) {
            vec3 starPos = vec3(
                hash(cell.xy + vec2(123.45 + seed, 678.90 + seed)),
                hash(cell.yz + vec2(234.56 + seed, 789.01 + seed)),
                hash(cell.zx + vec2(345.67 + seed, 890.12 + seed))
            );
            
            float dist = length(gridPos - starPos);
            float size = 0.03;
            
            if (dist < size) {
                float brightness = 1.0 - dist / size;
                brightness = brightness * brightness * 0.5;
                starIntensity += brightness;
            }
        }
    }
    
    // Средние звезды
    for (int i = 0; i < 3; i++) {
        float scale = 80.0;
        vec3 pos = dir * scale;
        vec3 cell = floor(pos);
        vec3 gridPos = fract(pos);
        
        float r1 = hash(cell.xy + vec2(98.76 + seed, 54.32 + seed));
        float r2 = hash(cell.yz + vec2(87.65 + seed, 43.21 + seed));
        
        float starProb = r1 * r2;
        
        if (starProb > 0.99) {
            vec3 starPos = vec3(
                hash(cell.xy + vec2(109.87 + seed, 65.43 + seed)),
                hash(cell.yz + vec2(210.98 + seed, 76.54 + seed)),
                hash(cell.zx + vec2(321.09 + seed, 87.65 + seed))
            );
            
            float dist = length(gridPos - starPos);
            float size = 0.06;
            
            if (dist < size) {
                float brightness = 1.0 - dist / size;
                brightness = brightness * brightness * 0.8;
                starIntensity += brightness;
            }
        }
    }
    
    // Крупные звезды
    for (int i = 0; i < 5; i++) {
        float scale = 40.0;
        vec3 pos = dir * scale;
        vec3 cell = floor(pos);
        vec3 gridPos = fract(pos);
        
        float starProb = hash(cell.xy + vec2(456.78 + seed, 987.65 + seed));
        
        if (starProb > 0.985) {
            vec3 starPos = vec3(
                hash(cell.xy + vec2(543.21 + seed, 678.90 + seed)),
                hash(cell.yz + vec2(654.32 + seed, 789.01 + seed)),
                hash(cell.zx + vec2(765.43 + seed, 890.12 + seed))
            );
            
            float dist = length(gridPos - starPos);
            float size = 0.1;
            
            if (dist < size) {
                float brightness = 1.0 - dist / size;
                brightness = brightness * brightness;
                starIntensity += brightness;
            }
        }
    }
    
    // Очень крупные звезды
    for (int i = 0; i < 3; i++) {
        float scale = 20.0;
        vec3 pos = dir * scale;
        vec3 cell = floor(pos);
        vec3 gridPos = fract(pos);
        
        float starProb = hash(cell.xy + vec2(111.22 + seed, 333.44 + seed));
        
        if (starProb > 0.97) {
            vec3 starPos = vec3(
                hash(cell.xy + vec2(555.66 + seed, 777.88 + seed)),
                hash(cell.yz + vec2(999.00 + seed, 111.22 + seed)),
                hash(cell.zx + vec2(333.44 + seed, 555.66 + seed))
            );
            
            float dist = length(gridPos - starPos);
            float size = 0.15;
            
            if (dist < size) {
                float brightness = 1.0 - dist / size;
                brightness = brightness * brightness * 1.2;
                starIntensity += brightness;
            }
        }
    }
    
    // Фоновые звезды (очень далекие)
    for (int i = 0; i < 2; i++) {
        float scale = 500.0;
        vec3 pos = dir * scale;
        vec3 cell = floor(pos);
        
        float starProb = hash(cell.xy + vec2(777.77 + seed, 888.88 + seed)) * 
                         hash(cell.yz + vec2(999.99 + seed, 111.11 + seed));
        
        if (starProb > 0.999) {
            starIntensity += 0.05;
        }
    }
    
    return clamp(starIntensity, 0.0, 1.2);
}

// ----------------------------------------------------------------------------
// Туманности с изменяемыми цветами и вариативностью
// ----------------------------------------------------------------------------

vec3 nebula(vec3 dir, float seed) {
    vec3 color = vec3(0.0);
    
    // Генерируем разные смещения на основе seed
    float offset1 = seed * 100.0;
    float offset2 = seed * 200.0;
    float offset3 = seed * 300.0;
    float offset4 = seed * 400.0;
    
    float n1 = nebulaNoise(dir * (0.5 * uNoiseScale) + vec3(100.0 + offset1, 200.0 + offset1, 300.0 + offset1), seed);
    n1 = pow(n1, 3.0);
    color += uNebulaColor1 * n1 * uNebulaIntensity1;
    
    float n2 = nebulaNoise(dir * (0.3 * uNoiseScale) + vec3(400.0 + offset2, 500.0 + offset2, 600.0 + offset2), seed);
    n2 = pow(n2, 2.5);
    color += uNebulaColor2 * n2 * uNebulaIntensity2;
    
    float n3 = nebulaNoise(dir * (0.4 * uNoiseScale) + vec3(700.0 + offset3, 800.0 + offset3, 900.0 + offset3), seed);
    n3 = pow(n3, 3.5);
    color += uNebulaColor3 * n3 * uNebulaIntensity3;
    
    float n4 = nebulaNoise(dir * (0.6 * uNoiseScale) + vec3(1000.0 + offset4, 1100.0 + offset4, 1200.0 + offset4), seed);
    n4 = pow(n4, 4.0);
    color += uNebulaColor4 * n4 * uNebulaIntensity4;
    
    return color;
}

// ----------------------------------------------------------------------------
// Цвет звезд
// ----------------------------------------------------------------------------

vec3 getStarColor(float intensity, vec3 dir, float seed) {
    if (intensity < 0.1) {
        return vec3(intensity);
    }
    
    float colorVar = hash(dir.xy * 100.0 + dir.yz * 50.0 + seed);
    
    vec3 warm = vec3(1.0, 0.95, 0.9);
    vec3 cool = vec3(0.9, 0.95, 1.0);
    
    vec3 baseColor = mix(warm, cool, colorVar);
    
    return baseColor * intensity;
}

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------

void fragment() {
    vec3 dir = normalize(EYEDIR);
    
    // Используем seed для рандомизации
    float seed = u_seed;
    
    // Звезды
    float starIntensity = stars(dir, seed);
    
    // Свечение вокруг ярких звезд
    float glow = max(0.0, starIntensity - 0.8) * 0.3;
    
    // Цвет звезд
    vec3 starColor = getStarColor(starIntensity, dir, seed);
    
    // Туманности
    vec3 nebulaColor = nebula(dir, seed);
    
    // Базовый цвет космоса
    vec3 finalColor = vec3(0.02, 0.02, 0.05);
    
    // Собираем всё вместе
    finalColor += nebulaColor;
    finalColor += starColor + vec3(glow * 0.5, glow * 0.6, glow);
    
    // Общая яркость
    finalColor *= uBrightness;
    
    COLOR = finalColor;
}

#pragma usage opaque

// Uniforms
uniform float iTime;
uniform vec3 iResolution;

// Varyings
varying vec2 v_texcoord;
varying vec3 v_position;

void vertex() {
    v_texcoord = TEXCOORD;
    v_position = POSITION;
}

// Оригинальные функции шума (без изменений)
vec4 permute_3d(vec4 x){ return mod(((x*34.0)+1.0)*x, 289.0); }
vec4 taylorInvSqrt3d(vec4 r){ return 1.79284291400159 - 0.85373472095314 * r; }

float simplexNoise3d(vec3 v)
{
    const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
    const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy) );
    vec3 x0 =   v - i + dot(i, C.xxx) ;

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min( g.xyz, l.zxy );
    vec3 i2 = max( g.xyz, l.zxy );

    //  x0 = x0 - 0. + 0.0 * C
    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;

    // Permutations
    i = mod(i, 289.0 );
    vec4 p = permute_3d( permute_3d( permute_3d( i.z + vec4(0.0, i1.z, i2.z, 1.0 )) + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))  + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

    // Gradients
    // ( N*N points uniformly over a square, mapped onto an octahedron.)
    float n_ = 1.0/7.0; // N=7
    vec3  ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4( x.xy, y.xy );
    vec4 b1 = vec4( x.zw, y.zw );

    vec4 s0 = floor(b0)*2.0 + 1.0;
    vec4 s1 = floor(b1)*2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
    vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

    vec3 p0 = vec3(a0.xy,h.x);
    vec3 p1 = vec3(a0.zw,h.y);
    vec3 p2 = vec3(a1.xy,h.z);
    vec3 p3 = vec3(a1.zw,h.w);

    // Normalise gradients
    vec4 norm = taylorInvSqrt3d(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value
    vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
    m = m * m;
    return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
}

float fbm3d(vec3 x, const in int it) {
    float v = 0.0;
    float a = 0.5;
    vec3 shift = vec3(100);
    
    for (int i = 0; i < 32; ++i) {
        if(i < it) {
            v += a * simplexNoise3d(x);
            x = x * 2.0 + shift;
            a *= 0.5;
        }
    }
    return v;
}

vec3 rotateZ(vec3 v, float angle) {
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    return vec3(
        v.x * cosAngle - v.y * sinAngle,
        v.x * sinAngle + v.y * cosAngle,
        v.z
    );
}

float facture(vec3 vector) {
    vec3 normalizedVector = normalize(vector);
    return max(max(normalizedVector.x, normalizedVector.y), normalizedVector.z);
}

vec3 emission(vec3 color, float strength) {
    return color * strength;
}

void fragment() {
    // Получаем базовые UV координаты
    vec2 baseUv;
    
    if (v_texcoord.x > -0.1 && v_texcoord.x < 1.1 && 
        v_texcoord.y > -0.1 && v_texcoord.y < 1.1) {
        baseUv = v_texcoord;
    } else {
        // Для цилиндра/круга используем нормализованные координаты позиции
        baseUv = v_position.xz * 0.5 + 0.5;
    }
    
    // Конвертируем в формат, который ожидает оригинальный шейдер
    // Оригинал: vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    vec2 fragCoord = baseUv * iResolution.xy;
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    
    // --- ВСЯ ОРИГИНАЛЬНАЯ МАТЕМАТИКА БЕЗ ИЗМЕНЕНИЙ ---
    vec3 color = vec3(uv.xy, 0.0);
    color.z += 0.5;
    
    color = normalize(color);
    color -= 0.2 * vec3(0.0, 0.0, iTime);
    
    float angle = -log2(length(uv)); // log base 0.5
    
    color = rotateZ(color, angle);
    
    float frequency = 1.4;
    float distortion = 0.01;
    color.x = fbm3d(color * frequency + 0.0, 5) + distortion;
    color.y = fbm3d(color * frequency + 1.0, 5) + distortion;
    color.z = fbm3d(color * frequency + 2.0, 5) + distortion;
    vec3 noiseColor = color; // save
    
    noiseColor *= 2.0;
    noiseColor -= 0.1;
    noiseColor *= 0.188;
    noiseColor += vec3(uv.xy, 0.0);
    
    float noiseColorLength = length(noiseColor);
    noiseColorLength = 0.770 - noiseColorLength;
    noiseColorLength *= 4.2;
    noiseColorLength = pow(noiseColorLength, 1.0);
    
    vec3 emissionColor = emission(vec3(0.961, 0.592, 0.078), noiseColorLength * 0.4);
    
    float fac = length(uv) - facture(color + 0.32);
    fac += 0.1;
    fac *= 3.0;
    
    color = mix(emissionColor, vec3(fac), fac + 1.2);
    // --- КОНЕЦ ОРИГИНАЛЬНОЙ МАТЕМАТИКИ ---
    
    // Для кругового портала добавляем маску (чтобы эффект был только на круге)
    float distFromCenter = length(uv * 0.8); // Немного уменьшаем маску, чтобы края не обрезались
    float circleMask = 1.0 - smoothstep(0.9, 1.1, distFromCenter);
    
    // Применяем маску
    color *= circleMask;
    
    // Устанавливаем альбедо и эмиссию для R3D
    ALBEDO = color * 0.2;      // Темный базовый цвет
    EMISSION = color * 2.5;     // Яркое свечение для bloom
    ALPHA = circleMask;          // Прозрачность по краям
}

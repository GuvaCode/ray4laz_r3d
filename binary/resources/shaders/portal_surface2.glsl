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

// Функции шума
vec2 random(vec2 st) {
    float x = fract(sin(dot(st.xy, vec2(3.0, 72.233))) * 43758.5453123);
    float y = fract(x * 77.0);
    return vec2(x, y);
}

float smoothNoise(vec2 uv) {
    vec2 repeatedUv = smoothstep(0.0, 1.0, fract(uv));
    vec2 tileCoords = floor(uv);

    float x1 = random(tileCoords).x;
    float x2 = random(tileCoords + vec2(1.0, 0.0)).x;
    
    float xValues = mix(x1, x2, repeatedUv.x);
    
    float y1 = random(tileCoords + vec2(0.0, 1.0)).x;
    float y2 = random(tileCoords + vec2(1.0, 1.0)).x;
    
    float yValues = mix(y1, y2, repeatedUv.x);
    
    return mix(xValues, yValues, repeatedUv.y);
}

float cellularNoise(vec2 uv, float size) {
    vec2 repeatedUv = fract(uv * size);
    vec2 uvCoords = floor(uv * size);
    
    vec2 point = vec2(0.5);
    float dist = 1.0;
    float currentDistance = 0.0;
    
    for(float i = -1.0; i <= 1.0; i++) {
        for(float j = -1.0; j <= 1.0; j++) {
            vec2 neighborTile = vec2(i, j);
            
            point = random(neighborTile + uvCoords);
            
            point += sin(iTime * 1.5 * point) * 0.3;
            
            currentDistance = distance(point + neighborTile, repeatedUv);
            
            dist = min(dist, currentDistance);
        }
    }
    
    return dist;
}

float border(vec2 uv) {
    float col = 0.02 / uv.x;
    col += 0.02 / uv.y;
    col = smoothstep(0.1, 1.0, col);
    
    return col * 0.4;
}

void fragment() {
    // Используем текстурные координаты если есть, иначе позицию
    vec2 uv;
    
    if (v_texcoord.x > -0.1 && v_texcoord.x < 1.1 && 
        v_texcoord.y > -0.1 && v_texcoord.y < 1.1) {
        uv = v_texcoord;
    } else {
        // Для цилиндра/круга используем нормализованные координаты
        uv = v_position.xz * 0.5 + 0.5; // Центрируем
    }
    
    // Основной шум
    float noise = smoothNoise(uv * 9.0) * 0.05;
    uv += noise;
    
    vec2 movingUv = uv;
    movingUv.y += iTime * 0.07;
    movingUv += noise;
    
    // Клеточный шум (voronoi)
    float cells1 = cellularNoise(movingUv, 3.0);
    cells1 = pow(cells1, 6.0) * 0.5;
    
    float cells2 = cellularNoise(movingUv, 6.0);
    cells2 = pow(cells2, 5.0) * 0.1;
    
    float cells = cells1 + cells2;
    
    // Границы
    float borders = border(1.05 - uv) + border(uv);
    
    // Цвета
    vec3 blue = vec3(0.259, 0.729, 1.000);
    
    // Финальный цвет
    vec3 finalColor = blue * (borders + cells);
    finalColor += blue * 0.1;
    
    // Для кругового портала добавляем маску
    float distFromCenter = length(uv - 0.5);
    float circleMask = 1.0 - smoothstep(0.4, 0.5, distFromCenter);
    
    // Применяем маску к цвету
    finalColor *= circleMask;
    
    // Устанавливаем альбедо и эмиссию
    ALBEDO = finalColor * 0.3; // Темный базовый цвет
    
    // Эмиссия для свечения - используем тот же цвет, но ярче
    EMISSION = finalColor * (1.5 + cells * 2.0);
    
    // Прозрачность по краям
    ALPHA = mix(1.0, 0.5, smoothstep(0.45, 0.5, distFromCenter));
}

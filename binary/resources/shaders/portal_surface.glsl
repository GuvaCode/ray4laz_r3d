#pragma usage opaque

// Uniforms
uniform float iTime;
uniform vec3 iResolution;

// Varyings for passing data from vertex to fragment
varying vec2 v_texcoord;
varying vec3 v_position;
varying vec3 v_normal;

void vertex() {
    // Pass data to fragment stage
    v_texcoord = TEXCOORD;
    v_position = POSITION;
    v_normal = NORMAL;
}

// Helper functions for portal effect
vec2 hash(vec2 p) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

float voronoi(vec2 x) {
    vec2 n = floor(x), f = fract(x);
    float F1 = 8.0;
    float F2 = 8.0;

    for (int j = -1; j <= 1; j++) {
        for(int i = -1; i <= 1; i++) {
            vec2 g = vec2(i, j);
            vec2 o = hash(n + g);
            vec2 r = g - f + o;
            float d = dot(r, r);

            if (d < F1) {
                F2 = F1;
                F1 = d;
            } else if (d < F2) {
                F2 = d;
            }
        }
    }
    return F2 - F1; // Возвращаем разницу для более интересного эффекта
}

vec2 twirl(vec2 UV, vec2 Center, float Strength) {
    vec2 delta = UV - Center;
    float angle = Strength * length(delta);
    vec2 uv = delta * mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    return uv + Center;
}

void fragment() {
    // Используем позицию для генерации эффекта портала на всей плоскости
    vec2 uv = v_position.xz * 0.8; // Масштабируем для лучшего вида
    
    // Добавляем вращение от времени
    float time = iTime * 0.3;
    
    // Создаем несколько слоев для глубины
    vec2 uv1 = uv * 2.0;
    vec2 uv2 = uv * 4.0 + time * 0.5;
    vec2 uv3 = uv * 8.0 - time * 0.3;
    
    // Применяем эффект закручивания
    uv1 = twirl(uv1, vec2(0.0, 0.0), 1.5);
    uv2 = twirl(uv2, vec2(0.0, 0.0), 2.0);
    uv3 = twirl(uv3, vec2(0.0, 0.0), 2.5);
    
    // Вычисляем вороной для каждого слоя
    float v1 = voronoi(uv1 + time);
    float v2 = voronoi(uv2 - time * 0.7);
    float v3 = voronoi(uv3 + time * 0.5);
    
    // Комбинируем слои для создания эффекта глубины
    float portal = (v1 * 0.6 + v2 * 0.3 + v3 * 0.1);
    
    // Создаем цветовой градиент для портала
    vec3 color1 = vec3(0.2, 0.5, 1.0); // Синий
    vec3 color2 = vec3(0.8, 0.2, 0.8); // Пурпурный
    vec3 color3 = vec3(0.1, 0.8, 0.9); // Голубой
    
    // Смешиваем цвета на основе вороного и позиции
    vec3 portalColor = mix(color1, color2, sin(uv.x * 3.0 + time) * 0.5 + 0.5);
    portalColor = mix(portalColor, color3, cos(uv.y * 3.0 - time) * 0.5 + 0.5);
    
    // Создаем эффект пульсации
    float pulse = sin(time * 3.0) * 0.2 + 0.8;
    
    // Создаем маску для круговой формы портала
    float distFromCenter = length(uv);
    float circleMask = 1.0 - smoothstep(0.8, 1.5, distFromCenter);
    float innerGlow = exp(-distFromCenter * 2.0) * 0.5;
    
    // Добавляем кольца
    float rings = sin(distFromCenter * 15.0 - time * 5.0) * 0.5 + 0.5;
    rings *= smoothstep(1.2, 0.8, distFromCenter);
    
    // Комбинируем все эффекты
    float finalIntensity = portal * circleMask + rings * 0.5 + innerGlow;
    finalIntensity = clamp(finalIntensity * pulse, 0.0, 1.0);
    
    // Финальный цвет с эмиссией для свечения
    ALBEDO = portalColor * finalIntensity * 0.5;
    EMISSION = portalColor * finalIntensity * 3.0; // Сильная эмиссия для свечения
    
    // Добавляем базовое освещение для остальной части плоскости
    ALBEDO = mix(vec3(0.1, 0.1, 0.15), ALBEDO, finalIntensity);
    
    ALPHA = 1.0;
}

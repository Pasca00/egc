#version 330

uniform sampler2D u_texture_0;
uniform vec3 globe_position;
uniform float time;
uniform vec3 eye_position;

uniform int isColumn;
uniform vec3 columnCellColor;
uniform vec3 columnLightPosition;

uniform int isCell;
uniform vec3 cellColor;
uniform vec3 cellLightPosition;

uniform int isSpotlight;
uniform vec3 spotlightColor;

uniform vec3 spotlightPositions[4];
uniform vec3 spotlightColors[4];
uniform vec3 spotlightDirections[4];

uniform int isDancer;
uniform vec3 dancerCellPositions[9];
uniform vec3 dancerCellColors[9];

uniform int globeIsActive;
uniform int floorIsActive;
uniform int spotlightsAreActive;

in vec2 tex_coord;
in vec3 world_position;
in vec3 world_normal;

float material_kd = 100.0;
float material_ks = 50.0;
float material_shininess = 30.0;

vec4 getGlobeLighting() {
	vec3 light_pos = normalize(globe_position);
	vec3 light_dir = normalize(world_position - globe_position);
	vec2 tex;

	tex.x = (1.0 / (2 * 3.14159)) * atan (light_dir.x, light_dir.z) + time * 0.1;
	tex.y = (1.0 / 3.14159) * acos (light_dir.y / length (light_dir));

	vec3 color = texture2D(u_texture_0, tex).xyz;

	vec3 norm = normalize(world_normal);

	vec3 L = normalize(globe_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);

	float diffuse_light = material_kd * 1 * max(dot(norm, L), 0.01);
	float specular_light = 0;
	if (diffuse_light > 0) {
        specular_light = material_ks * max(dot(norm, L), 0) * pow(max(dot(norm, H), 0), material_shininess);
    }

	float d = distance(globe_position, world_position);
	float attenuation_factor = 1 / (1 + 0.1 * d + 0.01 * d * d);
    float light = attenuation_factor * (diffuse_light + specular_light);

	return vec4(color, 1.f) * light;
}

float getColumnCellLighting() {
	vec3 light_pos = normalize(columnLightPosition);
	vec3 light_dir = normalize(world_position - columnLightPosition);

	vec3 L = normalize(columnLightPosition - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);

	vec3 norm = normalize(world_normal);

	float diffuse_light = material_kd * max(dot(norm, L), 0.01);
	float specular_light = 0;
	if (diffuse_light > 0) {
        specular_light = material_ks * max(dot(norm, L), 0) * pow(max(dot(norm, H), 0), material_shininess);
    }

	float d = distance(columnLightPosition, world_position);

	float light_radius = 2.5f;

	float att = 0;

	if (d < light_radius) {
		att = pow(light_radius - d, 2);
	}

	return att * (diffuse_light + specular_light);
}

vec3 getSpolightsLighting() {
	vec3 color = vec3(0);
	vec3 norm = normalize(world_normal);

	for (int i = 0; i < 4; i++) {
		vec3 light_pos = normalize(spotlightPositions[i]);
		vec3 light_dir = normalize(spotlightPositions[i] - world_position);

		vec3 L = normalize(spotlightPositions[i] - world_position);
		vec3 V = normalize(eye_position - world_position);
		vec3 H = normalize(L + V);

		float diffuse_light = material_kd * max(dot(norm, L), 0.01);
		float specular_light = 0;
		if (diffuse_light > 0) {
			specular_light = material_ks * max(dot(norm, L), 0) * pow(max(dot(norm, H), 0), material_shininess);
		}

		float cut_off = radians(20.f);
        float spot_light = dot(-L, spotlightDirections[i]);

		float light = 0;
        if (spot_light > cos(cut_off)) {
            float spot_light_limit = cos(cut_off);
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = pow(linear_att, 2);

            light = light_att_factor * (diffuse_light + specular_light);
        }

		color += light * spotlightColors[i];
	}

	return color;
}

vec3 getDancerFloorLighting() {
	vec3 color = vec3(0);
	vec3 norm = normalize(world_normal);
	
	for (int i = 0; i < 9; i++) {
		vec3 L = normalize(dancerCellPositions[i] - world_position);
		vec3 V = normalize(eye_position - world_position);
		vec3 H = normalize(L + V);

		float diffuse_light = material_kd * max(dot(norm, L), 0);
		float specular_light = 0;
		if (diffuse_light > 0) {
			specular_light = material_ks * max(dot(norm, L), 0) * pow(max(dot(norm, H), 0), material_shininess);
		}

		float d = distance(dancerCellPositions[i], world_position);
		float light_radius = 1.f;
		float att = 0;

		if (d < light_radius) {
			att = pow(light_radius - d, 2);
		}

		color += (diffuse_light + specular_light) * att * dancerCellColors[i];
	}

	return color;
}

void main() {
	vec4 finalColor = vec4(0);
	if (globeIsActive == 1) {
		finalColor = getGlobeLighting();
	}

	if (isColumn == 1) {
		if (floorIsActive == 1) { 
			finalColor += getColumnCellLighting() * vec4(columnCellColor, 1.f);		
		}

		if (spotlightsAreActive == 1) {
			finalColor += vec4(getSpolightsLighting(), 1.f);
		}
	}

	if (isCell == 1) {
		if (floorIsActive == 1) {
			float k_emissive = 1.f;
			finalColor += k_emissive * vec4(cellColor, 1.0);
		}

		if (spotlightsAreActive == 1) {
			finalColor += vec4(getSpolightsLighting(), 1.f);
		} 
	}

	if (isSpotlight == 1) {
		if (spotlightsAreActive == 1) {
			finalColor = vec4(spotlightColor, 0.5);
		} else {
			finalColor.a = 0;
		}
	}

	if (isDancer == 1) {
		finalColor *= 0.1;
		if (floorIsActive == 1) {
			finalColor += vec4(getDancerFloorLighting(), 1.f);
		}

		if (spotlightsAreActive == 1) {
			finalColor += vec4(getSpolightsLighting(), 1.f);
		}
	}

	gl_FragColor = finalColor;
}
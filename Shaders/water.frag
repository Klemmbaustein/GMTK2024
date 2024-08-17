//! #include "shared.frag"

// #params
uniform vec3 u_waterColor = vec3(0.25, 0.5, 1);
uniform vec3 u_brightWaterColor = vec3(0.3, 0.7, 1);
uniform sampler2D u_texture;

uniform float u_time;

void main()
{
	vec2 texCoords = v_position.xz;

	texCoords.x += sin(u_time + v_position.x / 10.0);
	texCoords.y -= sin(u_time + 2 + v_position.z / 10.0);

	texCoords /= 50;

	float brightWaterIntensity = clamp(sin(u_time + v_position.x / 30.0) * cos(u_time + v_position.z / 30.0), 0, 1);

	float waterIntensity = texture(u_texture, texCoords).r;
	vec3 color = mix(mix(u_waterColor, u_brightWaterColor, brightWaterIntensity), vec3(1), waterIntensity);

	color = GetLightingNormal(color, max(0.5 - waterIntensity - brightWaterIntensity * 0.3, 0.0) * 2, 0.5, normalize(vec3(0, 5, waterIntensity + brightWaterIntensity)));

	RETURN_COLOR(color);
}
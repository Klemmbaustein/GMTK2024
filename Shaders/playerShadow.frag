//! #include "shared.frag"

uniform float u_time = 0;

void main()
{
	vec2 coords = v_texcoord;
	coords = ivec2(coords * 32).xy / 32;

	float opacity = clamp(1 - (length(( - 0.5) * 2.5f) - sin(u_time * 3) / 8), 0, 1);
	if (opacity <= 0)
		discard;
	RETURN_COLOR_OPACITY(vec3(0.25, 0.25, 1), opacity);
}
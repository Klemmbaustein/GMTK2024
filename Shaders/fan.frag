//! #include "shared.frag"

// #params
uniform sampler2D u_fanTexture;

uniform float u_time;

void main()
{
	float Opacity = 1 - v_texcoord.y;

	vec2 texCoords = v_texcoord - vec2(0, u_time * 5);

	vec4 texValue = texture(u_fanTexture, texCoords);

	if (texValue.a < 0.5)
		discard;

	RETURN_COLOR_OPACITY(texValue.xyz, Opacity * texValue.a);
}
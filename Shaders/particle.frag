//! #include "shared.frag"

// #params
uniform sampler2D u_texture;
uniform float u_opacity = 1;

void main()
{
	vec4 texColor = texture(u_texture, v_texcoord);

	if (texColor.w < 0.33)
		discard;

	vec3 color = GetLightingNormal(texColor.xyz, 0, 0, vec3(0, 1, 0));

	RETURN_COLOR_OPACITY(color, u_opacity);
}

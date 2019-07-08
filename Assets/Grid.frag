#version 330 core
in vec2 TexCoord;
uniform float scale;
float grid(vec2 st, float res)
{
	vec2 grid = fract(st * res);
	return (step(res, grid.x) * step(res, grid.y));
}

void main() {
	vec2 grid_uv = TexCoord.xy * scale; // scale
	float x = grid(grid_uv, 0.1); // resolution
	gl_FragColor.rgb = vec3(0.5) * x;
	gl_FragColor.a = 1.0;
}

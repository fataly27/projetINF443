#version 330 core

in struct fragment_data
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;

	vec3 eye;
	vec4 clipSpace;
	vec2 textureCoords;
	vec2 textureCoords2;
} fragment;

layout(location=0) out vec4 FragColor;

uniform sampler2D image_texture;
uniform sampler2D dudv_texture;

uniform vec3 light = vec3(1.0, 1.0, 1.0);

uniform vec3 color = vec3(1.0, 1.0, 1.0); // Unifor color of the object
uniform float alpha = 1.0f; // alpha coefficient
uniform float Ka = 0.4; // Ambient coefficient
uniform float Kd = 0.8; // Diffuse coefficient
uniform float Ks = 0.4f;// Specular coefficient
uniform float specular_exp = 64.0; // Specular exponent
uniform bool use_texture = true;
uniform bool texture_inverse_y = false;

uniform float fog_falloff;
uniform float move_factor;

const float waveStrength = 0.005;

void main()
{
	vec2 ndc = (fragment.clipSpace.xy / fragment.clipSpace.w) / 2.0 + 0.5;
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	vec2 distortion1 = texture(dudv_texture, vec2(fragment.textureCoords.x + move_factor, fragment.textureCoords.y)).rg * 2.0 - 1.0;
	vec2 distortion2 = texture(dudv_texture, vec2(-fragment.textureCoords.x, fragment.textureCoords.y + move_factor)).rg * 2.0 - 1.0;

	vec2 distortionT = distortion1 + distortion2;

	reflectTexCoords += waveStrength * distortionT;
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

	vec4 reflectColour = texture(image_texture, reflectTexCoords);

	vec4 finalColour = mix(reflectColour, vec4(0.29, 0.51, 0.8, 1.0), 0.3 + 0.2 * (1.0 - (fragment.textureCoords2.x * fragment.textureCoords2.x + fragment.textureCoords2.y * fragment.textureCoords2.y)));

	//fog effect
	float depth = length(fragment.eye-fragment.position);
	float w_depth = exp(-fog_falloff*depth*depth);
	vec3 rayDir = normalize(fragment.eye-fragment.position);
	vec3 sunDir = normalize(vec3(1.0, 0.0, -0.5));
	float sunAmount = max( dot( rayDir, sunDir ), 0.0 );
	vec3  fogColor  = mix( vec3(0.75,0.82,0.9), // bluish
                           vec3(1.0,0.9,0.7), // yellowish
                           pow(sunAmount,8.0) );
	vec4 color_with_fog = w_depth*finalColour+(1-w_depth)*vec4(fogColor, 1.0);
	
	FragColor = color_with_fog;
}
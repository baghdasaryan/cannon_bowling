#version 150

in vec3 fN;		// normal at current position
in vec3 fV;		// vector from point to viewer
in vec3 fL;		// vector from point to light
in vec2 texCoord;

uniform vec4 lightColor;
uniform float Ambient;
uniform float Diffuse;
uniform float Specular;
uniform float Shininess;
uniform vec4 Color;

uniform sampler2D Tex;
uniform int	EnableTex;

void main()
{
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 V = normalize(fV);
	vec3 L = normalize(fL);
	vec3 H = normalize(L + V);
	//vec3 R = normalize(reflect(L, N));

	vec4 ambient = Ambient*Color;
	vec4 diffuse = (max(dot(L, N), 0.0))*Diffuse*Color;
	vec4 specular = (pow(max(dot(N, H), 0.0), Shininess))*Specular*lightColor;
	//vec4 specular = (pow(max(dot(R, E), 0.0), Shininess))*Specular*lightColor;

	if (EnableTex == 1)
	{
		diffuse *= texture2D(Tex, texCoord);
	}

	// discard the specular highlight if the light's behind the vertex
	if(dot(L, N) < 0.0)
	{
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	gl_FragColor = ambient + diffuse + specular;
	gl_FragColor.a = 1.0;

}

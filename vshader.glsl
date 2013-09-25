#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexture;

out vec3 fN;	// normal at current position
out vec3 fV;	// vector from point to viewer
out vec3 fL;	// vector from point to light
out vec2 texCoord;

uniform mat4 wMo;			// object to world
uniform mat4 cMw;			// world to camera
uniform mat4 proj;			// projection
//uniform mat2 wMt;			// texture to world

uniform vec4 camPos;		// position of camera
uniform vec4 lightPos;		// position of light

void main()
{
	gl_Position = proj*cMw*wMo*vPosition;

	fN = (wMo*vec4(vNormal.x, vNormal.y, vNormal.z, 0.0)).xyz;
	fV = (camPos - wMo*vPosition).xyz;
	fL = (lightPos - wMo*vPosition).xyz;

    if( lightPos.w != 0.0 ) 
    {
        fL = lightPos.xyz - vPosition.xyz;
    }

    texCoord = vTexture;
}

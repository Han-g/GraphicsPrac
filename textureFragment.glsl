#version 330 core

out vec4 FragColor;

in vec3 outColor;
In vec2 TexCoord;

uniform sampler2D outTexture1; //--- texture sampler outTexture1
uniform sampler2D outTexture2; //--- texture sampler outTexture2

void main()
{
	FragColor = (texture(outTexture1, TexCoord) + texture(outTexture2, TexCoord)) / 2.0; //--- 2개의 텍스처를 동시에 사용: 두 개의 텍스처 혼합
	//--- GLSL 지원 mix 함수
	//--- genType mix (genType x, genType y, genType a);
	//--- x와 y 값으로 선형보간한다.
	//--- 선형 보간 식: x * (1-a) + y*a
	//--- FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.8);
}
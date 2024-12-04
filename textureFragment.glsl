#version 330 core

out vec4 FragColor;

in vec3 outColor;
In vec2 TexCoord;

uniform sampler2D outTexture1; //--- texture sampler outTexture1
uniform sampler2D outTexture2; //--- texture sampler outTexture2

void main()
{
	FragColor = (texture(outTexture1, TexCoord) + texture(outTexture2, TexCoord)) / 2.0; //--- 2���� �ؽ�ó�� ���ÿ� ���: �� ���� �ؽ�ó ȥ��
	//--- GLSL ���� mix �Լ�
	//--- genType mix (genType x, genType y, genType a);
	//--- x�� y ������ ���������Ѵ�.
	//--- ���� ���� ��: x * (1-a) + y*a
	//--- FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.8);
}
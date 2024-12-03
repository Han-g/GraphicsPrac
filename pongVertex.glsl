#version 330 core

layout(location = 0) in vec3 vPos;   // ��ġ
layout(location = 1) in vec3 vNormal; // ��� ����

out vec3 FragPos;  // �����׸�Ʈ���� ����� ���� ��ǥ
out vec3 Normal;   // �����׸�Ʈ���� ����� ��� ����

uniform mat4 model;      // �� ��ȯ ���
uniform mat4 view;       // �� ��ȯ ���
uniform mat4 projection; // ���� ��ȯ ���

void main()
{
    FragPos = vec3(model * vec4(vPos, 1.0));                // ���� ��ǥ ���
    Normal = mat3(transpose(inverse(model))) * vNormal;     // ��� ���� ��ȯ
    gl_Position = projection * view * vec4(FragPos, 1.0);  // ���� ��ȯ ��ǥ
}
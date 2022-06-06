
static const int MAX_BONES = 32;


cbuffer cbuff0:register(b0) {

	matrix viewproj;//3D�ϊ��s��
	matrix world;
	float3 cameraPos;
};

cbuffer skinning:register(b3)
{
	matrix matSKinning[MAX_BONES];
}

//cbuffer cbuff1:register(b1) {
//	float3 m_ambient : packoffset(c0);//�A���r�G���g�W��
//	float3 m_diffuse : packoffset(c1);//�f�B�t���[�Y�W��
//	float3 m_specular : packoffset(c2);//�X�y�L�����[�W��
//	float m_alpha : packoffset(c2.w);//�A���t�@
//}

struct VSInput {
	float4 pos:POSITION;//�V�X�e���p���_���W
	float3 normal:NORMAL;//�@���x�N�g��
	float2 uv:TEXCOORD;//uv�l
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

//���_�V�F�[�_�[��s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput {
	float4 svpos:SV_POSITION;//�V�X�e���p���_���W
	float3 normal:NORMAL;//�@���x�N�g��
	float2 uv:TEXCOORD;//uv�l
};
#pragma once
class SkyBox : public Sphere
{
	// 3D  공간이 있는 프로그렘에서 하늘을 빠르게 묘사하기 위해 배치한 원시 모형
	// 스카이 박스의 특징 :  텍스쳐가 안을 향함
	// 아틀라스 :  세계지도, 여러 이미지를 한장에 붙인 큰 그림 모음. 구체의 경우 텍스쳐 1장

public:
	SkyBox(wstring file);
	~SkyBox();

	void Render();

private:	
	Texture* skyMap;
	RasterizerState* rasterizerState[2];
	// 정점과 벡터 위주로 만들어진 이미지가 있을 때,
	// 픽셀로 바꿔주는 렌더 파이프라인 단계

	DepthStencilState* depthStencilState[2];
	// 깊이 + 스텐실 상태
	// : 픽셀 혹은 셰이더 연산이 된 픽셀의 출력 우선순위를 설정하는 depth 상태와
	// 화소의 출력 가능 여부를 수동으로 설정하는 스탠실 상태를 
	// 하나의 기능으로 통합한 것

	// 


};


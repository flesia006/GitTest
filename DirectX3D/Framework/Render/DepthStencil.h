#pragma once
class DepthStencil
{
	// 뎁스 스텐실 클래스 : 뎁스 스텐실 스테이트에 관여되는 옵션을 직접 사용해서
					//    깊이에 의한 이미지 선별 / 장애물에 의한 이미지 선별 효과를 
					//	  개발자 임의로 만들기 위한 클래스	


public:
	DepthStencil(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT, bool isStencil = false);

	~DepthStencil();

	// 설정해제
	void Clear();

	ID3D11DepthStencilView*& GetDSV() { return dsv; }
	ID3D11ShaderResourceView*& GetSRV() { return srv; }

private:
	void CreateDSVTexture();
	void CreateDSV();
	void CreateSRV();

private:
	UINT width;
	UINT height;
	bool isStencil;


	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* srv;
	// 셰이더 리소스 접근 기능 :  셰이더가 텍스쳐에 접근하기 위한 절차
};


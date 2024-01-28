#pragma once
class RenderTarget
{
	// 렌더 타겟 : 렌더링이 있는 컴퓨터 프로그래밍에서, 렌더 타겟이 가진 본래의 공간/ 표적 좌표를
	//				다른 투사공간(뷰포트, 카메라)의 좌표에 이전하는 것과 그 결과물로 나온 이미지를 지칭
	// 

public:
	RenderTarget(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT
					, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);
	~RenderTarget();

	void Set(DepthStencil* depthStencil, Float4 clearColor = Float4(1, 1, 1 ,1));
	ID3D11ShaderResourceView*& GetSRV() { return srv; }
	ID3D11RenderTargetView*& GetRTV() { return rtv; }

private:
	void CreateRTVTexture();
	void CreateRTV();
	void CreateSRV();

private:
	UINT width;
	UINT height;
	DXGI_FORMAT format;

	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;

};


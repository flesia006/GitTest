#include "Framework.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(UINT width, UINT height, bool isStencil)
	:width(width), height(height), isStencil(isStencil)
{
	CreateDSVTexture();
	CreateDSV();
	CreateSRV();
}

DepthStencil::~DepthStencil()
{
	dsvTexture->Release();
	dsv->Release();
	srv->Release();

}

void DepthStencil::Clear()
{
	DC->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1, 0);

}

void DepthStencil::CreateDSVTexture()
{
	// 뎁스 스탠실 상태를 준비하고, 해당 화면을 따로 텍스쳐처럼 꾸미기
	D3D11_TEXTURE2D_DESC depthDesc = {};
	// 만든 다음에 옵션 설정
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = isStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	// 바인드 플레그 : 현재 뎁스 스텐슬, 셰이더 리소스 옵션을 적용하는 것으로 설정

	DEVICE->CreateTexture2D(&depthDesc, nullptr, &dsvTexture);

}

void DepthStencil::CreateDSV()
{
	// 텍스처 데이터를 기반으로 이 그림을 보기 위한 준비 절차
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = isStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	
	DEVICE->CreateDepthStencilView(dsvTexture, &depthViewDesc, &dsv); // 원본, 데이터, 결과 저장 위치

}

void DepthStencil::CreateSRV()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
	desc.Format = isStencil ? DXGI_FORMAT_R24_UNORM_X8_TYPELESS : DXGI_FORMAT_R32_FLOAT;
	// 스탠실이면 이제 스텐실에 가린 부분은 출력이 없음
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipLevels = 1;

	DEVICE->CreateShaderResourceView(dsvTexture, &desc, &srv);

}

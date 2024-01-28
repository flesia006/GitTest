#include "Framework.h"
#include "SkyBox.h"

SkyBox::SkyBox(wstring file)	
{
	// 텍스쳐와 셰이더 적용
	skyMap = Texture::Add(file);
	//material->SetShader(L"basic/texture/hlsl");
	material->SetShader(L"landscape/Skybox.hlsl");

	FOR(2)
	{
		rasterizerState[i] = new RasterizerState();
		depthStencilState[i] = new DepthStencilState();
	}

	rasterizerState[1]->FrontCounterClockwise(true);
	// 화소 우선순위 무시
	depthStencilState[1]->DepthEnable(false);


}

SkyBox::~SkyBox()
{
	FOR(2)
	{
		delete rasterizerState[i];
		delete depthStencilState[i];
	}

}

void SkyBox::Render()
{
	skyMap->PSSet(10); 
	// 10 보다 낮은 우선 순위의 픽셀이 먼저 연산됨
	
	// 스카이박스용 옵션이 적용된 상태를 호출
	rasterizerState[1]->SetState();
	depthStencilState[1]->SetState();

	Sphere::Render();

	rasterizerState[0]->SetState();
	depthStencilState[0]->SetState();
}

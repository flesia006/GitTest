#include "Framework.h"
#include "KunaiManager.h"

KunaiManager::KunaiManager()
{
	kunaiInstancing = new ModelInstancing("kunai");

	kunaies.reserve(size);
	FOR(size)
	{
		Transform* t = kunaiInstancing->Add();
		t->SetActive(true);
		Kunai* kunai = new Kunai(t);
		kunaies.push_back(kunai);
	}

}

KunaiManager::~KunaiManager()
{
	delete kunaiInstancing;
}

void KunaiManager::Update()
{
	kunaiInstancing->Update();
	for (auto& kunai : kunaies)
		kunai->Update();

}

void KunaiManager::Render()
{
	for (auto& kunai : kunaies)
		kunai->Render();
}

void KunaiManager::Throw(Vector3 pos, Vector3 dir)
{
	for (auto& kunai : kunaies)
	{
		if (!kunai->GetTransform()->Active())
		{
			kunai->Throw(pos, dir);
			return;
		}
	}
}

bool KunaiManager::IsCollision(Collider* collider)
{
	for (auto& kunai : kunaies)
	{
		if (kunai->GetCollider()->IsCollision(collider))
		{
			kunai->GetTransform()->SetActive(false);
			return true;
		}
	}
	return false;
}

#pragma once
class Kunai 
{
private:
	float LIFE_SPAN = 5;

public:
	Kunai(Transform* transform);
	~Kunai();

	void Update();
	void Render();

	void Throw(Vector3 pos, Vector3 dir);

	SphereCollider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }

private:
	Transform* transform;
	SphereCollider* collider;

	float speed = 1000;
	float time = 0;

	Vector3 direction;

};


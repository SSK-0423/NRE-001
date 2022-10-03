#pragma once

namespace NamelessEngine
{
	enum class COMPONENTID
	{
		DRAW = 0,
		TRANSFORM = 1,
		COLLISION = 2
	};

	class IComponent {
	public:
		virtual void Update(float deltatime) = 0;
		virtual COMPONENTID GetComponentID() = 0;
	};
}
#pragma once

namespace NamelessEngine::Component
{
	enum class COMPONENTID
	{
		DRAW	  = 0,
		TRANSFORM = 1,
		COLLISION = 2
	};

	class IComponent {
	public:
		virtual ~IComponent() = 0;
		virtual void Update(float deltatime) = 0;
		virtual void Draw() = 0;
		virtual COMPONENTID GetComponentID() = 0;
	};
}
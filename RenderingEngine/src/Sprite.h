#pragma once

class RenderingContext;

class Sprite {
public:
	Sprite() = default;
	~Sprite() = default;

private:
	void Draw(RenderingContext& renderContext);
};
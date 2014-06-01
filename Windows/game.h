#ifndef GAME_H
#define GAME_H

#include "gameObject.h"
#include "coreEngine.h"

class Game
{
public:
	Game() {}

	virtual void Init() {}
	void Input(float delta);
	void Update(float delta);
	void Render(RenderingEngine* renderingEngine);
	
	inline GameObject& GetRoot() { return m_root; }
	
	inline void SetEngine(CoreEngine* engine) { m_root.SetEngine(engine); }
protected:
	void AddToScene(GameObject* child) { m_root.AddChild(child); }
private:
	GameObject m_root;
};

#endif
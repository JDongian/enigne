#include <iostream>

class TemplateGame : public Game
{
public:
	TemplateGame() {}
	virtual void Init();
}

void TemplateGame::Init()
{

}

int main()
{
	TemplateGame game;

}
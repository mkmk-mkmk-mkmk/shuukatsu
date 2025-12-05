
#include "title.h"
#include "input.h"
#include "manager.h"
#include "game.h"


void Title::Init()
{

}

void Title::Uninit()
{

}

void Title::Update()
{
	if (Input::GetKeyPress(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
}

void Title::Draw()
{

}
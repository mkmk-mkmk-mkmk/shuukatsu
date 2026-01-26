
#include "manager.h"
#include "main.h"
#include "scene.h"
#include "player.h"
#include "box.h"
#include "breakableBox.h"
#include "untouchableBox.h"
#include "enemy.h"

#include "zoom.h"

void ZoomIn(float magnificatoin)
{
	//ƒvƒŒƒCƒ„[Šg‘å
	Manager::GetScene()->GetGameObject<Player>()->SetScale(Vector2(magnificatoin, magnificatoin));

	for (int i = 0; i < 10; i++)
	{

	}
}

void ZoomOut(float magnification)
{

}

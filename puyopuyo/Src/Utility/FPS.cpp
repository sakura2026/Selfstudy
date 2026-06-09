#include "FPS.h"
#include "DxLib.h"

void UpdateFPS(int& frameCount, int& fpstimer, int& currentFPS)
{
	// ƒtƒŒ[ƒ€”‚ð1‘‚â‚·
	frameCount++;
	if (GetNowCount() - fpstimer >= 1000)
	{
		currentFPS = frameCount;
		frameCount = 0;
		fpstimer = GetNowCount();
	}
}
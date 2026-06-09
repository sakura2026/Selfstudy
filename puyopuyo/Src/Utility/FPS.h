#pragma once

/// <summary>/// 
/// <param name="frameCount">毎フレーム加算していくフレームのカウンター</param> 
/// <param name="fpstimer">経過時間を測るタイマー</param>
/// <param name="currentFPS">現在のFPS値</param>
/// </summary>/// 
void UpdateFPS(int& frameCount, int& fpstimer, int& currentFPS);
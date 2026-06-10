#pragma once

#include "../SceneBase.h" 

class Title : public SceneBase
{
public:
    Title();
    ~Title();

    void Init() override;
    eSceneType Update() override;
    void Draw() const override;
    void Finalize() override;
    void TitleCursorUpdate(void);

private:
    // フォントハンドル
    int Font;
    int BigFont = -1;
    int SmallFont = -1;

    int Alpha;    // チカチカをなんかフェード的にしたい変数
    bool FadeIn;  // tureなら明るくなる、falseなら暗くなる 
};

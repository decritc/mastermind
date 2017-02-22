#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    CREATE_FUNC(HelloWorld);
    
    void update(float) override;
    
private:
    cocos2d::Sprite* yellow_ball;
    cocos2d::Sprite* red_ball;
    cocos2d::Sprite* green_ball;
    cocos2d::Sprite* blue_ball;
    cocos2d::Sprite* arrow;
    cocos2d::Node* currenthole;
    cocos2d::Node* currentpin;
    bool moved;
    int code [4];
    int guess [4];
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
};

#endif // __HELLOWORLD_SCENE_H__

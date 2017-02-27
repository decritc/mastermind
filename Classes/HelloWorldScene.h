#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
/*
 * Name        : HelloWorldScene.h
 * Author      : Derek Critchfield
 * Description : Mastermind game scene and logic header file
 */

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init(int guesses = 8, int colors = 4);
    CREATE_FUNC(HelloWorld);
    
    void update(float) override;
    
private:
    cocos2d::Sprite* yellow_ball;
    cocos2d::Sprite* red_ball;
    cocos2d::Sprite* green_ball;
    cocos2d::Sprite* blue_ball;
    cocos2d::Sprite* gray_ball;
    cocos2d::Sprite* purple_ball;
    cocos2d::Sprite* arrow;
    cocos2d::Node* currenthole;
    cocos2d::Node* currentpin;
    bool moved;
    int code [4];
    int guess [4];
    int guesses_choosen;
    int colors_choosen;
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::ui::CheckBox* colors_checkbox;
    cocos2d::ui::CheckBox* colors_checkbox2;
    cocos2d::ui::CheckBox* guesses_checkbox;
    cocos2d::ui::CheckBox* guesses_checkbox2;
    int factorial(int number);
    cocos2d::Label* difficulty_rating_label;
    void calculate_difficulty();
    
};

#endif // __HELLOWORLD_SCENE_H__

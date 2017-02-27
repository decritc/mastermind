/*
 * Name        : AppDelegate.cpp
 * Author      : Derek Critchfield
 * Description : Mastermind AppDelegate source file
 */

#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {

        glview = GLViewImpl::create("Mastermind Game");
		glview->setFrameSize(640, 640);
        director->setOpenGLView(glview);
    }

    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}

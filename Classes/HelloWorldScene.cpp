/*
 * Name        : HelloWorldScene.cpp
 * Author      : Derek Critchfield
 * Description : Mastermind game scene and logic source file
 */

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    
    return scene;
}

bool HelloWorld::init(int guesses_choice, int colors_choice)
{
	if (!Layer::init())
	{
		return false;
	}

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    int offset = 65;
    int row_width = 85;
    int holetag = 1;
    int pintag = 100;
    colors_choosen = colors_choice;
    guesses_choosen = guesses_choice;
    moved = true;
    
    for (int i = 0; i < colors_choosen; i++) {
        code[i] = RandomHelper::random_int(1, colors_choosen);
    }
    log("%i, %i, %i, %i", code[0], code[1], code[2], code[3]);
    
    auto bg = Sprite::create("bg.jpg");
    bg->setScale(visibleSize.width / bg->getContentSize().width, visibleSize.height / bg->getContentSize().height);
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    this->addChild(bg);

    for (int i = 1; i <= guesses_choosen; i++) {
        for(int j = 0; j < 4; j++) {
        auto hole = Sprite::create("hole.png");
            hole->setTag(holetag);
            hole->setPosition(row_width + (offset * j), offset * i);
            this->addChild(hole);
            if (i > 8) {
                hole->setVisible(false);
            }
            holetag++;
        }
    }
    currenthole = this->getChildByTag(1);
    
    for (int i = 1; i <= guesses_choosen; i++) {
        for(int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                int row_offset = -10;
                if (j % 2 == 0) {
                    row_offset = 10;
                }
                auto pin = Sprite::create("hole.png");
                pin->setTag(pintag);
                pin->setScale(0.25, 0.25);
                pin->setPosition(340 + (20 * k), (offset * i) + row_offset);
                this->addChild(pin);
                if (i > 8) {
                    pin->setVisible(false);
                }
                pintag++;
            }
        }
    }
    
    currentpin = this->getChildByTag(100);
    
    blue_ball = Sprite::create("blue_ball.png");
    blue_ball->setScale(0.2, 0.2);
    blue_ball->setPosition(Vec2(580, 120));
    this->addChild(blue_ball);
    
    green_ball = Sprite::create("green_ball.png");
    green_ball->setScale(0.2, 0.2);
    green_ball->setPosition(Vec2(580, 60));
    this->addChild(green_ball);
    
    red_ball = Sprite::create("red_ball.png");
    red_ball->setScale(0.2, 0.2);
    red_ball->setPosition(Vec2(500, 120));
    this->addChild(red_ball);
    
    yellow_ball = Sprite::create("yellow_ball.png");
    yellow_ball->setScale(0.2, 0.2);
    yellow_ball->setPosition(Vec2(500, 60));
    this->addChild(yellow_ball);
    
    if (colors_choosen == 6) {
        gray_ball = Sprite::create("gray_ball.png");
        gray_ball->setScale(0.2, 0.2);
        gray_ball->setPosition(Vec2(500, 180));
        this->addChild(gray_ball);
        
        purple_ball = Sprite::create("purple_ball.png");
        purple_ball->setScale(0.2, 0.2);
        purple_ball->setPosition(Vec2(580, 180));
        this->addChild(purple_ball);
    }
    Vec2 currentrow(30, offset);
    arrow = Sprite::create("arrow.png");
    arrow->setScale(0.07, 0.07);
    arrow->setPosition(currentrow);
    arrow->setTag(33);
    this->addChild(arrow);
    
    auto title = Label::createWithTTF("Mastermind", "fonts/Marker Felt.ttf", 60.0 );
    title->setTextColor(Color4B(218,165,32,255));
    title->enableOutline(Color4B(0,0,0,255),5);
    title->setPosition(Vec2(320, 600));
    this->addChild(title);
    
    auto colors = Label::createWithTTF("Colors", "fonts/arial.ttf", 14.0 );
    colors->setTextColor(Color4B(0,0,139,255));
    colors->setPosition(Vec2(455, 510));
    this->addChild(colors);
    
    colors_checkbox = ui::CheckBox::create("checkbox.png","checkmark.png");
    if (colors_choosen == 4) {
        colors_checkbox->setSelected(true);
    }
    colors_checkbox->setScale(0.05);
    colors_checkbox->setPosition(Vec2(460,490));
    colors_checkbox->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                colors_choosen = 4;
                calculate_difficulty();
                colors_checkbox2->setSelected(false);
                break;
            default:
                break;
        }
    });

    this->addChild(colors_checkbox);
    
    auto colors_four = Label::createWithTTF("4 Colors", "fonts/arial.ttf", 14.0 );
    colors_four->setTextColor(Color4B(0,0,139,255));
    colors_four->setPosition(Vec2(500, 490));
    this->addChild(colors_four);
    
    colors_checkbox2 = ui::CheckBox::create("checkbox.png","checkmark.png");
    if (colors_choosen == 6) {
        colors_checkbox2->setSelected(true);
    }
    colors_checkbox2->setScale(0.05);
    colors_checkbox2->setPosition(Vec2(460,470));
    colors_checkbox2->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                colors_choosen = 6;
                calculate_difficulty();
                colors_checkbox->setSelected(false);
                break;
            default:
                break;
        }
    });
    this->addChild(colors_checkbox2);
    
    auto colors_six = Label::createWithTTF("6 Colors", "fonts/arial.ttf", 14.0 );
    colors_six->setTextColor(Color4B(0,0,139,255));
    colors_six->setPosition(Vec2(500, 470));
    this->addChild(colors_six);
    
    auto guesses = Label::createWithTTF("Guesses", "fonts/arial.ttf", 14.0 );
    guesses->setTextColor(Color4B(0,0,139,255));
    guesses->setPosition(Vec2(465, 430));
    this->addChild(guesses);
    
    guesses_checkbox = ui::CheckBox::create("checkbox.png","checkmark.png");
    if (guesses_choosen == 8) {
        guesses_checkbox->setSelected(true);
    }
    guesses_checkbox->setScale(0.05);
    guesses_checkbox->setPosition(Vec2(460,410));
    guesses_checkbox->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                guesses_choosen = 8;
                calculate_difficulty();
                guesses_checkbox2->setSelected(false);
                break;
            default:
                break;
        }
    });

    this->addChild(guesses_checkbox);
    
    auto guesses_eight = Label::createWithTTF("8 Guesses", "fonts/arial.ttf", 14.0 );
    guesses_eight->setTextColor(Color4B(0,0,139,255));
    guesses_eight->setPosition(Vec2(510, 410));
    this->addChild(guesses_eight);
    
    guesses_checkbox2 = ui::CheckBox::create("checkbox.png","checkmark.png");
    if (guesses_choosen == 12) {
        guesses_checkbox2->setSelected(true);
    }
    guesses_checkbox2->setScale(0.05);
    guesses_checkbox2->setPosition(Vec2(460,390));
    guesses_checkbox2->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                guesses_choosen = 12;
                calculate_difficulty();
                guesses_checkbox->setSelected(false);
                break;
            default:
                break;
        }
    });

    this->addChild(guesses_checkbox2);
    
    auto guesses_twelve = Label::createWithTTF("12 Guesses", "fonts/arial.ttf", 14.0 );
    guesses_twelve->setTextColor(Color4B(0,0,139,255));
    guesses_twelve->setPosition(Vec2(510, 390));
    this->addChild(guesses_twelve);
    
    difficulty_rating_label = Label::createWithTTF("Difficulty Rating: ", "fonts/arial.ttf", 14.0 );
    difficulty_rating_label->setTextColor(Color4B(0,0,139,255));
    difficulty_rating_label->setPosition(Vec2(490, 340));
    calculate_difficulty();
    this->addChild(difficulty_rating_label);
    
    auto notice = Label::createWithTTF("* Changes take effect on new game only!", "fonts/arial.ttf", 14.0 );
    notice->setTextColor(Color4B(0,0,139,255));
    notice->setPosition(Vec2(500, 530));
    this->addChild(notice);
    
    auto button = ui::Button::create("new-game.png");
    button->setScale(0.7, 0.7);
    button->ui::Button::setPosition(Vec2(540, 280));
    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                init(guesses_choosen, colors_choosen);
                break;
            default:
                break;
        }
    });
    
    this->addChild(button);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event){
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        if (rect.containsPoint(locationInNode))
        {
            log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
            target->setOpacity(180);
            return true;
        }
        return false;
    };
    listener->onTouchMoved = [](Touch* touch, Event* event){
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        target->setPosition(target->getPosition() + touch->getDelta());
    };
    listener->onTouchEnded = [=](Touch* touch, Event* event) mutable {
        auto target = static_cast<Sprite*>(event->getCurrentTarget());
        log("sprite onTouchesEnded.. ");
        target->setOpacity(255);
        if (target == yellow_ball)
        {
            yellow_ball = Sprite::create("yellow_ball.png");
            yellow_ball->setScale(0.2, 0.2);
            yellow_ball->setPosition(Vec2(500, 60));
            this->addChild(yellow_ball);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), yellow_ball);
            if ((target->getPosition().x >= currenthole->getPosition().x + 20 || target->getPosition().x <= currenthole->getPosition().x - 20) ||
                (target->getPosition().y >= currenthole->getPosition().y + 20 || target->getPosition().y <= currenthole->getPosition().y - 20)) {
                    target->removeFromParentAndCleanup(true);
            } else {
                target->setPosition(currenthole->getPosition());
                _eventDispatcher->removeEventListenersForTarget(target);
                guess[currenthole->getTag() % 4] = 1;
                if (currenthole->getTag() < 33) {
                    currenthole = this->getChildByTag(currenthole->getTag() + 1);
                }
                moved = false;
            }
        }
        else if(target == red_ball)
        {
            red_ball = Sprite::create("red_ball.png");
            red_ball->setScale(0.2, 0.2);
            red_ball->setPosition(Vec2(500, 120));
            this->addChild(red_ball);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), red_ball);
            if ((target->getPosition().x >= currenthole->getPosition().x + 20 || target->getPosition().x <= currenthole->getPosition().x - 20) ||
                (target->getPosition().y >= currenthole->getPosition().y + 20 || target->getPosition().y <= currenthole->getPosition().y - 20)) {
                target->removeFromParentAndCleanup(true);
            } else {
                target->setPosition(currenthole->getPosition());
                _eventDispatcher->removeEventListenersForTarget(target);
                guess[currenthole->getTag() % 4] = 2;
                if (currenthole->getTag() < 33) {
                    currenthole = this->getChildByTag(currenthole->getTag() + 1);
                }
                moved = false;
            }
        }
        else if(target == blue_ball)
        {
            blue_ball = Sprite::create("blue_ball.png");
            blue_ball->setScale(0.2, 0.2);
            blue_ball->setPosition(Vec2(580, 120));
            this->addChild(blue_ball);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), blue_ball);
            if ((target->getPosition().x >= currenthole->getPosition().x + 20 || target->getPosition().x <= currenthole->getPosition().x - 20) ||
                (target->getPosition().y >= currenthole->getPosition().y + 20 || target->getPosition().y <= currenthole->getPosition().y - 20)) {
                target->removeFromParentAndCleanup(true);
            } else {
                target->setPosition(currenthole->getPosition());
                _eventDispatcher->removeEventListenersForTarget(target);
                guess[currenthole->getTag() % 4] = 3;
                if (currenthole->getTag() < 33) {
                    currenthole = this->getChildByTag(currenthole->getTag() + 1);
                }
                moved = false;

            }
        }
        else if(target == green_ball)
        {
            green_ball = Sprite::create("green_ball.png");
            green_ball->setScale(0.2, 0.2);
            green_ball->setPosition(Vec2(580, 60));
            this->addChild(green_ball);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), green_ball);
            if ((target->getPosition().x >= currenthole->getPosition().x + 20 || target->getPosition().x <= currenthole->getPosition().x - 20) ||
                (target->getPosition().y >= currenthole->getPosition().y + 20 || target->getPosition().y <= currenthole->getPosition().y - 20)) {
                target->removeFromParentAndCleanup(true);
            } else {
                target->setPosition(currenthole->getPosition());
                _eventDispatcher->removeEventListenersForTarget(target);
                guess[currenthole->getTag() % 4] = 4;
                if (currenthole->getTag() < 33) {
                    currenthole = this->getChildByTag(currenthole->getTag() + 1);
                }
                moved = false;
            }
        }
        else if(target == gray_ball)
        {
            gray_ball = Sprite::create("gray_ball.png");
            gray_ball->setScale(0.2, 0.2);
            gray_ball->setPosition(Vec2(500, 180));
            this->addChild(gray_ball);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), gray_ball);
            if ((target->getPosition().x >= currenthole->getPosition().x + 20 || target->getPosition().x <= currenthole->getPosition().x - 20) ||
                (target->getPosition().y >= currenthole->getPosition().y + 20 || target->getPosition().y <= currenthole->getPosition().y - 20)) {
                target->removeFromParentAndCleanup(true);
            } else {
                target->setPosition(currenthole->getPosition());
                _eventDispatcher->removeEventListenersForTarget(target);
                guess[currenthole->getTag() % 4] = 5;
                if (currenthole->getTag() < 33) {
                    currenthole = this->getChildByTag(currenthole->getTag() + 1);
                }
                moved = false;
            }
        }
        else if(target == purple_ball)
        {
            purple_ball = Sprite::create("purple_ball.png");
            purple_ball->setScale(0.2, 0.2);
            purple_ball->setPosition(Vec2(580, 180));
            this->addChild(purple_ball);
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), purple_ball);
            if ((target->getPosition().x >= currenthole->getPosition().x + 20 || target->getPosition().x <= currenthole->getPosition().x - 20) ||
                (target->getPosition().y >= currenthole->getPosition().y + 20 || target->getPosition().y <= currenthole->getPosition().y - 20)) {
                target->removeFromParentAndCleanup(true);
            } else {
                target->setPosition(currenthole->getPosition());
                _eventDispatcher->removeEventListenersForTarget(target);
                guess[currenthole->getTag() % 4] = 6;
                if (currenthole->getTag() < 33) {
                    currenthole = this->getChildByTag(currenthole->getTag() + 1);
                }
                moved = false;
            }
        }

    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, currentpin);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), yellow_ball);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), red_ball);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), blue_ball);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), green_ball);
    
    if (colors_choosen == 6) {
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), gray_ball);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), purple_ball);
    }
    
    this->scheduleUpdate();
	return true;
}
void HelloWorld::update(float delta){
    
    if ((currenthole->getTag() - 1) % 4 == 0 && moved == false) {
        bool win_state = true;
        
        log("%i, %i, %i, %i", code[0], code[1], code[2], code[3]);
        log("%i, %i, %i, %i", guess[0], guess[1], guess[2], guess[3]);
        for (int i = 0; i < 4; i++) {
            if (guess[i] != code[i]) {
                win_state = false;
            }
        }
        if (win_state) {
            _eventDispatcher->removeAllEventListeners();
            auto game_won = ui::Button::create("game_win.jpg");
            game_won->setPosition(Vec2(320,320));
            game_won->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
                switch (type)
                {
                    case ui::Widget::TouchEventType::BEGAN:
                        break;
                    case ui::Widget::TouchEventType::ENDED:
                        Director::getInstance()->replaceScene(createScene());
                        break;
                    default:
                        break;
                }
            });
            this->addChild(game_won);
            log("GAME WON");
        } else {
            int increment_count = 0;
            int correct_placement = 0;
            int correct_color = 0;
            for (int i = 0; i < 4; i++) {
                if (guess[i] == code[i]) {
                    correct_placement++;
                } else if (guess[i] == code[0] || guess[i] == code[1] || guess[i] == code[2] || guess[i] == code[3]) {
                    correct_color++;
                }
            }
            for (int i = 0; i < correct_placement; i++) {
                auto red_pin = Sprite::create("red_ball.png");
                red_pin->setScale(0.05, 0.05);
                red_pin->setPosition(currentpin->getPosition());
                this->addChild(red_pin);
                currentpin = this->getChildByTag(currentpin->getTag() + 1);
                increment_count++;
            }
            for (int i = 0; i < correct_color; i++) {
                auto white_pin = Sprite::create("white_ball.png");
                white_pin->setScale(0.05, 0.05);
                white_pin->setPosition(currentpin->getPosition());
                this->addChild(white_pin);
                currentpin = this->getChildByTag(currentpin->getTag() + 1);
                increment_count++;
            }
            if (increment_count < 4){
                currentpin = this->getChildByTag(currentpin->getTag() + (4 - increment_count));
            }
            if (currenthole->getTag() != 33 && !win_state) {
                auto moveAction = MoveBy::create(2, Vec2(0, 65));
                arrow->runAction(moveAction);
            } else {
                _eventDispatcher->removeAllEventListeners();
                auto game_lost = ui::Button::create("game_lost.jpg");
                game_lost->setPosition(Vec2(320,320));
                game_lost->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
                    switch (type)
                    {
                        case ui::Widget::TouchEventType::BEGAN:
                            break;
                        case ui::Widget::TouchEventType::ENDED:
                            Director::getInstance()->replaceScene(createScene());
                            break;
                        default:
                            break;
                    }
                });
                this->addChild(game_lost);
                log("GAME LOST");
            }
           
        }
        moved = true;
    }
}

int HelloWorld::factorial(int number) {
    if (number <= 1) {
        return 1;
    }
    return number * factorial(number -1);
}
void HelloWorld::calculate_difficulty() {
    int difficulty = pow(4, colors_choosen)/(factorial(guesses_choosen)/(factorial(guesses_choosen - colors_choosen) * factorial(colors_choosen)));
    std::ostringstream convert;
    convert << difficulty;
    difficulty_rating_label->setString("Difficulty Rating: " + convert.str());
}

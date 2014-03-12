/**
 * PictoConnection
 *
 * @file PictogramScene.cpp
 * @brief Scene which shows one pictogram
 *
 * @author Javier Alvargonzález <javier.alvargonzalez@itiox.com>
 *
 * @copyright Original work Copyright (C) 2014 ITIOX <itiox@itiox.com>
 *
 * @section LICENSE
 *
 * This file is part of PictoConnection.
 *
 * PictoConnection is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * PictoConnection is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 **/

#include "PictogramScene.h"

#include "AppDelegate.h"
#include "NavigationBar.h"
#include "PictoDatabase.h"
#include "PictogramGridScene.h"
#include "PictogramNode.h"
#include "PictoDefs.h"

USING_NS_CC;

CCScene* Pictogram::scene(CCArray* pictograms, int enter_animation)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Pictogram *layer = new Pictogram();
    
    if (layer && layer->init(pictograms, enter_animation)) {
        layer->autorelease();
        
        // add layer as a child to scene
        scene->addChild(layer);
        
        return scene;
    }
    CC_SAFE_DELETE(layer);
    
    // return the scene
    return NULL;
}

Pictogram::Pictogram() :
back_button_(NULL),
back_button_overlay_(NULL),
pictogram_node_(NULL),
pictogram_node_left_(NULL),
pictogram_node_right_(NULL),
pictograms_(NULL) {
    
}

Pictogram::~Pictogram() {
    
    CC_SAFE_RELEASE_NULL(pictograms_);
    removeAllChildrenWithCleanup(true);
    removeFromParentAndCleanup(true);
    CCTextureCache::purgeSharedTextureCache();
}

bool Pictogram::init(CCArray* pictograms, int enter_animation) {
    
    ccColor4B color = picto::conversions::int2color4B(CCUserDefault::sharedUserDefault()->getIntegerForKey("color_theme", DEFAULT_COLOR_THEME));
    
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
    pictograms_ = pictograms;
    CC_SAFE_RETAIN(pictograms_);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    setKeypadEnabled(true);
#endif
    
    // Get pictograms childs from database
    PictogramObject* pictogram = picto::database::pictogram(((CCString*)pictograms_->lastObject())->getCString());
    
    // Compute some UI parameters
    CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCSize bottom_bar_size(visible_size.width, 0.2*MIN(visible_size.width, visible_size.height));
    
    if (pictograms_->count() > 1)
        initBottomBar(bottom_bar_size, origin);
    
    CCSize top_bar_size(visible_size.width, 0.1*MIN(visible_size.width, visible_size.height));
    CCPoint top_bar_origin = ccp(origin.x, origin.y + visible_size.height - top_bar_size.height);
    initTopBar(top_bar_size, top_bar_origin);
    
    CCSize grid_size(visible_size.width, visible_size.height - bottom_bar_size.height - top_bar_size.height);
    CCPoint grid_origin = ccp(origin.x, bottom_bar_size.height);
    initContent(grid_size, grid_origin, pictogram);
    
    if (enter_animation < 0) {
        CCPoint position = pictogram_node_->getPosition();
        pictogram_node_->setPosition(ccp(position.x + 0.5*visible_size.width + 0.5*pictogram_node_->getContentSize().width, position.y));
        pictogram_node_->runAction(CCMoveTo::create(0.1, position));
    } else if (enter_animation > 0) {
        CCPoint position = pictogram_node_->getPosition();
        pictogram_node_->setPosition(ccp(position.x - 0.5*visible_size.width - 0.5*pictogram_node_->getContentSize().width, position.y));
        pictogram_node_->runAction(CCMoveTo::create(0.1, position));
    }
    
    touch_frame_ = CCRectMake(grid_origin.x, grid_origin.y, grid_size.width, grid_size.height);
    
    setTouchEnabled(true);
    
    return true;
}

void Pictogram::initBottomBar(const CCSize& size, const CCPoint& origin) {
    
    CCSprite* normal = CCSprite::create("back_button_background_normal.png");
    CCSprite* normal_overlay = CCSprite::create("back_button_normal.png");
    normal_overlay->setAnchorPoint(ccp(0.5, 0.5));
    normal_overlay->setPosition(ccp(0.5*normal->getContentSize().width,
                                    0.5*normal->getContentSize().height));
    normal->addChild(normal_overlay);
    
    CCSprite* selected = CCSprite::create("back_button_background_pressed.png");
    selected->setPosition(ccp(0.5*(normal->getContentSize().width - selected->getContentSize().width),
                              0.5*(normal->getContentSize().height - selected->getContentSize().height)));
    selected->setOpacity(160);
    CCSprite* selected_overlay = CCSprite::create("back_button_pressed.png");
    selected_overlay->setAnchorPoint(ccp(0.5, 0.5));
    selected_overlay->setPosition(ccp(0.5*selected->getContentSize().width,
                                      0.5*selected->getContentSize().height));
    selected->addChild(selected_overlay);
    
    back_button_ = CCMenuItemSprite::create(normal,
                                                      selected,
                                                      this,
                                                      menu_selector(Pictogram::backPressed));
    back_button_->setAnchorPoint(ccp(0.5, 0.5));
    back_button_->setContentSize(normal->getContentSize());
    back_button_->setPosition(ccp(0.5*size.width, 0.5*size.height));
    
    CCMenu* menu = CCMenu::create(back_button_, NULL);
    menu->setPosition(origin);
    addChild(menu);
}

void Pictogram::initTopBar(const CCSize& size, const CCPoint& origin) {
    
    NavigationBar* navigation_bar = NavigationBar::create(size, pictograms_);
    navigation_bar->setPosition(origin);
    
    addChild(navigation_bar);
}

void Pictogram::initContent(const CCSize& size, const CCPoint& origin, PictogramObject* pictogram) {
    
    // Compute node visible size
    float margin = 0.02*MAX(size.width, size.height);
    float ratio = 1;
    
    CCPoint center = ccp(origin.x + 0.5*size.width, origin.y + 0.5*size.height);
    
    CCSize node_size(size.width - 2*margin, size.height - 2*margin);
    
    node_size.height = MIN(node_size.height, node_size.width * ratio);
    node_size.width = node_size.height / ratio;
    
    pictogram_node_ = PictogramNode::create(pictogram, node_size);
    pictogram_node_->ignoreTouches(true);
    pictogram_node_->setPosition(center.x, center.y);
    addChild(pictogram_node_, 1);
}

void Pictogram::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void Pictogram::onEnter() {
    CCLayerColor::onEnter();
    
    setColor(picto::resources::backgroundColor());
    setTouchEnabled(true);
    setTouchPriority(0);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    
    if (back_button_) {
        back_button_->setColor(picto::resources::navigationBarBackgroundColor());
    }
    
    if (back_button_overlay_) {
        back_button_overlay_->setColor(picto::resources::navigationBarTextColor());
    }
}

void Pictogram::onExit() {
    CCLayerColor::onExit();
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

// default implements are used to call script callback if exist
bool Pictogram::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    
    CCPoint location = touch->getLocation();
    
    if (touch_frame_.containsPoint(location)) {
        touch_location_ = location;
        return true;
    }
    
    return false;
}

void Pictogram::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    CCPoint touch_location = touch->getLocation();
    
    float dx = touch_location_.x - touch_location.x;
    float dy = touch_location_.y - touch_location.y;
}

void Pictogram::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    CCPoint touch_location = touch->getLocation();
    
    float dx = touch_location_.x - touch_location.x;
    float dy = touch_location_.y - touch_location.y;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    if (abs(dy) < 0.4*MIN(visibleSize.width, visibleSize.height)) {
        if (abs(dx) > 0.2*MIN(visibleSize.width, visibleSize.height)) {
            if (dx > 0) {
                CCPoint position(ccp(-pictogram_node_->getContentSize().width,
                                     pictogram_node_->getPositionY()));
                CCCallFunc *finish_callback_ = CCCallFunc::create(this, callfunc_selector(Pictogram::onSlideRightwardsAnimationEnded));
                pictogram_node_->runAction(CCSequence::create(CCMoveTo::create(0.1, position),
                                                              finish_callback_));
            } else {
                CCPoint position(ccp(2*pictogram_node_->getPositionX() + pictogram_node_->getContentSize().width,
                                     pictogram_node_->getPositionY()));
                CCCallFunc *finish_callback_ = CCCallFunc::create(this, callfunc_selector(Pictogram::onSlideLeftwardsAnimationEnded));
                pictogram_node_->runAction(CCSequence::create(CCMoveTo::create(0.1, position),
                                                                 finish_callback_));
            }
        }
    }
}

void Pictogram::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
    
}

void Pictogram::backPressed(CCObject* sender) {
    CCArray* pictograms = CCArray::createWithArray(pictograms_);
    pictograms->removeLastObject();
    CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
}

void Pictogram::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    exit(0);
}

void Pictogram::onSlideLeftwardsAnimationEnded() {
    
    // Get pictograms childs from database
    CCString* pictogram = CCString::create(dynamic_cast<CCString*>(pictograms_->lastObject())->getCString());
    pictograms_->removeLastObject();
    CCString* parent_pictogram = dynamic_cast<CCString*>(pictograms_->lastObject());
    
    CCArray* childs = picto::database::childs(parent_pictogram->getCString());
    if (childs->count() <= 1)
        return;
    
    size_t i=0;
    CCObject *it;
    CCARRAY_FOREACH(childs, it) {
        PictogramObject *pictogram_object = dynamic_cast<PictogramObject*>(it);
        if (strcmp(pictogram->getCString(), pictogram_object->getIdentifier()->getCString()) == 0)
            break;
        i++;
    }
    
    i = (i-1 + childs->count()) % childs->count();
    PictogramObject *pictogram_object = dynamic_cast<PictogramObject*>(childs->objectAtIndex(i));
    pictograms_->addObject(CCString::create(pictogram_object->getIdentifier()->getCString()));
    CCDirector::sharedDirector()->replaceScene(Pictogram::scene(pictograms_, 1));
}

void Pictogram::onSlideRightwardsAnimationEnded() {
    
    // Get pictograms childs from database
    CCString* pictogram = CCString::create(dynamic_cast<CCString*>(pictograms_->lastObject())->getCString());
    pictograms_->removeLastObject();
    CCString* parent_pictogram = dynamic_cast<CCString*>(pictograms_->lastObject());
    
    CCArray* childs = picto::database::childs(parent_pictogram->getCString());
    if (childs->count() <= 1)
        return;
    
    size_t i=0;
    CCObject *it;
    CCARRAY_FOREACH(childs, it) {
        PictogramObject *pictogram_object = dynamic_cast<PictogramObject*>(it);
        if (strcmp(pictogram->getCString(), pictogram_object->getIdentifier()->getCString()) == 0)
            break;
        i++;
    }
    
    i = (i+1) % childs->count();
    PictogramObject *pictogram_object = dynamic_cast<PictogramObject*>(childs->objectAtIndex(i));
    pictograms_->addObject(CCString::create(pictogram_object->getIdentifier()->getCString()));
    CCDirector::sharedDirector()->replaceScene(Pictogram::scene(pictograms_, -1));
}

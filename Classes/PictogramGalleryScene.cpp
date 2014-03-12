/**
 * PictoConnection
 *
 * @file PictogramGalleryScene.cpp
 * @brief Horizontal scroll list representation of a set of pictograms
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

#include "PictogramGalleryScene.h"

#include "AppDelegate.h"
#include "NavigationBar.h"
#include "PictoDatabase.h"
#include "PictogramGridScene.h"
#include "PictogramNode.h"
#include "PictogramScene.h"
#include "PictoDefs.h"

USING_NS_CC;

CCScene* PictogramGallery::scene(CCArray* pictograms)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    scene->setUserObject(CCNode::create());
    
    // 'layer' is an autorelease object
    PictogramGallery *layer = new PictogramGallery();
    
    if (layer && layer->init(pictograms)) {
        layer->autorelease();
        
        // add layer as a child to scene
        scene->addChild(layer);
        
        return scene;
    }
    CC_SAFE_DELETE(layer);
    
    // return the scene
    return NULL;
}

PictogramGallery::PictogramGallery() :

back_button_(NULL),
pictograms_(NULL),
scene_mutex_(false),
scroll_view_(NULL) {
    
}

PictogramGallery::~PictogramGallery() {
    CCLOG("PictogramGallery::~PictogramGallery()");
    
    CC_SAFE_RELEASE_NULL(pictograms_);
    removeAllChildrenWithCleanup(true);
    removeFromParentAndCleanup(true);
    CCTextureCache::purgeSharedTextureCache();
}

bool PictogramGallery::init(CCArray* pictograms) {
    
    ccColor4B color = picto::conversions::int2color4B(CCUserDefault::sharedUserDefault()->getIntegerForKey("color_theme", DEFAULT_COLOR_THEME));
    
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    setKeypadEnabled(true);
#endif
    
    pictograms_ = pictograms;
    CC_SAFE_RETAIN(pictograms_);
    
    // Compute some UI parameters
    CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visible_origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Add bottom bar
    CCSize bottom_bar_size(visible_size.width, 0.2*MIN(visible_size.width, visible_size.height));
    if (pictograms_->count() > 1) {
        initBottomBar(bottom_bar_size, visible_origin);
    } else {
        bottom_bar_size.height = 0;
    }
    
    // Add top bar
    CCSize top_bar_size(visible_size.width, 0.1*MIN(visible_size.width, visible_size.height));
    CCPoint top_bar_origin = ccp(visible_origin.x, visible_origin.y + visible_size.height - top_bar_size.height);
    initTopBar(top_bar_size, top_bar_origin);
    
    // Add gallery content
    CCSize content_size(visible_size.width, visible_size.height - bottom_bar_size.height - top_bar_size.height);
    CCPoint content_origin = ccp(visible_origin.x, bottom_bar_size.height);
    initContent(content_size, content_origin);
    
    setTouchEnabled(true);
    
    return true;
}

void PictogramGallery::initBottomBar(const cocos2d::CCSize& size,
                                     const cocos2d::CCPoint& origin) {
    
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
                                            menu_selector(PictogramGallery::backPressed));
    back_button_->setAnchorPoint(ccp(0.5, 0.5));
    back_button_->setContentSize(normal->getContentSize());
    back_button_->setPosition(ccp(0.5*size.width, 0.5*size.height));
    
    CCMenu* menu = CCMenu::create(back_button_, NULL);
    menu->setPosition(origin);
    addChild(menu);
}

void PictogramGallery::initTopBar(const cocos2d::CCSize& size,
                                  const cocos2d::CCPoint& origin) {
    
    NavigationBar* navigation_bar = NavigationBar::create(size, pictograms_);
    navigation_bar->setPosition(origin);
    addChild(navigation_bar);
}

void PictogramGallery::initContent(const cocos2d::CCSize& size,
                                   const cocos2d::CCPoint& origin) {
    scroll_view_ = CCLayer::create();
    scroll_view_->setPosition(origin);
    addChild(scroll_view_);
    
    CCString* parent_pictogram = dynamic_cast<CCString*>(pictograms_->objectAtIndex(pictograms_->count() - 2));
    
    // Get siblings
    CCArray* siblings = picto::database::childs(parent_pictogram->getCString());
    
    PictogramObject* left_sibling = NULL;
    PictogramObject* right_sibling = NULL;
    PictogramObject* pictogram = NULL;
    
    for (int i=0; i < siblings->count(); i++) {
        PictogramObject* sibling = dynamic_cast<PictogramObject*>(siblings->objectAtIndex(i));
        if (strcmp(sibling->getIdentifier()->getCString(),
                   ((CCString*)pictograms_->lastObject())->getCString()) == 0) {
            pictogram = sibling;
            right_sibling = dynamic_cast<PictogramObject*>(siblings->objectAtIndex((i + 1) % siblings->count()));
            left_sibling = dynamic_cast<PictogramObject*>(siblings->objectAtIndex((i - 1 + siblings->count()) % siblings->count()));
            break;
        }
    }
    
    // Compute node visible size
    float margin = 0.02*MAX(size.width, size.height);
    float ratio = 1;
    
    CCPoint center = ccp(0.5*size.width, 0.5*size.height);
    
    CCSize node_size(size.width - 2*margin, size.height - 2*margin);
    
    node_size.height = MIN(node_size.height, node_size.width * ratio);
    node_size.width = node_size.height / ratio;
    
    // Central node
    PictogramNode* pictogram_node = PictogramNode::create(pictogram, node_size);
    pictogram_node->ignoreTouches(true);
    pictogram_node->setPosition(center.x, center.y);
    scroll_view_->addChild(pictogram_node);
    
    // Left sibling
    pictogram_node = PictogramNode::create(left_sibling, node_size);
    pictogram_node->ignoreTouches(true);
    pictogram_node->setPosition(center.x - size.width, center.y);
    scroll_view_->addChild(pictogram_node);
    
    // Right sibling
    pictogram_node = PictogramNode::create(right_sibling, node_size);
    pictogram_node->ignoreTouches(true);
    pictogram_node->setPosition(center.x + size.width, center.y);
    scroll_view_->addChild(pictogram_node);
}

void PictogramGallery::keyBackClicked() {
    CCArray* pictograms = CCArray::createWithArray(pictograms_);
    pictograms->removeLastObject();
    CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
}

void PictogramGallery::onEnter() {
    CCLayerColor::onEnter();
    
    setTouchEnabled(true);
    setTouchPriority(2);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    
    setColor(picto::resources::backgroundColor());
    
    if (back_button_) {
        back_button_->setColor(picto::resources::navigationBarBackgroundColor());
    }
}

void PictogramGallery::onExit() {
    CCLayerColor::onExit();
    
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    scene_mutex_ = false;
}

void PictogramGallery::backPressed(CCObject* sender) {
    CCArray* pictograms = CCArray::createWithArray(pictograms_);
    pictograms->removeLastObject();
    CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
}

bool PictogramGallery::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    
    CCPoint touch_location = touch->getLocation();
    
    // TODO Check or not to check touch location
    touch_location_ = touch_location;
    
    return true;
}

void PictogramGallery::ccTouchMoved(CCTouch *touch, CCEvent *event) {
    
    CCPoint touch_location = touch->getLocation();
    
    float dx = touch_location_.x - touch_location.x;
    
    scroll_view_->setPosition(scroll_view_->getPositionX() - dx, scroll_view_->getPositionY());
    
    touch_location_ = touch_location;
}

void PictogramGallery::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    
    // Compute some UI parameters
    CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visible_origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    float dx = scroll_view_->getPositionX() - visible_origin.x;
    if (fabsf(dx) < 0.5*visible_size.width) {
        scroll_view_->setPosition(ccp(0, scroll_view_->getPositionY()));
    } else {
        CCString* left_sibling = NULL;
        CCString* right_sibling = NULL;
        
        CCString* parent_pictogram = dynamic_cast<CCString*>(pictograms_->objectAtIndex(pictograms_->count() - 2));
        
        // Get siblings
        CCArray* siblings = picto::database::childs(parent_pictogram->getCString());
        for (int i=0; i < siblings->count(); i++) {
            PictogramObject* sibling = dynamic_cast<PictogramObject*>(siblings->objectAtIndex(i));
            if (strcmp(sibling->getIdentifier()->getCString(),
                       ((CCString*)pictograms_->lastObject())->getCString()) == 0) {
                right_sibling = dynamic_cast<PictogramObject*>(siblings->objectAtIndex((i + 1) % siblings->count()))->getIdentifier();
                left_sibling = dynamic_cast<PictogramObject*>(siblings->objectAtIndex((i - 1 + siblings->count()) % siblings->count()))->getIdentifier();
                break;
            }
        }
        
        pictograms_->removeLastObject();
        pictograms_->addObject((dx < 0)? right_sibling : left_sibling);
        scroll_view_->setPosition(ccp((dx < 0)? visible_size.width : -visible_size.width, getPositionY()));
        CCDirector::sharedDirector()->replaceScene(PictogramGallery::scene(pictograms_));
    }
}

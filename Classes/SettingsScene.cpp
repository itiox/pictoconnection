/**
 * PictoConnection
 *
 * @file SettingsScene.cpp
 * @brief Scene for managing app settings
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

#include "SettingsScene.h"

#include "NavigationBar.h"
#include "PickThemeScene.h"
#include "PictoDefs.h"

USING_NS_CC;

CCScene* Settings::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    
    // 'layer' is an autorelease object
    Settings *layer = Settings::create();
    
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

Settings::Settings() :
back_button_(NULL),
labels_(NULL) {
    
}

Settings::~Settings() {
    CC_SAFE_RELEASE_NULL(labels_);
}

bool Settings::init() {
    
    ccColor4B color = picto::conversions::int2color4B(CCUserDefault::sharedUserDefault()->getIntegerForKey("color_theme", DEFAULT_COLOR_THEME));
    
    ////////////////////
    // super init first
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    setKeypadEnabled(true);
#endif
    
    labels_ = CCArray::create();
    CC_SAFE_RETAIN(labels_);
    
    // Compute some UI parameters
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCSize bottomBarSize = visibleSize;
    bottomBarSize.height = 0.2*MIN(visibleSize.width, visibleSize.height);
    
    initBottomBar(bottomBarSize, origin);
    
    CCSize topBarSize = visibleSize;
    topBarSize.height = 0.1*MIN(visibleSize.width, visibleSize.height);
    CCPoint topBarOrigin = ccp(origin.x, origin.y + visibleSize.height - topBarSize.height);
    initTopBar(topBarSize, topBarOrigin);
    
    CCSize menuSize = visibleSize;
    menuSize.height = visibleSize.height - bottomBarSize.height - topBarSize.height;
    CCPoint menuOrigin = ccp(origin.x, bottomBarSize.height);
    initMenu(menuSize, menuOrigin);
    
    return true;
}

void Settings::initBottomBar(const CCSize& size, const CCPoint& origin) {
    
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
                                            CCDirector::sharedDirector(),
                                            menu_selector(CCDirector::popScene));
    back_button_->setAnchorPoint(ccp(0.5, 0.5));
    back_button_->setContentSize(normal->getContentSize());
    back_button_->setPosition(ccp(0.5*size.width, 0.5*size.height));
    
    CCMenu* menu = CCMenu::create(back_button_, NULL);
    menu->setPosition(origin);
    addChild(menu);
}

void Settings::initTopBar(const CCSize& size, const CCPoint& origin) {
    
    NavigationBar* navigation_bar = NavigationBar::create(size, "Ajustes");
    navigation_bar->setPosition(origin);
    addChild(navigation_bar);
}

void Settings::initMenu(const CCSize& size, const CCPoint& origin) {
    
    float scale = 1.0f;
    
    float font_size = MIN(size.width/8, size.height/8);
    CCLabelTTF* pick_theme_label = picto::cocos2d_utils::createLabel("Seleccionar tema", font_size);
    scale = MIN(scale, 0.7*size.width/pick_theme_label->getContentSize().width);
    labels_->addObject(pick_theme_label);
    CCMenuItemLabel *pick_theme = CCMenuItemLabel::create(pick_theme_label,
                                                         this,
                                                         menu_selector(Settings::pickTheme));
    pick_theme->ignoreAnchorPointForPosition(false);
    pick_theme->setAnchorPoint(ccp(0.5, 0.5));
    pick_theme->setPosition(ccp(0.5*size.width, 0.5*size.height));
    
    CCMenu* menu = CCMenu::create(pick_theme, NULL); // TODO menuCapitals
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(origin.x, origin.y));
    
    addChild(menu);
}

void Settings::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void Settings::onEnter() {
    CCLayerColor::onEnter();
    
    // Adjust colors to theme
    
    setColor(picto::resources::backgroundColor());
    
    CCObject* it;
    CCARRAY_FOREACH(labels_, it) {
        CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(it);
        label->setColor(picto::resources::menuTextColor());
    }
    
    if (back_button_) {
        back_button_->setColor(picto::resources::navigationBarBackgroundColor());
    }
}

void Settings::pickTheme(cocos2d::CCObject *sender) {
    CCDirector::sharedDirector()->pushScene(PickTheme::scene());
}
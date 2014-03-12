/**
 * PictoConnection
 *
 * @file NavigationBar.cpp
 * @brief Top bar shown in various scenes
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

#include "NavigationBar.h"

#include "AppDelegate.h"
#include "CustomMenuItemLabel.h"
#include "PictoDatabase.h"
#include "PictogramGridScene.h"
#include "PictogramObject.h"
#include "SettingsScene.h"
#include "PictoDefs.h"

USING_NS_CC;

NavigationBar* NavigationBar::create(const CCSize& size, const char* title)
{
    NavigationBar *bar = new NavigationBar();
    
    if (bar && bar->init(size, title)) {
        bar->autorelease();
        return bar;
    }
    CC_SAFE_DELETE(bar);
    
    // return the scene
    return NULL;
}

NavigationBar* NavigationBar::create(const CCSize& size, CCArray* pictograms)
{
    NavigationBar *bar = new NavigationBar();
    
    if (bar && bar->init(size, pictograms)) {
        bar->autorelease();
        return bar;
    }
    CC_SAFE_DELETE(bar);
    
    // return the scene
    return NULL;
}

NavigationBar::NavigationBar() :
labels_(NULL),
pictograms_(NULL) {}

NavigationBar::~NavigationBar() {
    
    CC_SAFE_RELEASE_NULL(labels_);
    CC_SAFE_RELEASE_NULL(pictograms_);
    removeAllChildrenWithCleanup(true);
    removeFromParentAndCleanup(true);
}
bool NavigationBar::init(const CCSize& size, const char* title) {
    
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255), size.width, size.height)) {
        return false;
    }
    
    labels_ = CCArray::create();
    CC_SAFE_RETAIN(labels_);
    
    CCSize sprite_size(size.height, size.height);
    float font_size = 0.4*size.height;
    
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    
    ////////////////////////////////
    // Add Home and Auxiliary Button
    addHomeButton(menu, sprite_size, ccp(0.5*sprite_size.width, 0.5*sprite_size.height));
    
    CCLabelTTF* separator = picto::cocos2d_utils::createLabel(">", font_size);
    separator->setColor(picto::resources::navigationBarTextColor());
    labels_->addObject(separator);
    separator->setAnchorPoint(ccp(0, 0.5));
    separator->setPosition(ccp(sprite_size.width, 0.5*sprite_size.height));
    addChild(separator);
    
    CCLabelTTF* title_label = picto::cocos2d_utils::createLabel(title, font_size);
    title_label->setColor(picto::resources::navigationBarTextColor());
    labels_->addObject(title_label);
    title_label->setAnchorPoint(ccp(-0.1, 0.5));
    title_label->setPosition(ccp(sprite_size.width + separator->getContentSize().width, 0.5*sprite_size.height));
    addChild(title_label);
    
    addChild(menu);
    
    return true;
}

bool NavigationBar::init(const CCSize& size, CCArray* pictograms) {
    
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 255), size.width, size.height)) {
        return false;
    }
    
    //ignoreAnchorPointForPosition(false);
    
    labels_ = CCArray::create();
    CC_SAFE_RETAIN(labels_);
    
    pictograms_ = pictograms;
    CC_SAFE_RETAIN(pictograms_);
    
    CCSize sprite_size(size.height, size.height);
    float font_size = 0.4*size.height;
    
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(ccp(0, 0));
    
    ////////////////////////////////
    // Add Home and Auxiliary Button
    addHomeButton(menu, sprite_size, ccp(0.5*sprite_size.width, 0.5*sprite_size.height));
    
    bool is_root = (pictograms->count() == 1);
    
    if (!is_root) {
        addHelpButton(menu, sprite_size, ccp(size.width - 0.5*sprite_size.width, 0.5*sprite_size.height));
    } else {
        addSettingsButton(menu, sprite_size, ccp(size.width - 0.5*sprite_size.width, 0.5*sprite_size.height));
    }
    
    if (pictograms->count() == 1) {
        addChild(menu);
        return true;
    }
    
    CCLabelTTF* separator = picto::cocos2d_utils::createLabel(">", font_size);
    separator->setColor(picto::resources::navigationBarTextColor());
    labels_->addObject(separator);
    separator->setAnchorPoint(ccp(0, 0.5));
    separator->setPosition(ccp(sprite_size.width, 0.5*sprite_size.height));
    addChild(separator);
    
    //PictoDatabase* database = AppDelegate::sharedDatabase();
    
    //////////////////////////////////////////////
    // Compute number of items that fit in nav bar
    
    float character_width = separator->getContentSize().width;
    float available_width = size.width - 2*sprite_size.width - 2*character_width;
    int num_items_in_nav_bar = 0;
    
    // From last item, compute the approximate width
    bool first_is_ellipsized = false;
    for (int i=pictograms->count()-1; i > 0; i--) {
        CCString* identifier = dynamic_cast<CCString*>(pictograms->objectAtIndex(i));
        PictogramObject* object = picto::database::pictogram(identifier->getCString());
        float item_width = sprite_size.width + (strlen(object->getName()->getCString()) + 1)*character_width;
        if (item_width <= available_width) {
            available_width -= item_width;
            num_items_in_nav_bar++;
        } else {
            first_is_ellipsized = true;
            break;
        }
    }
    
    ///////////////////////////
    // Add navigation bar items
    
    CCPoint position(sprite_size.width + character_width, 0.5*size.height);
    
    if (first_is_ellipsized) {
        int index = pictograms->count() - num_items_in_nav_bar - 1;
        CCString* identifier = dynamic_cast<CCString*>(pictograms->objectAtIndex(index));
        
        // Add item sprite
        PictogramObject* object = picto::database::pictogram(identifier->getCString());
        CCMenuItemImage* item_image = CCMenuItemImage::create(object->getImage()->getCString(),
                                                              object->getImage()->getCString(),
                                                              this,
                                                              menu_selector(NavigationBar::menuNavigationCallback));
        item_image->setTag(index + 1);
        item_image->setAnchorPoint(ccp(0.5, 0.5));
        item_image->setPosition(ccp(position.x + 0.5*sprite_size.width, position.y));
        item_image->setScale(0.7*sprite_size.height/item_image->getContentSize().height);
        
        menu->addChild(item_image);
        
        position.x += sprite_size.width;
        
        // Add item label
        CCLabelTTF* label = picto::cocos2d_utils::createLabel("...", font_size);
        label->setColor(picto::resources::navigationBarTextColor());
        labels_->addObject(label);
        CustomMenuItemLabel* item_label = CustomMenuItemLabel::create(label,
                                                                      this,
                                                                      menu_selector(NavigationBar::menuNavigationCallback), false);
        item_label->setTag(index + 1);
        item_label->setAnchorPoint(ccp(0, 0.5));
        item_label->setPosition(position);
        menu->addChild(item_label);
        
        position.x += item_label->getContentSize().width;
        
        // Add item separator
        CCLabelTTF* item_separator = picto::cocos2d_utils::createLabel(">", font_size);
        label->setColor(picto::resources::navigationBarTextColor());
        labels_->addObject(item_separator);
        item_separator->setAnchorPoint(ccp(0, 0.5));
        item_separator->setPosition(position);
        
        addChild(item_separator);
        
        position.x += item_separator->getContentSize().width;
    }
    
    for (int i=pictograms->count() - num_items_in_nav_bar; i < pictograms->count(); i++) {
        bool is_last_item = (i == (pictograms->count() - 1));
        
        CCString* identifier = dynamic_cast<CCString*>(pictograms->objectAtIndex(i));
        
        if (is_last_item && picto::database::countChilds(identifier->getCString()) == 0) {
            continue;
        }
        
        // Add item sprite
        PictogramObject* object = picto::database::pictogram(identifier->getCString());
        CCMenuItemImage* item_image = CCMenuItemImage::create(object->getImage()->getCString(),
                                                              object->getImage()->getCString(),
                                                              this,
                                                              menu_selector(NavigationBar::menuNavigationCallback));
        item_image->setTag(i + 1);
        item_image->setAnchorPoint(ccp(0.5, 0.5));
        item_image->setPosition(ccp(position.x + 0.5*sprite_size.width, position.y));
        item_image->setScale(0.7*sprite_size.height/item_image->getContentSize().height);
        item_image->setEnabled(!is_last_item);
        
        menu->addChild(item_image);
        
        position.x += sprite_size.width;
        
        // Add item label
        CCLabelTTF* label = picto::cocos2d_utils::createLabel(object->getName()->getCString(), font_size);
        label->setColor(picto::resources::navigationBarTextColor());
        labels_->addObject(label);
        
        CustomMenuItemLabel* item_label = CustomMenuItemLabel::create(label,
                                                                      this,
                                                                      menu_selector(NavigationBar::menuNavigationCallback), false);
        item_label->setEnabled(!is_last_item);
        item_label->setTag(i + 1);
        item_label->setAnchorPoint(ccp(0, 0.5));
        item_label->setPosition(position);
        
        menu->addChild(item_label);
        
        position.x += label->getContentSize().width;
        
        // Separator is not added at the last element
        if (!is_last_item) {
            
            // Add item separator
            CCLabelTTF* item_separator = picto::cocos2d_utils::createLabel(">", font_size);
            item_separator->setColor(picto::resources::navigationBarTextColor());
            labels_->addObject(item_separator);
            item_separator->setAnchorPoint(ccp(0, 0.5));
            item_separator->setPosition(position);
            
            addChild(item_separator);
            
            position.x += item_separator->getContentSize().width;
            
        }
    }
    
    addChild(menu);
    
    return true;
}

void NavigationBar::addHelpButton(cocos2d::CCMenu *menu, const cocos2d::CCSize& size, const cocos2d::CCPoint& position) {
    
    PictogramObject* object = picto::database::pictogram("help");
    CCMenuItemImage* item = CCMenuItemImage::create(object->getImage()->getCString(),
                                                    object->getImage()->getCString(),
                                                    this,
                                                    menu_selector(NavigationBar::helpPressed));
    item->setTag(1); // Root Stack level
    item->setAnchorPoint(ccp(0.5, 0.5));
    item->setPosition(position);
    item->setScale(size.height/item->getContentSize().height);
    
    menu->addChild(item);
}

void NavigationBar::addHomeButton(cocos2d::CCMenu *menu, const cocos2d::CCSize& size, const cocos2d::CCPoint& position) {
    
    PictogramObject* object = picto::database::pictogram("picto_connection");
    CCMenuItemImage* item = CCMenuItemImage::create(object->getImage()->getCString(),
                                                    object->getImage()->getCString(),
                                                    this,
                                                    menu_selector(NavigationBar::homePressed));
    item->setTag(1); // Root Stack level
    item->setAnchorPoint(ccp(0.5, 0.5));
    item->setPosition(position);
    item->setScale(size.height/item->getContentSize().height);
    
    menu->addChild(item);
}

void NavigationBar::addSettingsButton(cocos2d::CCMenu *menu, const cocos2d::CCSize& size, const cocos2d::CCPoint& position) {
    
    CCMenuItem* item = CCMenuItem::create(this,
                                          menu_selector(NavigationBar::settingsPressed));
    item->setContentSize(size);
    
    CCLayerColor* background = CCLayerColor::create(ccc4(0, 0, 0, 255), size.width, size.height);
    background->setColor(picto::resources::settingsButtonBackground());
    background->ignoreAnchorPointForPosition(false);
    background->setAnchorPoint(ccp(0.5, 0.5));
    background->setPosition(ccp(0.5*size.width, 0.5*size.height));
    item->addChild(background);
    
    CCSprite* sprite = CCSprite::create("settings.png");
    sprite->setScale(0.7*size.width/sprite->getContentSize().width);
    sprite->setAnchorPoint(ccp(0.5, 0.5));
    sprite->setPosition(ccp(0.5*size.width, 0.5*size.height));
    item->addChild(sprite);
    
    item->setTag(1); // Root Stack level
    item->setAnchorPoint(ccp(0.5, 0.5));
    item->setPosition(position);
    item->setScale(size.height/item->getContentSize().height);
    menu->addChild(item);
}

void NavigationBar::onEnter() {
    CCLayerColor::onEnter();
    setColor(picto::resources::navigationBarBackgroundColor());
    
    CCObject* it;
    CCARRAY_FOREACH(labels_, it) {
        CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(it);
        label->setColor(picto::resources::navigationBarTextColor());
    }
}

void NavigationBar::onExit() {
    CCLayerColor::onExit();
}

void NavigationBar::helpPressed(CCObject* sender) {
    // TODO
}

void NavigationBar::homePressed(CCObject* sender) {
    
    if (pictograms_) {
        CCArray* pictograms = CCArray::create(CCString::create("picto_connection"), NULL);
        CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
    }
}

void NavigationBar::menuNavigationCallback(cocos2d::CCObject* sender) {
    CCNode* nav_item = dynamic_cast<CCNode*>(sender);
    
    CCArray* pictograms = CCArray::createWithArray(pictograms_);
    
    while (pictograms->count() > nav_item->getTag())
        pictograms->removeLastObject();
    
    CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
}

void NavigationBar::settingsPressed(CCObject* sender) {
    CCDirector::sharedDirector()->pushScene(Settings::scene());
}
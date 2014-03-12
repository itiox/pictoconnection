/**
 * PictoConnection
 *
 * @file PickThemeScene.cpp
 * @brief Scene for picking the theme of this application.
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

#include "PickThemeScene.h"

#include "NavigationBar.h"
#include "PictoDefs.h"
#include "PictoTheme.h"

USING_NS_CC;

struct Theme {
    
};

CCScene* PickTheme::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    
    // 'layer' is an autorelease object
    PickTheme *layer = PickTheme::create();
    
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

PickTheme::PickTheme() :
back_button_(NULL),
labels_(NULL) {}

PickTheme::~PickTheme() {
    CC_SAFE_RELEASE_NULL(labels_);
}

bool PickTheme::init() {
    
    //////////////////////////////
    // 1. super init first
    if ( !CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)) ) {
        return false;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    setKeypadEnabled(true);
#endif
    
    // Compute some UI parameters
    CCSize visible_size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visible_origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Add bottom bar
    CCSize bottom_bar_size = visible_size;
    bottom_bar_size.height = 0.2*MIN(visible_size.width, visible_size.height);
    initBottomBar(bottom_bar_size, visible_origin);
    
    // Add top bar
    CCSize top_bar_size = visible_size;
    top_bar_size.height = 0.1*MIN(visible_size.width, visible_size.height);
    CCPoint top_bar_origin = ccp(visible_origin.x, visible_origin.y + visible_size.height - top_bar_size.height);
    initTopBar(top_bar_size, top_bar_origin);
    
    // Add content grid
    CCSize grid_size(visible_size.width, visible_size.height - bottom_bar_size.height - top_bar_size.height);
    CCPoint grid_origin = ccp(visible_origin.x, bottom_bar_size.height);
    initContent(grid_size, grid_origin);
    
    return true;
}

void PickTheme::initBottomBar(const CCSize& size,
                              const CCPoint& origin) {
    
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
                                                      menu_selector(PickTheme::backPressed));
    back_button_->setAnchorPoint(ccp(0.5, 0.5));
    back_button_->setContentSize(normal->getContentSize());
    back_button_->setPosition(ccp(0.5*size.width, 0.5*size.height));
    
    CCMenu* menu = CCMenu::create(back_button_, NULL);
    menu->setPosition(origin);
    addChild(menu);
}

void PickTheme::initTopBar(const CCSize& size, const CCPoint& origin) {
    
    NavigationBar* navigation_bar = NavigationBar::create(size, "Escoge un tema");
    navigation_bar->setPosition(origin);
    addChild(navigation_bar);
}

CCSize PickTheme::computeOptimumGrid(const int num_elements, const cocos2d::CCSize& gridSize, const float nodeRatio, const float margin) {
    
    CCSize bestGrid(1, num_elements);
    float maxArea = 0;
    float maxCompactness = 0;
    
    for (int i=1; i <= num_elements; i++) {
        
        float maxHeight = (gridSize.height - (i+1)*margin) / i;
        float maxWidth = maxHeight / nodeRatio;
        
        if (maxHeight*maxWidth > maxArea) {
            for (int j=num_elements; j >=1; j--) {
                if (i*j < num_elements)
                    break;
                
                float width = MIN((gridSize.width - (j+1)*margin) / j, maxWidth);
                float height = MIN(width * nodeRatio, maxHeight);
                float area = width*height;
                if (area > maxArea) {
                    
                    maxCompactness = (float)num_elements/(float)(i*j);
                    maxArea = area;
                    bestGrid.height = i;
                    bestGrid.width = j;
                    
                } else if (area == maxArea) {
                    
                    float compactness = (float)num_elements/(float)(i*j);
                    
                    if (compactness > maxCompactness) {
                        maxCompactness = compactness;
                        maxArea = area;
                        bestGrid.height = i;
                        bestGrid.width = j;
                    }
                }
            }
        }
    }
    
    return bestGrid;
}

cocos2d::CCMenuItem* PickTheme::createThemeItem(const CCSize& size,
                                                const ccColor3B& color) {
    
    CCRenderTexture* color_texture = CCRenderTexture::create(0.8*size.width, 0.8*size.height);
    color_texture->setAnchorPoint(ccp(0.5, 0.5));
    color_texture->beginWithClear(color.r/255., color.g/255., color.b/255., 1.);
    color_texture->end();
    
    CCRenderTexture* pressed_color_texture = CCRenderTexture::create(0.8*size.width, 0.8*size.height);
    pressed_color_texture->setAnchorPoint(ccp(0.5, 0.5));
    pressed_color_texture->beginWithClear(color.r/255., color.g/255., color.b/255., 0.8);
    pressed_color_texture->end();
    
    
    CCSprite* normal = CCSprite::create("white_frame.png");
    normal->setAnchorPoint(ccp(0.5, 0.5));
    normal->setPosition(ccp(0, 0));
    normal->setScale(size.width / normal->getContentSize().width);
    
    CCLayerColor* normal_content = CCLayerColor::create(ccc4(color.r, color.g, color.b, 255),
                                                        0.95*normal->getContentSize().width,
                                                        0.95*normal->getContentSize().height);
    normal_content->ignoreAnchorPointForPosition(false);
    normal_content->setAnchorPoint(ccp(0.5, 0.5));
    normal_content->setPosition(ccp(0.5*normal->getContentSize().width,
                                    0.5*normal->getContentSize().height));
    normal->addChild(normal_content);
    
    CCSprite* pressed = CCSprite::create("white_frame.png");
    pressed->setAnchorPoint(ccp(0.5, 0.5));
    pressed->setPosition(ccp(0, 0));
    pressed->setScale(1.02*size.width / pressed->getContentSize().width);
    
    CCLayerColor* pressed_content = CCLayerColor::create(ccc4(color.r, color.g, color.b, 192),
                                                         0.9*pressed->getContentSize().width,
                                                         0.9*pressed->getContentSize().height);
    pressed_content->ignoreAnchorPointForPosition(false);
    pressed_content->setAnchorPoint(ccp(0.5, 0.5));
    pressed_content->setPosition(ccp(0.5*pressed->getContentSize().width,
                                     0.5*pressed->getContentSize().height));
    pressed->addChild(pressed_content);
    
    CCSprite* selected = CCSprite::create("white_frame.png");
    selected->setAnchorPoint(ccp(0.5, 0.5));
    selected->setPosition(ccp(0, 0));
    selected->setScale(1.02*size.width / selected->getContentSize().width);
    
    CCLayerColor* selected_content = CCLayerColor::create(ccc4(color.r, color.g, color.b, 192),
                                                          0.9*selected->getContentSize().width,
                                                          0.9*selected->getContentSize().height);
    selected_content->ignoreAnchorPointForPosition(false);
    selected_content->setAnchorPoint(ccp(0.5, 0.5));
    selected_content->setPosition(ccp(0.5*selected->getContentSize().width,
                                      0.5*selected->getContentSize().height));
    selected->addChild(selected_content);
    
    CCMenuItemSprite* item = CCMenuItemSprite::create(normal, pressed, selected, this, menu_selector(PickTheme::menuThemeCallback));
    item->setContentSize(size);
    
    return item;
}

void PickTheme::initContent(const CCSize& size, const CCPoint& origin) {
    
    CCArray* themes = PictoTheme::themes();
    unsigned int selected_theme = CCUserDefault::sharedUserDefault()->getIntegerForKey("selected_theme", 0);
    
    CCMenu* menu = CCMenu::create();
    //menu->ignoreAnchorPointForPosition(false);
    //menu->setAnchorPoint(ccp(0.5, 0.5));
    //menu->setPosition(ccp(origin.x + 0.5*size.width, origin.y + 0.5*size.height));
    menu->setPosition(origin);
    
    // Compute node visible size
    float margin = 0.02*MAX(size.width, size.height);
    float ratio = 1;
    
    CCSize optimum_grid = computeOptimumGrid(themes->count(), size, ratio, margin);
    
    // Compute arrangement for pictograms
    int cols = optimum_grid.width;
    int rows = optimum_grid.height;
    
    CCPoint center = ccp(0.5*size.width, 0.5*size.height);
    
    CCSize node_size;
    node_size.width = (size.width - (cols + 1)*margin) / cols;
    node_size.height = (size.height - (rows + 1)*margin) / rows;
    
    node_size.height = MIN(node_size.height, node_size.width * ratio);
    node_size.width = node_size.height / ratio;
    
    CCSize step = CCSizeMake(node_size.width + margin, node_size.height + margin);
    
    CCPoint top_left_node_pos = center;
    top_left_node_pos.y +=  + (rows - 1)*step.height; // Start from top
    if (rows % 2 == 0)
        top_left_node_pos.y += (0.5 - (rows / 2))*step.height;
    else
        top_left_node_pos.y -= ((rows - 1) / 2)*step.height;
    if (cols % 2 == 0)
        top_left_node_pos.x += (0.5 - (cols / 2))*step.width;
    else
        top_left_node_pos.x -= ((cols - 1) / 2)*step.width;
    
    for (int i=0, n=0; i < rows; i++) {
        
        CCPoint left_node_pos = top_left_node_pos;
        left_node_pos.y -= i*step.height; // Start from top
        int icols = MIN(cols, themes->count() - n);
        if (icols != cols) {
            if (icols % 2 == 0)
                left_node_pos.x = center.x + (0.5 - (icols / 2))*step.width;
            else
                left_node_pos.x = center.x - ((icols - 1) / 2)*step.width;
        }
        
        for (int j=0; j < icols; j++, n++) {
            
            PictoTheme* theme = dynamic_cast<PictoTheme*>(themes->objectAtIndex(n));
            CCMenuItem* item = createThemeItem(node_size, theme->backgroundColor);
            if (n==selected_theme) {
                item->setEnabled(false);
            }
            item->setAnchorPoint(ccp(0.5, 0.5));
            item->setPosition(left_node_pos.x + j*step.width, left_node_pos.y);
            item->setTag(theme->identifier);
            menu->addChild(item);
        }
    }
    
    addChild(menu);
}

void PickTheme::backPressed(CCObject *sender) {
    CCDirector::sharedDirector()->popScene();
}

void PickTheme::keyBackClicked() {
    CCDirector::sharedDirector()->popScene();
}

void PickTheme::menuThemeCallback(CCObject *sender) {
    CCLOG("PickTheme::menuThemeCallback()");
    
    int identifier = dynamic_cast<CCNode*>(sender)->getTag();
    PictoTheme* theme = PictoTheme::create(identifier);
    theme->select();
    
    CCDirector::sharedDirector()->replaceScene(PickTheme::scene());
}

void PickTheme::onEnter() {
    CCLayerColor::onEnter();
    setColor(picto::resources::backgroundColor());
    
    if (back_button_) {
        back_button_->setColor(picto::resources::navigationBarBackgroundColor());
    }
}
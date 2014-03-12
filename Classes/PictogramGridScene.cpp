/**
 * PictoConnection
 *
 * @file PictogramGridScene.cpp
 * @brief Grid representation of a set of pictograms
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

#include "PictogramGridScene.h"

#include "AppDelegate.h"
#include "NavigationBar.h"
#include "PictoDatabase.h"
#include "PictogramGalleryScene.h"
#include "PictogramNode.h"
#include "PictogramScene.h"
#include "PictoDefs.h"

USING_NS_CC;

CCScene* PictogramGrid::scene(CCArray* pictograms)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    scene->setUserObject(CCNode::create());
    
    // 'layer' is an autorelease object
    PictogramGrid *layer = new PictogramGrid();
    
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

PictogramGrid::PictogramGrid() :

back_button_(NULL),
pictograms_(NULL),
scene_mutex_(false) {
    
}

PictogramGrid::~PictogramGrid() {
    CCLOG("PictogramGrid::~PictogramGrid()");
    
    CC_SAFE_RELEASE_NULL(pictograms_);
    removeAllChildrenWithCleanup(true);
    removeFromParentAndCleanup(true);
    CCTextureCache::purgeSharedTextureCache();
}

bool PictogramGrid::init(CCArray* pictograms) {
    
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
    
    // Get pictograms childs from database
    CCArray* childs = picto::database::childs(((CCString*)pictograms->lastObject())->getCString());
    
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
    
    // Add grid
    CCSize grid_size(visible_size.width, visible_size.height - bottom_bar_size.height - top_bar_size.height);
    CCPoint grid_origin = ccp(visible_origin.x, bottom_bar_size.height);
    initGridOfPictograms(grid_size, grid_origin, childs);
    
    return true;
}

void PictogramGrid::initBottomBar(const cocos2d::CCSize& size,
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
                                            menu_selector(PictogramGrid::backPressed));
    back_button_->setAnchorPoint(ccp(0.5, 0.5));
    back_button_->setContentSize(normal->getContentSize());
    back_button_->setPosition(ccp(0.5*size.width, 0.5*size.height));
    
    CCMenu* menu = CCMenu::create(back_button_, NULL);
    menu->setPosition(origin);
    addChild(menu);
}

void PictogramGrid::initTopBar(const cocos2d::CCSize& size,
                               const cocos2d::CCPoint& origin) {
    
    NavigationBar* navigation_bar = NavigationBar::create(size, pictograms_);
    navigation_bar->setPosition(origin);
    addChild(navigation_bar);
}

CCSize computeOptimumGrid(const int num_elements, const cocos2d::CCSize gridSize, const float nodeRatio, const float margin) {
    
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

void PictogramGrid::initGridOfPictograms(const cocos2d::CCSize& size,
                                         const cocos2d::CCPoint& origin,
                                         cocos2d::CCArray* childs) {
    
    // Compute node visible size
    float margin = 0.02*MAX(size.width, size.height);
    float ratio = 1;
    
    CCSize optimumGrid = computeOptimumGrid(childs->count(), size, ratio, margin);
    
    // Compute arrangement for pictograms
    int cols = optimumGrid.width;
    int rows = optimumGrid.height;
    
    CCPoint center = ccp(origin.x + 0.5*size.width, origin.y + 0.5*size.height);
    
    CCSize nodeSize;
    nodeSize.width = (size.width - (cols + 1)*margin) / cols;
    nodeSize.height = (size.height - (rows + 1)*margin) / rows;
    
    nodeSize.height = MIN(nodeSize.height, nodeSize.width * ratio);
    nodeSize.width = nodeSize.height / ratio;
    
    CCSize step = CCSizeMake(nodeSize.width + margin, nodeSize.height + margin);
    
    CCPoint topLeftNodePos = center;
    topLeftNodePos.y +=  + (rows - 1)*step.height; // Start from top
    if (rows % 2 == 0)
        topLeftNodePos.y += (0.5 - (rows / 2))*step.height;
    else
        topLeftNodePos.y -= ((rows - 1) / 2)*step.height;
    if (cols % 2 == 0)
        topLeftNodePos.x += (0.5 - (cols / 2))*step.width;
    else
        topLeftNodePos.x -= ((cols - 1) / 2)*step.width;
    
    for (int i=0, n=0; i < rows; i++) {
        
        CCPoint leftNodePos = topLeftNodePos;
        leftNodePos.y -= i*step.height; // Start from top
        int icols = MIN(cols, childs->count() - n);
        if (icols != cols) {
            if (icols % 2 == 0)
                leftNodePos.x = center.x + (0.5 - (icols / 2))*step.width;
            else
                leftNodePos.x = center.x - ((icols - 1) / 2)*step.width;
        }
        for (int j=0; j < icols; j++, n++) {
            PictogramObject *object = dynamic_cast<PictogramObject*>(childs->objectAtIndex(n));
            PictogramNode *node = PictogramNode::create(object, nodeSize);
            node->setTarget(this, menu_selector(PictogramGrid::pictogramPressed));
            node->setPosition(leftNodePos.x + j*step.width, leftNodePos.y);
            addChild(node, 1);
        }
    }
}

void PictogramGrid::keyBackClicked() {
    /* TODO if (root_scene_flag_)
     CCDirector::sharedDirector()->end();
     else
     CCDirector::sharedDirector()->popScene();*/
}

void PictogramGrid::onEnter() {
    CCLayerColor::onEnter();
    setColor(picto::resources::backgroundColor());
    
    if (back_button_) {
        back_button_->setColor(picto::resources::navigationBarBackgroundColor());
    }
}

void PictogramGrid::onExit() {
    CCLayerColor::onExit();
    scene_mutex_ = false;
}

void PictogramGrid::backPressed(CCObject* sender) {
    CCArray* pictograms = CCArray::createWithArray(pictograms_);
    pictograms->removeLastObject();
    CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
}

void PictogramGrid::pictogramPressed(CCObject *sender) {
    PictogramNode* node = dynamic_cast<PictogramNode*>(sender);
    
    if (CCDirector::sharedDirector()->getRunningScene() != getParent()) {
        return;
    }
    
    if (scene_mutex_) {
        return;
    }
    
    scene_mutex_ = true;
    
    if (picto::database::countChilds(node->getData()->getIdentifier()->getCString()) > 0) {
        CCArray* pictograms = CCArray::createWithArray(pictograms_);
        pictograms->addObject(CCString::create(node->getData()->getIdentifier()->getCString()));
        CCDirector::sharedDirector()->replaceScene(PictogramGrid::scene(pictograms));
    } else {
        CCArray* pictograms = CCArray::createWithArray(pictograms_);
        pictograms->addObject(CCString::create(node->getData()->getIdentifier()->getCString()));
        //CCDirector::sharedDirector()->replaceScene(Pictogram::scene(pictograms, 0));
        CCDirector::sharedDirector()->replaceScene(PictogramGallery::scene(pictograms));
    }
}


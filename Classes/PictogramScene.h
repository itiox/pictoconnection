/**
 * PictoConnection
 *
 * @file PictogramScene.h
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

#ifndef __PICTOGRAM_SCENE_H__
#define __PICTOGRAM_SCENE_H__

#include "cocos2d.h"

#include "PictogramNode.h"
#include "PictogramObject.h"

class Pictogram : public cocos2d::CCLayerColor
{
    
public: // constructors and creators
    
    static cocos2d::CCScene* scene(cocos2d::CCArray* pictograms, int enter_animation);
    
    Pictogram();
    ~Pictogram();
    
private: // initializers
    
    bool init(cocos2d::CCArray* pictograms, int enter_animation);
    void initBottomBar(const cocos2d::CCSize& size, const cocos2d::CCPoint& origin);
    void initTopBar(const cocos2d::CCSize& size, const cocos2d::CCPoint& origin);
    void initContent(const cocos2d::CCSize& size, const cocos2d::CCPoint& origin, PictogramObject* pictogram);
    
private: // private methods

    void backPressed(cocos2d::CCObject* sender);
    void keyBackClicked();
    void menuCloseCallback(cocos2d::CCObject* sender);
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void onEnter();
    void onExit();
    void onSlideLeftwardsAnimationEnded();
    void onSlideRightwardsAnimationEnded();
    
private: // private variables
    
    cocos2d::CCMenuItem* back_button_;
    cocos2d::CCSprite* back_button_overlay_;
    PictogramNode* pictogram_node_;
    PictogramNode* pictogram_node_left_;
    PictogramNode* pictogram_node_right_;
    cocos2d::CCArray* pictograms_;
    
    cocos2d::CCRect touch_frame_;
    cocos2d::CCPoint touch_location_;
};

#endif // __PICTOGRAM_SCENE_H__
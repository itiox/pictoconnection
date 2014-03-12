/**
 * PictoConnection
 *
 * @file PictogramNode.h
 * @brief Visual representation of a pictogram
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

#ifndef __PICTOGRAM_NODE_H__
#define __PICTOGRAM_NODE_H__

#include "cocos2d.h"

#include "PictogramObject.h"

class PictogramNode : public cocos2d::CCLayerColor
{
    
public: // constructors and creators
    
    static PictogramNode* create(PictogramObject *pictogram, const cocos2d::CCSize& size);
    
    PictogramNode();
    ~PictogramNode();
    
private: // init methods
    
    bool init(PictogramObject *pictogram, const cocos2d::CCSize& size);
    
public: // public methods
    
    void ignoreTouches(bool ignore);
    void setTarget(cocos2d::CCObject *receiver, cocos2d::SEL_MenuHandler selector);
    
private: // private methods
    
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void onEnter();
    void onExit();
    void speakerCallback(cocos2d::CCObject* sender);
    
public: // public variables
    
    CC_SYNTHESIZE_READONLY(PictogramObject*, data_, Data);
    
private: // private variables
    
    bool ignore_touches_;
    
    cocos2d::CCLayer* speaker_button_;
    cocos2d::CCSprite* speaker_background_;
    cocos2d::CCSprite* speaker_icon_;
    cocos2d::CCLabelTTF* speaker_label_;
    
    cocos2d::CCObject* receiver_;
    cocos2d::SEL_MenuHandler selector_;
};

#endif // __PICTOGRAM_NODE_H__
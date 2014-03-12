/**
 * PictoConnection
 *
 * @file PictogramGridScene.h
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

#ifndef __PICTOGRAM_GRID_SCENE_H__
#define __PICTOGRAM_GRID_SCENE_H__

#include "cocos2d.h"

class PictogramGrid : public cocos2d::CCLayerColor
{
    
public: // constructors and creators
    
    static cocos2d::CCScene* scene(cocos2d::CCArray* pictograms);
    
    PictogramGrid();
    ~PictogramGrid();
    
private: // init methods
    
    bool init(cocos2d::CCArray* pictograms);
    void initBottomBar(const cocos2d::CCSize& size, const cocos2d::CCPoint& origin);
    void initTopBar(const cocos2d::CCSize& size, const cocos2d::CCPoint& origin);
    void initGridOfPictograms(const cocos2d::CCSize& size, const cocos2d::CCPoint& origin, cocos2d::CCArray* childs);
    
private: // private methods
    
    void onEnter();
    void onExit();
    
    void backPressed(cocos2d::CCObject* sender);
    void keyBackClicked();
    void pictogramPressed(cocos2d::CCObject* sender);
    
private: // private variables
    
    bool scene_mutex_;
    
    cocos2d::CCMenuItem* back_button_;
    cocos2d::CCArray* pictograms_;
};

#endif // __PICTOGRAM_GRID_SCENE_H__
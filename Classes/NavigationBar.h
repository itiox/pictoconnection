/**
 * PictoConnection
 *
 * @file NavigationBar.h
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

#ifndef __NAVIGATION_BAR_H__
#define __NAVIGATION_BAR_H__

#include "cocos2d.h"

#include "PictogramObject.h"

class NavigationBar : public cocos2d::CCLayerColor
{
    
public: // constructors and creators
    
    static NavigationBar* create(const cocos2d::CCSize& size, const char* title);
    static NavigationBar* create(const cocos2d::CCSize& size, cocos2d::CCArray* pictograms);
    
    NavigationBar();
    ~NavigationBar();
    
private: // init methods
    
    bool init(const cocos2d::CCSize& size, const char* title);
    bool init(const cocos2d::CCSize& size, cocos2d::CCArray* pictograms);
    void addHelpButton(cocos2d::CCMenu* menu, const cocos2d::CCSize& size, const cocos2d::CCPoint& position);
    void addHomeButton(cocos2d::CCMenu* menu, const cocos2d::CCSize& size, const cocos2d::CCPoint& position);
    void addSettingsButton(cocos2d::CCMenu* menu, const cocos2d::CCSize& size, const cocos2d::CCPoint& position);
    
private: // private methods
    
    void onEnter();
    void onExit();
    void helpPressed(cocos2d::CCObject* sender);
    void homePressed(cocos2d::CCObject* sender);
    void menuNavigationCallback(cocos2d::CCObject* sender);
    void settingsPressed(cocos2d::CCObject* sender);
    
private: // private variables
    
    cocos2d::CCArray* labels_;
    cocos2d::CCArray* pictograms_;
};

#endif // __NAVIGATION_BAR_H__
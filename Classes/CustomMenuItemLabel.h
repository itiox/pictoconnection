/**
 * PictoConnection
 *
 * @file CustomMenuItemLabel.h
 * @brief Custom subclass of CCMenuItemLabel
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

#ifndef __CUSTOM_MENU_ITEM_LABEL_H__
#define __CUSTOM_MENU_ITEM_LABEL_H__

#include "cocos2d.h"

class CustomMenuItemLabel : public cocos2d::CCMenuItemLabel {
    
public: // public constructors and creators
    
    static CustomMenuItemLabel* create(cocos2d::CCNode *label, cocos2d::CCObject *target, cocos2d::SEL_MenuHandler selector, bool scaling);
    
private: // init methods typically called by creators
    
    bool init(cocos2d::CCNode *label, cocos2d::CCObject *target, cocos2d::SEL_MenuHandler selector, bool scaling);
    
private: // private methods
    
    void selected();
    void unselected();
    
private: // private variables
    
    bool scaling_;
};

#endif // __CUSTOM_MENU_ITEM_LABEL_H__

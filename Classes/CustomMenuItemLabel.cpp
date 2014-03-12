/**
 * PictoConnection
 *
 * @file CustomMenuItemLabel.cpp
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

#include "CustomMenuItemLabel.h"

USING_NS_CC;

CustomMenuItemLabel* CustomMenuItemLabel::create(CCNode *label, CCObject *target, SEL_MenuHandler selector, bool scaling) {
    CustomMenuItemLabel *instance = new CustomMenuItemLabel();
    
    if (instance && instance->init(label, target, selector, scaling)) {
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    
    return NULL;
}

bool CustomMenuItemLabel::init(CCNode *label, CCObject *target, SEL_MenuHandler selector, bool scaling) {
    
    if (!CCMenuItemLabel::initWithLabel(label, target, selector)) {
        return false;
    }
    
    scaling_ = scaling;
    
    return true;
}

void CustomMenuItemLabel::selected() {
    if (scaling_) {
        CCMenuItemLabel::selected();
    }
}

void CustomMenuItemLabel::unselected() {
    if (scaling_) {
        CCMenuItemLabel::unselected();
    }
}

/**
 * PictoConnection
 *
 * @file PictoTheme.h
 * @brief Object which stores theme color configurations
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

//
//  PictoTheme.h
//  PictoConnection
//
//  Created by Javier Alvargonzalez on 10/03/14.
//
//

#ifndef __PICTO_THEME_H__
#define __PICTO_THEME_H__

#include "cocos2d.h"

class PictoTheme : public cocos2d::CCObject {
    
public: // public constructors and creators
    
    static cocos2d::CCArray* themes();
    
    static PictoTheme* create(const unsigned int identifier);
    
    PictoTheme(const unsigned int identifier,
               const unsigned int backgroundColor,
               const unsigned int menuTextColor,
               const unsigned int navigationBarBackgroundColor,
               const unsigned int navigationBarTextColor,
               const unsigned int speakerButtonBackgroundColor,
               const unsigned int speakerButtonTextColor);
    
public: // public methods
    
    void select();
    
public: // public variables
    
    const unsigned int identifier;
    const cocos2d::ccColor3B backgroundColor;
    const cocos2d::ccColor3B menuTextColor;
    const cocos2d::ccColor3B navigationBarBackgroundColor;
    const cocos2d::ccColor3B navigationBarTextColor;
    const cocos2d::ccColor3B speakerButtonBackgroundColor;
    const cocos2d::ccColor3B speakerButtonTextColor;
};

#endif // __PICTO_THEME_H__

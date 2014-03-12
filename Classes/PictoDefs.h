/**
 * PictoConnection
 *
 * @file PictoDefs.h
 * @brief Generic and utility methods used in the application.
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

#ifndef __PICTO_DEFS_H__
#define __PICTO_DEFS_H__

#include "cocos2d.h"

#include <stddef.h>

#define DEFAULT_COLOR_THEME 0x3694B3

namespace picto
{
    namespace cocos2d_utils
    {
        cocos2d::CCLabelTTF* createLabel(const char* text, const float font_size);
        bool playEffect(const char* effect);
    }
    
    namespace conversions
    {
        int color2int(cocos2d::ccColor3B color);
        cocos2d::ccColor3B int2color3B(unsigned int rgb);
        cocos2d::ccColor4B int2color4B(unsigned int rgb);
        
        std::wstring toupper(wchar_t in);
        std::string toupper(std::string in);
        std::wstring toupper(std::wstring in);
        std::wstring utf8_to_utf16(const std::string& utf8);
    }
    
    namespace resources
    {
        cocos2d::ccColor3B backgroundColor();
        cocos2d::ccColor3B menuTextColor();
        cocos2d::ccColor3B navigationBarBackgroundColor();
        cocos2d::ccColor3B navigationBarTextColor();
        cocos2d::ccColor3B speakerButtonBackgroundColor();
        cocos2d::ccColor3B speakerButtonTextColor();
        cocos2d::ccColor3B settingsButtonBackground();
    }
}

#endif //__PICTO_DEFS_H__
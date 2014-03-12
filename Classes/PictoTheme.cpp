/**
 * PictoConnection
 *
 * @file PictoTheme.cpp
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

#include "PictoTheme.h"

#include "PictoDefs.h"

USING_NS_CC;

using namespace picto::conversions;

CCArray* PictoTheme::themes() {
    CCArray* themes = CCArray::create();
    for (int i=0; i < 4; i++) {
        themes->addObject(PictoTheme::create(i));
    }
    return themes;
}

PictoTheme* PictoTheme::create(const unsigned int identifier) {
    PictoTheme* theme = NULL;
    
    switch (identifier) {
            
        case 0: // Original blue theme
            theme = new PictoTheme(identifier, 0x5cc4dc, 0xffffff, 0x024e68, 0xffffff, 0xffff00, 0x000000);
            break;
        case 1: // Pink theme
            theme = new PictoTheme(identifier, 0xfa8ead, 0xffffff, 0xff0094, 0xffffff, 0xffff00, 0x000000);
            break;
        case 2: // Red/yellow theme
            theme = new PictoTheme(identifier, 0xe2eb8d, 0x000000, 0xc50000, 0xffffff, 0x00a5cb, 0x000000);
            break;
        case 3: // Purple theme
            theme = new PictoTheme(identifier, 0x990fd1, 0xffffff, 0x550fd2, 0xffffff, 0xe5e5e5, 0x000000);
            break;
        default:
            break;
    }
    
    theme->autorelease();
    return theme;
}

PictoTheme::PictoTheme(const unsigned int identifier_,
                       const unsigned int backgroundColor_,
                       const unsigned int menuTextColor_,
                       const unsigned int navigationBarBackgroundColor_,
                       const unsigned int navigationBarTextColor_,
                       const unsigned int speakerButtonBackgroundColor_,
                       const unsigned int speakerButtonTextColor_) :
identifier(identifier_),
backgroundColor(int2color3B(backgroundColor_)),
menuTextColor(int2color3B(menuTextColor_)),
navigationBarBackgroundColor(int2color3B(navigationBarBackgroundColor_)),
navigationBarTextColor(int2color3B(navigationBarTextColor_)),
speakerButtonBackgroundColor(int2color3B(speakerButtonBackgroundColor_)),
speakerButtonTextColor(int2color3B(speakerButtonTextColor_)) {}

void PictoTheme::select() {
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey("selected_theme", identifier);
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey("backgroundColor",
                                                         picto::conversions::color2int(backgroundColor));
    CCUserDefault::sharedUserDefault()->setIntegerForKey("menuTextColor",
                                                         picto::conversions::color2int(menuTextColor));
    CCUserDefault::sharedUserDefault()->setIntegerForKey("navigationBarBackgroundColor",
                                                         picto::conversions::color2int(navigationBarBackgroundColor));
    CCUserDefault::sharedUserDefault()->setIntegerForKey("navigationBarTextColor",
                                                         picto::conversions::color2int(navigationBarTextColor));
    CCUserDefault::sharedUserDefault()->setIntegerForKey("speakerButtonBackgroundColor",
                                                         picto::conversions::color2int(speakerButtonBackgroundColor));
    CCUserDefault::sharedUserDefault()->setIntegerForKey("speakerButtonTextColor",
                                                         picto::conversions::color2int(speakerButtonTextColor));
}

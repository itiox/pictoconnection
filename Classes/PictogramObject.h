/**
 * PictoConnection
 *
 * @file PictogramObject.h
 * @brief Object which stores pictogram data
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

#ifndef __PICTOGRAM_OBJECT_H__
#define __PICTOGRAM_OBJECT_H__

#include "cocos2d.h"

class PictogramObject : public cocos2d::CCObject {
    
public: // constructors and creators
    
    static PictogramObject* create(const char* identifier,
                                   const char* locale,
                                   const char* name,
                                   const char* image,
                                   const char* sound,
                                   const char* thumb);
    
    PictogramObject();
    ~PictogramObject();
    
private: // initializers
    
    bool init(const char* identifier,
              const char* locale,
              const char* name,
              const char* image,
              const char* sound,
              const char* thumb);
    
public: // public variables
    
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, identifier_, Identifier);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, image_, Image);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, locale_, Locale);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, name_, Name);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, sound_, Sound);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, thumb_, Thumb);
};

#endif // __PICTOGRAM_OBJECT_H__
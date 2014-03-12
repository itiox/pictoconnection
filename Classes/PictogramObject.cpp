/**
 * PictoConnection
 *
 * @file PictogramObject.cpp
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

#include "PictogramObject.h"

USING_NS_CC;

PictogramObject* PictogramObject::create(const char* identifier, const char* locale, const char* name, const char* image, const char* sound, const char* thumb) {
    
    if (identifier == NULL || locale == NULL || name == NULL || image == NULL || sound == NULL) {
        CCLOGERROR("Pictogram couldn't be created [id=%s, locale=%s, name=%s, image=%s, sound=%s, thumb=%s]", identifier, locale, name, image, sound, thumb);
        return NULL;
    }
    
    PictogramObject* pictogram = new PictogramObject();
    if (pictogram && pictogram->init(identifier, locale, name, image, sound, thumb)) {
        pictogram->autorelease();
        return pictogram;
    }
    CC_SAFE_DELETE(pictogram);
    
    return NULL;
}

PictogramObject::PictogramObject() :
identifier_(NULL),
image_(NULL),
locale_(NULL),
name_(NULL),
sound_(NULL),
thumb_(NULL) {}

PictogramObject::~PictogramObject() {
    CC_SAFE_RELEASE_NULL(identifier_);
    CC_SAFE_RELEASE_NULL(image_);
    CC_SAFE_RELEASE_NULL(locale_);
    CC_SAFE_RELEASE_NULL(name_);
    CC_SAFE_RELEASE_NULL(sound_);
    CC_SAFE_RELEASE_NULL(thumb_);
}

bool PictogramObject::init(const char *identifier,
                           const char *locale,
                           const char *name,
                           const char *image,
                           const char *sound,
                           const char *thumb) {
    
    CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
    
    identifier_ = CCString::create(fileUtils->fullPathForFilename(identifier));
    CC_SAFE_RETAIN(identifier_);
    
    image_ = CCString::create(fileUtils->fullPathForFilename(image));
    CC_SAFE_RETAIN(image_);
    
    locale_ = CCString::create(fileUtils->fullPathForFilename(locale));
    CC_SAFE_RETAIN(locale_);
    
    name_ = CCString::create(fileUtils->fullPathForFilename(name));
    CC_SAFE_RETAIN(name_);
    
    sound_ = CCString::create(fileUtils->fullPathForFilename(sound));
    CC_SAFE_RETAIN(sound_);
    
    thumb_ = CCString::create(fileUtils->fullPathForFilename(thumb));
    CC_SAFE_RETAIN(thumb_);
    
    return true;
}
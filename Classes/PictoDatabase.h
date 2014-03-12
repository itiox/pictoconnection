/**
 * PictoConnection
 *
 * @file PictoDatabase.h
 * @brief Methods for managing this app's database
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

#ifndef __PICTO_DATABASE_H__
#define __PICTO_DATABASE_H__

#include "cocos2d.h"

#include "PictogramObject.h"

namespace picto {
    
    namespace database {
        
        void load();
        void unload();
        
        PictogramObject *pictogram(const char* identifier, const char* locale = "es");
        cocos2d::CCArray *childs(const char* identifier, const char* locale = "es");
        size_t countChilds(const char* identifier, const char* locale = "es");
    }
}

#endif // __PICTO_DATABASE_H__
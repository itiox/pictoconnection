/**
 * PictoConnection
 *
 * @file AppDelegate.h
 * @brief Application delegate file
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

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#include "PictoDatabase.h"

/**
 @brief    The cocos2d Application.
 
 The reason for implement as private inheritance is to hide some interface call by CCDirector.
 */
class  AppDelegate : private cocos2d::CCApplication
{
    
public: // constructors
    
    AppDelegate();
    ~AppDelegate();
    
public: // public methods
    
    /**
     @brief    Implement CCDirector and CCScene init code here.
     @return true    Initialize success, app continue.
     @return false   Initialize failed, app terminate.
     */
    bool applicationDidFinishLaunching();
    
    /**
     @brief  The function be called when the application enter background
     @param  the pointer of the application
     */
    void applicationDidEnterBackground();
    
    /**
     @brief  The function be called when the application enter foreground
     @param  the pointer of the application
     */
    void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_


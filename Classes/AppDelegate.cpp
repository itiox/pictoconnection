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

#include "AppDelegate.h"
#include "PictogramGridScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
    picto::database::unload();
}

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    CCDirector* director = CCDirector::sharedDirector();
    director->setOpenGLView(CCEGLView::sharedOpenGLView());
	
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    CCFileUtils *file_utils = CCFileUtils::sharedFileUtils();
    file_utils->addSearchPath("images");
    file_utils->addSearchPath("sounds");
    file_utils->addSearchPath("thumbs");
    
    picto::database::load();
    
    // create a scene. it's an autorelease object
    CCArray* pictograms = CCArray::create(CCString::create("picto_connection"), NULL);
    CCScene *scene = PictogramGrid::scene(pictograms);

    // run
    director->runWithScene(scene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
}

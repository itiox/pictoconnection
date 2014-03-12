/**
 * PictoConnection
 *
 * @file PictogramNode.cpp
 * @brief Visual representation of a pictogram
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

#include "PictogramNode.h"

#include "AppDelegate.h"
#include "PictoDefs.h"

USING_NS_CC;

PictogramNode* PictogramNode::create(PictogramObject* pictogram, const CCSize& size)
{
    PictogramNode *node = new PictogramNode();
    
    if (node && node->init(pictogram, size)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    
    // return the node
    return NULL;
}

PictogramNode::PictogramNode() :
data_(NULL),
ignore_touches_(false),
speaker_background_(NULL),
speaker_button_(NULL),
speaker_icon_(NULL),
speaker_label_(NULL) {}

PictogramNode::~PictogramNode() {
    
    CC_SAFE_RELEASE_NULL(data_);
    removeAllChildrenWithCleanup(true);
    removeFromParentAndCleanup(true);
    CCTextureCache::purgeSharedTextureCache();
}

bool PictogramNode::init(PictogramObject* pictogram, const CCSize& size) {
    
    //////////////////////////////
    // 1. super init first
    
    if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 0), size.width, size.height) ) {
        return false;
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
    
    data_ = pictogram;
    CC_SAFE_RETAIN(data_);
    
    ignoreAnchorPointForPosition(false);
    setTouchEnabled(true);
    
    // Background
    CCSprite* background = CCSprite::create("white_frame.png");
    background->setAnchorPoint(ccp(0.5, 0.5));
    background->setPosition(ccp(0.5*size.width, 0.5*size.height));
    background->setScale(size.width / background->getContentSize().width);
    addChild(background);
    
    // Image sprite
    CCSize image_size = CCSizeMake(4*size.height/5, 4*size.height/5);
    CCSprite* image = NULL;
    
    if (image_size.width <= 128) {
        image = CCSprite::create(pictogram->getThumb()->getCString());
    } else {
        image = CCSprite::create(pictogram->getImage()->getCString());
    }
    
    image->setAnchorPoint(ccp(0.5, 0.5));
    image->setPosition(ccp(0.5*size.width, 3*size.height/5));
    image->setScale(image_size.width/image->getContentSize().width);
    addChild(image);
    
    //////////////////
    // Speaker button
    
    speaker_button_ = CCLayer::create();
    speaker_button_->setContentSize(CCSizeMake(size.width -0.04*size.width, size.height/5 -0.03*size.width));
    speaker_button_->ignoreAnchorPointForPosition(false);
    speaker_button_->setAnchorPoint(ccp(0.5, 0.5));
    speaker_button_->setPosition(ccp(0.5*size.width, 0.5*size.height/5.0));
    
    // Speaker background rounded frame
    speaker_background_ = CCSprite::create("speaker_background.png");
    speaker_background_->setScaleX(speaker_button_->getContentSize().width / speaker_background_->getContentSize().width);
    speaker_background_->setScaleY(speaker_button_->getContentSize().height / speaker_background_->getContentSize().height);
    speaker_background_->ignoreAnchorPointForPosition(false);
    speaker_background_->setAnchorPoint(CCPointZero);
    speaker_background_->setPosition(CCPointZero);
    speaker_button_->addChild(speaker_background_);
    
    // Speaker icon sprite
    speaker_icon_ = CCSprite::create("speaker.png");
    speaker_icon_->setPosition(ccp(0.1*speaker_button_->getContentSize().height,
                                   0.5*speaker_button_->getContentSize().height));
    speaker_icon_->setAnchorPoint(ccp(0, 0.5));
    speaker_icon_->setScale(0.5*speaker_button_->getContentSize().height / speaker_icon_->getContentSize().height);
    speaker_button_->addChild(speaker_icon_);
    
    // Title label
    float font_size = 0.7*speaker_button_->getContentSize().height;
    speaker_label_ = picto::cocos2d_utils::createLabel(pictogram->getName()->getCString(), font_size);
    speaker_label_->ignoreAnchorPointForPosition(false);
    speaker_label_->setAnchorPoint(ccp(0.5, 0.5));
    speaker_label_->setPosition(ccp(0.5*speaker_button_->getContentSize().width,
                                    0.5*speaker_button_->getContentSize().height));
    
    // Scale the label to not overlap the speaker icon
    speaker_label_->setScale(MIN(1, 0.75*speaker_button_->getContentSize().width / speaker_label_->getContentSize().width));
    speaker_button_->addChild(speaker_label_);
    
    addChild(speaker_button_);
    
    return true;
}

void PictogramNode::ignoreTouches(bool ignore) {
    ignore_touches_ = ignore;
}

void PictogramNode::setTarget(cocos2d::CCObject *receiver, cocos2d::SEL_MenuHandler selector) {
    receiver_ = receiver;
    selector_ = selector;
}

bool PictogramNode::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    
    if (speaker_button_ && speaker_button_->boundingBox().containsPoint(convertToNodeSpace(touch->getLocation()))) {
        
        if (picto::cocos2d_utils::playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename(data_->getSound()->getCString()).c_str())) {
            
            // Animate button with a scale up/down effect
            speaker_button_->runAction(CCSequence::create(CCScaleBy::create(0.3f, 1.05),
                                                      CCScaleTo::create(0.7f, 1),
                                                      NULL));
        }
        
        return true;
    }
    else if (!ignore_touches_ && boundingBox().containsPoint(getParent()->convertToNodeSpace(touch->getLocation()))) {
        
        if (receiver_ && selector_) {
            (receiver_->*selector_)(this);
        }
        
        return true;
    }
    
    return false;
}

void PictogramNode::onEnter() {
    CCLayerColor::onEnter();
    setTouchEnabled(true);
    setTouchPriority(1);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    // Adjust colors
    speaker_background_->setColor(picto::resources::speakerButtonBackgroundColor());
    speaker_label_->setColor(picto::resources::speakerButtonTextColor());
    speaker_icon_->setColor(picto::resources::speakerButtonTextColor());
}

void PictogramNode::onExit() {
    CCLayerColor::onExit();
    setTouchEnabled(false);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
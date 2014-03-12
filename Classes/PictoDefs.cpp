/**
 * PictoConnection
 *
 * @file PictoDefs.cpp
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

#include "PictoDefs.h"

#include <clocale>
#include <locale>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/time.h>
#include <vector>

#include <utf8.h>

#include "SimpleAudioEngine.h"

USING_NS_CC;

namespace picto {
    
    namespace cocos2d_utils
    {
        CCLabelTTF* createLabel(const char* text, const float font_size) {
            
            if (CCUserDefault::sharedUserDefault()->getBoolForKey("use_capitals", true))
                return CCLabelTTF::create(picto::conversions::toupper(text).c_str(), "Arial", font_size);
            else
                return CCLabelTTF::create(text, "Arial", font_size);
        }
        
        clock_t g_snd_played_time_ = clock();
        bool playEffect(const char* effect) {
            CCLOG("Play effect: %s", effect);
            
            if (clock() == -1 || (clock() - g_snd_played_time_) > CLOCKS_PER_SEC ) {
                g_snd_played_time_ = clock();
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effect);
                return true;
            }
            
            return false;
        }
    }
    
    namespace conversions
    {
        
        int color2int(ccColor3B color) {
            return (color.r << 16) | (color.g << 8) | color.b;
        }
        
        ccColor3B int2color3B(unsigned int rgb) {
            return ccc3((0xFF0000 & rgb) >> 16, (0x00FF00 & rgb) >> 8, 0x0000FF & rgb);
        }
        
        ccColor4B int2color4B(unsigned int rgb) {
            return ccc4((0xFF0000 & rgb) >> 16, (0x00FF00 & rgb) >> 8, 0x0000FF & rgb, 255);
        }
        
        std::wstring toupper(wchar_t in) {
            wchar_t out = std::toupper(in);
            std::wstring wsout;
            
            if (out != in || isupper(in) || isspace(in) || isdigit(in))
                wsout = out;
            else {
                const std::wstring lowercase = L"áàâäãéèêëíìîïóòôöõúùûüçñ";
                const std::wstring uppercase = L"ÁÀÂÄÃÉÈÊËÍÌÎÏÓÒÔÖÕÚÙÛÜÇÑ";
                size_t pos = lowercase.find(in);
                if (pos != std::string::npos) {
                    wsout = uppercase[pos];
                } else if(in == L'ß')
                    wsout = L"SS";
                else {
                    wsout = out;
                }
            }
            
            return wsout;
        }
        
        std::wstring toupper(std::wstring in) {
            std::wstring out;
            out.reserve(in.length());
            for (int i=0; i < in.length(); i++)
                out += toupper(in[i]);
            
            return out;
        }
        
        std::string toupper(std::string in) {
            
            std::wstring win;
            win.reserve(in.size());
            utf8::utf8to16(in.begin(), in.end(), back_inserter(win));
            std::wstring wout = toupper(win);
            std::string out;
            out.reserve(2*wout.size());
            utf8::utf16to8(wout.begin(), wout.end(), back_inserter(out));
            
            return out;
        }
    }
    
    namespace resources
    {
        ccColor3B backgroundColor() {
            int color = CCUserDefault::sharedUserDefault()->getIntegerForKey("backgroundColor", 0x5cc4dc);
            return conversions::int2color3B(color);
        }
        
        ccColor3B menuTextColor() {
            int color = CCUserDefault::sharedUserDefault()->getIntegerForKey("menuTextColor", 0xffffff);
            return conversions::int2color3B(color);
        }
        
        ccColor3B navigationBarBackgroundColor() {
            int color = CCUserDefault::sharedUserDefault()->getIntegerForKey("navigationBarBackgroundColor", 0x024e68);
            return conversions::int2color3B(color);
        }
        
        ccColor3B navigationBarTextColor() {
            int color = CCUserDefault::sharedUserDefault()->getIntegerForKey("navigationBarTextColor", 0xffffff);
            return conversions::int2color3B(color);
        }
        
        ccColor3B speakerButtonBackgroundColor() {
            int color = CCUserDefault::sharedUserDefault()->getIntegerForKey("speakerButtonBackgroundColor", 0xffff00);
            return conversions::int2color3B(color);
        }
        
        ccColor3B speakerButtonTextColor() {
            int color = CCUserDefault::sharedUserDefault()->getIntegerForKey("speakerButtonTextColor", 0x000000);
            return conversions::int2color3B(color);
        }
        
        ccColor3B settingsButtonBackground() {
            return conversions::int2color3B(0xcccccc);
        }
    }
}
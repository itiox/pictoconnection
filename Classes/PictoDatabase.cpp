/**
 * PictoConnection
 *
 * @file PictoDatabase.cpp
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

#include "PictoDatabase.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

#include "sqlite3.h"

USING_NS_CC;

namespace picto
{
    namespace database
    {
        sqlite3* g_db_ = NULL;
        
        void load()
        {
            CCFileUtils *fileUtils = CCFileUtils::sharedFileUtils();
            
            std::string path = fileUtils->getWritablePath() + "picto_connection.db";
            CCLOG("PictoDatabase path: %s", path.c_str());
            
            unlink(path.c_str());
            
            // If database doesn't exists, copy it from resources
            if(!fileUtils->isFileExist(path)) {
                CCLOG("Copy database from resources");
                
                unsigned long data_size;
                unsigned char* data = fileUtils->getFileData("picto_connection.db", "r", &data_size);
                CCAssert((data != NULL), "Database couldn't be readed from resources. [data == NULL]");
                CCAssert((data_size > 0), "Database couldn't be readed from resources. [data_size <= 0]");
                
                std::ofstream file;
                file.open(path.c_str());
                
                file.write((char*)data, data_size);
                file.close();
                
                delete data;
            }
            
            CCAssert(fileUtils->isFileExist(path), "Database doesn't exists");
            
            g_db_ = NULL;
            int rc = sqlite3_open(path.c_str(), &g_db_);
            CCAssert(rc == SQLITE_OK, sqlite3_errmsg(g_db_));
            
            CCLOG("Database opened successfully");
        }
        
        void unload() {
            if (g_db_ != NULL) {
                CCLOG("Closing database");
                sqlite3_close(g_db_);
                g_db_ = NULL;
            }
        }
        
        static int pictogram_callback(void *data, int argc, char **argv, char **azColName) {
            
            CCArray *results = static_cast<CCArray*>(data);
            
            char *identifier = NULL;
            char *image = NULL;
            char *locale = NULL;
            char *name = NULL;
            char *sound = NULL;
            char *thumb = NULL;
            
            int i;
            for (i=0; i < argc; i++) {
                if (strcmp("id", azColName[i]) == 0)
                    identifier = argv[i];
                else if (strcmp("image", azColName[i]) == 0)
                    image = argv[i];
                else if (strcmp("locale", azColName[i]) == 0)
                    locale = argv[i];
                else if (strcmp("name", azColName[i]) == 0)
                    name = argv[i];
                else if (strcmp("sound", azColName[i]) == 0)
                    sound = argv[i];
                else if (strcmp("thumb", azColName[i]) == 0)
                    thumb = argv[i];
            }
            
            PictogramObject *pictogram = PictogramObject::create(identifier, locale, name, image, sound, thumb);
            if (pictogram != NULL)
                results->addObject(pictogram);
            
            return 0;
        }
        
        PictogramObject *pictogram(const char* identifier, const char* locale) {
            CCAssert(g_db_, "Database isn't loaded");
            
            char sql[256];
            sprintf(sql, "SELECT * FROM pictograms WHERE id='%s'", identifier);
            
            CCArray *data = CCArray::create();
            char *zErrMsg = 0;
            int rc = sqlite3_exec(g_db_, sql, pictogram_callback, (void*)data, &zErrMsg);
            
            if (rc != SQLITE_OK) {
                CCLOGERROR("SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            
            PictogramObject *result = NULL;
            
            CCObject *it;
            CCARRAY_FOREACH(data, it) {
                PictogramObject *pictogram = dynamic_cast<PictogramObject*>(it);
                
                if (strcmp(pictogram->getLocale()->getCString(), locale) == 0) {
                    result = pictogram;
                    break;
                } else if (strcmp(pictogram->getLocale()->getCString(), ""))
                    result = pictogram;
            }
            
            return result;
        }
        
        static int childs_callback(void *data, int argc, char **argv, char **azColName) {
            
            CCArray *results = static_cast<CCArray*>(data);
            
            int i;
            for (i=0; i < argc; i++) {
                if (strcmp("child", azColName[i]) == 0) {
                    results->addObject(CCString::create(argv[i]));
                    break;
                }
            }
            
            return 0;
        }
        
        CCArray *childs(const char* identifier, const char* locale) {
            CCAssert(g_db_, "Database isn't loaded");
            
            CCArray *childs = CCArray::create();
            
            CCArray *childs_ids = CCArray::create();
            
            char sql[256];
            sprintf(sql, "SELECT * FROM relationships WHERE parent='%s'", identifier);
            
            char *zErrMsg = 0;
            int rc = sqlite3_exec(g_db_, sql, childs_callback, (void*)childs_ids, &zErrMsg);
            
            if (rc != SQLITE_OK) {
                CCLOGERROR("SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }
            
            CCObject *it;
            CCARRAY_FOREACH(childs_ids, it) {
                CCString *id = dynamic_cast<CCString*>(it);
                PictogramObject *pictogram = picto::database::pictogram(id->getCString(), locale);
                if (pictogram != NULL)
                    childs->addObject(pictogram);
            }
            
            return childs;
        }
        
        size_t countChilds(const char* identifier, const char* locale) {
            CCAssert(g_db_, "Database isn't loaded");
            
            size_t count = 0;
            
            CCArray *childs_ids = CCArray::create();
            
            char sql[256];
            sprintf(sql, "SELECT * FROM relationships WHERE parent='%s'", identifier);
            
            char *zErrMsg = 0;
            int rc = sqlite3_exec(g_db_, sql, childs_callback, (void*)childs_ids, &zErrMsg);
            
            if (rc != SQLITE_OK) {
                CCLOGERROR("SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } else
                count = childs_ids->count();
            
            return count;
        }
    }
}


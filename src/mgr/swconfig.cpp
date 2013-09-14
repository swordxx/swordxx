/******************************************************************************
 *
 *  swconfig.cpp -	used for saving and retrieval of configuration
 *			information
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <swconfig.h>
#include <utilstr.h>
#include <filemgr.h>
#include <fcntl.h>


SWORD_NAMESPACE_START


SWConfig::SWConfig() {
}


SWConfig::SWConfig(const char * ifilename) {
	filename = ifilename;
	Load();
}


SWConfig::~SWConfig() {
}


void SWConfig::Load() {

	if (!filename.size()) return;	// assert we have a filename

	FileDesc *cfile;
	char *buf, *data;
	SWBuf line;
	ConfigEntMap cursect;
	SWBuf sectname;
	bool first = true;
	
	Sections.erase(Sections.begin(), Sections.end());
	
	cfile = FileMgr::getSystemFileMgr()->open(filename.c_str(), FileMgr::RDONLY);
	if (cfile->getFd() > 0) {
		bool goodLine = FileMgr::getLine(cfile, line);

		// clean UTF encoding tags at start of file
		while (goodLine && line.length() && 
				((((unsigned char)line[0]) == 0xEF) ||
				 (((unsigned char)line[0]) == 0xBB) ||
				 (((unsigned char)line[0]) == 0xBF))) {
			line << 1;
		}
		
		while (goodLine) {
			// ignore commented lines
			if (!line.startsWith("#")) {
				buf = new char [ line.length() + 1 ];
				strcpy(buf, line.c_str());
				if (*strstrip(buf) == '[') {
					if (!first)
						Sections.insert(SectionMap::value_type(sectname, cursect));
					else first = false;
					
					cursect.erase(cursect.begin(), cursect.end());

					strtok(buf, "]");
					sectname = buf+1;
				}
				else {
					strtok(buf, "=");
					if ((*buf) && (*buf != '=')) {
						if ((data = strtok(NULL, "")))
							cursect.insert(ConfigEntMap::value_type(buf, strstrip(data)));
						else cursect.insert(ConfigEntMap::value_type(buf, ""));
					}
				}
				delete [] buf;
			}
			goodLine = FileMgr::getLine(cfile, line);
		}
		if (!first)
			Sections.insert(SectionMap::value_type(sectname, cursect));

		FileMgr::getSystemFileMgr()->close(cfile);
	}
}


void SWConfig::Save() {

	if (!filename.size()) return;	// assert we have a filename

	FileDesc *cfile;
	SWBuf buf;
	SectionMap::iterator sit;
	ConfigEntMap::iterator entry;
	SWBuf sectname;
	
	cfile = FileMgr::getSystemFileMgr()->open(filename.c_str(), FileMgr::RDWR|FileMgr::CREAT|FileMgr::TRUNC);
	if (cfile->getFd() > 0) {
		
		for (sit = Sections.begin(); sit != Sections.end(); sit++) {
			buf =  "\n[";
			buf += (*sit).first.c_str();
			buf += "]\n";
			cfile->write(buf.c_str(), buf.length());
			for (entry = (*sit).second.begin(); entry != (*sit).second.end(); entry++) {
				buf = (*entry).first.c_str();
				buf += "=";
				buf += (*entry).second.c_str();
				buf += "\n";
				cfile->write(buf.c_str(), buf.length());
			}
		}
		buf = "\n";
		cfile->write(buf.c_str(), buf.length());
		FileMgr::getSystemFileMgr()->close(cfile);
	}
}


void SWConfig::augment(SWConfig &addFrom) {

	SectionMap::iterator section;
	ConfigEntMap::iterator entry, start, end;

	for (section = addFrom.Sections.begin(); section != addFrom.Sections.end(); section++) {
		for (entry = (*section).second.begin(); entry != (*section).second.end(); entry++) {
			start = Sections[section->first].lower_bound(entry->first);
			end   = Sections[section->first].upper_bound(entry->first);
			if (start != end) {
				if (((++start) != end)
						|| ((++(addFrom.Sections[section->first].lower_bound(entry->first))) != addFrom.Sections[section->first].upper_bound(entry->first))) {
					for (--start; start != end; start++) {
						if (!strcmp(start->second.c_str(), entry->second.c_str()))
							break;
					}
					if (start == end)
						Sections[(*section).first].insert(ConfigEntMap::value_type((*entry).first, (*entry).second));
				}
				else	Sections[section->first][entry->first.c_str()] = entry->second.c_str();
			}		
			else	Sections[section->first][entry->first.c_str()] = entry->second.c_str();
		}
	}
}


ConfigEntMap & SWConfig::operator [] (const char *section) {
    return Sections[section];
}


SWORD_NAMESPACE_END


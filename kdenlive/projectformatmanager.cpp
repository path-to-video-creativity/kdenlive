/***************************************************************************
                          projectformatmanager  -  description
                             -------------------
    begin                : Wed Dec 3 2003
    copyright            : (C) 2003 by Jason Wood
    email                : jasonwood@blueyonder.co.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "projectformatmanager.h"

#include <qfile.h>

#include <kdebug.h>
#include <kio/job.h>
#include <kio/netaccess.h>
#include <kmimetype.h>
#include <ktempfile.h>
#include <kurl.h>

#include "kdenlivedoc.h"

#include "loadprojectnativefilter.h"
#include "saveprojectnativefilter.h"

ProjectFormatManager::ProjectFormatManager()
{
	m_saveFilters.setAutoDelete(true);
	m_loadFilters.setAutoDelete(true);

	registerLoadFilter(new LoadProjectNativeFilter());
	registerSaveFilter(new SaveProjectNativeFilter());
}


ProjectFormatManager::~ProjectFormatManager()
{
}

bool ProjectFormatManager::openDocument(const KURL& url, KdenliveDoc *document)
{
	if(url.isEmpty()) return false;

	KMimeType::Ptr format = KMimeType::findByURL(url);

	LoadProjectFilter *filter = findLoadFormat(format->name());
	// if(url.filename().right(9) == ".kdenlive")
	if(filter) {
		QString tmpfile;
		if(KIO::NetAccess::download( url, tmpfile )) {
			QFile file(tmpfile);
			if(file.open(IO_ReadOnly)) {
				filter->load(file, document);
    			document->setURL(url);
			}
			KIO::NetAccess::removeTempFile( tmpfile );
			document->setModified(false);
			return true;
		}
	} else {
		document->clipManager().insertClip(url);
	}

	return false;
}

bool ProjectFormatManager::saveDocument(const KURL& url, KdenliveDoc *document)
{
	if(url.isEmpty()) return false;

	KMimeType::Ptr format = KMimeType::findByURL(url);
	SaveProjectFilter *filter = findSaveFormat(format->name());

	if(filter) {
		KTempFile file;
		//file.setAutoDelete(true);

		if( (filter->save(*file.file(), document))) {
			file.close();
			if(!KIO::NetAccess::upload(file.name(), url)) {
				kdError() << "Could not upload file to correct location" << endl;
			}
		} else {
			kdError() << "Save failed" << endl;
		}
	}

	document->setModified(false);
	return true;
}

void ProjectFormatManager::registerSaveFilter(SaveProjectFilter *filter)
{
	m_saveFilters.append(filter);
}

void ProjectFormatManager::registerLoadFilter(LoadProjectFilter *filter)
{
	m_loadFilters.append(filter);
}

LoadProjectFilter *ProjectFormatManager::findLoadFormat(const QString &format)
{
	QPtrListIterator<LoadProjectFilter> itt(m_loadFilters);

	while(itt.current()) {
		if(itt.current()->handlesFormat(format)) {
			return itt.current();
		}
		++itt;
	}

	return 0;
}

SaveProjectFilter *ProjectFormatManager::findSaveFormat(const QString &format)
{
	QPtrListIterator<SaveProjectFilter> itt(m_saveFilters);

	while(itt.current()) {
		if(itt.current()->handlesFormat(format)) {
			return itt.current();
		}
		++itt;
	}

	return 0;
}

/** Returns the mime types that can be loaded in */
QString ProjectFormatManager::loadMimeTypes()
{
	QPtrListIterator<LoadProjectFilter> itt(m_loadFilters);

	QStringList list;

	while(itt.current()) {
		QStringList extraList = itt.current()->handledFormats();

		for(QStringList::Iterator extraItt = extraList.begin();
				extraItt != extraList.end();
				++extraItt) {
			list.append(*extraItt);
		}

		++itt;
	}

	return list.join(" ");
}
	
/** Returns the mime types that can be saved out */
QString ProjectFormatManager::saveMimeTypes()
{
	QPtrListIterator<SaveProjectFilter> itt(m_saveFilters);

	QStringList list;

	while(itt.current()) {
		QStringList extraList = itt.current()->handledFormats();

		for(QStringList::Iterator extraItt = extraList.begin();
				extraItt != extraList.end();
				++extraItt) {
			list.append(*extraItt);
		}

		++itt;
	}

	return list.join(" ");
}

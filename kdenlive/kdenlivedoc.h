/***************************************************************************
                          kdenlivedoc.h  -  description
                             -------------------
    begin                : Fri Feb 15 01:46:16 GMT 2002
    copyright            : (C) 2002 by Jason Wood
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

#ifndef KDENLIVEDOC_H
#define KDENLIVEDOC_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 

#include <qobject.h>
#include <qstring.h>
#include <qvaluelist.h>
#include <qmap.h>

#include <kurl.h>

#include "doctrackbaselist.h"
#include "docclipbaselist.h"
#include "krender.h"
#include "rangelist.h"
#include "clipmanager.h"

// forward declaration of the Kdenlive classes
class KdenliveApp;
class KdenliveView;
class DocClipAVFile;
class DocClipProject;
class DocumentBaseNode;

/**	KdenliveDoc provides a document object for a document-view model.
  *
  * The KdenliveDoc class provides a document object that can be used in conjunction with the classes
  * KdenliveApp and KdenliveView to create a document-view model for standard KDE applications based on
  * KApplication and KMainWindow. Thereby, the document object is created by the KdenliveApp instance and
  * contains the document structure with the according methods for manipulation of the document data by
  * KdenliveView objects. Also, KdenliveDoc contains the methods for serialization of the document data
  * from and to files.
  *
  * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
  * @version KDevelop version 1.2 code generation
  */

class KdenliveDoc : public QObject
{
  Q_OBJECT
  public:
	/** Constructor for the fileclass of the application */
	KdenliveDoc(KdenliveApp *app, QWidget *parent, const char *name=0);

	/** Destructor for the fileclass of the application */
	~KdenliveDoc();

	/** removes a view from the list of currently connected views */
    	void removeView(KdenliveView *view);

	/** returns if the document is modified or not. Use this to determine if your document needs
	 *  saving by the user on closing. */

	bool isModified(){ return m_modified; };
	/** deletes the document's contents */
	void deleteContents();
	/** initializes the document generally */
	bool newDocument();
	/** closes the acutal document */
	void closeDocument();
	/** returns the KURL of the document */
	const KURL& URL() const;
	/** sets the URL of the document */
	void setURL(const KURL& url);
  	/** Itterates through the tracks in the project. This works in the same way
	* as QPtrList::next(), although the underlying structures may be different. */
	DocTrackBase * nextTrack();
  	/** Returns the first track in the project, and resets the itterator to the first track.
	*This effectively is the same as QPtrList::first(), but the underyling implementation
	* may change. */
	DocTrackBase * firstTrack();

	/** Returns true if at least one clip in the project is selected. */
	bool hasSelectedClips();

	/** Returns a clip that is currently selected. Only one clip is returned! 
	 * This function is intended for times when you need a "master" clip. but have no preferred
	 * choice. */
	DocClipRef *selectedClip();
	
	// HACK HACK - we need a way to prevent the document from spewing hundreds of scenelist
	// generation requests - this is it.
	void activeSceneListGeneration(bool active);

	ClipManager &clipManager() { return m_clipManager; }
	
	/** Returns all clips that reference the specified clip. */
	DocClipRefList referencedClips(DocClipBase *clip);

	void setProjectClip(DocClipProject *projectClip);
	DocClipProject &projectClip() { return *m_projectClip; }

  	/** Returns the number of frames per second. */
 	double framesPerSecond() const;
	uint numTracks();
	/** returns the Track which holds the given clip. If the clip does not
	exist within the document, returns 0; */
	DocTrackBase * findTrack(DocClipRef *clip);
	/** Returns the track with the given index, or returns NULL if it does not exist. */
	DocTrackBase * track(int track);
	/** Returns the index value for this track, or -1 on failure.*/
	int trackIndex(DocTrackBase *track) const;
	/** Sets the modified state of the document, if this has changed, emits modified(state) */
	void setModified(bool state);
	/** Returns a scene list generated from the current document. */
	QDomDocument generateSceneList();
	/** Renders the current document timeline to the specified url. */
	void renderDocument(const KURL &url);
	/** Returns renderer associated with this document. */
	KRender * renderer();

	/** returns the duration of the project. */
	GenTime projectDuration() const;
	/** HACK - in some cases, we can modify the document without it knowing - we tell it here
	 * for the moment, although really, this means we have access to things that either we should
	 * only modify via an interface to the document, or that the things that we are modifying should
	 * automatically tell the document. */
	void indirectlyModified();
	/** Moves the currectly selected clips by the offsets specified, or returns false if this
	is not possible. */
	bool moveSelectedClips(GenTime startOffset, int trackOffset);

	/** Return the document clip hierarch */
	DocumentBaseNode *clipHierarch() { return m_clipHierarch; }

	/** Return the document base node with the given name, or null if it does not exist. */
	DocumentBaseNode *findClipNode(const QString &name);

	/** Delete the named documentBaseNode */
	void deleteClipNode(const QString &name);

	/** Add the given base node to the named parent */
	void addClipNode(const QString &parent, DocumentBaseNode *newNode);

	/** Generates a command that will clean the project. The command is returned so that it can
	 * be executed by the client. */
	KMacroCommand *createCleanProjectCommand();
  private:
	/** The base clip for this document. This must be a project clip, as it lists the tracks within
	 * the project, etc. */
	DocClipProject *m_projectClip;

 	/** the modified flag of the current document */
	bool m_modified;
	KURL m_doc_url;

	/** This renderer is for multipurpose use, such as background rendering, and for
	getting the file properties of the various AVFiles. */
	KRender * m_render;
	/** The range of times in the timeline that are currently out of date in the scene list.
	 * This list is used to re-sync the scene list. */
	RangeList<GenTime> m_invalidSceneTimes;
	/** Application pointer. */
	KdenliveApp * m_app;
	/** This is the scenelist that get's passed from the clip to a renderer. */
	QDomDocument m_domSceneList;
	/** HACK HACK - generate scenelist if true, don't if false) */
	bool m_sceneListGeneration;


	/** The clip hierarchy for this project. Clips can be put into groups. */
	DocumentBaseNode *m_clipHierarch;
	/** Clip manager maintains the list of clips that exist in the document. */
	ClipManager m_clipManager;
	/** Connects the various project clip signals/slots up to the document. This should be done whenever
	a new document project clip is created.*/
	void connectProjectClip();
public slots:
  	/** Adds a sound track to the project */
  	void addSoundTrack();
  	/** Adds an empty video track to the project */
  	void addVideoTrack();
	/** Called when an error occurs whilst retrieving a file's properties. */
	void AVFilePropertiesError(const QString &path, const QString &errmsg);
private slots: // Private slots
	/** Called when the document is modifed in some way. */
	void hasBeenModified();
	/** Emitted when a particular clip has changed in someway. E.g, it has recieved it's duration. */
	void clipChanged(DocClipBase *file);
signals: // Signals
  	/** This signal is emitted whenever tracks are added to or removed from the project. */
  	void trackListChanged();
 	/** This is signal is emitted whenever the avFileList changes, either through the addition
	 * or removal of an AVFile, or when an AVFile changes. */
  	void clipListUpdated();
	/** Emitted when the modified state of the document changes. */
	void modified(bool);
	/** Emitted when a particular clip has changed in someway. E.g, it has recieved it's duration. */
	void clipChanged(DocClipRef *file);
	/** Emitted when a particular clip has been deleted.*/
	void nodeDeleted(DocumentBaseNode *);
	/** emitted when the document has changed in some way. */
	void documentChanged();
	/** Also emitted when the document has changed in some way, fires off the project clip with it */
	void documentChanged(DocClipBase *);
	/** Emitted whenever a clip gets selected. */
	void signalClipSelected(DocClipRef *);
};

#endif // KDENLIVEDOC_H

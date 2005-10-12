/***************************************************************************
                          trackviewbackgrounddecorator  -  description
                             -------------------
    begin                : Fri Nov 28 2003
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
#ifndef TRACKVIEWAUDIOBACKGROUNDDECORATOR_H
#define TRACKVIEWAUDIOBACKGROUNDDECORATOR_H

#include <doctrackdecorator.h>

#include <qcolor.h>

#include <ktimeline.h>

/**
Draws the base preview image for a clip; in correct acpect ration and stretch togiven height .

@author Marco Gittler
*/
namespace Gui{

class TrackViewAudioBackgroundDecorator : public DocTrackDecorator
{
public:
	TrackViewAudioBackgroundDecorator(KTimeLine* timeline,
									KdenliveDoc* doc,
									const QColor &selected,
									const QColor &unselected,
									int size=-1);

	virtual ~TrackViewAudioBackgroundDecorator();

    virtual void paintClip(double startX, double endx, QPainter &painter, DocClipRef *clip, QRect &rect, bool selected) ;
private:
	int m_height;
	QColor m_selected;
	QColor m_unselected;
};

};
#endif

/*
    Copyright 2007-2008 by Robert Knight <robertknight@gmail.com>
    Copyright 2020 by Tomaz Canabrava <tcanabrava@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301  USA.
*/

#ifndef FILTER_CHAIN
#define FILTER_CHAIN

#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QRegion>

#include "HotSpot.h"

class QLeaveEvent;
class QPainter;

namespace Konsole
{
class Filter;
class HotSpot;
class TerminalDisplay;

/**
 * A chain which allows a group of filters to be processed as one.
 * The chain owns the filters added to it and deletes them when the chain itself is destroyed.
 *
 * Use addFilter() to add a new filter to the chain.
 * When new text to be filtered arrives, use addLine() to add each additional
 * line of text which needs to be processed and then after adding the last line, use
 * process() to cause each filter in the chain to process the text.
 *
 * After processing a block of text, the reset() method can be used to set the filter chain's
 * internal cursor back to the first line.
 *
 * The hotSpotAt() method will return the first hotspot which covers a given position.
 *
 * The hotSpots() method return all of the hotspots in the text and on
 * a given line respectively.
 */
class FilterChain
{
public:
    explicit FilterChain(TerminalDisplay *terminalDisplay);
    virtual ~FilterChain();

    /** Adds a new filter to the chain.  The chain will delete this filter when it is destroyed */
    void addFilter(Filter *filter);
    /** Removes a filter from the chain.  The chain will no longer delete the filter when destroyed */
    void removeFilter(Filter *filter);
    /** Removes all filters from the chain */
    void clear();

    /** Resets each filter in the chain */
    void reset();
    /**
     * Processes each filter in the chain
     */
    void process();

    /** Sets the buffer for each filter in the chain to process. */
    void setBuffer(const QString *buffer, const QList<int> *linePositions);

    /** Returns the first hotspot which occurs at @p line, @p column or 0 if no hotspot was found */
    QSharedPointer<HotSpot> hotSpotAt(int line, int column) const;
    /** Returns a list of all the hotspots in all the chain's filters */
    QList<QSharedPointer<HotSpot>> hotSpots() const;

    /* Returns the region of the hotspot inside of the TerminalDisplay */
    QRegion hotSpotRegion() const;

    /* Returns the amount of hotspots of the given type */
    int count(HotSpot::Type type) const;
    QList<QSharedPointer<HotSpot>> filterBy(HotSpot::Type type) const;

    void mouseMoveEvent(TerminalDisplay *td, QMouseEvent *ev, int charLine, int charColumn);
    void mouseReleaseEvent(TerminalDisplay *td, QMouseEvent *ev, int charLine, int charColumn);
    void keyPressEvent(TerminalDisplay *td, QKeyEvent *ev, int charLine, int charColumn);
    void keyReleaseEvent(TerminalDisplay *td, QKeyEvent *ev, int charLine, int charColumn);
    void leaveEvent(TerminalDisplay *td, QEvent *ev);

    void paint(TerminalDisplay *td, QPainter &painter);

    void setReverseUrlHints(bool value);
    void setUrlHintsModifiers(Qt::KeyboardModifiers value);
protected:
    QList<Filter *> _filters;
    TerminalDisplay *_terminalDisplay;
    QSharedPointer<HotSpot> _hotSpotUnderMouse;

    /* TODO: this should be profile related, not here. but
    * currently this removes a bit of code from TerminalDisplay,
    * so it's a good compromise
    * */
    bool _showUrlHint;
    bool _reverseUrlHints;
    Qt::KeyboardModifiers _urlHintsModifiers;
};

}
#endif

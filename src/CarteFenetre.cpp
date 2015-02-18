/*************************************************************************
 *    Copyright (C) 2007 by Romain Campioni                              *
 *    Copyright (C) 2009 by Renaud Guezennec                             *
 *    Copyright (C) 2011 by Joseph Boudou                                *
 *                                                                       *
 *      http://www.rolisteam.org/                                        *
 *                                                                       *
 *   Rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/


#include "CarteFenetre.h"

#include <QtGui>

#include "Carte.h"


#include "variablesGlobales.h"



CarteFenetre::CarteFenetre(Carte *uneCarte,MainWindow* mainWindow, QWidget *parent)
    : QScrollArea(parent),m_mainWindow(mainWindow)
{
    setObjectName("CarteFenetre");
    setWindowIcon(QIcon(":/resources/icones/vignette plan.png"));
    setFocusPolicy(Qt::StrongFocus);
    setAlignment(Qt::AlignCenter);

    carteAssociee = uneCarte;
    setWidget(carteAssociee);

    #ifdef WIN32
        resize(carteAssociee->width()+2, carteAssociee->height()+2);
    #elif defined (MACOS)
        resize(carteAssociee->width()+4, carteAssociee->height()+4);
    #elif defined Q_WS_X11
        resize(carteAssociee->width()+2, carteAssociee->height()+2);
    #endif

    connect(carteAssociee, SIGNAL(commencerDeplacementCarteFenetre(QPoint)),
            this, SLOT(commencerDeplacement(QPoint)));
    connect(carteAssociee, SIGNAL(deplacerCarteFenetre(QPoint)),
            this, SLOT(deplacer(QPoint)));
}


CarteFenetre::~CarteFenetre()
{
    actionAssociee->~QAction();
    m_mainWindow->enleverCarteDeLaListe(carteAssociee->identifiantCarte());
}

void CarteFenetre::closeEvent(QCloseEvent *event)
{
    hide();
    actionAssociee->setChecked(false);
    event->ignore();
}


void CarteFenetre::associerAction(QAction *action)
{
    actionAssociee = action;
}


Carte * CarteFenetre::carte()
{
    return carteAssociee;
}


void CarteFenetre::commencerDeplacement(QPoint position)
{
    pointDepart = position;
    horizontalDepart = horizontalScrollBar()->value();
    verticalDepart = verticalScrollBar()->value();
}

void CarteFenetre::deplacer(QPoint position)
{

    QPoint diff = pointDepart - position;
    horizontalScrollBar()->setValue(horizontalDepart + diff.x());
    verticalScrollBar()->setValue(verticalDepart + diff.y());
}


void CarteFenetre::focusInEvent(QFocusEvent * event)
{
    emit activated(carteAssociee);

    QWidget::focusInEvent(event);
}

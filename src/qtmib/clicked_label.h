/*
 * Copyright (C) 2013-2014 RCP100 Team (rcpteam@yahoo.com)
 *
 * This file is part of qtmib project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef CLICKED_LABEL_H
#define CLICKED_LABEL_H
#include <QLabel>

class ClickedLabel : public QLabel
{
Q_OBJECT
	public:
	ClickedLabel( const QString & text, QWidget * parent = 0 );
	~ClickedLabel(){}

signals:
	void clicked();

public slots:
	void slotClicked();

protected:
	void mousePressEvent ( QMouseEvent * event ) ;

};
#endif

/*
 File: main.cpp
 Created on: 04/06/2016
 Author: Felix de las Pozas Alvarez

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Project
#include <SimpleHasher.h>

// Qt
#include <QApplication>
#include <QDebug>

// C++
#include <iostream>

//-----------------------------------------------------------------
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  const char symbols[] =
  { 'I', 'E', '!', 'X' };

  QString output = QString("[%1] %2").arg(symbols[type]).arg(msg);
  std::cerr << output.toStdString() << std::endl;
  if (type == QtFatalMsg) abort();
}

//-----------------------------------------------------------------
int main(int argc, char *argv[])
{
  QStringList files;
  if(argc != 1)
  {
    for(int i = 1; i < argc; ++i)
    {
      files << QString(argv[i]);
    }
  }

  qInstallMessageHandler(myMessageOutput);

	QApplication app(argc, argv);

	SimpleHasher hasher{files};
	hasher.show();

	return app.exec();
}

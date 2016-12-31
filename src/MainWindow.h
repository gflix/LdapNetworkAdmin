/*
 * MainWindow.h
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>

namespace Flix {

class MainWindow: public QMainWindow {
    Q_OBJECT

    QAction* actionConnect;
    QAction* actionDisconnect;
    QAction* actionQuit;

    void initActions(void);
    void initMenuBar(void);
public:
    MainWindow();
    virtual ~MainWindow();

public slots:
    void showConnectionsDialog(void);
};

} /* namespace Flix */

#endif /* SRC_MAINWINDOW_H_ */

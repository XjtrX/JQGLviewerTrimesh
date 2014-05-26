#ifndef JMAINWINDOW_H
#define JMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include "JQGLViewerTriMesh/jqglvewertrimesh.h"
namespace Ui {
class JMainWindow;
}

class JMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JMainWindow(QWidget *parent = 0);
    ~JMainWindow();

private slots:
    void on_actionLoadSourceMesh_triggered();

    void on_actionLoadTargetMesh_triggered();

private:
    Ui::JMainWindow *ui;
    JQGLVewerTriMesh * jViewer;
};

#endif // JMAINWINDOW_H

#include "jmainwindow.h"
#include "ui_jmainwindow.h"

JMainWindow::JMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JMainWindow)
{
    ui->setupUi(this);
    jViewer = new JQGLVewerTriMesh();
    this->setCentralWidget(jViewer);
}

JMainWindow::~JMainWindow()
{
    delete ui;
    if (jViewer)
        delete jViewer;
}

void JMainWindow::on_actionLoadSourceMesh_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File1"),
                                                    "D:/Projects/correspondence/data",
                                                    "File(*.obj *.OBJ *OFF *off *.PLY *.ply)");
    if (!fileName.isEmpty())
    {
        this->statusBar()->showMessage(tr("Opening %1 ...").arg(fileName));
        if(jViewer->readMesh(fileName.toLocal8Bit().data(),jViewer->sourceMesh))
            this->statusBar()->showMessage(tr("Opening %1 ... Done.").arg(fileName));
        else
        {
            this->statusBar()->showMessage(tr("Opening %1 ... Error.").arg(fileName));
            jViewer->readSource = false;
        }
    }
}

void JMainWindow::on_actionLoadTargetMesh_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File1"),
                                                    "D:/Projects/correspondence/data",
                                                    "File(*.obj *.OBJ *OFF *off *.PLY *.ply)");
    if (!fileName.isEmpty())
    {
        this->statusBar()->showMessage(tr("Opening %1 ...").arg(fileName));
        if(jViewer->readMesh(fileName.toLocal8Bit().data(),jViewer->targetMesh))
            this->statusBar()->showMessage(tr("Opening %1 ... Done.").arg(fileName));
        else
        {
            this->statusBar()->showMessage(tr("Opening %1 ... Error.").arg(fileName));
            jViewer->readTarget = false;
        }
    }
}

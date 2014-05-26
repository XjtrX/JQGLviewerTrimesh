#ifndef JQGLVEWERTRIMESH_H
#define JQGLVEWERTRIMESH_H
/*
 * Date:2014-5-17 by JTR@BNU
 * Descripte: combine the TriMesh and QGLViewer libraries
 * Change LOGS:
 * 1>. date:2014-5-19 by JTR@BNU
 *      a. 添加了鼠标选点操作，按住SHIFT使用鼠标左键进行选点，使用拖动鼠标进行矩形选点比较容易，直接选点比较困难。
 * 0>. date:2014-5-17 by JTR@BNU
 *      b. 创建工程，基本可以将QGLViewer和TriMesh两个库结合到一起
 *
 * */
#include <qglviewer.h>
#include "TriMesh.h"
#include "object.h"
#include "frame.h"

//using namespace trimesh;

class JQGLVewerTriMesh : public QGLViewer
{
    Q_OBJECT
public:
    explicit JQGLVewerTriMesh();
    ~JQGLVewerTriMesh();

signals:

public slots:




//copy from QGLViewer mutiselect-example
protected:
    virtual void draw();
    virtual void init();
    virtual QString helpString() const;

    // Selection functions
    virtual void drawWithNames();
    virtual void endSelection(const QPoint&);

    // Mouse events functions
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

    //------JTR@BNU for TriMesh
public:
    bool readMesh(const char *filename, trimesh::TriMesh *&triMesh/*, const char* xffilename = ""*/);
    void drawmesh(trimesh::TriMesh *& mesh);
    void drawTriangleMesh(trimesh::TriMesh *& mesh);
private :
  void startManipulation();
  void drawSelectionRectangle() const;
  void addIdToSelection(int id);
  void removeIdFromSelection(int id);

  // Current rectangular selection
  QRect rectangle_;

  // Different selection modes
  enum SelectionMode { NONE, ADD, REMOVE };
  SelectionMode selectionMode_;

  QList<Object*> objects_;
  QList<int> selection_;

  //-----------------------JTR@BNU
  //begin to combine the trimesh library
public:
    trimesh::TriMesh *sourceMesh, *targetMesh;//两个三角形网格
    qglviewer::Frame sourceFrame,targetFrame;//对应两个三角形网格的位置旋转等等信息
    QList<int> selectSource,selectTarget; //记录选择的点
    bool readSource,readTarget; //读取标记
  //end of combining the trimesh library
};

#endif // JQGLVEWERTRIMESH_H

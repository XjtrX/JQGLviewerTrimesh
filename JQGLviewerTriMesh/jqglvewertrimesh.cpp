#include "jqglvewertrimesh.h"

#include "manipulatedFrameSetConstraint.h"

#include <manipulatedFrame.h>

#include <QMouseEvent>

using namespace qglviewer;

JQGLVewerTriMesh::JQGLVewerTriMesh()

{
    selectionMode_ = NONE;

        // Fill the scene with objects positionned on a regular grid.
        // Consider increasing selectBufferSize() if you use more objects.
//    const int nb = 10;
//    for (int i=-nb; i<=nb; ++i)
//        for (int j=-nb; j<=nb; ++j)
//        {
//            Object* o = new Object();
//            o->frame.setPosition(Vec(i/float(nb), j/float(nb), 0.0));
//            objects_.append(o);
//        }

    // jtr init
    //resize(840,720);
    sourceMesh = NULL;
    targetMesh = NULL;
    readSource = false;
    readTarget = false;
    selectSource.clear();
    selectTarget.clear();

}
JQGLVewerTriMesh::~JQGLVewerTriMesh()
{
    if(sourceMesh)
    {
        delete sourceMesh;
    }
    if(targetMesh)
    {
        delete targetMesh;
    }
}

void JQGLVewerTriMesh::init()
{
  // A ManipulatedFrameSetConstraint will apply displacements to the selection
  setManipulatedFrame(new ManipulatedFrame());
  manipulatedFrame()->setConstraint(new ManipulatedFrameSetConstraint());

  // Used to display semi-transparent relection rectangle
  //glBlendFunc(GL_ONE, GL_ONE);
  glClearColor(.0f,1.0f,1.0f,1.0f);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  restoreStateFromFile();
  //help();
}

QString JQGLVewerTriMesh::helpString() const
{
  QString text("<h2>m u l t i S e l e c t </h2>");
  text += "This example illustrates an application of the <code>select()</code> function that ";
  text += "enables the selection of several objects.<br><br>";
  text += "Object selection is preformed using the left mouse button. Press <b>Shift</b> to add objects ";
  text += "to the selection, and <b>Alt</b> to remove objects from the selection.<br><br>";
  text += "Individual objects (click on them) as well as rectangular regions (click and drag mouse) can be selected. ";
  text += "To do this, the selection region size is modified and the <code>endSelection()</code> function ";
  text += "has been overloaded so that <i>all</i> the objects of the region are taken into account ";
  text += "(the default implementation only selects the closest object).<br><br>";
  text += "The selected objects can then be manipulated by pressing the <b>Control</b> key. ";
  text += "Other set operations (parameter edition, deletion...) can also easily be applied to the selected objects.";
  return text;
}


//  D r a w i n g   f u n c t i o n

void JQGLVewerTriMesh::draw()
{
  // Draws selected objects only.
//  glColor3f(0.9f, 0.3f, 0.3f);
//    for (QList<int>::const_iterator it=selection_.begin(), end=selection_.end(); it != end; ++it)
//      objects_.at(*it)->draw();

//  // Draws all the objects. Selected ones are not repainted because of GL depth test.
//  glColor3f(0.8f, 0.8f, 0.8f);
//  for (int i=0; i<int(objects_.size()); i++)
//    objects_.at(i)->draw();

  // Draws manipulatedFrame (the set's rotation center)
  if (manipulatedFrame()->isManipulated())
    {
      glPushMatrix();
      glMultMatrixd(manipulatedFrame()->matrix());
      drawAxis(0.5);
      glPopMatrix();
    }

  // Draws rectangular selection area. Could be done in postDraw() instead.
  if (selectionMode_ != NONE)
    drawSelectionRectangle();

  //-------------JTR@BNU

  if (readSource)
  {
    glPushMatrix();


//    glScalef(1.0f/sourceMesh->bsphere.r,
//             1.0f/sourceMesh->bsphere.r,
//             1.0f/sourceMesh->bsphere.r);

    glMultMatrixd(sourceFrame.matrix());
    //glMultMatrixd(manipulatedFrame()->matrix());
   // glTranslated(-sourceMesh->bsphere.center.at(0),-sourceMesh->bsphere.center.at(1),-sourceMesh->bsphere.center.at(2));

    //绘制选中的点
    glColor3f(0.0,.0,1.0);
    glBegin(GL_POINTS);
    for(auto itr = selectSource.begin();itr != selectSource.end();++itr)
    {
        trimesh::point p = sourceMesh->vertices.at(*itr);
        glVertex3fv(p);
    }
    glEnd();
    glPointSize(3);
    glPointSize(1);
//  绘制选中的点结束
    glColor3f(0.8f,0.8f,0.8f);
//    drawmesh(sourceMesh);
    drawTriangleMesh(sourceMesh);

    glPopMatrix();
  }

  if(readTarget)
  {

      glPushMatrix();
      glColor3f(0.0,1.0,0.0);
//      glScalef(1.0f/targetMesh->bsphere.r,
//               1.0f/targetMesh->bsphere.r,
//               1.0f/targetMesh->bsphere.r);
      glMultMatrixd(targetFrame.matrix());
      //glMultMatrixd(manipulatedFrame()->matrix());
     // glTranslated(-sourceMesh->bsphere.center.at(0),-sourceMesh->bsphere.center.at(1),-sourceMesh->bsphere.center.at(2));
//      drawmesh(targetMesh);
      drawTriangleMesh(targetMesh);
      glPopMatrix();
  }
}


//   C u s t o m i z e d   m o u s e   e v e n t s

void JQGLVewerTriMesh::mousePressEvent(QMouseEvent* e)
{
    // Start selection. Mode is ADD with Shift key and TOGGLE with Alt key.
    rectangle_ = QRect(e->pos(), e->pos());

    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ShiftModifier))
        selectionMode_ = ADD;
    else
        if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
            selectionMode_ = REMOVE;
        else
        {
            if (e->modifiers() == Qt::ControlModifier)
                startManipulation();
            QGLViewer::mousePressEvent(e);
        }
}

void JQGLVewerTriMesh::mouseMoveEvent(QMouseEvent* e)
{
  if (selectionMode_ != NONE)
    {
      // Updates rectangle_ coordinates and redraws rectangle
      rectangle_.setBottomRight(e->pos());
      updateGL();
    }
  else
    QGLViewer::mouseMoveEvent(e);
}

void JQGLVewerTriMesh::mouseReleaseEvent(QMouseEvent* e)
{
  if (selectionMode_ != NONE)
    {
      // Actual selection on the rectangular area.
      // Possibly swap left/right and top/bottom to make rectangle_ valid.
      rectangle_ = rectangle_.normalized();
      // Define selection window dimensions
      setSelectRegionWidth(rectangle_.width());
      setSelectRegionHeight(rectangle_.height());
      // Compute rectangle center and perform selection
      select(rectangle_.center());
      // Update display to show new selected objects
      updateGL();
    }
  else
    QGLViewer::mouseReleaseEvent(e);
}


//   C u s t o m i z e d   s e l e c t i o n   p r o c e s s

void JQGLVewerTriMesh::drawWithNames()
{
//  for (int i=0; i<int(objects_.size()); i++)
//    {
//      glPushName(i);
//      objects_.at(i)->draw();
//      glPopName();
//    }
    glPushName(0);
  if(readSource)
  {
      glPushMatrix();
      glLoadName(1);
      glMultMatrixd(sourceFrame.matrix());

//      glScalef(1.0f/sourceMesh->bsphere.r,
//               1.0f/sourceMesh->bsphere.r,
//               1.0f/sourceMesh->bsphere.r);

      glPointSize(6);
      for(size_t i = 0;i<sourceMesh->vertices.size();++i)
      {
          trimesh::point tem_p = sourceMesh->vertices.at(i);
          glPushName(i);
          glBegin(GL_POINTS);
          glVertex3fv(tem_p);
          glEnd();
          glPopName();
      }
      glPointSize(1);
      glPopMatrix();
  }
//  std::cerr<<"*********Name***********"<<std::endl;
  if(readTarget)
  {
      glPushMatrix();
      glMultMatrixd(targetFrame.matrix());
//      glScalef(1.0f/targetMesh->bsphere.r,
//               1.0f/targetMesh->bsphere.r,
//               1.0f/targetMesh->bsphere.r);
      glLoadName(2);
      glPointSize(2);
      for(size_t i = 0;i<targetMesh->vertices.size();++i)
      {
          trimesh::point tem_p = targetMesh->vertices.at(i);
          glPushName(i);
          glBegin(GL_POINTS);
          glVertex3fv(tem_p);
          glEnd();
          glPopName();
      }
      glPointSize(1);
      glPopMatrix();
  }
}

void JQGLVewerTriMesh::endSelection(const QPoint&)
{
  // Flush GL buffers
  glFlush();

  // Get the number of objects that were seen through the pick matrix frustum. Reset GL_RENDER mode.
  GLint nbHits = glRenderMode(GL_RENDER);
  std::cerr<<"*********Has Select***********"<<std::endl<<nbHits<<std::endl;
  if (nbHits > 0)
    {
      //std::cerr<<"*********Has Select***********"<<std::endl;
      // Interpret results : each object created 4 values in the selectBuffer().
      // (selectBuffer())[4*i+3] is the id pushed on the stack.
      std::cerr<<"********Select********"<<std::endl;
      for (int i=0; i<nbHits; ++i)
    switch (selectionMode_)
      {
      case ADD    : {
        std::cerr<<"["<<(selectBuffer())[5*i+3]<<" , "<<(selectBuffer())[5*i+4]<<"];"<<std::endl;
        if((selectBuffer())[5*i+3] ==  1)
        {
            if(!selectSource.contains((selectBuffer())[5*i+4]))
                selectSource.append((selectBuffer())[5*i+4]);
        }else if((selectBuffer())[5*i+3] ==  2)
        {
            if(!selectTarget.contains((selectBuffer())[5*i+4]))
                selectTarget.append((selectBuffer())[5*i+4]);
        }

            //addIdToSelection((selectBuffer())[4*i+3]);

        break;
        }
      case REMOVE : {
          if((selectBuffer())[5*i+3] ==  1)
          {
              selectSource.removeAll((selectBuffer())[5*i+4]);
          }else if((selectBuffer())[5*i+3] ==  2)
          {
              selectTarget.removeAll((selectBuffer())[5*i+4]);
          }
        //removeIdFromSelection((selectBuffer())[4*i+3]);
          break;
    }
      default : break;
      }
    }
  selectionMode_ = NONE;
}

void JQGLVewerTriMesh::startManipulation()
{
  Vec averagePosition;
  ManipulatedFrameSetConstraint* mfsc = (ManipulatedFrameSetConstraint*)(manipulatedFrame()->constraint());
  mfsc->clearSet();

  for (QList<int>::const_iterator it=selection_.begin(), end=selection_.end(); it != end; ++it)
    {
      mfsc->addObjectToSet(objects_[*it]);
      averagePosition += objects_[*it]->frame.position();
    }

  if (selection_.size() > 0)
    manipulatedFrame()->setPosition(averagePosition / selection_.size());
}


//   S e l e c t i o n   t o o l s

void JQGLVewerTriMesh::addIdToSelection(int id)
{
  if (!selection_.contains(id))
    selection_.push_back(id);
}

void JQGLVewerTriMesh::removeIdFromSelection(int id)
{
  selection_.removeAll(id);
}

void JQGLVewerTriMesh::drawSelectionRectangle() const
{
  startScreenCoordinatesSystem();
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);

  glColor4f(0.0, 0.0, 0.3f, 0.3f);
  glBegin(GL_QUADS);
  glVertex2i(rectangle_.left(),  rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(),  rectangle_.bottom());
  glEnd();

  glLineWidth(2.0);
  glColor4f(0.4f, 0.4f, 0.5f, 0.5f);
  glBegin(GL_LINE_LOOP);
  glVertex2i(rectangle_.left(),  rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.top());
  glVertex2i(rectangle_.right(), rectangle_.bottom());
  glVertex2i(rectangle_.left(),  rectangle_.bottom());
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  stopScreenCoordinatesSystem();
}

//-------------------------begin to combine Trimesh library add by JTR@BNU 2014-5-17

bool JQGLVewerTriMesh::readMesh(const char *filename, trimesh::TriMesh *&triMesh/*, const char *xffilename*/)
{
    using namespace std;

    if(triMesh)
    {
        delete triMesh;
        triMesh = NULL;
    }
    triMesh = trimesh::TriMesh::read(filename);
    if(!triMesh)
    {
        std::cerr<<"read file "<<filename<<" error."<<endl;
//        exit(-1);

        return false;
    }
    triMesh->need_bsphere();

//归一化，直接修改读进来的点的坐标，xyz同时进行bsphere.r的放缩。
    for(auto itr = triMesh->vertices.begin(); itr != triMesh->vertices.end();++itr)
    {
        (*itr) /= triMesh->bsphere.r;
    }
//    triMesh->bsphere.valid = false;//为了重新计算包围球的中心和半径，中心应该变为除以r，半径变为1
//    triMesh->need_bsphere();
    //下面的两句等效于上面两句，使用下面效率或许高
    triMesh->bsphere.center /= triMesh->bsphere.r;
    triMesh->bsphere.r = 1.0;
//归一化
    triMesh->need_normals();
    triMesh->need_tstrips();
    //----
    trimesh::point p = triMesh->bsphere.center;
    if(triMesh == sourceMesh)
    {
        sourceFrame.setPosition(
                    (p[0]*-1.0) - 0.5 * triMesh->bsphere.r,
                    p[1] * -1.0,
                    p[2] * -1.0);

        readSource = true;
    }
    else if(triMesh == targetMesh)
    {
//        targetFrame.setPosition(p[0],p[1],p[2]);
//        targetFrame.translate(triMesh->bsphere.r,0,0);
        targetFrame.setPosition(
                    (p[0]*-1.0) + 0.5 * triMesh->bsphere.r,
                    p[1] * -1.0,
                    p[2] * -1.0);
        readTarget = true;
//        std::cerr<<"**********Target**********"<<endl<<p[0]<<endl;
    }
    //std::cerr<<"********************"<<endl<<p[0]<<endl;
    return true;
}

void JQGLVewerTriMesh::drawmesh(trimesh::TriMesh *&mesh)
{
    if(!mesh) return;
    if(mesh->vertices.empty())
        return;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &mesh->vertices[0][0]);

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINTS);
    //glColor3f(.0f, 1.0f, .0f);
    glDrawArrays(GL_POINTS, 0, mesh->vertices.size());

    glDisableClientState(GL_VERTEX_ARRAY);
}

void JQGLVewerTriMesh::drawTriangleMesh(trimesh::TriMesh *&mesh)
{
    if(!mesh) return;
    if(mesh->vertices.empty()) return;

//    glColor3f(1.0,.0,.0);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (size_t i = 0;i < mesh->faces.size();++i)
    {
        glBegin(GL_TRIANGLES);
        glNormal3fv(mesh->normals.at((mesh->faces.at(i)[0])));
        glVertex3fv(mesh->vertices.at((mesh->faces.at(i)[0])));
        glNormal3fv(mesh->normals.at((mesh->faces.at(i)[1])));
        glVertex3fv(mesh->vertices.at((mesh->faces.at(i)[1])));
        glNormal3fv(mesh->normals.at((mesh->faces.at(i)[2])));
        glVertex3fv(mesh->vertices.at((mesh->faces.at(i)[2])));
        glEnd();
    }

}







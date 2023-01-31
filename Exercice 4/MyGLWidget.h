// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();


  protected:
    virtual void initializeGL ();
    virtual void iniMaterialTerra ();
    virtual void carregaShaders();
    virtual void iniEscena ();
    virtual void modelTransformMorty();
    virtual void modelTransformFantasma();
    virtual void viewTransform();
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void projectTransform();
    virtual void paintGL();

    GLuint posfoc1loc,posfoc2loc,posfoc3loc,posfoc4loc,colfoc1loc,colfoc2loc, posfoc5loc,focoMloc;
    glm::vec3 posfoc1, posfoc2, posfoc3,posfoc4, colfoc1, posfoc5, colfoc2;
    float girMorty = 90;
    bool lila = true;
    bool groc = true;
    float suma = 0.0;
    bool enxampatpat = false;

  private:
    int printOglError(const char file[], int line, const char func[]);
};

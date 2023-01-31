#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void paintGL ();
    virtual void RickTransform();
    virtual void PortalTransform ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent (QMouseEvent *e);
    virtual void MortyTransform ();
    virtual void iniEscena();
    virtual void iniCamera();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void MortyMalo();
    virtual void MortyMaloTransform ();
    virtual void carregaShaders();
    GLuint maloLoc;


  private:
    int printOglError(const char file[], int line, const char func[]);
//     float girTheta, girPsi;
    float znear, zfar, fov, ra, radi;
    float girRick = 180;
    float girPortal = 0;
    float girMorty, girMortyMalo;
    bool P = true;
    bool encendido = true;
    bool pulsada = false;
    bool morty = false;
    bool nou = true;

};

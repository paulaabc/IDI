// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) {
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget(){
}

void MyGLWidget::iniEscena (){
  centreEsc = glm::vec3(5,3,5);
  radiEsc = 8;

  posfoc1 = glm::vec3(0.0,6.0,0.0);
  posfoc2 = glm::vec3(0.0,6.0,10.0);
  posfoc3 = glm::vec3(10.0,6.0,0.0);
  posfoc4 = glm::vec3(10.0,6.0,10.0);
  colfoc1 = glm::vec3(0.7,0.0,0.7);

  posfoc5 = glm::vec3(60.0,-90.0,-7.0);
  colfoc2 = glm::vec3(0.9,0.9,0.0);

  glUniform3fv(posfoc1loc,1,&posfoc1[0]); //actualizar fragment shader
  glUniform3fv(posfoc2loc,1,&posfoc2[0]);
  glUniform3fv(posfoc3loc,1,&posfoc3[0]);
  glUniform3fv(posfoc4loc,1,&posfoc4[0]);
  glUniform3fv(posfoc5loc,1,&posfoc5[0]);
  glUniform3fv(colfoc1loc,1,&colfoc1[0]); //actualizar fragment shader
  glUniform3fv(colfoc2loc,1,&colfoc2[0]);
}

void MyGLWidget::initializeGL (){
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  carregaShaders();

  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersTerraIParets();

  iniEscena();
  iniCamera();
}

void MyGLWidget::paintGL () 
{
//   En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
//   useu els paràmetres que considereu (els que hi ha són els de per defecte)
  glViewport (0, 0, ample, alt);
  
//   Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
//   MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
//   FANTASMA
  if(not lila and not enxampatpat){
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma ();
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);}

  
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders(){
    LL4GLWidget::carregaShaders();

    posfoc1loc = glGetUniformLocation (program->programId(), "POSFOCO1");
    posfoc2loc = glGetUniformLocation (program->programId(), "POSFOCO2");
    posfoc3loc = glGetUniformLocation (program->programId(), "POSFOCO3");
    posfoc4loc = glGetUniformLocation (program->programId(), "POSFOCO4");
    colfoc1loc = glGetUniformLocation (program->programId(), "COLORMAGENTA");

    focoMloc = glGetUniformLocation (program->programId(), "TGMorty");
    posfoc5loc = glGetUniformLocation (program->programId(), "POSLINTERNA");
    colfoc2loc = glGetUniformLocation (program->programId(), "COLORAMARILLO");

}

void MyGLWidget::projectTransform (){
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(float(M_PI/3.0), ra, radiEsc, 3.0f*radiEsc);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform (){
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::modelTransformMorty (){
  TG_morty = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG_morty = glm::rotate(TG_morty, float(glm::radians(girMorty)),glm::vec3(0.f, 1.f, 0.f));
  TG_morty = glm::scale(TG_morty, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG_morty = glm::translate(TG_morty, -centreBaseMorty);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG_morty[0][0]);
  glUniformMatrix4fv (focoMloc, 1, GL_FALSE, &TG_morty[0][0]);
}

void MyGLWidget::modelTransformFantasma ()
{
  glm::mat4 TG; /*suma*/
  TG = glm::translate(glm::mat4(1.f), glm::vec3(1 + suma,0.5,5));
  TG = glm::rotate(TG, float(glm::radians(90.0)),glm::vec3(0.f, 1.f, 0.f));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::iniMaterialTerra (){
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0.1,0.1);
  diff = glm::vec3(0.4,0.4,0.4);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      girMorty += 45;
      if(girMorty >= 360)girMorty -= 360.0;
      if(girMorty > 180) enxampatpat = true;
      if(girMorty <= 180) enxampatpat = false;
      if (not enxampatpat and not lila) suma+=0.5;
      if(5-(1+suma) <= 0.5 and not lila and not enxampatpat){ // if (glm::distance(glm::vec3(1,0.5,5) + posFantasma, glm::vec3(5.0,0.5,5.0)) <= 0.5)reset(); deberia ser asi :)
        lila = true;
        groc = true;
        suma = 0;
        girMorty = 90;
        colfoc2 = glm::vec3(0.9,0.9,0.0);
        glUniform3fv(colfoc2loc,1,&colfoc2[0]);
        colfoc1 = glm::vec3(0.7,0.0,0.7);
        glUniform3fv(colfoc1loc,1,&colfoc1[0]);
      }
//         projectTransform();
//         viewTransform();
    break;
	}
  case Qt::Key_D: {
      girMorty -= 45;
      if (girMorty < 0)girMorty += 360.0;
      if(girMorty > 180) enxampatpat = true;
      if(girMorty <= 180) enxampatpat = false;

      if (not enxampatpat and not lila) suma+=0.5;
      if(5-(1+suma) <= 0.5 and not lila and not enxampatpat){
        lila = true;
        groc = true;
        suma = 0;
        girMorty = 90;
        colfoc2 = glm::vec3(0.9,0.9,0.0);
        glUniform3fv(colfoc2loc,1,&colfoc2[0]);
        colfoc1 = glm::vec3(0.7,0.0,0.7);
        glUniform3fv(colfoc1loc,1,&colfoc1[0]);
      }
//       viewTransform();
//       projectTransform();
    break;
	}
  case Qt::Key_M: {
      groc = !groc;
      if(not groc)colfoc2 = glm::vec3(0.0,0.0,0.0);
      if(groc) colfoc2 = glm::vec3(0.9,0.9,0.0);
      glUniform3fv(colfoc2loc,1,&colfoc2[0]);
      //paintGL(); no hace falta
    break;
	}
  case Qt::Key_E: {
      lila = !lila;
      if(not lila) colfoc1 = glm::vec3(0.0,0.0,0.0);
      if(lila) colfoc1 = glm::vec3(0.7,0.0,0.7);
      glUniform3fv(colfoc1loc,1,&colfoc1[0]);
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  viewTransform();
  projectTransform();
  update();
}

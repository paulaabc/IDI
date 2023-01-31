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

// funcio per iniciar la escena al resetear 
void MyGLWidget::iniEscena(){ 
   radi = sqrt(7.5*7.5+5*5+1.5*1.5);
   radiEscena = radi;
   centreEscena = glm::vec3(0,1.5,0);
}

void MyGLWidget::iniCamera(){
   fov = asin(radiEscena/(radiEscena*2))*2;
   ra = 1.0f;
   znear = radiEscena;
   zfar  = radiEscena*3;

   factorAngleX = glm::radians(45.0f);
   factorAngleY = glm::radians(0.0f);

   projectTransform();
   viewTransform();
}

void MyGLWidget::projectTransform (){

   glm::mat4 Proj(1.0f);
   if(!pulsada)  Proj = glm::perspective (fov, ra, znear, zfar); // valors inicials
   else  Proj = glm::perspective (fov, ra, 0.25f, zfar); // 1a persona cambies znear
   glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

// cambiar la camara de 3a persona a 1a persona IMP
void MyGLWidget::viewTransform (){
     //Matriu de posició i orientació de l'observador
    glm::mat4 View(1.0f);
    if(!pulsada){ // 3a persona
        View = glm::translate(View, glm::vec3(0,0,-radiEscena*2));
        View = glm::rotate(View,factorAngleX,glm::vec3(1,0,0));
        View = glm::rotate(View,-factorAngleY,glm::vec3(0,1,0));
        View = glm::translate(View, glm::vec3(-centreEscena));
    }
    else{ // cabeza morty 1a persona 
        View = glm::translate(View, glm::vec3(0,-0.8,0));
        View = glm::rotate(View,glm::radians(-girMorty),glm::vec3(0,1,0));
        View = glm::rotate(View,glm::radians(-180.0f),glm::vec3(0,1,0));
        View = glm::translate(View, glm::vec3(-posMorty));
    }
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::paintGL (){
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Rick
  glBindVertexArray (VAO_models[RICK]);
  RickTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

  // Morty
  glBindVertexArray (VAO_models[MORTY]);
  MortyTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);

  if(morty){ // es per el morty malo 
        glBindVertexArray (VAO_models[MORTY]);
        MortyMaloTransform();
        glUniform1i(maloLoc, 1);
        glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
        glUniform1i(maloLoc, 0);
    }

  // Portal
  if (P){ //unicament si toca pintar el portal
    glBindVertexArray (VAO_models[PORTAL]);
    PortalTransform();
    glDrawArrays(GL_TRIANGLES, 0, models[PORTAL].faces().size()*3);
  }

  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

// portar al rick a l'inici escalar girar i moure al punt inicial
void MyGLWidget::RickTransform (){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-2.5, 0, 0));
  TG = glm::rotate(TG,glm::radians(girRick),glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
  TG = glm::translate(TG, -centreBaseModels[RICK]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

//hi han dos morties calen dos tg una per cadascun

// portar el morty a l'inici escalar girar i moure al punt inicial
void MyGLWidget::MortyTransform (){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, posMorty);
  TG = glm::rotate(TG,glm::radians(girMorty), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  TGMorty = TG;
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

// portar el morty malo a l'inici escalar girar i moure al punt inicial
void MyGLWidget::MortyMaloTransform (){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-2.5,0,0));
  TG = glm::rotate(TG,glm::radians(girPortal),glm::vec3(0,1,0));
  TG = glm::translate(TG,glm::vec3(0,0,-3));
  TG = glm::rotate(TG,glm::radians(girMortyMalo), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
  TG = glm::translate(TG, -centreBaseModels[MORTY]);
  TGMorty2 = TG;
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PortalTransform (){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(-2.5,0,0));
  TG = glm::rotate(TG,glm::radians(girPortal),glm::vec3(0,1,0));
  TG = glm::translate(TG,glm::vec3(0,0,-3));
  TG = glm::scale(TG, glm::vec3(0.25,escalaModels[PORTAL],escalaModels[PORTAL]));
  TG = glm::translate(TG, -centreBaseModels[PORTAL]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyMalo(){
   if(LL2GLWidget::MortyPortalTest(glm::radians(girRick),posMorty)){
        morty = true;
        P = false;
        girMortyMalo = girMorty;
    }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
        case Qt::Key_Q:{
            girRick += 45;
            if (girRick >= 360)
                girRick -= 360.0;
            encendido = true;
        break;
        }
        case Qt::Key_E:{
            girRick -= 45;
            if (girRick <= 0)
                girRick += 360.0;
            encendido = true;
        break;
        }
        case Qt::Key_P:{
            if(encendido){
                P = true;
                encendido = false;
            }
            else P = !P;
            morty = false;
            girPortal = girRick+180;
        break;
        }
        case Qt::Key_Up: {
            if (girMorty == 0){
                posMorty = posMorty + glm::vec3(0,0.0,1)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 45){
                posMorty = posMorty + glm::vec3(sqrt(2),0.0,sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 90){
                posMorty = posMorty + glm::vec3(1,0.0,0)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 135){
                posMorty = posMorty + glm::vec3(sqrt(2),0.0,-sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 180){
                posMorty = posMorty + glm::vec3(0,0.0,-1)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 225){
                posMorty = posMorty + glm::vec3(-sqrt(2),0.0,-sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 270){
                posMorty = posMorty + glm::vec3(-1,0.0,0)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else{
                posMorty = posMorty + glm::vec3(-sqrt(2),0.0,sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
        MortyMalo();
        break;
        }
        case Qt::Key_Down: {
            if (girMorty == 0){
                posMorty = posMorty - glm::vec3(0,0.0,1)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 45){
                posMorty = posMorty - glm::vec3(sqrt(2),0.0,sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 90){
                posMorty = posMorty - glm::vec3(1,0.0,0)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 135){
                posMorty = posMorty - glm::vec3(sqrt(2),0.0,-sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 180){
                posMorty = posMorty - glm::vec3(0,0.0,-1)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 225){
                posMorty = posMorty - glm::vec3(-sqrt(2),0.0,-sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else if (girMorty == 270){
                posMorty = posMorty - glm::vec3(-1,0.0,0)*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            else{
                posMorty = posMorty - glm::vec3(-sqrt(2),0.0,sqrt(2))*glm::vec3(0.1);
                projectTransform();
                viewTransform();
            }
            MortyMalo();
        break;
        }
        case Qt::Key_Left:{
            girMorty += 45;
            if (girMorty >= 360)
                girMorty -= 360.0;
            projectTransform();
            viewTransform();
        break;
        }
        case Qt::Key_Right:{
            girMorty -= 45;
            if (girMorty < 0)
                girMorty += 360.0;
            viewTransform();
            projectTransform();
        break;
        }
        case Qt::Key_R:{
            P = true;
            girRick = 180;
            girPortal = 0;
            girMorty = 0;
            posMorty = glm::vec3(1,0,0);
            factorAngleX = glm::radians(45.0f);
            factorAngleY = glm::radians(0.0f);
            morty = false;
            encendido = true;
            znear = radiEscena;
            viewTransform();
        break;
        }
        case Qt::Key_C:{
            pulsada = !pulsada;
            projectTransform();
            viewTransform();
        break;
        }
        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  makeCurrent();
  if(!pulsada){
    if(DoingInteractive == ROTATE){
        factorAngleY+= (e->x()-xClick)*M_PI/ample;
        factorAngleX+= (yClick-e->y())*M_PI/alt;
        viewTransform();
    }
    update ();
  }
}

void MyGLWidget::mousePressEvent (QMouseEvent *e){
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::carregaShaders(){
  LL2GLWidget::carregaShaders();
  maloLoc  = glGetUniformLocation (program->programId(), "malo");
}




#include "MyQLabel.h"
#include <cmath>

MyQLabel:: MyQLabel(QWidget *parent): QLabel(parent){
}

void MyQLabel::reset(){
    setStyleSheet("color:red");
    QLabel::setNum(0);
    color();
}

void MyQLabel::NotaFinal(double notaF){
    nf = notaF;
    update();

}


void MyQLabel::NotaLab(double notaL){
    nl = notaL;
    update();
}


void MyQLabel::entregable(bool e){
    if(e) entregables++;
    else entregables--;
    update();
}


void MyQLabel::NotaParcial(double notaP){
     np = notaP;
     update();
}

void MyQLabel::update(){
    if (nl >= 4.5){
        ne = pow((entregables/4.0),2.2);
        notafinal = nf*0.50 + np*0.25 + (nl+ne)*0.25;
    }
    else notafinal = nf*0.50 + np*0.25 + nl*0.25;
    notafinal = round(notafinal*100)/100;

    if(notafinal > 10 )QLabel::setNum(10);
    else QLabel::setNum(notafinal);
    color();
}

void MyQLabel::color(){
    if(notafinal >= 5.00) setStyleSheet("color:green");
    else setStyleSheet("color:red");
}

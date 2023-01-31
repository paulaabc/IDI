#include "MyQLCDNumber.h"

MyQLCDNumber::MyQLCDNumber(QWidget *parent): QLCDNumber(parent){
}

void MyQLCDNumber::reset(){
    QLCDNumber::display(10);
}

void MyQLCDNumber::NotaLabmin(double notaL){
    nlm = notaL * 0.25;
    update();
}

void MyQLCDNumber::NotaParcialmin(double notaP){
      npm = notaP * 0.25;
      update();
}

void MyQLCDNumber::update(){
    notamin = (5 - (npm + nlm))/ 0.5;
    if(notamin > 0)QLCDNumber::display(notamin);
    else QLCDNumber::display(0);
}

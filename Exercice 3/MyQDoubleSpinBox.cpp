#include "MyQDoubleSpinBox.h"

MyQDoubleSpinBox::MyQDoubleSpinBox(QWidget *parent): QDoubleSpinBox(parent){
}

void MyQDoubleSpinBox::reset(){
    QDoubleSpinBox::setValue(0);
}

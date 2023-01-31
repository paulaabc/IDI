#include "MyQCheckBox.h"

MyQCheckBox::MyQCheckBox(QWidget *parent): QCheckBox(parent){
}

void MyQCheckBox::reset(){

    if(QCheckBox::isChecked()){
        QCheckBox::setChecked(false);
    }

}

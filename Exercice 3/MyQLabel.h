#include <QLabel>

class MyQLabel: public QLabel
{
    Q_OBJECT
    public:
    double nf = 0;
    double np = 0;
    double nl = 0;
    double ne = 0;
    double notafinal = 0;
    double entregables = 0;
    MyQLabel(QWidget *parent);

    public slots:
    void reset();
    void color();
    void NotaFinal(double);
    void NotaLab(double);
    void NotaParcial(double);
    void update();
    void entregable(bool);


    signals:
    void returnPressed (const QString &);
//maximo dos decimales porfavor i falta checkbox de lab

};

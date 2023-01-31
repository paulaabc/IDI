#include <QLCDNumber>

class MyQLCDNumber: public QLCDNumber
{
    Q_OBJECT
    public:
    MyQLCDNumber(QWidget *parent);
    double notamin = 0;
    double nlm = 0;
    double npm = 0;


    public slots:
    void reset();
    void NotaLabmin(double);
    void NotaParcialmin(double);
    void update();

    signals:
    void returnPressed (const QString &);
};

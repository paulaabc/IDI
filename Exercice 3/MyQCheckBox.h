#include <QCheckBox>

class MyQCheckBox: public QCheckBox{
    Q_OBJECT
    public:
    MyQCheckBox(QWidget *parent);

    public slots:
    void reset();


    signals:
    void returnPressed (const QString &);
};

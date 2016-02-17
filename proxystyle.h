#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H
#include <QProxyStyle>
#include <QObject>
#include <QWidget>
class ProxyStyle : public QProxyStyle
{
    Q_OBJECT
public:
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;

};

#endif // PROXYSTYLE_H

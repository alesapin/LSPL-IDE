#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H

#include <QProxyStyle>
class ProxyStyle : public QProxyStyle
{
public:
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const;

};

#endif // PROXYSTYLE_H

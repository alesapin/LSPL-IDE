#include "proxystyle.h"




int ProxyStyle::styleHint(QStyle::StyleHint hint, const QStyleOption *option,
                          const QWidget *widget, QStyleHintReturn *returnData) const {
    //потому что на gtk не отображается галочка
    //у combobox.
    if(hint == QStyle::SH_ComboBox_Popup){
        return int(false);
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

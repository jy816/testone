/****************************************************************************
** Meta object code from reading C++ file 'inputdialog.h'
**
** Created: Mon Jul 8 23:13:11 2019
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "inputdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'inputdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_inputDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      36,   12,   12,   12, 0x08,
      47,   12,   12,   12, 0x08,
      71,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_inputDialog[] = {
    "inputDialog\0\0on_etrButton_clicked()\0"
    "TimerOut()\0on_pushButton_clicked()\0"
    "on_pushButton_savequit_clicked()\0"
};

void inputDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        inputDialog *_t = static_cast<inputDialog *>(_o);
        switch (_id) {
        case 0: _t->on_etrButton_clicked(); break;
        case 1: _t->TimerOut(); break;
        case 2: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData inputDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject inputDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_inputDialog,
      qt_meta_data_inputDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &inputDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *inputDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *inputDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_inputDialog))
        return static_cast<void*>(const_cast< inputDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int inputDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'start_Monitor.h'
**
** Created: Mon May 16 16:03:28 2016
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "start_Monitor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'start_Monitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_start_Monitor[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_start_Monitor[] = {
    "start_Monitor\0\0Time_out()\0timer_update()\0"
};

void start_Monitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        start_Monitor *_t = static_cast<start_Monitor *>(_o);
        switch (_id) {
        case 0: _t->Time_out(); break;
        case 1: _t->timer_update(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData start_Monitor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject start_Monitor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_start_Monitor,
      qt_meta_data_start_Monitor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &start_Monitor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *start_Monitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *start_Monitor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_start_Monitor))
        return static_cast<void*>(const_cast< start_Monitor*>(this));
    return QObject::qt_metacast(_clname);
}

int start_Monitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void start_Monitor::Time_out()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'serialthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../newbalance/serialthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serialthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SerialThread_t {
    QByteArrayData data[14];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SerialThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SerialThread_t qt_meta_stringdata_SerialThread = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SerialThread"
QT_MOC_LITERAL(1, 13, 17), // "changeStartButton"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 23), // "changeCalibrationButton"
QT_MOC_LITERAL(4, 56, 15), // "errorSerialOpen"
QT_MOC_LITERAL(5, 72, 9), // "errorCode"
QT_MOC_LITERAL(6, 82, 19), // "refreshSpeedDisplay"
QT_MOC_LITERAL(7, 102, 3), // "fre"
QT_MOC_LITERAL(8, 106, 6), // "dataOk"
QT_MOC_LITERAL(9, 113, 3), // "d11"
QT_MOC_LITERAL(10, 117, 3), // "d12"
QT_MOC_LITERAL(11, 121, 3), // "d21"
QT_MOC_LITERAL(12, 125, 3), // "d22"
QT_MOC_LITERAL(13, 129, 1) // "f"

    },
    "SerialThread\0changeStartButton\0\0"
    "changeCalibrationButton\0errorSerialOpen\0"
    "errorCode\0refreshSpeedDisplay\0fre\0"
    "dataOk\0d11\0d12\0d21\0d22\0f"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SerialThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    1,   41,    2, 0x06 /* Public */,
       6,    1,   44,    2, 0x06 /* Public */,
       8,    5,   47,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal, QMetaType::QReal,    9,   10,   11,   12,   13,

       0        // eod
};

void SerialThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SerialThread *_t = static_cast<SerialThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeStartButton(); break;
        case 1: _t->changeCalibrationButton(); break;
        case 2: _t->errorSerialOpen((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->refreshSpeedDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->dataOk((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3])),(*reinterpret_cast< qreal(*)>(_a[4])),(*reinterpret_cast< qreal(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SerialThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SerialThread::changeStartButton)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SerialThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SerialThread::changeCalibrationButton)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SerialThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SerialThread::errorSerialOpen)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SerialThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SerialThread::refreshSpeedDisplay)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (SerialThread::*_t)(qreal , qreal , qreal , qreal , qreal );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SerialThread::dataOk)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject SerialThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SerialThread.data,
      qt_meta_data_SerialThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SerialThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SerialThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SerialThread.stringdata0))
        return static_cast<void*>(const_cast< SerialThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SerialThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SerialThread::changeStartButton()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void SerialThread::changeCalibrationButton()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void SerialThread::errorSerialOpen(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SerialThread::refreshSpeedDisplay(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SerialThread::dataOk(qreal _t1, qreal _t2, qreal _t3, qreal _t4, qreal _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

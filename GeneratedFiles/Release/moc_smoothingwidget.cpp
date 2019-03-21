/****************************************************************************
** Meta object code from reading C++ file 'smoothingwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../smoothingwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smoothingwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmoothingWidget_t {
    QByteArrayData data[19];
    char stringdata0[254];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SmoothingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SmoothingWidget_t qt_meta_stringdata_SmoothingWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SmoothingWidget"
QT_MOC_LITERAL(1, 16, 14), // "erosionClicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "neighborMode"
QT_MOC_LITERAL(4, 45, 15), // "dilationClicked"
QT_MOC_LITERAL(5, 61, 14), // "openingClicked"
QT_MOC_LITERAL(6, 76, 12), // "erosionTimes"
QT_MOC_LITERAL(7, 89, 13), // "dilationTimes"
QT_MOC_LITERAL(8, 103, 14), // "closingClicked"
QT_MOC_LITERAL(9, 118, 11), // "closeWidget"
QT_MOC_LITERAL(10, 130, 10), // "dragWidget"
QT_MOC_LITERAL(11, 141, 1), // "x"
QT_MOC_LITERAL(12, 143, 1), // "y"
QT_MOC_LITERAL(13, 145, 8), // "dragDone"
QT_MOC_LITERAL(14, 154, 10), // "moveWidget"
QT_MOC_LITERAL(15, 165, 21), // "on_pb_erosion_clicked"
QT_MOC_LITERAL(16, 187, 22), // "on_pb_dilation_clicked"
QT_MOC_LITERAL(17, 210, 21), // "on_pb_opening_clicked"
QT_MOC_LITERAL(18, 232, 21) // "on_pb_closing_clicked"

    },
    "SmoothingWidget\0erosionClicked\0\0"
    "neighborMode\0dilationClicked\0"
    "openingClicked\0erosionTimes\0dilationTimes\0"
    "closingClicked\0closeWidget\0dragWidget\0"
    "x\0y\0dragDone\0moveWidget\0on_pb_erosion_clicked\0"
    "on_pb_dilation_clicked\0on_pb_opening_clicked\0"
    "on_pb_closing_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmoothingWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       5,    3,   80,    2, 0x06 /* Public */,
       8,    3,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   94,    2, 0x08 /* Private */,
      10,    2,   95,    2, 0x08 /* Private */,
      13,    2,  100,    2, 0x08 /* Private */,
      14,    2,  105,    2, 0x08 /* Private */,
      15,    0,  110,    2, 0x08 /* Private */,
      16,    0,  111,    2, 0x08 /* Private */,
      17,    0,  112,    2, 0x08 /* Private */,
      18,    0,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    6,    7,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    7,    6,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SmoothingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SmoothingWidget *_t = static_cast<SmoothingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->erosionClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->dilationClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->openingClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->closingClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: _t->closeWidget(); break;
        case 5: _t->dragWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->dragDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->moveWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->on_pb_erosion_clicked(); break;
        case 9: _t->on_pb_dilation_clicked(); break;
        case 10: _t->on_pb_opening_clicked(); break;
        case 11: _t->on_pb_closing_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (SmoothingWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmoothingWidget::erosionClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (SmoothingWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmoothingWidget::dilationClicked)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (SmoothingWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmoothingWidget::openingClicked)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (SmoothingWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmoothingWidget::closingClicked)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SmoothingWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SmoothingWidget.data,
      qt_meta_data_SmoothingWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SmoothingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmoothingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmoothingWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SmoothingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SmoothingWidget::erosionClicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SmoothingWidget::dilationClicked(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SmoothingWidget::openingClicked(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SmoothingWidget::closingClicked(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'freeformwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../freeformwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'freeformwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FreeFormWidget_t {
    QByteArrayData data[15];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FreeFormWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FreeFormWidget_t qt_meta_stringdata_FreeFormWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "FreeFormWidget"
QT_MOC_LITERAL(1, 15, 24), // "freeFormParameterChecked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 6), // "Method"
QT_MOC_LITERAL(4, 48, 6), // "Radius"
QT_MOC_LITERAL(5, 55, 11), // "closeWidget"
QT_MOC_LITERAL(6, 67, 10), // "dragWidget"
QT_MOC_LITERAL(7, 78, 1), // "x"
QT_MOC_LITERAL(8, 80, 1), // "y"
QT_MOC_LITERAL(9, 82, 8), // "dragDone"
QT_MOC_LITERAL(10, 91, 10), // "moveWidget"
QT_MOC_LITERAL(11, 102, 7), // "confirm"
QT_MOC_LITERAL(12, 110, 11), // "clickCircle"
QT_MOC_LITERAL(13, 122, 11), // "clickSquare"
QT_MOC_LITERAL(14, 134, 8) // "clickBox"

    },
    "FreeFormWidget\0freeFormParameterChecked\0"
    "\0Method\0Radius\0closeWidget\0dragWidget\0"
    "x\0y\0dragDone\0moveWidget\0confirm\0"
    "clickCircle\0clickSquare\0clickBox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FreeFormWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   64,    2, 0x08 /* Private */,
       6,    2,   65,    2, 0x08 /* Private */,
       9,    2,   70,    2, 0x08 /* Private */,
      10,    2,   75,    2, 0x08 /* Private */,
      11,    0,   80,    2, 0x08 /* Private */,
      12,    1,   81,    2, 0x08 /* Private */,
      13,    1,   84,    2, 0x08 /* Private */,
      14,    1,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void FreeFormWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FreeFormWidget *_t = static_cast<FreeFormWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->freeFormParameterChecked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->closeWidget(); break;
        case 2: _t->dragWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->dragDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->moveWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->confirm(); break;
        case 6: _t->clickCircle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->clickSquare((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->clickBox((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (FreeFormWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FreeFormWidget::freeFormParameterChecked)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FreeFormWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FreeFormWidget.data,
      qt_meta_data_FreeFormWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FreeFormWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FreeFormWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FreeFormWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FreeFormWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FreeFormWidget::freeFormParameterChecked(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

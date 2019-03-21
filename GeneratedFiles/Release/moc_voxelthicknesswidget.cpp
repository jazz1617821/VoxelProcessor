/****************************************************************************
** Meta object code from reading C++ file 'voxelthicknesswidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../voxelthicknesswidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'voxelthicknesswidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VoxelThicknessWidget_t {
    QByteArrayData data[16];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VoxelThicknessWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VoxelThicknessWidget_t qt_meta_stringdata_VoxelThicknessWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "VoxelThicknessWidget"
QT_MOC_LITERAL(1, 21, 9), // "thickDone"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 10), // "VoxelItem*"
QT_MOC_LITERAL(4, 43, 4), // "item"
QT_MOC_LITERAL(5, 48, 8), // "minValue"
QT_MOC_LITERAL(6, 57, 8), // "maxValue"
QT_MOC_LITERAL(7, 66, 11), // "closeWidget"
QT_MOC_LITERAL(8, 78, 10), // "dragWidget"
QT_MOC_LITERAL(9, 89, 1), // "x"
QT_MOC_LITERAL(10, 91, 1), // "y"
QT_MOC_LITERAL(11, 93, 8), // "dragDone"
QT_MOC_LITERAL(12, 102, 7), // "confirm"
QT_MOC_LITERAL(13, 110, 6), // "cancel"
QT_MOC_LITERAL(14, 117, 15), // "setSelectedItem"
QT_MOC_LITERAL(15, 133, 5) // "index"

    },
    "VoxelThicknessWidget\0thickDone\0\0"
    "VoxelItem*\0item\0minValue\0maxValue\0"
    "closeWidget\0dragWidget\0x\0y\0dragDone\0"
    "confirm\0cancel\0setSelectedItem\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VoxelThicknessWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   56,    2, 0x08 /* Private */,
       8,    2,   57,    2, 0x08 /* Private */,
      11,    2,   62,    2, 0x08 /* Private */,
      12,    0,   67,    2, 0x08 /* Private */,
      13,    0,   68,    2, 0x08 /* Private */,
      14,    1,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float, QMetaType::Float,    4,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void VoxelThicknessWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VoxelThicknessWidget *_t = static_cast<VoxelThicknessWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->thickDone((*reinterpret_cast< VoxelItem*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 1: _t->closeWidget(); break;
        case 2: _t->dragWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->dragDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->confirm(); break;
        case 5: _t->cancel(); break;
        case 6: _t->setSelectedItem((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VoxelThicknessWidget::*_t)(VoxelItem * , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelThicknessWidget::thickDone)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VoxelThicknessWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VoxelThicknessWidget.data,
      qt_meta_data_VoxelThicknessWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VoxelThicknessWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VoxelThicknessWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VoxelThicknessWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VoxelThicknessWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void VoxelThicknessWidget::thickDone(VoxelItem * _t1, float _t2, float _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

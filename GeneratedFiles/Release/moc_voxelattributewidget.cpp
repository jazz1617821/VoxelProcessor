/****************************************************************************
** Meta object code from reading C++ file 'voxelattributewidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../voxelattributewidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'voxelattributewidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VoxelAttributeWidget_t {
    QByteArrayData data[11];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VoxelAttributeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VoxelAttributeWidget_t qt_meta_stringdata_VoxelAttributeWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "VoxelAttributeWidget"
QT_MOC_LITERAL(1, 21, 12), // "stateChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "updateAttribute"
QT_MOC_LITERAL(4, 51, 12), // "VoxelObject*"
QT_MOC_LITERAL(5, 64, 2), // "vo"
QT_MOC_LITERAL(6, 67, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(7, 84, 2), // "it"
QT_MOC_LITERAL(8, 87, 1), // "i"
QT_MOC_LITERAL(9, 89, 30), // "clearVoxelObjectAttributePanel"
QT_MOC_LITERAL(10, 120, 14) // "changeDFEnable"

    },
    "VoxelAttributeWidget\0stateChanged\0\0"
    "updateAttribute\0VoxelObject*\0vo\0"
    "QTreeWidgetItem*\0it\0i\0"
    "clearVoxelObjectAttributePanel\0"
    "changeDFEnable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VoxelAttributeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   42,    2, 0x0a /* Public */,
       3,    2,   45,    2, 0x0a /* Public */,
       9,    0,   50,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int,    7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void VoxelAttributeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VoxelAttributeWidget *_t = static_cast<VoxelAttributeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->updateAttribute((*reinterpret_cast< VoxelObject*(*)>(_a[1]))); break;
        case 2: _t->updateAttribute((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->clearVoxelObjectAttributePanel(); break;
        case 4: _t->changeDFEnable((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VoxelAttributeWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelAttributeWidget::stateChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VoxelAttributeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VoxelAttributeWidget.data,
      qt_meta_data_VoxelAttributeWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VoxelAttributeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VoxelAttributeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VoxelAttributeWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VoxelAttributeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void VoxelAttributeWidget::stateChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

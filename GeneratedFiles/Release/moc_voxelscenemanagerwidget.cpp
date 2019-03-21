/****************************************************************************
** Meta object code from reading C++ file 'voxelscenemanagerwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../voxelscenemanagerwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'voxelscenemanagerwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VoxelSceneManagerWidget_t {
    QByteArrayData data[25];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VoxelSceneManagerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VoxelSceneManagerWidget_t qt_meta_stringdata_VoxelSceneManagerWidget = {
    {
QT_MOC_LITERAL(0, 0, 23), // "VoxelSceneManagerWidget"
QT_MOC_LITERAL(1, 24, 19), // "actionOnVoxelObject"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 12), // "VoxelObject*"
QT_MOC_LITERAL(4, 58, 3), // "obj"
QT_MOC_LITERAL(5, 62, 6), // "action"
QT_MOC_LITERAL(6, 69, 12), // "deleteObject"
QT_MOC_LITERAL(7, 82, 14), // "sendVoxelScene"
QT_MOC_LITERAL(8, 97, 11), // "VoxelModel*"
QT_MOC_LITERAL(9, 109, 2), // "sc"
QT_MOC_LITERAL(10, 112, 16), // "updateVoxelModel"
QT_MOC_LITERAL(11, 129, 17), // "toggleContextMenu"
QT_MOC_LITERAL(12, 147, 3), // "pos"
QT_MOC_LITERAL(13, 151, 13), // "voxelSelected"
QT_MOC_LITERAL(14, 165, 10), // "VoxelItem*"
QT_MOC_LITERAL(15, 176, 4), // "item"
QT_MOC_LITERAL(16, 181, 12), // "itemSelected"
QT_MOC_LITERAL(17, 194, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(18, 211, 5), // "witem"
QT_MOC_LITERAL(19, 217, 5), // "index"
QT_MOC_LITERAL(20, 223, 25), // "openImageProjectionWidget"
QT_MOC_LITERAL(21, 249, 17), // "deleteVoxelObject"
QT_MOC_LITERAL(22, 267, 19), // "openVoxelClipWidget"
QT_MOC_LITERAL(23, 287, 14), // "addVoxelObject"
QT_MOC_LITERAL(24, 302, 18) // "exportDistanceData"

    },
    "VoxelSceneManagerWidget\0actionOnVoxelObject\0"
    "\0VoxelObject*\0obj\0action\0deleteObject\0"
    "sendVoxelScene\0VoxelModel*\0sc\0"
    "updateVoxelModel\0toggleContextMenu\0"
    "pos\0voxelSelected\0VoxelItem*\0item\0"
    "itemSelected\0QTreeWidgetItem*\0witem\0"
    "index\0openImageProjectionWidget\0"
    "deleteVoxelObject\0openVoxelClipWidget\0"
    "addVoxelObject\0exportDistanceData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VoxelSceneManagerWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       6,    0,   79,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   83,    2, 0x0a /* Public */,
      11,    1,   86,    2, 0x0a /* Public */,
      13,    1,   89,    2, 0x0a /* Public */,
      16,    2,   92,    2, 0x0a /* Public */,
      20,    0,   97,    2, 0x08 /* Private */,
      21,    0,   98,    2, 0x08 /* Private */,
      22,    0,   99,    2, 0x08 /* Private */,
      23,    0,  100,    2, 0x08 /* Private */,
      24,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 17, QMetaType::Int,   18,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VoxelSceneManagerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VoxelSceneManagerWidget *_t = static_cast<VoxelSceneManagerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->actionOnVoxelObject((*reinterpret_cast< VoxelObject*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->deleteObject(); break;
        case 2: _t->sendVoxelScene((*reinterpret_cast< VoxelModel*(*)>(_a[1]))); break;
        case 3: _t->updateVoxelModel((*reinterpret_cast< VoxelModel*(*)>(_a[1]))); break;
        case 4: _t->toggleContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->voxelSelected((*reinterpret_cast< VoxelItem*(*)>(_a[1]))); break;
        case 6: _t->itemSelected((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->openImageProjectionWidget(); break;
        case 8: _t->deleteVoxelObject(); break;
        case 9: _t->openVoxelClipWidget(); break;
        case 10: _t->addVoxelObject(); break;
        case 11: _t->exportDistanceData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VoxelSceneManagerWidget::*_t)(VoxelObject * , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelSceneManagerWidget::actionOnVoxelObject)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VoxelSceneManagerWidget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelSceneManagerWidget::deleteObject)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VoxelSceneManagerWidget::*_t)(VoxelModel * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelSceneManagerWidget::sendVoxelScene)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VoxelSceneManagerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VoxelSceneManagerWidget.data,
      qt_meta_data_VoxelSceneManagerWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VoxelSceneManagerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VoxelSceneManagerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VoxelSceneManagerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VoxelSceneManagerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void VoxelSceneManagerWidget::actionOnVoxelObject(VoxelObject * _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VoxelSceneManagerWidget::deleteObject()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VoxelSceneManagerWidget::sendVoxelScene(VoxelModel * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'voxelprocessor.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../voxelprocessor.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'voxelprocessor.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VoxelProcessor_t {
    QByteArrayData data[11];
    char stringdata0[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VoxelProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VoxelProcessor_t qt_meta_stringdata_VoxelProcessor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VoxelProcessor"
QT_MOC_LITERAL(1, 15, 14), // "openVoxelModel"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "const char*"
QT_MOC_LITERAL(4, 43, 8), // "filepath"
QT_MOC_LITERAL(5, 52, 14), // "saveVoxelModel"
QT_MOC_LITERAL(6, 67, 16), // "createVoxelModel"
QT_MOC_LITERAL(7, 84, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(8, 108, 23), // "on_actionQuit_triggered"
QT_MOC_LITERAL(9, 132, 22), // "on_actionNew_triggered"
QT_MOC_LITERAL(10, 155, 23) // "on_actionSave_triggered"

    },
    "VoxelProcessor\0openVoxelModel\0\0"
    "const char*\0filepath\0saveVoxelModel\0"
    "createVoxelModel\0on_actionOpen_triggered\0"
    "on_actionQuit_triggered\0on_actionNew_triggered\0"
    "on_actionSave_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VoxelProcessor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    0,   52,    2, 0x06 /* Public */,
       6,    0,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,
       9,    0,   56,    2, 0x0a /* Public */,
      10,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VoxelProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VoxelProcessor *_t = static_cast<VoxelProcessor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openVoxelModel((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 1: _t->saveVoxelModel(); break;
        case 2: _t->createVoxelModel(); break;
        case 3: _t->on_actionOpen_triggered(); break;
        case 4: _t->on_actionQuit_triggered(); break;
        case 5: _t->on_actionNew_triggered(); break;
        case 6: _t->on_actionSave_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VoxelProcessor::*_t)(const char * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelProcessor::openVoxelModel)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (VoxelProcessor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelProcessor::saveVoxelModel)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (VoxelProcessor::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelProcessor::createVoxelModel)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VoxelProcessor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_VoxelProcessor.data,
      qt_meta_data_VoxelProcessor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VoxelProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VoxelProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VoxelProcessor.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int VoxelProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
void VoxelProcessor::openVoxelModel(const char * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VoxelProcessor::saveVoxelModel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void VoxelProcessor::createVoxelModel()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

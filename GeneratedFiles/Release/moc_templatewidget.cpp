/****************************************************************************
** Meta object code from reading C++ file 'templatewidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../templatewidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'templatewidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TemplateWidget_t {
    QByteArrayData data[11];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TemplateWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TemplateWidget_t qt_meta_stringdata_TemplateWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "TemplateWidget"
QT_MOC_LITERAL(1, 15, 16), // "loadParaFinished"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 7), // "confirm"
QT_MOC_LITERAL(4, 41, 19), // "on_rb_empty_clicked"
QT_MOC_LITERAL(5, 61, 7), // "checked"
QT_MOC_LITERAL(6, 69, 18), // "on_rb_ball_clicked"
QT_MOC_LITERAL(7, 88, 18), // "on_rb_cube_clicked"
QT_MOC_LITERAL(8, 107, 22), // "on_rb_cylinder_clicked"
QT_MOC_LITERAL(9, 130, 24), // "on_cb_ballHollow_clicked"
QT_MOC_LITERAL(10, 155, 24) // "on_cb_cubeHollow_clicked"

    },
    "TemplateWidget\0loadParaFinished\0\0"
    "confirm\0on_rb_empty_clicked\0checked\0"
    "on_rb_ball_clicked\0on_rb_cube_clicked\0"
    "on_rb_cylinder_clicked\0on_cb_ballHollow_clicked\0"
    "on_cb_cubeHollow_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TemplateWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x0a /* Public */,
       4,    1,   56,    2, 0x08 /* Private */,
       6,    1,   59,    2, 0x08 /* Private */,
       7,    1,   62,    2, 0x08 /* Private */,
       8,    1,   65,    2, 0x08 /* Private */,
       9,    1,   68,    2, 0x08 /* Private */,
      10,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,

       0        // eod
};

void TemplateWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TemplateWidget *_t = static_cast<TemplateWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadParaFinished(); break;
        case 1: _t->confirm(); break;
        case 2: _t->on_rb_empty_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_rb_ball_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_rb_cube_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_rb_cylinder_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_cb_ballHollow_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_cb_cubeHollow_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TemplateWidget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TemplateWidget::loadParaFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TemplateWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TemplateWidget.data,
      qt_meta_data_TemplateWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TemplateWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TemplateWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TemplateWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TemplateWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TemplateWidget::loadParaFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

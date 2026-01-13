/****************************************************************************
** Meta object code from reading C++ file 'GLOptionWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/GLWidget/GLOptionWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GLOptionWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GLOptionWidgetT_t {
    QByteArrayData data[13];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GLOptionWidgetT_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GLOptionWidgetT_t qt_meta_stringdata_GLOptionWidgetT = {
    {
QT_MOC_LITERAL(0, 0, 15), // "GLOptionWidgetT"
QT_MOC_LITERAL(1, 16, 16), // "ONOFF_DrawNormal"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "onff"
QT_MOC_LITERAL(4, 39, 14), // "ONOFF_DrawEdge"
QT_MOC_LITERAL(5, 54, 16), // "ONOFF_DrawVertex"
QT_MOC_LITERAL(6, 71, 14), // "ONOFF_DrawFace"
QT_MOC_LITERAL(7, 86, 15), // "ONOFF_Centering"
QT_MOC_LITERAL(8, 102, 22), // "ONOFF_SelectPointsMode"
QT_MOC_LITERAL(9, 125, 21), // "ONOFF_SelectFacesMode"
QT_MOC_LITERAL(10, 147, 21), // "ONOFF_DrawBoundingBox"
QT_MOC_LITERAL(11, 169, 19), // "ONOFF_DrawWorkspace"
QT_MOC_LITERAL(12, 189, 14) // "ONOFF_DrawAxis"

    },
    "GLOptionWidgetT\0ONOFF_DrawNormal\0\0"
    "onff\0ONOFF_DrawEdge\0ONOFF_DrawVertex\0"
    "ONOFF_DrawFace\0ONOFF_Centering\0"
    "ONOFF_SelectPointsMode\0ONOFF_SelectFacesMode\0"
    "ONOFF_DrawBoundingBox\0ONOFF_DrawWorkspace\0"
    "ONOFF_DrawAxis"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLOptionWidgetT[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x0a /* Public */,
       4,    1,   67,    2, 0x0a /* Public */,
       5,    1,   70,    2, 0x0a /* Public */,
       6,    1,   73,    2, 0x0a /* Public */,
       7,    1,   76,    2, 0x0a /* Public */,
       8,    1,   79,    2, 0x0a /* Public */,
       9,    1,   82,    2, 0x0a /* Public */,
      10,    1,   85,    2, 0x0a /* Public */,
      11,    1,   88,    2, 0x0a /* Public */,
      12,    1,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void GLOptionWidgetT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLOptionWidgetT *_t = static_cast<GLOptionWidgetT *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ONOFF_DrawNormal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->ONOFF_DrawEdge((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->ONOFF_DrawVertex((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->ONOFF_DrawFace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->ONOFF_Centering((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->ONOFF_SelectPointsMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->ONOFF_SelectFacesMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->ONOFF_DrawBoundingBox((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->ONOFF_DrawWorkspace((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->ONOFF_DrawAxis((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject GLOptionWidgetT::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GLOptionWidgetT.data,
      qt_meta_data_GLOptionWidgetT,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GLOptionWidgetT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLOptionWidgetT::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GLOptionWidgetT.stringdata0))
        return static_cast<void*>(const_cast< GLOptionWidgetT*>(this));
    return QWidget::qt_metacast(_clname);
}

int GLOptionWidgetT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
